#include <stdlib.h>
#include <stdio.h>
#include "reconstruction.h"

void inverse_quant(int16_t *component, uint8_t *quant_table){
    /*bouchon*/
    return;
}

void zag_zig(int16_t *component){
    /*bouchon*/
    return;
}

void idct(int16_t *component){
    int16_t coefficients[63];
    for (size_t lambda = 0; lambda < 8; lambda++) {
        for (size_t mu = 0; mu < 8; mu++) {
            coefficients[mu*8 + lambda] =
        }
    }
    return;
}

void reconstruct_mcu(struct mcu *mcu, const struct jpeg_desc *jpeg){
    /*bouchon*/
    return;
}
