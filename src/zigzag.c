/*******************************************************************************
Nom ......... : zigzag.c
Role ........ : Fonctions de zigzag
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdlib.h>
#include "zigzag.h"

/* Met a jour un point donne en fonction d'un mouvement courant */
void update_point(struct point *point, enum move move)
{
    switch(move) {
        case MOVE_RIGHT:
            (point->x)++;
            break;
        case MOVE_DOWN:
            (point->y)++;
            break;
        case MOVE_UP_DIAG:
            (point->x)++;
            (point->y)--;
            break;
        case MOVE_DOWN_DIAG:
            (point->x)--;
            (point->y)++;
            break;
        default:
            break;
    }
}

/* Indique si un point donne se situe sur la premiere ligne d'un bloc */
bool first_line(const struct point *point)
{
    return (point->y == 0);
}

/* Indique si un point donne se situe sur la premiere colonne d'un bloc */
bool first_column(const struct point *point)
{
    return (point->x == 0);
}

/* Indique si un point donne se situe sur la derniere ligne d'un bloc d'une
taille donnee */
bool last_line(const struct point *point, size_t size)
{
    return (point->y == (size - 1));
}

/* Indique si un point donne se situe sur la derniere colonne d'un bloc d'une
taille donnee */
bool last_column(const struct point *point, size_t size)
{
    return (point->x == (size - 1));
}

/* Indique le prochain mouvement a effectuer a partir d'un point et d'un
mouvement donnes sur un bloc d'une taille donnee */
enum move next_move(const struct point *point, enum move move, size_t size)
{
    switch(move) {
        case MOVE_RIGHT:
            return (first_line(point) ? MOVE_UP_DIAG : MOVE_DOWN_DIAG);
        case MOVE_DOWN:
            return (last_column(point, size) ? MOVE_DOWN_DIAG : MOVE_UP_DIAG);
        case MOVE_UP_DIAG:
            if (!(first_line(point) || last_column(point, size))) {
                return move;
            } else if (last_column(point, size)) {
                return MOVE_DOWN;
            } else {
                return MOVE_RIGHT;
            }
        case MOVE_DOWN_DIAG:
            if (!(first_column(point) || last_line(point, size))) {
                return move;
            } else if (last_line(point, size)) {
                return MOVE_RIGHT;
            } else {
                return MOVE_DOWN;
            }
        default:
            return MOVE_UNKNOWN;
    }
}

/* Retourne le vecteur zigzag inverse d'un vecteur donne */
int16_t *get_inverse(const int16_t *coefficients, size_t size)
{
    size_t length = size * size;
    int16_t *inverse = calloc(length, sizeof(int16_t));
    struct point point = {0, 0};
    enum move move = MOVE_RIGHT;
    for (size_t i = 0; i < length; ++i) {
        inverse[point.y*size+point.x] = coefficients[i];
        update_point(&point, move);
        move = next_move(&point, move, size);
    }
    return inverse;
}
