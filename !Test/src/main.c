#include <stdio.h>
#include <stdlib.h>

typedef struct BIGINT_S
{
    unsigned short data[0x10000];
    size_t size;
} bigint_t;

void bigint_print(bigint_t a)
{
    printf("(%zu) [ ", a.size);

    for (size_t i = 0; i < a.size; i++)
        printf("%04x ", a.data[i]);

    printf("]");
}

void bigint_println(bigint_t a)
{
    bigint_print(a);

    printf("\n");
}

void bigint_add_assign_bigint(bigint_t *a, bigint_t b)
{
    size_t max_size = a->size > b.size ? a->size : b.size;

    for (size_t i = 0; i < max_size; i++)
    {
        unsigned int curr_sum = a->data[i] + b.data[i];

        printf("%x\n", curr_sum);

        a->data[i] = curr_sum % 0x10000;

        a->data[i + 1] += curr_sum / 0x10000;
    }


    if (a->data[max_size])
        a->size = max_size + 1;
    else if (a->data[max_size - 1])
        a->size = max_size;
}

void bigint_add_assign_long(bigint_t *a, unsigned long long b)
{
    for (size_t i = 0; i < 4; i++)
    {
        unsigned int curr_sum = a->data[i] + (b >> 16 * i);

        a->data[i] = curr_sum % 0x10000;

        a->data[i + 1] = curr_sum / 0x10000;
    }

    size_t max_size = a->size > 4 ? a->size : 4;

    if (a->data[max_size])
        a->size = max_size + 1;
    else if (a->data[max_size - 1])
        a->size = max_size;
}

// size_t bigint_collatz(bigint_t *a, size_t init_size)
// {
//     if (a->data[0] & 1)
//     {
//         for (size_t i = 0; i < a->size - 1; i++)
//         {
//             a->data[i] <<= 1;
//         }
//     }
// }

void bigint_halve(bigint_t *a)
{
    for (size_t i = 0; i < a->size - 1; i++)
    {
        a->data[i] >>= 1;

        a->data[i] |= a->data[i + 1] << 15;
    }

    a->size -= !(a->data[a->size - 1] >>= 1) && a->size > 1;
}

void bigint_double(bigint_t *a)
{
    // 8000 0001

    // 0000 0003

    a->size += a->data[a->size - 1] >> 15;

    for (size_t i = a->size - 1; i >= 0; i--)
    {
        a->data[i] <<= 1;


    }


    // for (size_t i = 0; i < a->size; i++)
    // {
    //     a->data[i + 1]

    //     a->data[i] <<= 1;
    // }
}

int main()
{
    bigint_t lower_bound = {
        .data = { 0x0001, 0x0000, 0x0000, 0x0000, 0x0020 },
        .size = 5
    };

    bigint_t upper_bound = {
        .data = { 0x0001, 0x0000, 0x0000, 0x0000, 0x0100 },
        .size = 5
    };

    bigint_t a = {
        .data = { 0x0000, 0x0001 },
        .size = 2
    };

    bigint_halve(&a);

    bigint_println(a);

    // printf("%d\n", 2863311531 * 99);

    for (unsigned short i = 0; i < 0xFFFF; i += 3)
        printf("%d ", (unsigned char) (i * 171));
        // printf("%d ", (unsigned short) (i * 11 >> 5));

    return EXIT_SUCCESS;
}