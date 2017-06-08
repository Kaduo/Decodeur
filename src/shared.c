/*******************************************************************************
Nom ......... : shared.c
Role ........ : Variables globales partagees entre modules
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include "shared.h"

/* Taille d'un bloc */
const size_t BLOCK_SIZE = 8;

/* Taille d'une composante */
const size_t COMPONENT_SIZE = BLOCK_SIZE * BLOCK_SIZE;
