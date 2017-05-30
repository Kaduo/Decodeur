/*******************************************************************************
Nom ......... : rgb.c
Role ........ : Fonctions du format RGB
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdlib.h>
#import "rgb.h"

/* Cree un pixel RGB */
struct pixel *create_pixel(uint8_t red, uint8_t green, uint8_t blue)
{
    struct pixel *pixel = malloc(sizeof(struct pixel));
    pixel->red = red;
    pixel->green = green;
    pixel->blue = blue;
    return pixel;
}


/* Recupere la composante R du RGB depuis un format YCbCr donne */
uint8_t get_red(int16_t y, int16_t cb, int16_t cr)
{
    return y - 0.0009267 * (cb - 128) + 1.4016868 * (cr - 128);
}

/* Recupere la composante G du RGB depuis un format YCbCr donne */
uint8_t get_green(int16_t y, int16_t cb, int16_t cr)
{
    return y - 0.3436954 * (cb - 128) - 0.7141690 * (cr - 128);
}

/* Recupere la composante B du RGB depuis un format YCbCr donne */
uint8_t get_blue(int16_t y, int16_t cb, int16_t cr)
{
    return y + 1.7721604 * (cb - 128) + 0.0009902 * (cr - 128);
}

/* Libere de la memoire la place occupee par un pixel RBG */
void free_pixel(struct pixel *pixel)
{
    free(pixel);
    pixel = NULL;
}
