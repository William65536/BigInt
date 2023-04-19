// Inefficient

#pragma once

#include <stdlib.h>

#define bigint bigint_t

// Check for null in each function

typedef struct _BIGINT
{
    signed char *data;
    size_t size;
    signed char signum;
} bigint_t;

bigint *bigint_alloc(size_t size)
{
    bigint *ret = malloc(sizeof *ret);

    if (ret == NULL)
    bigint_alloc_error: {
        printf("ERROR: Memory allocation failed at bigint_alloc()\n");
        
        exit(EXIT_FAILURE);
    }

    ret->signum = 1;
    ret->size = size;

    ret->data = calloc(ret->size, sizeof *ret->data);

    if (ret->data == NULL)
        goto bigint_alloc_error;

    return ret;
}

void bigint_free(bigint **num)
{
    if (*num != NULL)
        free((*num)->data);

    free(*num);

    *num = NULL;
}

bigint *bigint_cpy(bigint *num)
{
    if (num == NULL)
        return NULL;

    bigint *ret = bigint_alloc(num->size);
    ret->signum = num->signum;

    for (size_t i = 0; i < ret->size; i++)
        ret->data[i] = num->data[i];

    return ret;
}

bigint *bigint_from_str(char *str)
{
    if (str == NULL || str[0] == '\0')
        return NULL;

    char signum = 1;

    size_t i = 0, len = 0;

    while (str[len] != '\0')
        len++;

    if (str[0] == '-')
    {
        signum = -1;

        i++;
    }

    if (str[0] == '+')
    {
        signum = 1;

        i++;
    }

    while (str[i] == '0')
        i++;

    len -= i;

    if (!len)
    {
        bigint *ret = bigint_alloc(1);
        ret->signum = 1;
        ret->data[0] = 0;

        return ret;
    }

    bigint *ret = bigint_alloc(len / 2 + len % 2);

    ret->signum = signum;

    for (size_t j = 0; j < ret->size; j++)
    {
        char a = str[len + i - 1 - j * 2], b = str[len + i - 1 - j * 2 - 1];

        if (len % 2 && j == ret->size - 1)
            b = '0';

        if (a < '0' || '9' < a || b < '0' || '9' < b)
        {
            printf("ERROR: Invalid character '%c' in argument at bigint_from_str()\n", a < '0' || '9' < a ? a : b);

            exit(EXIT_FAILURE);
        }

        ret->data[j] = (a - '0') + (b - '0') * 10;
    }

    return ret;
}

char *bigint_to_str(bigint *num)
{
    if (num == NULL)
        return NULL;

    size_t str_len = num->size * 2 - (num->data[num->size - 1] < 10) + 1;

    char *ret = malloc((str_len + 1) * sizeof *ret);

    ret[0] = num->signum == 1 ? '+' : '-';

    for (size_t i = 0; i < num->size; i++)
    {
        ret[str_len - 2 * i - 1] = num->data[i] % 10 + '0';

        if (!(i == num->size - 1 && !(num->data[i] / 10)))
        {
            ret[str_len - 2 * i - 2] = num->data[i] / 10 + '0';
        }
    }

    ret[str_len] = '\0';

    return ret;
}

void bigint_print(bigint *num)
{
    char *str = bigint_to_str(num);

    printf("%s", str);

    free(str);
}

void bigint_println(bigint *num)
{
    bigint_print(num);

    printf("\n");
}

void bigint_print_data(bigint *num)
{
    if (num == NULL)
    {
        printf("(null)");

        return;
    }

    printf("(%d) ", num->size);
    
    printf("[ ");

    for (size_t i = 0; i < num->size; i++)
        printf("%02d ", num->data[i]);

    printf("%c ]\n", num->signum == 1 ? '+' : '-');
}

int bigint_compare(bigint *a, bigint *b) // Inefficient; reduce if-statements
{
    if (a == NULL || b == NULL)
    {
        printf("ERROR: NULL is an invalid input at bigint_compare()\n");

        exit(EXIT_FAILURE);
    }

    if (a->signum == 1 && b->signum == -1)
        return 1;
    if (a->signum == -1 && b->signum == 1)
        return -1;

    if (a->size > b->size)
        return a->signum;
    if (b->size > a->size)
        return -b->signum;

    for (int i = a->size - 1; i >= 0; i--)
        if (a->data[i] > b->data[i])
            return a->signum;
        else if (a->data[i] < b->data[i])
            return -b->signum;


    return 0;
}

