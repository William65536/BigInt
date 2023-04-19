#pragma once

#define bigint bigint_t

typedef unsigned long long size_t;

typedef struct BIGINT_S {
    unsigned short *data;
    size_t size;
    signed char signum;
} bigint_t;



// Initializers and casts

bigint bigint_init(unsigned short *data, size_t size, signed char signum);

#define BIGINT_SIZE_MULTIPLIER 0.20762050593046014