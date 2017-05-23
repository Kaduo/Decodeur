#include "rgb.h"

uint32_t *block_ybr_to_rgb(const struct block *block){
uint32_t block_rgb[64] = {0};
 return block_rgb;
}
uint32_t *new_image(size_t n_pixel) {
	uint32_t *image = calloc(n_pixel, sizeof(uint32_t));
	return image;
} // end noexw_image

void print_image(uint32_t *image, size_t n_pixel) {
	printf("Image address : 0x%p\nNumber of pixels : %zu\n\n", image, n_pixel);
	printf("|| Id | Red | Green | Blue ||\n");
	for (size_t i=0; i< n_pixel; i++) {
		printf("%zu : ", i);
		print_pixel(image[i]);
	} // end for
} // end print_image

void print_pixel(uint32_t pixel) {
	printf("%u ", get_red(pixel));
	printf("%u ", get_green(pixel));
	printf("%u\n", get_blue(pixel));
} // end print_pixel

void set_color_rgb(uint32_t *pixel, uint8_t r, uint8_t g, uint8_t b) {
	set_blue(pixel, b);
	set_green(pixel, g);
	set_red(pixel, r);
	// end set_color_rgb
}

void set_color_code(uint32_t *pixel, uint32_t color) {
	*pixel = color;
} // end set_color_code

void set_blue(uint32_t *pixel, uint8_t value) {
	*pixel = *pixel & 0x00FFFF00; // Set to 0, blue color in pixel.
	*pixel = *pixel | value; // OU logic, pixel and value.
			// end set_blue
}

void set_green(uint32_t *pixel, uint8_t value) {
	*pixel = *pixel & 0xFF00FF; // Set to 0, green color in pixel.
	*pixel = *pixel | (value << 8); // OU logic, pixel and value.
} // end set_green

void set_red(uint32_t *pixel, uint8_t value) {
	*pixel = *pixel & 0x0000FFFF; // Set to 0, red color in pixel.
	*pixel = *pixel | (value << 16); // OU logic, pixel and value.
			// end set_red
}

uint8_t get_blue(uint32_t pixel) {
	pixel = pixel & 0x0000FF;
	return (uint8_t) pixel;
}

uint8_t get_green(uint32_t pixel) {
	pixel = pixel & 0x00FF00;
	return (uint8_t) (pixel >> 8);
} // end get_green

uint8_t get_red(uint32_t pixel) {
	pixel = pixel & 0xFF0000;
	return (uint8_t) (pixel >> 16);
} // end get_red

// end image.c
