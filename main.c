#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "bit_copy.h"

static uint8_t output[8], input[8];

int main()
{
    memset(&input[0], 0xAF, sizeof(input));

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
                bitcpy(&output[0], k, &input[0], j, i);
#endif
                dump_binary(&output[0], 8);
                printf("\n");
            }
        }
    }

    return 0;
}
