/*******************************************************************************
Nom ......... : decodage.h
Role ........ : Prototypes des fonctions de decodage de MCUs
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __DECODAGE_H__
#define __DECODAGE_H__

#include "jpeg_reader.h"
#include "block.h"
#include "upsampling.h"
#include "rgb.h"

struct mcu {
    size_t width;
    size_t height;
    struct block *blocks[COMP_NB];
};

/* Retourne chaque blocs du MCU */
extern struct block *block get_blocks();

extern struct mcu *mcu get_mcus();
extern struct picture *picture get_picture();

#endif
