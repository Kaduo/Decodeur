#include <stdlib.h>
#include <stdio.h>
#include "ppm.h"

extern struct block *upsampling(const struct mcu *mcu, uint8_t sampling_factors[3][2]){
    // ATTENTION BOUCHON
    if (sampling_factors[0][0] == 1){
        struct block *block = malloc(sizeof(struct block));
        for (size_t i = 0; i < mcu->nb_ys; i++) {
            block->y[i] = mcu->components_y[0][i];
        }
        return block;
    }

    return NULL;
}
