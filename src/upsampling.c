/*******************************************************************************
Nom ......... : upsampling.c
Role ........ : Fonctions de surechantillonnage
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
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
    } else {
        struct component *upsample = create_component(h_factor * v_factor);
        /* 2e cas : echantillonnage horizontal */
        if (2 * component->nb_blocks == h_factor * v_factor) {
            int16_t **split_up = split_in_two((component->blocks)[0]);
            int16_t **split_down = split_in_two((component->blocks)[1]);
            size_t height = ((component->blocks)[0])->size;
            size_t width = height / 2;
            (upsample->blocks)[TOP_LEFT] = horizontal_replication(split_up[0], width, height);
            (upsample->blocks)[TOP_LEFT] = horizontal_replication(split_up[1], width, height);
            (upsample->blocks)[BOTTOM_LEFT] = horizontal_replication(split_down[0], width, height);
            (upsample->blocks)[BOTTOM_RIGHT] = horizontal_replication(split_down[1], width, height);
            free(split_up);
            split_up = NULL;
            free(split_down);
            split_down = NULL;
        /* 3e cas : echantillonnage horizontal et vertical */
        } else {
            int16_t **split = split_in_four((component->blocks)[0]);
            size_t size = ((component->blocks)[0])->size;
            (upsample->blocks)[TOP_LEFT] = two_dimension_replication(split[0], size);
            (upsample->blocks)[TOP_LEFT] = horizontal_replication(split[1], size);
            (upsample->blocks)[BOTTOM_LEFT] = horizontal_replication(split[2], size);
            (upsample->blocks)[BOTTOM_RIGHT] = horizontal_replication(split[3], size);
            free(split);
            split = NULL;
        }
        return upsample;
    }
}

/* Separe en deux (gauche / droite) les coefficients d'un bloc donne */
int16_t **split_in_two(const struct block *block)
{
    int16_t **split = malloc(2 * sizeof(int16_t*));
    size_t half = block->size / 2;
    *(split[0]) = calloc(half, sizeof(int16_t));
    *(split[1]) = calloc(half, sizeof(int16_t));
    for (size_t y = 0; y < block->size; ++y) {
        for (size_t xl = 0, xr = half; x < half || x < block->size; ++xl, ++xr) {
            split[0][y*block->size+x] = (block->coefficients)[y*block->size+xl];
            split[1][y*block->size+xr] = (block->coefficients)[y*block->size+xr];
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
    *(split[0]) = calloc(quarter, sizeof(int16_t));
    *(split[1]) = calloc(quarter, sizeof(int16_t));
    *(split[2]) = calloc(quarter, sizeof(int16_t));
    *(split[3]) = calloc(quarter, sizeof(int16_t));
    for (size_t yu = 0, yb = half; yu < half || yb < block->size; ++yu, ++yb) {
        for (size_t xl = 0, xr = half; x < half || x < block->size; ++xl, ++xr) {
            split[0][yu*block->size+x] = (block->coefficients)[yu*block->size+xl];
            split[1][yu*block->size+xr] = (block->coefficients)[yu*block->size+xr];
            split[2][yb*block->size+x] = (block->coefficients)[yb*block->size+xl];
            split[3][yb*block->size+xr] = (block->coefficients)[yb*block->size+xr];
        }
    }
    return split;
}

/* Replique horizontalement des coefficients donnes */
int16_t *horizontal_replication(const int16_t *coefficients,
                                        size_t width,
                                        size_t height)
{
    size_t current_length = width * height;
    size_t new_length = 2 * current_length;
    int16_t *replication = calloc(new_length, sizeof(int16_t));
    for (size_t xc = 0, xr = 0; xc < current_length || xr < new_length; ++xc, xr += 2) {
        replication[xr] = coefficients[xc];
        replication[xr+1] = coefficients[xc];
    }
    return replication;
}

/* Replique horizontalement et verticalement des coefficients donnes */
int16_t *two_dimension_replication(const int16_t *coefficients,
                                    size_t size)
{
    size_t current_length = size * size;
    size_t new_length = current_length * current_length;
    int16_t *replication = calloc(new_length, sizeof(int16_t));
    for (size_t yc = 0, yr = 0; yc < current_length || yr < new_length; ++yc, yr += 2) {
        for (size_t xc = 0, xr < 0; xc < current_length || xr < new_length; ++xc, xr += 2) {
            replication[xr] = coefficients[xc];
            replication[xr+1] = coefficients[xc];
            replication[yr*new_length+xr] = coefficients[xc];
        }
    }
    return replication;
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
