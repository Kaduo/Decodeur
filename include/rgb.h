/*******************************************************************************
Nom ......... : rgb.h
Role ........ : Prototypes des fonctions du format RGB
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __RGB_H__
#define __RGB_H__

#include <stdint.h>

/* Structure representant un pixel RGB */
struct pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

/* Composantes RGB */
enum rgb {
    RGB_R,
    RGB_G,
    RGB_B
};


/* Cree un pixel RGB */
extern struct pixel *create_pixel(uint8_t red, uint8_t green, uint8_t blue);

/* Recupere la composante R du RGB depuis un format YCbCr donne */
extern uint8_t get_red(int16_t y, int16_t cb, int16_t cr);

/* Recupere la composante G du RGB depuis un format YCbCr donne */
extern uint8_t get_green(int16_t y, int16_t cb, int16_t cr);

/* Recupere la composante B du RGB depuis un format YCbCr donne */
extern uint8_t get_blue(int16_t y, int16_t cb, int16_t cr);

/* Libere de la memoire la place occupee par un pixel RBG */
extern void free_pixel(struct pixel *pixel);

#endif
