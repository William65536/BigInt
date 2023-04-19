#pragma once

#include <stdlib.h>

typedef struct _BIGNUM
{
    signed char *whole;
    signed char *frac;
    size_t size;
    signed char signum;
} bignum_t;

#ifndef bignum
#define bignum bignum_t
#endif