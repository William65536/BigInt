#include <stdio.h> // Maybe just reference the functions that are necessary
#include <stdlib.h>

typedef struct BIGINT_S
{
    unsigned short *data;
    size_t size;
    signed char signum;
} bigint_t;



// Initializers and casts

bigint_t bigint_init(unsigned short *data, size_t size, signed char signum)
{
    return (bigint_t) {
        .data = data,
        .size = size,
        .signum = signum
    };
}

#define BIGINT_SIZE_MULTIPLIER 0.20762050593046014