int bigint_compare_temps(bigint **a, bigint **b)
{
    int ret = bigint_compare(*a, *b);

    bigint_free(a);
    bigint_free(b);

    return ret;
}

bigint *bigint_abs(bigint *num)
{
    if (num == NULL)
        return NULL;

    bigint *ret = bigint_cpy(num);

    ret->signum = 1;
    
    return ret;
}

bigint *bigint_abs_temp(bigint **num)
{
    bigint *ret = bigint_abs(*num);

    bigint_free(num);

    return ret;
}

bigint *bigint_sub(bigint *a, bigint *b);

bigint *bigint_add(bigint *a, bigint *b)
{
    if (a == NULL || b == NULL)
        return NULL;

    if (a->signum == 1 && b->signum == -1)
    {
        bigint *pos_b = bigint_abs(b), *ret = bigint_sub(a, pos_b);

        bigint_free(&pos_b);

        return ret;
    }
    
    if (a->signum == -1 && b->signum == 1)
    {
        bigint *pos_a = bigint_abs(a), *ret = bigint_sub(b, pos_a);

        bigint_free(&pos_a);

        return ret;
    }

    bigint *ret = bigint_alloc((a->size >= b->size ? a->size : b->size) + 1);

    for (size_t i = 0; i < ret->size - 1; i++)
    {
        ret->data[i] += (i < a->size ? a->data[i] : 0) + (i < b->size ? b->data[i] : 0);

        ret->data[i + 1] = ret->data[i] / 100;

        ret->data[i] %= 100;
    }

    if (!ret->data[ret->size - 1])
        ret->data = realloc(ret->data, --ret->size); // Does realloc free memory? If not, there may be a memory leak here.

    ret->signum = a->signum;

    return ret;
}

bigint *bigint_add_temps(bigint **a, bigint **b)
{
    bigint *ret = bigint_add(*a, *b);

    bigint_free(a);
    bigint_free(b);

    return ret;
}

bigint *bigint_sub(bigint *a, bigint *b)
{
    if (a == NULL || b == NULL)
        return NULL;

    if (a->signum == 1 && b->signum == -1)
    {
        bigint *pos_b = bigint_abs(b), *ret = bigint_add(a, pos_b);

        bigint_free(&pos_b);

        return ret;
    }

    if (a->signum == -1 && b->signum == 1)
    {
        bigint *neg_b = bigint_cpy(b);
        neg_b->signum = -1;

        bigint *ret = bigint_add(a, neg_b);

        bigint_free(&neg_b);

        return ret;
    }

    bigint *abs_a = bigint_abs(a);
    bigint *abs_b = bigint_abs(b);

    int comparison = bigint_compare(abs_a, abs_b);

    bigint_free(&abs_a);
    bigint_free(&abs_b);

    bigint *temp_a = bigint_cpy(a);
    bigint *temp_b = bigint_cpy(b);

    int change_signum = 0;

    if (comparison == -1)
    {
        bigint temp_temp_a = *temp_a;
        *temp_a = *temp_b; // Does this work?
        *temp_b = temp_temp_a;

        change_signum = 1;
    }

    bigint *ret = bigint_alloc(temp_a->size);
    ret->signum = temp_a->signum * (1 - 2 * change_signum);

    for (size_t i = 0; i < ret->size; i++) // signed or unsigned char data[]?
    {
        ret->data[i] += temp_a->data[i] - (i < temp_b->size ? temp_b->data[i] : 0);

        if (ret->data[i] < 0)
        {
            ret->data[i + 1] = -1;

            ret->data[i] += 100;
        }
    }

    bigint_free(&temp_a);
    bigint_free(&temp_b);

    return ret;
}

bigint *bigint_sub_temps(bigint **a, bigint **b)
{
    bigint *ret = bigint_sub(*a, *b);

    bigint_free(a);
    bigint_free(b);

    return ret;
}

bigint *bigint_mult(bigint *a, bigint *b) // Basic multiply ~ polynomial multiply later
{
    if (a == NULL || b == NULL)
        return NULL;

    char signum = a->signum * b->signum;


    return NULL;
}

bigint *bigint_mult_temps(bigint **a, bigint **b)
{
    bigint *ret = bigint_mult(*a, *b);

    bigint_free(a);
    bigint_free(b);

    return ret;
}