#ifndef __RGB_H__
#define __RGB_H__

#include <stdint.h>
#include <stdbool.h>
#include "bitstream.h"
#include "extraction.h"

/* RGB dans un uint32_t:
0x00RRGGBB
*/
/*Convertit un bloc YBR en bloc RGB.*/
extern uint32_t *block_ybr_to_rgb(const struct block *block);


// new_image with calloc int32_t * size
uint32_t *new_image(size_t n_pixel);

void print_image(uint32_t *image, size_t n_pixel);
void print_pixel(uint32_t pixel);

		// set_color_rgb : Set color thanks three parameters, Red, Green and Blue.
void set_color_rgb(uint32_t *pixel, uint8_t r, uint8_t g, uint8_t b);

// set_color_code : Set color for one pixel with hexa code : 0x00RRGGBB.
void set_color_code(uint32_t *pixel, uint32_t color);

void set_blue(uint32_t *pixel, uint8_t value);
void set_green(uint32_t *pixel, uint8_t value);
void set_red(uint32_t *pixel, uint8_t value);

uint8_t get_blue(uint32_t pixel);
uint8_t get_green(uint32_t pixel);
uint8_t get_red(uint32_t pixel);


#endif
