#ifndef __RECONSTRUCTION_H__
#define __RECONSTRUCTION_H__

#include <stdint.h>
#include <stdbool.h>
#include "bitstream.h"
#include "extraction.h"


/* Quantification inverse : multiplie la composante par la table de quantification donnée */
extern void inverse_quant(int16_t *component, uint8_t *quant_table);

/* Zag zig la composante */
extern void zag_zig(int16_t *component);

extern float16_t C(int16_t ksi);

/* Applique la transformée en cosinus discrète inverse sur la composante */
extern void idct(int16_t *component);

/* Transforme un MCU fréquentiel en spatial */
extern void reconstruct_mcu(struct mcu *mcu, const struct jpeg_desc *jpeg);

#endif
