/*******************************************************************************
Nom ......... : upsampling.h
Role ........ : Prototypes des fonctions de surechantillonnage
Auteurs .... : A. He - M. Nebra - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __UPSAMPLING_H__
#define __UPSAMPLING_H__

#include <stdlib.h>
#include <stdint.h>
#include "block.h"

/* Sur-echantillonne un composant a partir d'un composant sous-echantillonne
donne et des facteurs h et v (horizontal / vertical) */
extern struct block *upsample(const struct component *component,
                                size_t h_factor,
                                size_t v_factor);

/* Separe en deux (gauche / droite) les coefficients d'un bloc donne */
extern int16_t **split_in_two(const struct block *block);

/* Separe en quatre les coefficients d'un bloc donne */
extern int16_t **split_in_four(const struct block *block);

/* Replique horizontalement des coefficients donnes */
extern int16_t *horizontal_replication(const int16_t *coefficients,
                                        size_t width,
                                        size_t height);

/* Replique horizontalement et verticalement des coefficients donnes */
extern int16_t *two_dimension_replication(const int16_t *coefficients,
                                            size_t size);

#endif
