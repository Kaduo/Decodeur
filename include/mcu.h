/*******************************************************************************
Nom ......... : mcu.h
Role ........ : Prototypes des fonctions de la structure MCU
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __MCU_H__
#define __MCU_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "bitstream.h"
#include "jpeg_reader.h"
#include "huffman.h"
#include "component.h"

/* Structure représentant un MCU */
struct mcu {
  int16_t **components_y;
  int16_t **components_cb;
  int16_t **components_cr;
  uint8_t nb_ys;
  uint8_t nb_cbs;
  uint8_t nb_crs;
};

/* Crée une structure MCU */
struct mcu *create_mcu(uint8_t nb_components_y, uint8_t nb_components_cb, uint8_t nb_components_cr);

/* Extrait une MCU complète. 'ordre_des_composantes' est un tableau indiquant
l'ordre des composantes dans la MCU. 'huff_tables' est un tableau de couples de
tables de Huffman, de forme : [[Y_dc, Y_ac], [C_dc, C_ac]] */
extern struct mcu *extract_mcu(struct bitstream *bitstream,
                        uint8_t nb_components_y,
                        uint8_t nb_components_cb,
                        uint8_t nb_components_cr,
                        int16_t *previous_dc_y,
                        int16_t *previous_dc_cb,
                        int16_t *previous_dc_cr,
                        enum component *ordre_des_composantes,
                        struct huff_table ***huff_tables,
                        uint8_t **quant_tables);

/* Libère la place occupée en mémoire par une MCU */
extern void free_mcu(struct mcu *mcu);

#endif
