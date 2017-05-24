#ifndef __EXTRACTION_H__
#define __EXTRACTION_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "bitstream.h"
#include "jpeg_reader.h"

/* Structure représentant un MCU */
struct mcu {
  int16_t **components_y;
  int16_t **components_cb;
  int16_t **components_cr;
  uint8_t nb_ys;
  uint8_t nb_cbs;
  uint8_t nb_crs;
};

extern struct mcu *create_mcu(uint8_t nb_components_y,
                                uint8_t nb_components_cb,
                                uint8_t nb_components_cr);

void free_mcu(struct mcu *mcu);

/* Extrait un MCU d'un bitstream et d'une description donnés */
extern struct mcu *extract_mcu(const struct bitstream *bitstream,
                                uint8_t nb_components_y,
                                uint8_t nb_components_cb,
                                uint8_t nb_components_cr,
                                const struct jpeg_desc *jpeg);

/* Extrait un type de composante d'un bitstream donné */
void extract_component(const struct bitstream *bitstream,
                                    const struct huff_table *huff_dc,
                                    const struct huff_table *huff_ac,
                                    int16_t previous_dc,
                                    int16_t *component);

/* Extrait DC */
extern int16_t extract_dc(const struct bitstream *bitstream, const struct huff_table *huff);

/* Extrait AC */
extern int16_t *extract_ac(const struct bitstream *bitstream, const struct huff_table *huff);

/*Calcule les DCs d'origine*/
extern void initial_DCs(struct mcu *mcus, size_t size);


#endif
