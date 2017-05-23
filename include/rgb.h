#ifndef __RGB_H__
#define __RGB_H__

#include <stdint.h>
#include <stdbool.h>
#include "bitstream.h"
#include "extraction.h"

struct pixel_rgb {
  uint8_t r;
  uint8_t g;
  uint8_t b;
}

/*Convertit un bloc en RGB.*/
struct pixel_rgb *YCbCr_to_RGB(const struct block *block);

/*Rassemble des tableaux RGBs en une image complète.*/
struct pixel_rgb *reconstruct_image(const struct **pixel_rgb pixels, size_t width, size_t height);

#endif
