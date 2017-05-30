/*******************************************************************************
Nom ......... : picture.h
Role ........ : Prototypes des fonctions de gestion d'image
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __PICTURE_H__
#define __PICTURE_H__

#include <stdbool.h>
#include "pixel.h"

/* Structure reprensant une image RGB */
struct picture {
    size_t width;
    size_t height;
    bool colored;
    union pixel *pixels;
};

/* Cree une image a partir de dimensions donnes */
extern struct picture *create_picture(size_t width, size_t height, bool colored);

/* Genere un fichier PPM binaire correspondant a une image donnee */
extern void write_ppm(const struct picture *picture, const char *filename);

/* Genere l'entete d'un fichier PPM */
extern void write_ppm_header(const struct picture *picture, const char *filename);

/* Genere les donnees d'un fichier PPM */
extern void write_ppm_data(const struct picture *picture, const char *filename);

/* Ouvre un fichier de maniere securisee */
extern void secured_open_file(FILE **file,
                                const char *filename,
                                const char *mode);

/* Ferme un fichier de maniere securisee */
extern void secured_close_file(FILE **file, const char *filename);

/* Retourne le nombre magique correspondant au format */
extern uint8_t get_magic_number(bool colored);

/* Libere en memoire l'espace occupe par une image donnee */
extern void free_picture(struct picture *picture);

#endif
