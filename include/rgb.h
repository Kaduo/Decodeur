/*******************************************************************************
Nom ......... : rgb.h
Role ........ : Prototypes des fonctions du format RGB
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __RGB_H__
#define __RGB_H__

#include <stdint.h>

struct rgb {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

/* Composantes RGB */
enum rgb_component {
    RGB_R,
    RGB_G,
    RGB_B
};

/* Recupere la composante R du RGB depuis un format YCbCr donne */
extern uint8_t get_red(int16_t y, int16_t cb, int16_t cr);

/* Recupere la composante G du RGB depuis un format YCbCr donne */
extern uint8_t get_green(int16_t y, int16_t cb, int16_t cr);

/* Recupere la composante B du RGB depuis un format YCbCr donne */
extern uint8_t get_blue(int16_t y, int16_t cb, int16_t cr);

#endif
