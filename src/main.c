#include <stdio.h>
#include <stdlib.h>

#define __USE_BIGINT_DEVELOPER

#include "bigint.h"



int main()
{
    // FILE *file = fopen("test.txt", "w");

    // bigint curr = bigint_from_str("295147905179352825857");
    // bigint curr = bigint_from_str("7");
    bigint curr = bigint_from_str("3");

    // bigint upper_bound = bigint_from_str("4722366482869645213697");
    bigint upper_bound = bigint_from_str("10");

    bigint two = bigint_from_str("2");

    bigint one = bigint_from_str("1");

    bigint_free(&curr);

    // bigint_print_data(curr);

    // while (bigint_compare(curr, one) == 1)
    // {
    //     curr = bigint_collatz_temp(&curr);

    //     bigint_print_data(curr);
    // }

    // size_t steps;

    while (bigint_compare(curr, upper_bound) == -1)
    {
        // steps = 0;

        bigint temp = bigint_copy(curr);

        // while (bigint_compare(temp, one) == 1)
        // {
        //     temp = bigint_collatz_temp(&temp);

        //     // steps++;
        // }

        bigint_free(&temp);

        bigint_print(curr);

        // printf(": %zu\n", steps);

        temp = bigint_copy(curr);

        bigint_free(&curr);

        curr = bigint_add(temp, two);

        bigint_free(&temp);
    }







    return EXIT_SUCCESS;
}

// Seach up on stack overflow what is wrong with the InelliSense

/*
int main()
{
    // bigint a = bigint_from_str("1024");

    // bigint b = bigint_from_str("1023");

    // bigint res = bigint_sub_temps(&a, &b);

    // bigint_println(res);

    // bigint_free_data(&res);

    bigint a = bigint_from_str("9090");
    bigint b = bigint_from_str("999"); // 9080910

    bigint res = bigint_mult(a, b);

    bigint_println(res);

    bigint_reassign(&a, "899910");
    bigint_reassign(&b, "08181000");

    res = bigint_add(a, b);

    bigint_println(res);

    return EXIT_SUCCESS;
}
*/