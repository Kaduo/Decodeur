/*******************************************************************************
Nom ......... : ycbcr.h
Role ........ : Prototypes des fonctions du format YCbCr
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __YCBCR_H__
#define __YCBCR_H__

/* Structure representant le format YCbCr */
struct ycbcr {
    uint8_t y;
    uint8_t cb;
    uint8_t cr;
};

/* Calcule la conversion vers la composante R du format RGB */
extern uint8_t get_red(const struct ycbcr *ycbcr);

/* Calcule la conversion vers la composante G du format RGB */
extern uint8_t get_blue(const struct ycbcr *ycbcr);

/* Calcule la conversion vers la composante B du format RGB */
extern uint8_t get_green(const struct ycbcr *ycbcr);

#endif
