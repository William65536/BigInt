#include <stdlib.h>
#include <stdio.h> // Maybe just reference the functions that are necessary

typedef struct BIGINT_S
{
    unsigned short *data;
    size_t size;
    signed char signum;
} bigint_t;

static size_t bigint_alloc_count = 0;



// Allocators and deallocators

bigint_t *bigint_alloc(size_t size)
{
    bigint_t *ret = malloc(sizeof *ret);

    ret->signum = 1;
    ret->size = size;
    ret->data = calloc(ret->size, sizeof *ret->data);

    if (ret->data == NULL)
    {
        printf("ERROR: Memory allocation failure at bigint_alloc()\n");

        exit(EXIT_FAILURE);
    }

    #ifdef __USE_BIGINT_ALLOC_DEBUG
    printf("Bigint alloc %zu\n", ++bigint_alloc_count);
    #endif

    return ret;
}

void bigint_free(bigint_t **num_ptr)
{
    if (*num_ptr != NULL)
    {
        free((*num_ptr)->data);

        (*num_ptr)->data = NULL;

        #ifdef __USE_BIGINT_ALLOC_DEBUG
        printf("Bigint free %zu\n", --bigint_alloc_count);
        #endif
    }

    free(*num_ptr);

    *num_ptr = NULL;
}



// Initializers and casts

bigint_t *bigint_from_long(long long num)
{
    signed char signum = 1 - 2 * (num < 0);

    num = signum * num;

    bigint_t *ret = bigint_alloc(1 + !!(num >> 16) + !!(num >> 32) + !!(num >> 48));

    ret->signum = signum;

    for (size_t i = 0; i < ret->size; i++)
        ret->data[i] = num >> 16 * i;

    return ret;
}

#define SIZE_MULTIPLIER 0.20762050593046014

bigint_t *bigint_from_str(const char *const str)
{
    if (str == NULL || str[0] == '\0')
    {
        printf("ERROR: Argument cannot be NULL nor an empty string at bigint_from_str()\n");

        exit(EXIT_FAILURE);
    }

    if ((str[0] == 'n' || str[0] == 'N') && (str[1] == 'u' || str[1] == 'U') && (str[2] == 'l' || str[2] == 'L') && (str[3] == 'l' || str[3] == 'L') && str[4] == '\0')
        return NULL;
    
    signed char signum = 1 - 2 * (str[0] == '-');

    size_t i = str[0] == '-' || str[0] == '+', len = 0;

    while (str[i] == '0')
        i++;
    
    while (str[len + i] != '\0')
    {
        len++;

        if (str[i] < '0' || '9' < str[i])
        {
            printf("ERROR: Invalid character '%c' at index %zu in argument at bigint_from_str()\n", str[i], i);

            exit(EXIT_FAILURE);
        }
    }

    if (!len)
    {
        bigint_t *ret = bigint_alloc(1);
        ret->signum = 1;
        ret->data[0] = 0;

        return ret;
    }

    bigint_t *ret = bigint_alloc(len * SIZE_MULTIPLIER + 1); // ceil, but just + 1 because argument of ceil is never rational and therefore never an integer
    ret->signum = signum;

    size_t str_len = len + i;

    for (; i < str_len; i++)
    {
        unsigned int val = str[i] - '0';

        for (size_t j = 0; j < ret->size; j++)
        {
            unsigned int digit = ret->data[j] * 10 + val;

            ret->data[j] = digit;

            val = digit >> 16;
        }
    }

    if (!ret->data[ret->size - 1])
    {
        ret->size--;

        ret->data = realloc(ret->data, ret->size);
    }

    return ret;
}

bigint_t *bigint_copy(bigint_t *const num)
{
    if (num == NULL)
        return NULL;
    
    bigint_t *ret = bigint_alloc(num->size);
    ret->signum = num->signum;

    for (size_t i = 0; i < num->size; i++)
        ret->data[i] = num->data[i];

    return ret;
}

void bigint_reassign(bigint_t **num_ptr, const char *const str) // Check if this works
{
    // bigint_t *temp = *num_ptr;

    // *num_ptr = bigint_from_str(str);

    // bigint_free(&temp);

    bigint_free(num_ptr);

    *num_ptr = bigint_from_str(str);
}



// I/O functionality

void bigint_print_data(bigint_t *const num)
{
    if (num == NULL)
    {
        printf("(null)\n");

        return;
    }

    printf("(%zu) [ ", num->size);

    for (size_t i = 0; i < num->size; i++)
        printf("%04x ", num->data[i]);

    printf("%c ]\n", num->signum == 1 ? '+' : '-');
}

