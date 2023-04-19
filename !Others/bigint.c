// Inefficient

#include <stdlib.h>
#include <stdio.h> // ?

// Make a bigint reassign function that takes in a bigint reference and a bigint value and make +=, -=, *=, etc. functions

// Make a generic for two different types of free functions: one which takes in a pointer, one which takes in a value

// Make n-ary versions of all the binary operations

// Make a way to make constant bigints

#define bigint bigint_t

#define BIGINT_NULL ((bigint) { .size = 0, .signum = 0, .data = NULL })

typedef struct _BIGINT
{
    unsigned char *data;
    size_t size;
    signed char signum;
} bigint_t;

bigint bigint_alloc(size_t size)
{
    bigint ret = {
        .signum = 1,
        .size = size,
        .data = calloc(size, sizeof(unsigned char))
    };

    if (ret.data == NULL)
    {
        printf("ERROR: Memory allocation failure at bigint_alloc()\n");

        exit(EXIT_FAILURE);
    }

    return ret;
}

void bigint_free_data(bigint *num)
{
    if (num != NULL && num->data != NULL)
    {
        free(num->data);

        num->data = NULL;

        num->signum = 0;
        num->size = 0;
    }
}

int bigint_is_null(bigint num)
{
    return num.data == NULL || !num.signum || !num.size;
}

bigint bigint_copy_data(bigint num)
{
    if (bigint_is_null(num))
        return BIGINT_NULL;

    bigint ret = bigint_alloc(num.size);
    ret.signum = num.signum;

    for (size_t i = 0; i < ret.size; i++)
        ret.data[i] = num.data[i];

    return ret;
}

bigint bigint_from_str(char *str)
{
    if (str == NULL || str[0] == '\0')
    {
        printf("ERROR: Argument cannot be NULL nor an empty string at bigint_from_str()\n");

        exit(EXIT_FAILURE);
    }

    if ((str[0] == 'n' || str[0] == 'N') && (str[1] == 'u' || str[1] == 'U') && (str[2] == 'l' || str[2] == 'L') && (str[3] == 'l' || str[3] == 'L') && str[4] == '\0')
        return BIGINT_NULL;

    signed char signum = 1;

    size_t i = 0, len = 0;

    while (str[len] != '\0')
        len++;

    if (str[0] == '-' || str[0] == '+')
    {
        if (str[0] == '-')
            signum = -1;

        i++;
    }

    while (str[i] == '0')
        i++;

    len -= i;

    if (!len)
    {
        bigint ret = bigint_alloc(1);
        ret.signum = 1;
        ret.data[0] = 0;

        return ret;
    }

    bigint ret = bigint_alloc(len / 2 + len % 2);
    ret.signum = signum;

    for (size_t j = 0; j < ret.size; j++)
    {
        char a = str[len + i - 1 - j * 2], b = str[len + i - 1 - j * 2 - 1];

        if (len % 2 && j == ret.size - 1)
            b = '0';

        if (a < '0' || '9' < a || b < '0' || '9' < b)
        {
            printf("ERROR: Invalid character '%c' in argument at bigint_from_str()\n", a < '0' || '9' < a ? a : b);

            exit(EXIT_FAILURE);
        }

        ret.data[j] = (a - '0') + (b - '0') * 10;
    }

    return ret;
}

void bigint_remove_zeros(bigint *num)
{
    while (!num->data[num->size - 1] && num->size > 1)
        num->size -= 1;

    num->data = realloc(num->data, num->size * sizeof *num->data);
}

void bigint_reassign(bigint *num, char *str)
{
    bigint temp = *num;

    *num = bigint_from_str(str);

    bigint_free_data(&temp);
}

char *bigint_to_str(bigint num)
{
    if (bigint_is_null(num))
        return NULL;

    size_t str_len = num.size * 2 - (num.data[num.size - 1] < 10) + 1;

    char *ret = malloc((str_len + 1) * sizeof *ret);

    ret[0] = num.signum == 1 ? '+' : '-';

    for (size_t i = 0; i < num.size; i++)
    {
        ret[str_len - 2 * i - 1] = num.data[i] % 10 + '0';

        if (!(i == num.size - 1 && !(num.data[i] / 10)))
        {
            ret[str_len - 2 * i - 2] = num.data[i] / 10 + '0';
        }
    }

    ret[str_len] = '\0';

    return ret;
}

