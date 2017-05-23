#ifndef __EXTRACTION_H__
#define __EXTRACTION_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "bitstream.h"
#include "jpeg_reader.h"

/* Structure représentant un MCU */
struct mcu {
  int16_t **components;
};

extern struct mcu *create_mcu(uint8_t nb_components);

/* Extrait un MCU d'un bitstream et d'une description donnés */
extern struct mcu *extract_mcu(const struct bitstream *bitstream, const uint8_t nb_components, const struct jpeg_desc *jpeg);

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

/*Calcule les DCs d'origine*/
extern void initial_DCs(struct mcu *mcus, size_t size);

/* Libère un MCU */
extern void free_mcu(const struct mcu *mcu);

#endif