void bigint_print(bigint_t *const num)
{
    printf("%c", '-' - 2 * (num->signum == 1));

    for (signed long long i = num->size - 1; i >= 0; i--)
    {
        for (signed long long j = 3; j >= 0; j--)
        {
            // printf("%c", (num->data[i] >> j & 0xF) + '0' + 17 * (10 <= num->data[i] && num->data[i] < 16));
            printf("%x", num->data[i] >> 4 * j & 0x0F);
        }
    }
}

void bigint_println(bigint_t *const num)
{
    bigint_print(num);

    printf("\n");
}



// ALU operations

int bigint_compare(bigint_t *const a, bigint_t *const b)
{
    if (a == NULL || b == NULL)
        return 2;
    
    if (a->signum != b->signum || a->size > b->size)
        return a->signum;

    if (a->size < b->size)
        return -b->signum;

    for (signed long long i = a->size - 1; i >= 0; i--)
        if (a->data[i] > b->data[i])
            return a->signum;
        else if (a->data[i] < b->data[i])
            return -b->signum;

    return 0;
}

int bigint_compare_temps(bigint_t **a_ptr, bigint_t **b_ptr)
{
    int ret = bigint_compare(*a_ptr, *b_ptr);

    bigint_free(a_ptr);
    bigint_free(b_ptr);

    return ret;
}

// bigint_t *bigint_shift_left(bigint_t *const num, size_t amount)
// {
//     if (amount >= num->size * 16)
//         return bigint_alloc(1);
    
//     bigint_t *ret = bigint_alloc(num->size + amount / 16 + !!(amount % 16));

//     // for (size_t i = 0; i < num->size; i++)
//     // {
//     //     ret->data[i + amount] = num->data[i];
//     // }
// }

// bigint_t *bigint_shift_right(bigint_t *const num, size_t amount)
// {

// }

bigint_t *bigint_abs(bigint_t *const num)
{
    if (num == NULL)
        return NULL;

    bigint_t *ret = bigint_copy(num);

    ret->signum = 1;

    return ret;
}

bigint_t *bigint_abs_temp(bigint_t **num_ptr)
{
    bigint_t *ret = bigint_abs(*num_ptr);

    bigint_free(num_ptr);

    return ret;
}

bigint_t *bigint_sub(bigint_t *const a, bigint_t *const b);

bigint_t *bigint_add(bigint_t *const a, bigint_t *const b)
{
    if (a == NULL || b == NULL)
        return NULL;

    if (a->signum == 1 && b->signum == -1)
    {
        bigint_t *pos_b = bigint_abs(b), *ret = bigint_sub(a, pos_b);

        bigint_free(&pos_b);

        return ret;
    }

    if (a->signum == -1 && b->signum == 1)
    {
        bigint_t *pos_a = bigint_abs(a), *ret = bigint_sub(b, pos_a);

        bigint_free(&pos_a);

        return ret;
    }
    
    bigint_t *ret = bigint_alloc((a->size >= b->size ? a->size : b->size) + 1);

    for (size_t i = 0; i < ret->size - 1; i++)
    {
        unsigned int digit = a->data[i] * (i < a->size) + b->data[i] * (i < b->size);

        ret->data[i] += digit;

        ret->data[i + 1] = digit / 0x10000;

        ret->data[i] %= 0x10000;
    }

    if (!ret->data[ret->size - 1])
        ret->data = realloc(ret->data, --ret->size * sizeof *ret->data);
    
    ret->signum = a->signum;

    return ret;
}

bigint_t *bigint_add_temps(bigint_t **a_ptr, bigint_t **b_ptr)
{
    bigint_t *ret = bigint_add(*a_ptr, *b_ptr);

    bigint_free(a_ptr);
    bigint_free(b_ptr);

    return ret;
}

bigint_t *bigint_sub(bigint_t *const a, bigint_t *const b)
{
    if (a == NULL || b == NULL)
        return NULL;

    if (a->signum == 1 && b->signum == -1)
    {
        bigint_t *pos_b = bigint_abs(b), *ret = bigint_add(a, pos_b);

        bigint_free(&pos_b);

        return ret;
    }

    if (a->signum == -1 && b->signum == 1)
    {
        bigint_t *neg_b = bigint_copy(b);
        neg_b->signum = -1;

        bigint_t *ret = bigint_add(a, neg_b);

        bigint_free(&neg_b);

        return ret;
    }

    bigint_t *abs_a = bigint_abs(a), *abs_b = bigint_abs(b);

    int comparison = bigint_compare_temps(&abs_a, &abs_b);

    bigint_t *temp_a = comparison == -1 ? b : a, *temp_b = comparison == -1 ? a : b;

    bigint_t *ret = bigint_alloc(temp_a->size);

    if (comparison == -1)
        ret->signum = -ret->signum;

    for (size_t i = 0; i < ret->size; i++)
    {
        signed int digit = temp_a->data[i] - temp_b->data[i] * (i < temp_b->size);

        ret->data[i] += digit;

        if (digit < 0)
        {
            ret->data[i + 1]--;

            ret->data[i] += 0x10000;
        }
    }

    while (!ret->data[ret->size - 1] && ret->size > 1)
        ret->size--;

    ret->data = realloc(ret->data, ret->size);

    return ret;
}

