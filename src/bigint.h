#pragma once

#define bigint bigint_t *

typedef struct BIGINT_S bigint_t;



// Allocators and deallocators

#ifdef __USE_BIGINT_DEVELOPER
bigint bigint_alloc(size_t size);
#endif

void bigint_free(bigint *num_ptr);



// Initializers and casts

bigint bigint_from_long(long long num);

bigint bigint_from_str(const char *const str);

bigint bigint_copy(bigint const num);

void bigint_reassign(bigint *num_ptr, const char *const str);



// I/O functionality

#ifdef __USE_BIGINT_DEVELOPER
void bigint_print_data(bigint const num);
#endif

void bigint_print(bigint const num);

void bigint_println(bigint const num);



// ALU operations

int bigint_compare(bigint const a, bigint const b);

int bigint_compare_temps(bigint *a_ptr, bigint *b_ptr);

bigint bigint_abs(bigint const num);

bigint bigint_abs_temp(bigint *num_ptr);

bigint bigint_add(bigint const a, bigint const b);

bigint bigint_add_temps(bigint *a_ptr, bigint *b_ptr);

bigint bigint_sub(bigint const a, bigint const b);

bigint bigint_sub_temps(bigint *a_ptr, bigint *b_ptr);

bigint bigint_mult(bigint const a, bigint const b);

bigint bigint_mult_temps(bigint *a_ptr, bigint *b_ptr);



// Collatz operations

// bigint bigint_divide_by_two(bigint const num);

bigint bigint_collatz(bigint const num);

bigint bigint_collatz_temp(bigint *num_ptr);

// bigint bigint_to_str(bigint const num);

// Number theory operations

// Signum, modulo, rem, successor, etc.