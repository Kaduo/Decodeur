/*******************************************************************************
Nom ......... : pixel.c
Role ........ : Fonctions de gestion de pixels
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdlib.h>
#import "pixel.h"

/* Cree un pixel suivant une luminance donnee */
union pixel *create_pixel_bw(uint8_t y)
{
    union pixel *pixel = malloc(sizeof(union pixel));
    pixel->y = y;
    return pixel;
}

/* Cree un pixel suivant des couleurs RGB donnes */
union pixel *create_pixel_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    union pixel *pixel = malloc(sizeof(union pixel));
    pixel->rgb->red = red;
    pixel->rgb->green = green;
    pixel->rgb->blue = blue;
    return pixel;
}

/* Libere de la memoire la place occupee par un pixel */
void free_pixel(union pixel *pixel)
{
    free(pixel);
    pixel = NULL;
}