bigint_t *bigint_sub_temps(bigint_t **a_ptr, bigint_t **b_ptr)
{
    bigint_t *ret = bigint_sub(*a_ptr, *b_ptr);

    bigint_free(a_ptr);
    bigint_free(b_ptr);

    return ret;
}

bigint_t *bigint_mult(bigint_t *const a, bigint_t *const b)
{
    if (a == NULL || b == NULL)
        return NULL;

    bigint_t *ret = bigint_alloc(a->size + b->size);
    ret->signum = a->signum * b->signum;

    for (size_t i = 0; i < b->size; i++)
    {
        unsigned short *prod = calloc(a->size + 1, sizeof *prod);
        // char prod[a->size + 1]; // Why doesn't this work?

        for (size_t j = 0; j < a->size; j++)
        {
            unsigned int digit = b->data[i] * a->data[j];

            prod[j] += digit % 0x10000;

            prod[j + 1] = digit / 0x10000;
        }

        for (size_t j = 0; j < a->size + !!prod[a->size]; j++)
        {
            ret->data[j + i] += prod[j];

            ret->data[j + 1 + i] += ret->data[j + i] / 100;

            ret->data[j + i] %= 0x10000;
        }

        free(prod);
    }

    while (!ret->data[ret->size - 1] && ret->size > 1)
        ret->size--;

    ret->data = realloc(ret->data, ret->size);

    return ret;
}

bigint_t *bigint_mult_temps(bigint_t **a_ptr, bigint_t **b_ptr)
{
    bigint_t *ret = bigint_mult(*a_ptr, *b_ptr);

    bigint_free(a_ptr);
    bigint_free(b_ptr);

    return ret;
}

// bigint_t *bigint_div(bigint_t *const a, bigint_t *const b)
// {
//     // Check for the case that b is 0

//     if (a == NULL || b == NULL)
//         return NULL;

//     int comparison = bigint_compare(a, b);
    
//     if (comparison == -1)
//         return bigint_alloc(1);
    
//     if (comparison == 0)
//         return bigint_from_long(1);
    
//     bigint_t *ret = bigint_alloc(a->size);

//     while (1) // ?
//     {
        
//     }
// }

// Collatz operations

bigint_t *bigint_divide_by_two(bigint_t *const num)
{
    bigint_t *ret = bigint_alloc(num->size - (!(num->data[num->size - 1] >> 1) && num->size != 1));

    // bigint_t *ret = bigint_alloc(num->size); // ?
    ret->signum = num->signum;

    for (size_t i = 0; i < ret->size; i++)
        ret->data[i] = (num->data[i] >> 1) | (num->data[i + 1] * (i + 1 < num->size) << 15);


    return ret;
}

bigint_t *bigint_add_long(bigint_t *const a, long long b)
{
    bigint_t *bigint_b = bigint_from_long(b);

    bigint_t *ret = bigint_add(a, bigint_b);

    bigint_free(&bigint_b);

    return ret;
}

bigint_t *bigint_mult_long(bigint_t *const a, long long b)
{
    bigint_t *bigint_b = bigint_from_long(b);

    bigint_t *ret = bigint_mult(a, bigint_b);

    bigint_free(&bigint_b);

    return ret;
}

bigint_t *bigint_collatz(bigint_t *const num)
{
    if (num->data[0] & 1)
    {
        bigint_t *prod = bigint_mult_long(num, 3);

        bigint_t *prod_sum = bigint_add_long(prod, 1);

        bigint_free(&prod);

        bigint_t *ret = bigint_divide_by_two(prod_sum);

        bigint_free(&prod_sum);

        return ret;
    }

    return bigint_divide_by_two(num);
}

bigint_t *bigint_collatz_temp(bigint_t **num_ptr)
{
    bigint_t *ret = bigint_collatz(*num_ptr);

    bigint_free(num_ptr);

    return ret;
}

// bigint_t *collatz_loop(bigint_t **num) // Maybe cache previous iterations
// {

// }







