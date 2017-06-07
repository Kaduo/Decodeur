/*******************************************************************************
Nom ......... : block.c
Role ........ : Fonctions de reconstruction de blocs
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdio.h>
#include "block.h"
#include "rgb.h"
#include "shared.h"

/* Crée une structure block à partir d'une taille donné */
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

    /* Si l'image est en couleur, upsampling potentiel */
    if (mcu->nb_cbs > 0) {
        if (no_upsampling_cb) {
            for (size_t i = 0; i < nb_blocks; i++) {
                blocks[i][COMP_Cb] = mcu->components_cb[i];
            }
        } else {
            /* Upsampling de chaque composante Cb */
            for (size_t i = 0; i < mcu->nb_cbs; i++) {
                blocks[i*factors[COMP_Cb][DIR_V]*factors[COMP_Cb][DIR_H]][COMP_Cb] = mcu->components_cb[i];
                upsampling_recursif(blocks, COMP_Cb, i*factors[COMP_Cb][DIR_V]*factors[COMP_Cb][DIR_H],
                    factors[COMP_Y][DIR_H], factors[COMP_Y][DIR_V], factors[COMP_Cb][DIR_H], factors[COMP_Cb][DIR_V]);
            }
            /* Traite le cas 2x2, 2x1, 1x1 */
            if (factors[COMP_Y][DIR_H] == 2 && factors[COMP_Y][DIR_V] == 2 && factors[COMP_Cb][DIR_H] == 2) {
                int16_t *temp = blocks[1][COMP_Cb];
                blocks[1][COMP_Cb] = blocks[2][COMP_Cb];
                blocks[2][COMP_Cb] = temp;
            }
        }

        if (no_upsampling_cr) {
            for (size_t i = 0; i < nb_blocks; i++) {
                blocks[i][COMP_Cr] = mcu->components_cr[i];
            }
        } else {
            /* Upsampling de chaque composante Cr */
            for (size_t i = 0; i < mcu->nb_crs; i++) {
                blocks[i*factors[COMP_Cr][DIR_V]*factors[COMP_Cr][DIR_H]][COMP_Cr] = mcu->components_cr[i];
                upsampling_recursif(blocks, COMP_Cr, i*factors[COMP_Cr][DIR_V]*factors[COMP_Cr][DIR_H],
                    factors[COMP_Y][DIR_H], factors[COMP_Y][DIR_V], factors[COMP_Cr][DIR_H], factors[COMP_Cr][DIR_V]);
            }
            /* Traite le cas 2x2, 1x1, 2x1 */
            if (factors[COMP_Y][DIR_H] == 2 && factors[COMP_Y][DIR_V] == 2 && factors[COMP_Cr][DIR_H] == 2) {
                int16_t *temp = blocks[1][COMP_Cr];
                blocks[1][COMP_Cr] = blocks[2][COMP_Cr];
                blocks[2][COMP_Cr] = temp;
            }

        }
    }
    return blocks;
}

/* Sur-échantillonne le bloc à l'indice donné */
void upsampling_recursif(block *blocks, enum component comp, uint8_t indice, uint8_t h1, uint8_t v1, uint8_t h, uint8_t v)
{
    /* Absence d'upsampling / upsampling termine */
    if (h1 == h && v1 == v) {
        return;
    /* Upsampling vertical */
    } else if (v1 != v) {
        v *= 2;
        uint8_t nb_elements = h * v;
        upsample_vertical(blocks, comp, indice, indice + h1*v1/nb_elements);
        upsampling_recursif(blocks, comp, indice, h1, v1, h, v);
        upsampling_recursif(blocks, comp, indice + h1*v1/nb_elements, h1, v1, h, v);
    /* Upsampling horizontal */
    } else {
        h *= 2;
        uint8_t nb_elements = h * v;
        upsample_horizontal(blocks, comp, indice, indice + h1*v1/nb_elements);
        upsampling_recursif(blocks, comp, indice, h1, v1, h, v);
        upsampling_recursif(blocks, comp, indice + h1*v1/nb_elements, h1, v1, h, v);
    }
}

/* Sur-échantillonne horizontalement le bloc à l'indice donné */
void upsample_horizontal(block *blocks, enum component comp, uint8_t indice, uint8_t indice_cible)
{
    if (blocks[indice][comp] == NULL) {
        perror("Impossible de diviser une composante inexistante !");
        exit(EXIT_FAILURE);
    }

    if (blocks[indice_cible][comp] != NULL) {
        perror("L'indice cible est déjà occupé !");
        exit(EXIT_FAILURE);
    }

    blocks[indice_cible][comp] = malloc(COMPONENT_SIZE * sizeof(int16_t));

    /* On complète la composante cible (celle de droite) */
    for (size_t i = 0; i < COMPONENT_SIZE; i += 2) {
        blocks[indice_cible][comp][i] = blocks[indice][comp][BLOCK_SIZE/2 + BLOCK_SIZE*(i/BLOCK_SIZE) + (i%BLOCK_SIZE)/2];
        blocks[indice_cible][comp][i+1] = blocks[indice_cible][comp][i];
    }
    /* On complète la première composante en place (celle de gauche) */
    for (size_t i = COMPONENT_SIZE - 1; i > 1; i -= 2) {
        blocks[indice][comp][i] = blocks[indice][comp][BLOCK_SIZE*(i/BLOCK_SIZE) + (i%BLOCK_SIZE)/2];
        blocks[indice][comp][i-1] = blocks[indice][comp][i];
    }
}

/* Sur-échantillonne verticalement le bloc a l'indice donné */
void upsample_vertical(block *blocks, enum component comp, uint8_t indice, uint8_t indice_cible)
{
    if (blocks[indice][comp] == NULL) {
        perror("Impossible de diviser une composante inexistante !");
        exit(EXIT_FAILURE);
    }

    if (blocks[indice_cible][comp] != NULL) {
        perror("L'indice cible est déjà occupé !");
        exit(EXIT_FAILURE);
    }

    blocks[indice_cible][comp] = malloc(COMPONENT_SIZE * sizeof(int16_t));

    /* On complète la composante cible (celle du bas) */
    for (size_t i = 0; i < COMPONENT_SIZE; i++) {
        blocks[indice_cible][comp][i] = blocks[indice][comp][COMPONENT_SIZE/2 + (i%BLOCK_SIZE) + BLOCK_SIZE*(i/(BLOCK_SIZE*2))];
        blocks[indice_cible][comp][i + BLOCK_SIZE] = blocks[indice_cible][comp][i];
        if (i%BLOCK_SIZE == (BLOCK_SIZE-1)) {
            i += BLOCK_SIZE;
        }
    }

    /* On complète la première composante en place (celle du haut) */
    for (int16_t i = COMPONENT_SIZE - 1; i >= (int16_t) BLOCK_SIZE; --i) {
        blocks[indice][comp][i] = blocks[indice][comp][(i%BLOCK_SIZE) + BLOCK_SIZE*(i/(BLOCK_SIZE*2))];
        blocks[indice][comp][i - BLOCK_SIZE] = blocks[indice][comp][i];
        if (i%BLOCK_SIZE == 0) {
            i -= BLOCK_SIZE;
        }
    }
}

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

/* Libère de la mémoire la place occupée par un bloc */
void free_block(block block, bool is_color)
{
    /* Libération de la composante Y */
    free(block[0]);
    if(is_color){
        /* Libération des composantes Cb et Cr */
        free(block[1]);
        free(block[2]);
    }
    free(block);
    block = NULL;
}
