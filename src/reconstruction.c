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

float16_t C(int16_t ksi){
    if (ksi == 0) {
        return 1/sqrt(2);
    }
    return 0
}

int8_t *idct(int16_t *component){
    float16_t coefficients[63];
    float16_t resultat_float[63];
    for (size_t lambda = 0; lambda < 8; lambda++) {
        for (size_t mu = 0; mu < 8; mu++) {
            coefficients[mu*8 + lambda] = C(mu)*C(lambda)*component[mu*8 + lambda];
        }
    }

    for (size_t x = 0; x < 8; x++) {
        for (size_t y = 0; y < 8; y++) {
            for (size_t lambda = 0; lambda < 8; lambda++) {
                for (size_t mu = 0; mu < 8; mu++) {
                    resultat_float[y*8 + x] = 1;
                }
            }
        }
    }
    return resultat_float;
}

void reconstruct_mcu(struct mcu *mcu, const struct jpeg_desc *jpeg){
    /*bouchon*/
    return;
}
