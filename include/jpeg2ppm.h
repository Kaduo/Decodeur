#ifndef __JPEG2PPM_H__
#define __JPEG2PPM_H__

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "jpeg_reader.h"
#include "bitstream.h"

#include "ppm.h"
#include "rgb.h"

bool est_couleur(const struct jpeg_desc *jpeg);

struct huff_table ***get_huff_tables(const struct jpeg_reader *jpeg);

uint8_t **get_quant_tables(const struct jpeg_reader *jpeg);

#endif
