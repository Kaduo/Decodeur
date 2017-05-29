/*******************************************************************************
Nom ......... : zigzag.h
Role ........ : Prototypes des fonctions de zigzag
Auteurs .... : A. He - M. Nebra - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __ZIGZAG_H__
#define __ZIGZAG_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Structure representant un point ordonne */
struct point {
    size_t x;
    size_t y;
};

/* Enumeration des mouvements possibles dans le parcours en zigzag */
enum move {
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_UP_DIAG,
    MOVE_DOWN_DIAG,
    MOVE_UNKNOWN
};

/* Met a jour un point donne en fonction d'un mouvement courant */
extern void update_point(struct point *point, enum move move);

/* Indique si un point donne se situe sur la premiere ligne d'un bloc */
extern bool first_line(const struct point *point);

/* Indique si un point donne se situe sur la premiere colonne d'un bloc */
extern bool first_column(const struct point *point);

/* Indique si un point donne se situe sur la derniere ligne d'un bloc d'une
taille donnee */
extern bool last_line(const struct point *point, size_t size);

/* Indique si un point donne se situe sur la derniere colonne d'un bloc d'une
taille donnee */
extern bool last_column(const struct point *point, size_t size);

/* Indique le prochain mouvement a effectuer a partir d'un point et d'un
mouvement donnes sur un bloc d'une taille donnee */
extern enum move next_move(const struct point *point,
                            enum move move,
                            size_t size);

/* Retourne le vecteur zigzag inverse d'un vecteur donne */
extern int16_t *get_inverse(const int16_t *coefficients, size_t size);

#endif
