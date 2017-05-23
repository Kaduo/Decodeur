#ifndef __PPM_H__
#define __PPM_H__

#include <stdint.h>
#include <stdbool.h>
#include "bitstream.h"

/*Convertit un tableau de pixels en un ppm.*/
void pixels_to_ppm(const struct *pixel_rgb pixels);

#endif
