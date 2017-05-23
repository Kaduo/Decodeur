#include <stdlib.h>
#include <stdio.h>
#include "reconstruction.h"

extern void inverse_quant(int16_t *component, uint8_t *quant_table){
    for (size_t i = 0; i < sizeof(component)/sizeof(int16_t); i++) {
        component[i] = component[i]*quant_table[i];
    }
}

extern void zag_zig(int16_t *component){
    /*bouchon*/
    return;
}

extern void idct(int16_t *component){
    /*bouchon*/
    return;
}

extern void reconstruct_mcu(struct mcu *mcu, const struct jpeg_desc *jpeg){
    /*bouchon*/
    return;
}
