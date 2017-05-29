/*******************************************************************************
Nom ......... : upsampling.c
Role ........ : Fonctions de surechantillonnage
Auteurs .... : A. He - M. Nebra - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include "upsampling.h"

/* Cree un composant a partir d'un nombre de blocs donne */
struct component *create_component(size_t nb_blocks)
{
    struct component *component = malloc(sizeof(struct component));
    component->nb_blocks = nb_blocks;
    component->blocks = NULL;
    return component;
}

/* Sur-echantillonne un composant a partir d'un composant sous-echantillonne
donne et des facteurs h et v (horizontal / vertical) */
struct component *upsample(const struct component *component,
                                    size_t h_factor,
                                    size_t v_factor)
{
    /* 1er cas : pas d'echantillonnage */
    if (component->nb_blocks == h_factor * v_factor) {
        return component;
    /* 2e cas : echantillonnage horizontal et vertical */
    } else if (component->nb_blocks == ) {

    }
}

/* Separe en deux (gauche / droite) les coefficients d'un bloc donne */
int16_t **split_in_two(const struct block *block)
{
    int16_t **split = malloc(2 * sizeof(int16_t*));
    size_t half = block->size / 2;
    *(split[TOP_LEFT]) = calloc(half, sizeof(int16_t));
    *(split[TOP_RIGHT]) = calloc(half, sizeof(int16_t));
    for (size_t y = 0; y < block->size; ++y) {
        for (size_t xl = 0, xr = half; x < half || x < block->size; ++xl, ++xr) {
            split[TOP_LEFT][y*block->size+x] = (block->coefficients)[y*block->size+xl];
            split[TOP_RIGHT][y*block->size+xr] = (block->coefficients)[y*block->size+xr];
        }
    }
    return split;
}

/* Separe en quatre les coefficients d'un bloc donne */
int16_t **split_in_four(const struct block *block)
{
    int16_t **split = malloc(4 * sizeof(int16_t*));
    size_t half = block->size / 2;
    size_t quarter = block->size / 4;
    *(split[TOP_LEFT]) = calloc(quarter, sizeof(int16_t));
    *(split[TOP_RIGHT]) = calloc(quarter, sizeof(int16_t));
    *(split[BOTTOM_LEFT]) = calloc(quarter, sizeof(int16_t));
    *(split[BOTTOM_RIGHT]) = calloc(quarter, sizeof(int16_t));
    for (size_t yu = 0, yb = half; yu < half || yb < block->size; ++yu, ++yb) {
        for (size_t xl = 0, xr = half; x < half || x < block->size; ++xl, ++xr) {
            split[TOP_LEFT][yu*block->size+x] = (block->coefficients)[yu*block->size+xl];
            split[TOP_RIGHT][yu*block->size+xr] = (block->coefficients)[yu*block->size+xr];
            split[BOTTOM_LEFT][yb*block->size+x] = (block->coefficients)[yb*block->size+xl];
            split[BOTTOM_RIGHT][yb*block->size+xr] = (block->coefficients)[yb*block->size+xr];
        }
    }
    return split;
}

/* Replique horizontalement des coefficients donnes */
int16_t *horizontal_replication(const int16_t *coefficients, size_t size)
{
    size_t current_length = size * size;
    size_t new_length = 2 * current_length;
    int16_t *replication = calloc(new_length, sizeof(int16_t));
    for (size_t xc = 0, xr = 0; xc < current_length || xr < new_length; ++xc, xr += 2) {
        replication[xr] = coefficients[xc];
        replication[xr+1] = coefficients[xc];
    }
    return replication;
}

/* Replique horizontalement et verticalement des coefficients donnes */
int16_t *two_dimension_replication(const int16_t *coefficients, size_t size)
{
    size_t current_length = size * size;
    size_t new_length = current_length * current_length;
    int16_t *replication = calloc(new_length, sizeof(int16_t));
    for (size_t yc = 0, yr = 0; yc < size)

}

/* Libere en memoire l'espace occupe par un composant donne */
void free_component(struct component *component)
{
    for (size_t i = 0; i < component->nb_blocks; ++i) {
        free_block((component->blocks)[i]);
    }
    free(component);
    component = NULL;
}
