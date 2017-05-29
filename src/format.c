/*******************************************************************************
Nom ......... : format.c
Role ........ : Fonctions de formats d'image
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdlib.h>
#import "format.h"

/* Convertit une representation YCbCr en RGB */
struct rgb *to_rgb(struct ycbcr *ycbcr)
{
    struct rgb *rgb = malloc(sizeof(rgb));
    rgb->r = to_red(ycbcr);
    rgb->g = to_green(ycbcr);
    rgb->b = to_blue(ycbcr);
    return rgb;
}


/* Recupere la composante R du RGB depuis un format YCbCr donne */
uint8_t to_red(struct ycbcr *ycbcr)
{
    return ycbcr->y
            - 0.0009267
            * (ycbcr->cb - 128)
            + 1.4016868
            * (ycbcr->cr - 128);
}

/* Recupere la composante G du RGB depuis un format YCbCr donne */
uint8_t to_green(struct ycbcr *ycbcr)
{
    return ycbcr->y
            - 0.3436954
            * (ycbcr->cb - 128)
            - 0.7141690
            * (ycbcr->cr - 128);
}

/* Recupere la composante B du RGB depuis un format YCbCr donne */
uint8_t to_blue(struct ycbcr *ycbcr)
{
    return ycbcr->y
            + 1.7721604
            * (ycbcr->cb - 128)
            + 0.0009902
            * (ycbcr->cr - 128);
}
