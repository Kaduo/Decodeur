/*******************************************************************************
Nom ......... : block.h
Role ........ : Prototypes des fonctions de reconstruction de blocs
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <stdlib.h>
#include <stdint.h>
#include "mcu.h"

/* Structure representant un bloc */

typedef int16_t **block;

/* Cree une structure block vide */
extern block create_block();

/* Extrait les blocs d'une MCU */
extern block *extract_blocks(struct mcu *mcu, uint8_t **factors);

/* Sur-echantillonne un composant donne sous-echantillonne en deux */
// extern int16_t **upsample_to_two(const struct component *component);

/* Sur-echantillonne un composant donne sous-echantillonne en quatre */
// extern int16_t **upsample_to_four(const struct component *component);

/* Convertit un bloc YCbCr en bloc RGB */
void convert_to_rgb(block block);

/* Libere de la memoire la place occupee par un bloc */
extern void free_block(block block);

#endif
