#include <stdio.h>
#include <stdlib.h>

// #include <time.h>

#define CONST_1_3 12297829382473034411ULL

    // __uint128_t

long long child(long long a, long long n)
{
    if (n % 3 == 0)
        return -1; // ?

    return ((a << ((n << 1) - a % 3 + 1)) - 1) / 3;
}

int main()
{
    // child(a, 2);

    // printf("%d\n", 0 / 0);





    return EXIT_SUCCESS;
}