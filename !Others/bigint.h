// Inefficient

#pragma once

#define bigint bigint_t

#define BIGINT_NULL ((bigint) { .size = 0, .signum = 0, .data = NULL })

typedef struct _BIGINT
{
    unsigned char *data;
    size_t size;
    signed char signum;
} bigint_t;

bigint bigint_alloc(size_t size);

void bigint_free_data(bigint *num);

int bigint_is_null(bigint num);

bigint bigint_cpy_data(bigint num);

bigint bigint_from_str(char *str);

void bigint_remove_zeros(bigint *num);

void bigint_reassign(bigint *num, char *str);

char *bigint_to_str(bigint num);

void bigint_print(bigint num);

void bigint_println(bigint num);

void bigint_print_data(bigint num);

int bigint_compare(bigint a, bigint b);

int bigint_compare_temps(bigint *a, bigint *b);

bigint bigint_abs(bigint num);

bigint bigint_abs_temp(bigint *num);

bigint bigint_add(bigint a, bigint b);

bigint bigint_add_temps(bigint *a, bigint *b);

bigint bigint_sub(bigint a, bigint b);

bigint bigint_sub_temps(bigint *a, bigint *b);

bigint bigint_digit_shift_left(bigint a, unsigned long long shift_amount);

bigint bigint_digit_shift_left_temp(bigint *a, unsigned long long shift_amount);

bigint bigint_digit_shift_right(bigint a, unsigned long long shift_amount);

bigint bigint_digit_shift_right_temp(bigint *a, unsigned long long shift_amount);

bigint bigint_mult(bigint a, bigint b);

// bigint bigint_mult_temps(bigint *a, bigint *b);