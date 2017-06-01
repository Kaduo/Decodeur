#ifndef __JPEG2PPM_H__
#define __JPEG2PPM_H__

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "jpeg_reader.h"
#include "bitstream.h"

#include "rgb.h"
#include "picture.h"

bool est_couleur(const struct jpeg_desc *jpeg);

extern struct huff_table ***get_huff_tables(const struct jpeg_desc *jpeg);

uint8_t **get_quant_tables(const struct jpeg_desc *jpeg);

enum component *get_components_order(const struct jpeg_desc *jpeg, uint8_t factors[COMP_NB][DIR_NB]);


#endif