void bigint_print(bigint num)
{
    char *str = bigint_to_str(num);

    printf("%s", str);

    free(str);
}

void bigint_println(bigint num)
{
    bigint_print(num);

    printf("\n");
}

void bigint_print_data(bigint num)
{
    if (bigint_is_null(num))
    {
        printf("(null)");

        return;
    }

    printf("(%u) ", (unsigned) num.size); // ?
    
    printf("[ ");

    for (size_t i = 0; i < num.size; i++)
        printf("%02d ", num.data[i]);

    printf("%c ]\n", num.signum == 1 ? '+' : '-');
}

int bigint_compare(bigint a, bigint b)
{
    if (bigint_is_null(a) || bigint_is_null(b))
    {
        return 2; // Weird
    }

    if (a.signum == 1 && b.signum == -1)
        return 1;
    if (a.signum == -1 && b.signum == 1)
        return -1;

    if (a.size > b.size)
        return a.signum;
    if (b.size > a.size)
        return -b.signum;

    for (int i = a.size - 1; i >= 0; i--)
        if (a.data[i] > b.data[i])
            return a.signum;
        else if (a.data[i] < b.data[i])
            return -b.signum;


    return 0;
}

int bigint_compare_temps(bigint *a, bigint *b)
{
    int ret = bigint_compare(*a, *b);

    bigint_free_data(a);
    bigint_free_data(b);

    return ret;
}

bigint bigint_abs(bigint num)
{
    bigint ret = bigint_copy_data(num);

    ret.signum = !bigint_is_null(ret) ? 1 : 0;

    return ret;
}

bigint bigint_abs_temp(bigint *num)
{
    bigint ret = bigint_abs(*num);

    bigint_free_data(num);

    return ret;
}

bigint bigint_sub(bigint a, bigint b);

bigint bigint_add(bigint a, bigint b)
{
    if (bigint_is_null(a) || bigint_is_null(b))
        return BIGINT_NULL;

    if (a.signum == 1 && b.signum == -1)
    {
        bigint pos_b = bigint_abs(b), ret = bigint_sub(a, pos_b);
        
        bigint_free_data(&pos_b);

        return ret;
    }
    
    if (a.signum == -1 && b.signum == 1)
    {
        bigint pos_a = bigint_abs(a), ret = bigint_sub(b, pos_a);

        bigint_free_data(&pos_a);

        return ret;
    }

    bigint ret = bigint_alloc((a.size >= b.size ? a.size : b.size) + 1);

    for (size_t i = 0; i < ret.size - 1; i++)
    {
        ret.data[i] += (i < a.size ? a.data[i] : 0) + (i < b.size ? b.data[i] : 0);

        ret.data[i + 1] = ret.data[i] / 100;

        ret.data[i] %= 100;
    }

    if (!ret.data[ret.size - 1])
        ret.data = realloc(ret.data, --ret.size * sizeof *ret.data); // Does realloc free memory? If not, there may be a memory leak here.

    ret.signum = a.signum;

    return ret;
}

bigint bigint_add_temps(bigint *a, bigint *b)
{
    bigint ret = bigint_add(*a, *b);

    bigint_free_data(a);
    bigint_free_data(b);

    return ret;
}

