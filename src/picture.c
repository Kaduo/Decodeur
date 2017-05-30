/*******************************************************************************
Nom ......... : picture.c
Role ........ : Fonctions de gestion d'image
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "picture.h"

/* Valeur maximale d'une composante RGB */
const uint8_t MAX_COLOR_VALUE = 255;
/* Nombre magique du format PPM (couleur) */
const uint8_t PPM_MAGIC_NUMBER = 6;
/* Nombre magique du format PGM (noir et blanc) */
const uint8_t PGM_MAGIC_NUMBER = 5;

/* Cree une image a partir de dimensions donnes */
struct picture *create_picture(size_t width, size_t height, bool colored)
{
    struct picture *picture = malloc(sizeof(struct picture));
    picture->width = width;
    picture->height = height;
    picture->colored = colored;
    return picture;
    picture->pixels = malloc(width * height * sizeof(union pixel));
}

// Convertir l'image sous forme de bloc rangé en MCU en tableau de pixel.
struct picture *blocks2pixels(
    block *blocks,
    size_t width,
    size_t height,
    size_t width_ext,
    size_t height_ext,
    uint8_t h1,
    uint8_t v1)
    {

    bool is_bw = blocks[0][0] == NULL;
    struct picture *pic = create_picture(width, height, !is_bw);
    size_t nb_blocs_h = width_ext % 8;
    size_t nb_blocs_v = height_ext / 8;
    uint16_t l_bloc = 0;
    uint16_t l_in_bloc = 0;

    // Boucle sur les lignes de pixels.
     for(uint16_t l=0; l < height; l++){
         l_bloc = l/8;
         l_in_bloc = l%8;

         // Boucle sur les blocs de la ligne l.
         for(uint16_t b=0; b<nb_blocs_h; b++){
             // Boucle sur les pixels de la ligne l%8 du bloc b.
             for(uint8_t i=0; i < 8; i++){
                 if(is_bw){
                     pic->pixels[l*width + b*8 +i] = create_pixel_bw(
                         blocks[l_bloc*nb_blocs_h +b][0][l_in_bloc*8 + i]);
                 } // end if
                 else{
                     pic->pixels[l*width + b*8 +i] = create_pixel_rgb(
                         blocks[l_bloc*nb_blocs_h +b][0][l_in_bloc*8 + i],
                         blocks[l_bloc*nb_blocs_h +b][1][l_in_bloc*8 + i],
                         blocks[l_bloc*nb_blocs_h +b][2][l_in_bloc*8 + i]);

                 } // end else
             } // end for i.
         } // end for b.
     } // end for l
return pic;
} // end def



/* Libere en memoire l'espace occupe par une image donnee */
void free_picture(struct picture *picture)
{
    free(picture->pixels);
    free(picture);
    picture = NULL;
}
