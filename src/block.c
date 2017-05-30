/*******************************************************************************
Nom ......... : block.c
Role ........ : Fonctions de reconstruction de blocs
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include "block.h"
#include "rgb.h"

/* Taille d'un bloc */
const size_t TAILLE_BLOC = 8;

/* Cree une structure block a partir d'une taille donne */
struct block *create_block()
{
    struct block *block = malloc(sizeof(struct block));
    for (uint8_t i = 0; i < COMP_NB; ++i) {
        block[i] = NULL;
    }
    return block;
}

/* Extrait les blocs d'une MCU */
extern struct block *extract_blocks(struct mcu *mcu, uint8_t **factors)
{
    uint8_t nb_blocks = factors[COMP_Y][DIR_H] * factors[COMP_Y][DIR_V];
    struct block **blocks = malloc(nb_blocks * sizeof(struct block*));
    /* Creation des blocs et initialisation des composantes Y */
    for (uint8_t i = 0; i < nb_blocks; ++i) {
        blocks[i] = create_block();
        (blocks[i])[COMP_Y] = mcu->components_y[i];
        (blocks[i])[COMP_CB] = mcu->components_cb[i];
        (blocks[i])[COMP_CR] = mcu->components_cr[i];
    }
    return blocks;
}

/*

int16_t *get_coefficients_area(const int16_t *component,
                                uint8_t x,
                                uint8_t y,
                                uint8_t taille_bloc)
{
    int16_t *coeff = calloc(taille_bloc * taille_bloc, sizeof(int16_t));
    for (uint8_t j = y; j < y + taille_bloc; ++j) {
        for (uint8_t i = x; i < x + taille_bloc; ++i) {
            coeff[j*taille_bloc+i] = component[j*taille_bloc+i];
        }
    }
    return coeff;
}

void upsampling(const int16_t *component, int16_t **coeff, size_t taille_bloc, uint8_t h1, uint8_t v1, uint8_t h, uint8_t v)
{
    if (h1 == h && v1 == v) {
        coeff = component;
    } else {
        size_t index = 0;
        size_t pas_y = taille_bloc * v / v1;
        size_t pas_x = taille_bloc * h / h1;
        if (h1 < v1) {
          for (size_t y = 0; y < taille_bloc; y += pas_y) {
              for (size_t x = 0; x < taille_bloc; x += pas_x) {
                  coeff[index] = get_coefficients_area(component, x, y, taille_bloc);
                  index++;
              }
          }
        } else {

        }
    }
}

*/

/*
void test(const struct component *component, int16_t *tab, uint8_t indice, uint8_t h1, uint8_t v1, uint8_t h, uint8_t v)
{
    if (h1 == h && v1 == v) {
        return component;
    } else if (v1 != v) {
        v *= 2;
        uint8_t nb_elements = h * v;

        test(component, tab, indice, h1, v1, h, v);
        test(component, tab, indice + h1*v1/nb_elements, h1, v1, h, v);
    } else {
        h *= 2;
        uint8_t nb_elements = h * v;

        test(component,tab,indice,h1,v1,h,v);
        test(component, tab, indice + h2*v2/nb_elements, h1, v1, h, v);
    }
}
*/

/* Sur-echantillonne un composant donne sous-echantillonne en deux */
int16_t **upsample_to_two(const struct component *component);

/* Sur-echantillonne un composant donne sous-echantillonne en quatre */
int16_t **upsample_to_four(const struct component *component);

/* Convertit un bloc YCbCr en bloc RGB */
void convert_to_rgb(struct block *block) {
    int16_t y, cb, cr;
    for (uint8_t i = 0; i < TAILLE_BLOC * TAILLE_BLOC; ++i) {
        y = block[i][COMP_Y];
        cb = block[i][COMP_Cb];
        cr = block[i][COMP_Cr];
        block[RGB_R][i] = get_red(y, cb, cr);
        block[RGB_B][i] = get_blue(y, cb, cr);
        block[RGB_G][i] = get_green(y, cb, cr);
    }
}

/* Libere de la memoire la place occupee par un bloc */
void free_block(struct block *block)
{
  free(block);
  block = NULL;
}
