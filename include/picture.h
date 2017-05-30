/*******************************************************************************
Nom ......... : picture.h
Role ........ : Prototypes des fonctions de gestion d'image
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __PICTURE_H__
#define __PICTURE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "pixel.h"
#include "block.h"

/* Structure reprensant une image RGB */
struct picture {
    size_t width;
    size_t height;
    bool colored;
    union pixel **pixels;
};

/* Cree une image a partir de dimensions donnes */
extern struct picture *create_picture(size_t width, size_t height, bool colored);

// Convertir l'image sous forme de bloc rangé en MCU en tableau de pixel.
struct picture *blocks2pixels(
    block *blocks,
    size_t width,
    size_t height,
    size_t width_ext,
    size_t height_ext,
    uint8_t h1,
    uint8_t v1);


/* Libere en memoire l'espace occupe par une image donnee */
extern void free_picture(struct picture *picture);

#endif
