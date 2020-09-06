#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "bit_copy.h"


void perf()
{
    static uint8_t output[8], input[8];

    memset(&input[0], 0xAF, sizeof(input));
    memset(&output[0], 0x00, sizeof(output));

    for (int i = 1; i <= 32; ++i) {
        for (int j = 0; j < 16; ++j) {
            for (int k = 0; k < 16; ++k) {
                // memset(&output[0], 0x00, sizeof(output));
                printf("\nCopy from src %d to dest %d cnt = %d\n", j, k, i);
                printf("src :    ");
                dump_binary(&input[0], 8);
                printf("\ndest:    ");
                dump_binary(&output[0], 8);
                printf("\n");

                printf("%2d:%2d:%2d ", i, k, j);
#ifdef ORI
                bitcpy_ori(&output[0], k, &input[0], j, i);
#else
#ifdef NAIVE
                bitcpy_naive(&output[0], k, &input[0], j, i);
#else

                bitcpy(&output[0], k, &input[0], j, i);
#endif
#endif
                dump_binary(&output[0], 8);
                printf("\n");
            }
        }
    }
}


void cal_time()
{
    static uint8_t output[1000], input[1000];

    memset(&input[0], 0x71, sizeof(input));
    for (int size = 8; size < 7000; size += 8) {
        // old
        memset(&output[0], 0x00, sizeof(output));

        struct timespec tt1, tt2;
        clock_gettime(CLOCK_MONOTONIC, &tt1);
        bitcpy_naive(&output[0], 2, &input[0], 5, size);
        clock_gettime(CLOCK_MONOTONIC, &tt2);

        long long old = (long long) (tt2.tv_sec * 1e9 + tt2.tv_nsec) -
                        (long long) (tt1.tv_sec * 1e9 + tt1.tv_nsec);

        // new
        memset(&output[0], 0x00, sizeof(output));
        clock_gettime(CLOCK_MONOTONIC, &tt1);
        bitcpy(&output[0], 2, &input[0], 5, size);
        clock_gettime(CLOCK_MONOTONIC, &tt2);

        long long new = (long long) (tt2.tv_sec * 1e9 + tt2.tv_nsec) -
                        (long long) (tt1.tv_sec * 1e9 + tt1.tv_nsec);

        printf("%d, %lld, %lld\n", size, old, new);
    }
}

int main()
{
    // perf();
    cal_time();
    return 0;
}
