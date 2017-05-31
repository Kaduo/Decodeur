/*******************************************************************************
Nom ......... : block.c
Role ........ : Fonctions de reconstruction de blocs
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdio.h>
#include "block.h"
#include "rgb.h"
#include "shared.h"

/* Cree une structure block a partir d'une taille donne */
block create_block()
{
    block bloc = malloc(COMP_NB * sizeof(int16_t *));
    for (uint8_t i = 0; i < COMP_NB; ++i) {
        bloc[i] = NULL;
    }
    return bloc;
}

/* Extrait les blocs d'une MCU */
extern block *extract_blocks(struct mcu *mcu, uint8_t factors[COMP_NB][DIR_NB])
{
    uint8_t nb_blocks = factors[COMP_Y][DIR_H] * factors[COMP_Y][DIR_V];
    block *blocks = malloc(nb_blocks * sizeof(block));
    /* Creation des blocs et initialisation des composantes Y */
    for (uint8_t i = 0; i < nb_blocks; ++i) {
        blocks[i] = create_block();
        blocks[i][COMP_Y] = mcu->components_y[i];

        if (mcu->components_cb != NULL) {
            // À modifier !!
            blocks[i][COMP_Cb] = mcu->components_cb[i];
            blocks[i][COMP_Cr] = mcu->components_cr[i];
        }
    }
    return blocks;
}

/*

int16_t *get_coefficients_area(const int16_t *component,
                                uint8_t x,
                                uint8_t y,
                                uint8_t BLOCK_SIZE)
{
    int16_t *coeff = calloc(BLOCK_SIZE * BLOCK_SIZE, sizeof(int16_t));
    for (uint8_t j = y; j < y + BLOCK_SIZE; ++j) {
        for (uint8_t i = x; i < x + BLOCK_SIZE; ++i) {
            coeff[j*BLOCK_SIZE+i] = component[j*BLOCK_SIZE+i];
        }
    }
    return coeff;
}

void upsampling(const int16_t *component, int16_t **coeff, size_t BLOCK_SIZE, uint8_t h1, uint8_t v1, uint8_t h, uint8_t v)
{
    if (h1 == h && v1 == v) {
        coeff = component;
    } else {
        size_t index = 0;
        size_t pas_y = BLOCK_SIZE * v / v1;
        size_t pas_x = BLOCK_SIZE * h / h1;
        if (h1 < v1) {
          for (size_t y = 0; y < BLOCK_SIZE; y += pas_y) {
              for (size_t x = 0; x < BLOCK_SIZE; x += pas_x) {
                  coeff[index] = get_coefficients_area(component, x, y, BLOCK_SIZE);
                  index++;
              }
          }
        } else {

        }
    }
}

*/

/*
void upsampling_recursif(const struct component *component, int16_t *tab, uint8_t indice, uint8_t h1, uint8_t v1, uint8_t h, uint8_t v)
{
    if (h1 == h && v1 == v) {
        return component;
    } else if (v1 != v) {
        v *= 2;
        uint8_t nb_elements = h * v;

        upsampling_recursif(component, tab, indice, h1, v1, h, v);
        upsampling_recursif(component, tab, indice + h1*v1/nb_elements, h1, v1, h, v);
    } else {
        h *= 2;
        uint8_t nb_elements = h * v;

        upsampling_recursif(component,tab,indice,h1,v1,h,v);
        upsampling_recursif(component, tab, indice + h2*v2/nb_elements, h1, v1, h, v);
    }
}
*/

/* Sur-echantillonne un composant donne sous-echantillonne en deux */
// int16_t **upsample_to_two(const struct component *component);

/* Sur-echantillonne un composant donne sous-echantillonne en quatre */
// int16_t **upsample_to_four(const struct component *component);

/* Convertit un bloc YCbCr en bloc RGB */
void convert_to_rgb(block block) {
    int16_t y, cb, cr;
    for (uint8_t i = 0; i < COMPONENT_SIZE; ++i) {
        y = block[i][COMP_Y];
        cb = block[i][COMP_Cb];
        cr = block[i][COMP_Cr];
        block[RGB_R][i] = get_red(y, cb, cr);
        block[RGB_B][i] = get_blue(y, cb, cr);
        block[RGB_G][i] = get_green(y, cb, cr);
    }
}

/* Libere de la memoire la place occupee par un bloc */
void free_block(block block)
{
  free(block);
  block = NULL;
}
