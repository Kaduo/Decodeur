/*******************************************************************************
Nom ......... : format.h
Role ........ : Prototypes des fonctions de formats d'image
Auteurs .... : A. He - M. Nebra - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __FORMAT_H__
#define __FORMAT_H__

#include <stdint.h>

/* Structure representant le format YCbCr */
struct ycbcr {
    uint8_t y;
    uint8_t cb;
    uint8_t cr;
};

/* Structure representant le format RGB */
struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

/* Convertit une representation YCbCr en RGB */
extern struct rgb *to_rgb(struct ycbcr *ycbcr);

/* Recupere la composante R du RGB depuis un format YCbCr donne */
extern uint8_t to_red(struct ycbcr *ycbcr);

/* Recupere la composante G du RGB depuis un format YCbCr donne */
extern uint8_t to_green(struct ycbcr *ycbcr);

/* Recupere la composante B du RGB depuis un format YCbCr donne */
extern uint8_t to_blue(struct ycbcr *ycbcr);

#endif
