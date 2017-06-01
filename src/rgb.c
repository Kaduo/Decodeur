/*******************************************************************************
Nom ......... : rgb.c
Role ........ : Fonctions du format RGB
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdlib.h>
#import "rgb.h"

/* Recupere la composante R du RGB depuis un format YCbCr donne */
uint8_t get_red(int16_t y, int16_t cb, int16_t cr)
{
    return clamp(y - 0.0009267 * (cb - 128) + 1.4016868 * (cr - 128));
}

/* Recupere la composante G du RGB depuis un format YCbCr donne */
uint8_t get_green(int16_t y, int16_t cb, int16_t cr)
{
    return clamp(y - 0.3436954 * (cb - 128) - 0.7141690 * (cr - 128));
}

/* Recupere la composante B du RGB depuis un format YCbCr donne */
uint8_t get_blue(int16_t y, int16_t cb, int16_t cr)
{
    return clamp(y + 1.7721604 * (cb - 128) + 0.0009902 * (cr - 128));
}

/* Clampe une valeur color dans l'intervalle [0,255] */
uint8_t clamp(int16_t color)
{
    if (color >= 0 && color <= 255) {
        return (uint8_t) color;
    } else if (color < 0) {
        return 0;
    } else {
        return 255;
    }
}