bigint bigint_sub(bigint a, bigint b)
{
    if (bigint_is_null(a) || bigint_is_null(b))
        return BIGINT_NULL;

    if (a.signum == 1 && b.signum == -1)
    {
        bigint pos_b = bigint_abs(b), ret = bigint_add(a, pos_b);

        bigint_free_data(&pos_b);

        return ret;
    }

    if (a.signum == -1 && b.signum == 1)
    {
        bigint neg_b = bigint_copy_data(b);
        neg_b.signum = -1;

        bigint ret = bigint_add(a, neg_b);

        bigint_free_data(&neg_b);

        return ret;
    }

    bigint abs_a = bigint_abs(a), abs_b = bigint_abs(b);

    int comparison = bigint_compare_temps(&abs_a, &abs_b);

    bigint temp_a = comparison == -1 ? b : a, temp_b = comparison == -1 ? a : b;

    bigint ret = bigint_alloc(temp_a.size);

    if (comparison == -1)
        ret.signum = -ret.signum;

    for (size_t i = 0; i < ret.size; i++)
    {
        ret.data[i] += temp_a.data[i] - (i < temp_b.size ? temp_b.data[i] : 0);

        if (ret.data[i] > 100) // Because of unsigned arithmetic
        {
            ret.data[i + 1] = -1;

            ret.data[i] += 100;
        }
    }

    bigint_remove_zeros(&ret); // Maybe there's a better way of doing this

    return ret;
}

bigint bigint_sub_temps(bigint *a, bigint *b)
{
    bigint ret = bigint_sub(*a, *b);

    bigint_free_data(a);
    bigint_free_data(b);

    return ret;
}

bigint bigint_digit_shift_left(bigint num, unsigned long long shift_amount)
{
    bigint ret = bigint_alloc(num.size + shift_amount);
    ret.signum = num.signum;

    size_t i = 0;

    for (; i < shift_amount; i++)
        ret.data[i] = 0;

    for (; i < ret.size; i++)
        ret.data[i] = num.data[i - shift_amount];

    return ret;
}

bigint bigint_digit_shift_left_temp(bigint *num, unsigned long long shift_amount)
{
    bigint ret = bigint_digit_shift_left(*num, shift_amount);

    bigint_free_data(num);

    return ret;
}

bigint bigint_digit_shift_right(bigint num, unsigned long long shift_amount)
{
    if (shift_amount >= num.size)
        return bigint_from_str("0");

    bigint ret = bigint_alloc(num.size - shift_amount);
    ret.signum = num.signum;

    for (size_t i = 0; i < ret.size; i++)
        ret.data[i] = num.data[i + shift_amount];

    return ret;
}

bigint bigint_digit_shift_right_temp(bigint *num, unsigned long long shift_amount)
{
    bigint ret = bigint_digit_shift_right(*num, shift_amount);

    bigint_free_data(num);

    return ret;
}

bigint bigint_mult(bigint a, bigint b)
{
    // Maybe reordering arguments based on size is be faster
    
    if (bigint_is_null(a) || bigint_is_null(b))
        return BIGINT_NULL;

    bigint ret = bigint_alloc(a.size + b.size);
    ret.signum = a.signum * b.signum;

    for (size_t i = 0; i < b.size; i++)
    {
        char *prod = calloc(a.size + 1, sizeof *prod);
        // char prod[a.size + 1]; // Why doesn't this work?

        for (size_t j = 0; j < a.size; j++)
        {
            prod[j] += b.data[i] * a.data[j] % 100;

            prod[j + 1] = b.data[i] * a.data[j] / 100;
        }

        for (size_t k = 0; k < a.size + (prod[a.size] > 0); k++)
        {
            ret.data[k + i] += prod[k];

            ret.data[k + 1 + i] += ret.data[k + i] / 100;

            ret.data[k + i] %= 100;
        }

        free(prod);
    }

    bigint_remove_zeros(&ret);

    return ret;
}

// bigint bigint_mult(bigint a, bigint b) // Basic multiply; polynomial multiply later
// {
//     if (bigint_is_null(a) || bigint_is_null(b))
//         return BIGINT_NULL;

//     bigint ret = bigint_alloc(a.size + b.size);
//     ret.signum = a.signum * b.signum;

//     for (size_t i = 0; i < a.size; i++)
//     {
//         for (size_t j = 0; j < b.size; j++)
//         {
//             unsigned short temp = a.data[i] * b.data[i];

//             a.data[i + j] += temp % 100;

//             a.data[i + j + 1] = temp / 100;


//         }
//     }
// }

// bigint bigint_mult_temps(bigint *a, bigint *b)
// {
//     bigint ret = bigint_mult(*a, *b);

//     bigint_free_data(a);
//     bigint_free_data(b);

//     return ret;
// }




// Make a format function that removes zeros