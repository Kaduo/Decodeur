#ifndef __MCU_H__
#define __MCU_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "bitstream.h"
#include "jpeg_reader.h"
#include "huffman.h"

/* Structure représentant un MCU */
struct mcu {
  int16_t **components_y;
  int16_t **components_cb;
  int16_t **components_cr;
  uint8_t nb_ys;
  uint8_t nb_cbs;
  uint8_t nb_crs;
};

struct mcu *create_mcu(uint8_t nb_components_y, uint8_t nb_components_cb, uint8_t nb_components_cr);

extern struct mcu *extract_mcu(struct bitstream *bitstream,
                        uint8_t nb_components_y,
                        uint8_t nb_components_cb,
                        uint8_t nb_components_cr,
                        enum component *ordre_des_composantes,
                        struct huff_table ***huff_tables,
                        uint8_t **quant_tables);

void free_components(int16_t **components, uint8_t size_components);

void free_mcu(struct mcu *mcu);

#endif
