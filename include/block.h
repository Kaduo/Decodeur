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
#include "jpeg_reader.h"

/* Définition d'un bloc */
typedef int16_t** block;

/* Crée une structure block vide */
extern block create_block();

/* Extrait les blocs d'une MCU */
extern block *extract_blocks(struct mcu *mcu,
                                const uint8_t factors[COMP_NB][DIR_NB]);

/* Sur-échantillonne le bloc à l'indice donné */
void upsampling_recursif(block *blocks,
                            enum component comp,
                            uint8_t indice,
                            uint8_t h1,
                            uint8_t v1,
                            uint8_t h,
                            uint8_t v);

/* Sur-échantillonne horizontalement le bloc à l'indice donné */
void upsample_horizontal(block *blocks,
                            enum component comp,
                            uint8_t indice,
                            uint8_t indice_cible);

/* Sur-échantillonne verticalement le bloc a l'indice donné */
void upsample_vertical(block *blocks,
                            enum component comp,
                            uint8_t indice,
                            uint8_t indice_cible);

/* Convertit un bloc YCbCr en bloc RGB */
void convert_to_rgb(block block);

/* Libère de la mémoire la place occupée par un bloc */
extern void free_block(block block, bool is_color);

#endif
