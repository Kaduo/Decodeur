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

/* Cree une image a partir de dimensions donnes */
struct picture *create_picture(size_t width, size_t height)
{
    struct picture *picture = malloc(sizeof(struct picture));
    picture->width = width;
    picture->height = height;
    picture->pixels = NULL;
    return picture;
}

// Convertir l'image sous forme de bloc rangé en MCU en tableau de pixel.
struct image *blocks2pixels(
    block *blocks,
    size_t width,
    size_t height,
    size_t width_ext,
    size_t height_ext,
    uint8_t h1,
    uint8_t v1)
    {
    
    bool_t is_bw = blocs[0][0] == NULL;
    struct picture *pic = create_image(width, height, !is_bw);
    size_t nb_blocs_h = width_ext : 8;
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
                     pic[l*width + b*8 +i] = create_pixel_bw(
                         blocks[l_bloc*nb_blocs_h +b][0][l_in_bloc*8 + i]);   b
                 } // end if
                 else{
                     pic[l*width + b*8 +i] = create_pixel_rgb(
                         blocks[l_bloc*nb_blocs_h +b][0][l_in_bloc*8 + i],
                         blocks[l_bloc*nb_blocs_h +b][1][l_in_bloc*8 + i],
                         blocks[l_bloc*nb_blocs_h +b][2][l_in_bloc*8 + i]);
                         
                 } // end else
             } // end for i.
         } // end for b.
     } // end for l
return pic;
} // end def


/* Genere un fichier PPM binaire correspondant a une image donnee */
void write_ppm(const struct picture *picture, const char *filename)
{
    /* Ecriture de l'entete */
    write_ppm_header(picture, filename);
    /* Ecriture des donnees */
    write_ppm_data(picture, filename);
}

/* Genere l'entete d'un fichier PPM */
void write_ppm_header(const struct picture *picture, const char *filename)
{
    FILE* outfile = NULL;
    /* Ouverture du fichier */
    secured_open_file(&outfile, filename, "w");
    /* Ecriture de l'entete PPM */
    fprintf(outfile, "P6\n");
    fprintf(outfile, "%zd %zd\n", picture->width, picture->height);
    fprintf(outfile, "%hhu\n", MAX_COLOR_VALUE);
    /* Fermeture du fichier */
    secured_close_file(&outfile, filename);
}

/* Genere les donnees d'un fichier PPM */
void write_ppm_data(const struct picture *picture, const char *filename)
{
    FILE* outfile = NULL;
    /* Ouverture du fichier */
    secured_open_file(&outfile, filename, "ab");
    /* Ecriture des donnees */
    for (size_t i = 0; i < picture->width * picture->height; ++i) {
        fwrite(&(picture->pixels[i]), sizeof(struct rgb), 1, outfile);
    }
    /* Fermeture du fichier */
    secured_close_file(&outfile, filename);
}

/* Ouvre un fichier de maniere securisee */
void secured_open_file(FILE **file, const char *filename, const char *mode)
{
    if ((*file = fopen(filename, mode)) == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s.\n", filename);
        exit(EXIT_FAILURE);
    }
}

/* Ferme un fichier de maniere securisee */
void secured_close_file(FILE **file, const char *filename)
{
    if (fclose(*file) != 0) {
        fprintf(stderr, "Impossible de fermer le fichier %s.\n", filename);
        exit(EXIT_FAILURE);
    }
}

/* Libere en memoire l'espace occupe par une image donnee */
void free_picture(struct picture *picture)
{
    free(picture->pixels);
    free(picture);
    picture = NULL;
}
