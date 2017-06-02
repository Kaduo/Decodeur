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
extern block *extract_blocks(struct mcu *mcu, const uint8_t factors[COMP_NB][DIR_NB])
{
    uint8_t nb_blocks = factors[COMP_Y][DIR_H] * factors[COMP_Y][DIR_V];
    block *blocks = malloc(nb_blocks * sizeof(block));

    bool no_upsampling_cb = (factors[COMP_Cb][DIR_H] == factors[COMP_Y][DIR_H] && factors[COMP_Cb][DIR_V] == factors[COMP_Y][DIR_V]);
    bool no_upsampling_cr = (factors[COMP_Cr][DIR_H] == factors[COMP_Y][DIR_H] && factors[COMP_Cr][DIR_V] == factors[COMP_Y][DIR_V]);

    /* Creation des blocs et initialisation des composantes Y */
    for (uint8_t i = 0; i < nb_blocks; ++i) {
        blocks[i] = create_block();
        blocks[i][COMP_Y] = mcu->components_y[i];
    }

    if (mcu->components_cb != NULL) {

        if (no_upsampling_cb) {
            for (size_t i = 0; i < nb_blocks; i++) {
                blocks[i][COMP_Cb] = mcu->components_cb[i];
            }
        }
        else {
            blocks[0][COMP_Cb] = mcu->components_cb[0];
            upsampling_recursif(blocks, COMP_Cb, 0, factors[COMP_Y][DIR_H], factors[COMP_Y][DIR_V], factors[COMP_Cb][DIR_H], factors[COMP_Cb][DIR_V]);
        }

        if (no_upsampling_cr) {
            for (size_t i = 0; i < nb_blocks; i++) {
                blocks[i][COMP_Cr] = mcu->components_cr[i];
            }
        }
        else {
            blocks[0][COMP_Cr] = mcu->components_cr[0];
            upsampling_recursif(blocks, COMP_Cr, 0, factors[COMP_Y][DIR_H], factors[COMP_Y][DIR_V], factors[COMP_Cr][DIR_H], factors[COMP_Cr][DIR_V]);
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


void upsampling_recursif(block *blocks, enum component comp, uint8_t indice, uint8_t h1, uint8_t v1, uint8_t h, uint8_t v)
{

    if (h1 == h && v1 == v) {
        return;

    } else if (v1 != v) {
        v *= 2;
        uint8_t nb_elements = h * v;

        upsample_vertical(blocks, comp, indice, indice + h1*v1/nb_elements);
        upsampling_recursif(blocks, comp, indice, h1, v1, h, v);
        upsampling_recursif(blocks, comp, indice + h1*v1/nb_elements, h1, v1, h, v);

    } else {
        printf("\n\nWHAT /§§§§ §§§§SDF ALERT ROUGES MLJDGKJGKJSKDHJDSG\n\n\n");
        h *= 2;
        uint8_t nb_elements = h * v;

        //upsample_vertical(blocks, comp, indice, indice + h1*v1/nb_elements);
        upsampling_recursif(blocks, comp, indice,h1,v1,h,v);
        upsampling_recursif(blocks, comp, indice + h1*v1/nb_elements, h1, v1, h, v);
    }
}

void upsample_vertical(block *blocks, enum component comp, uint8_t indice, uint8_t indice_cible)
{
    printf("\ncible : %d\n", indice_cible);
    printf("indice : %d\n", indice);
    if (blocks[indice][comp] == NULL) {
        perror("Impossible de diviser une composante inexistante !");
        exit(EXIT_FAILURE);
    }

    if (blocks[indice_cible][comp] != NULL) {
        perror("RAAAAAAAAAAAAAAH !!");
        exit(EXIT_FAILURE);
    }

    blocks[indice_cible][comp] = calloc(64, sizeof(int16_t));

    for (size_t i = 0; i < 64; i+=2) {
        // On complète la deuxième composante (celle de droite)
        blocks[indice_cible][comp][i] = blocks[indice][comp][4 + 8*(i/8) + (i%8)/2];
        blocks[indice_cible][comp][i+1] = blocks[indice_cible][comp][i];
    }
    for (size_t i = 0; i < 64; i+=2) {
        // On complète la première composante en place (celle de gauche)
        for (size_t i = 63; i > 1; i-=2) {
            blocks[indice][comp][i] = blocks[indice][comp][8*(i/8) + (i%8)/2];
            blocks[indice][comp][i-1] = blocks[indice][comp][i];
        }
    }
}

void upsample_vertical_avance(block *blocks, enum component comp, uint8_t indice, uint8_t indice_cible)
{
    printf("\ncible : %d\n", indice_cible);
    printf("indice : %d\n", indice);
    if (blocks[indice][comp] == NULL) {
        perror("Impossible de diviser une composante inexistante !");
        exit(EXIT_FAILURE);
    }

    if (blocks[indice_cible][comp] != NULL) {
        perror("RAAAAAAAAAAAAAAH !!");
        exit(EXIT_FAILURE);
    }

    blocks[indice_cible][comp] = calloc(64, sizeof(int16_t));

    for (size_t i = 0; i < 64; i++) {
        // On complète la deuxième composante (celle de droite)
        if (i%2) {
            blocks[indice_cible][comp][i] = blocks[indice][comp][4 + 8*(i/8) + (i%8)/2];
        }
        else {
            printf("blocks[indice_cible][comp] : %p\n", blocks[indice_cible][comp]);
            blocks[indice_cible][comp][i] = (blocks[indice][comp][4 + 8*(i/8) + (i%8)/2 - 1] + blocks[indice][comp][4 + 8*(i/8) + (i%8)/2])/2;
        }
    }
    for (size_t i = 0; i < 64; i++) {
        // On complète la première composante en place (celle de gauche)
        for (size_t i = 63; i > 0; i--) {
            if (i%2) {
                blocks[indice][comp][i] = (blocks[indice][comp][(i%8)/2 + 8*(i/8)] + blocks[indice][comp][(i%8)/2 + 1 + 8*(i/8)])/2;
            }
            else {
                blocks[indice][comp][i] = blocks[indice][comp][(i%8)/2 + 8*(i/8)];
            }
        }
    }
}

/*void upsample_horizontal(int16_t **blocks, uint8_t indice)
{
    if (blocks[indice][comp] == NULL) {
        perror("Impossible de diviser un composante inexistante !");
        exit(EXIT_FAILURE);
    }

    if (blocks[indice_cible][comp] == NULL) {
        blocks[indice_cible][comp] = malloc(64*sizeof(int16_t));
        for (size_t i = 0; i < 64; i++) {
            // On complète la deuxième composante (celle du bas)
            if ((i/8)%2) {
                blocks[indice_cible][comp][i] = (blocks[indice][comp][32 + i/2 - 1] + blocks[indice][comp][32 + i/2])/2;
            }
            else {
                blocks[indice_cible][comp][i] = blocks[indice][comp][32 + i/2];
            }
        }
        for (size_t i = 0; i < 64; i++) {
            // On complète la première composante en place (celle du haut)
            for (size_t i = 63; i > 0; i--) {
                if ((i/8)%2) {
                    blocks[indice][comp][i] = blocks[indice][comp][i/2];
                }
                else {
                    blocks[indice][comp][i] = (blocks[indice][comp][i/2] + blocks[indice][comp][i/2 + 1])/2;
                }
            }
        }
    }
    else {
        perror("Cette composante devrait être vide !");
        exit(EXIT_FAILURE);
    }

}*/


/* Sur-echantillonne un composant donné sous-echantillonné en deux */
//int16_t **upsample_to_two(const struct component *component);

/* Sur-echantillonne un composant donné sous-echantillonné en quatre */
//int16_t **upsample_to_four(const struct component *component);

/* Convertit un bloc YCbCr en bloc RGB */
void convert_to_rgb(block block) {
    int16_t y, cb, cr;
    for (uint8_t i = 0; i < COMPONENT_SIZE; ++i) {
        y = block[COMP_Y][i];
        cb = block[COMP_Cb][i];
        cr = block[COMP_Cr][i];
        block[RGB_R][i] = get_red(y, cb, cr);
        block[RGB_G][i] = get_green(y, cb, cr);
        block[RGB_B][i] = get_blue(y, cb, cr);
    }
}

/* Libere de la memoire la place occupee par un bloc */
void free_block(block block)
{
  free(block);
  block = NULL;
}
