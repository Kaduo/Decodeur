/*******************************************************************************
Nom ......... : pixel.h
Role ........ : Fonctions du format RGB
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdlib.h>
#import "rgb.h"

/* Union representant un pixel (soit suivant une luminance, soit RGB) */
union pixel {
    uint8_t y;
    struct rgb *rgb;
};

/* Cree un pixel suivant une luminance donnee */
extern union pixel *create_pixel_bw(uint8_t y);

/* Cree un pixel suivant des couleurs RGB donnes */
extern union pixel *create_pixel_rgb(uint8_t r, uint8_t g, uint8_t b);

/* Libere la place en memoire occupee par un pixel */
extern void free_pixel(union pixel *pixel);
