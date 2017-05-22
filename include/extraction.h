#ifndef __EXTRACTION_H__
#define __EXTRACTION_H__

#include <stdint.h>
#include <stdbool.h>
#include "bitstream.h"

/* Structure représentant un MCU */
struct mcu {
  /* Vecteurs de la composante Y */
  int16_t **y;
  /* Vecteurs de la composante Cb */
  int16_t **cb;
   /* Vecteurs de la composante Cr */
  int16_t **cr;
};

/* Extrait un MCU d'un bitstream et d'une description donnés */
extern struct *mcu extract_mcu(const struct bitstream *bitstream, const struct jpeg_desc *jpeg);

/* Extrait un type de composante d'un bitstream donné */
extern int16_t *extract_component(const struct bitstream *bitstream,
                                    const struct huff_table *y_dc,
                                    const struct huff_table *y_ac,
                                    const struct huff_table *c_dc,
                                    const struct huff_table *c_ac,
                                    enum component comp);

/* Extrait DC */
extern int16_t extract_dc(const struct bitstream *bitstream, const struct huff_table *huff);

/* Extrait AC */
extern int16_t *extract_ac(const struct bitstream *bitstream, const struct huff_table *huff);

/* Libère un MCU */
extern void free_mcu(const struct mcu *mcu);

#endif
