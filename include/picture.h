/*******************************************************************************
Nom ......... : picture.h
Role ........ : Prototypes des fonctions de gestion d'image
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __PICTURE_H__
#define __PICTURE_H__

#include <stdbool.h>
#include "block.h"
#include "rgb.h"

/* Union représentant un pixel (soit suivant une luminance, soit RGB) */
union pixel {
    uint8_t y;
    struct rgb rgb;
};

/* Structure représentant une image RGB */
struct picture {
    size_t width;
    size_t height;
    bool colored;
    union pixel *pixels;
};

/* Crée une image à partir de dimensions données */
struct picture *create_picture(size_t width, size_t height, bool colored);

/* Convertit l'image sous forme de bloc rangé en MCU en tableau de pixels */
extern struct picture *blocks2pixels(
    block *blocks,
    size_t width,
    size_t height,
    size_t width_ext,
    uint8_t h1,
    uint8_t v1);

/* Genere un fichier PPM binaire correspondant a une image donnee */
extern void write_ppm(const struct picture *picture, const char *filename);

/* Genere l'entete d'un fichier PPM */
void write_ppm_header(const struct picture *picture, const char *filename);

/* Genere les donnees d'un fichier PPM */
void write_ppm_data(const struct picture *picture, const char *filename);

/* Ouvre un fichier de maniere securisee */
void secured_open_file(FILE **file,
                                const char *filename,
                                const char *mode);

/* Ferme un fichier de maniere securisee */
void secured_close_file(FILE **file, const char *filename);

/* Retourne le nombre magique correspondant au format */
uint8_t get_magic_number(bool colored);

/* Libere en memoire l'espace occupe par une image donnee */
extern void free_picture(struct picture *picture);

#endif
