/*******************************************************************************
Nom ......... : picture.c
Role ........ : Fonctions de gestion d'image
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "picture.h"
#include "shared.h"
#include "trace.h"

/* Valeur maximale d'une composante RGB */
const uint8_t MAX_COLOR_VALUE = 255;
/* Nombre magique du format PPM (couleur) */
const uint8_t PPM_MAGIC_NUMBER = 6;
/* Nombre magique du format PGM (noir et blanc) */
const uint8_t PGM_MAGIC_NUMBER = 5;

/* Crée une image à partir de dimensions données */
struct picture *create_picture(size_t width, size_t height, bool colored)
{
    struct picture *picture = malloc(sizeof(struct picture));
    picture->width = width;
    picture->height = height;
    picture->colored = colored;
    picture->pixels = malloc(width * height * sizeof(union pixel));
    return picture;
}

/* Convertit l'image sous forme de bloc rangé en MCU en tableau de pixels */
struct picture *blocks2pixels(block *blocks,
                                size_t width,
                                size_t height,
                                size_t width_ext,
                                uint8_t h1,
                                uint8_t v1)
{
    bool is_bw = blocks[0][COMP_Cb] == NULL;
    struct picture *pic = create_picture(width, height, !is_bw);
    size_t nb_blocs_h = width_ext / BLOCK_SIZE;
    uint32_t l_bloc = 0;
    uint32_t l_in_bloc = 0;
    uint32_t id_pixel = 0;

    /* Boucle sur les lignes de pixels */
    for(uint32_t l = 0; l < height; ++l){
        l_bloc = (l/BLOCK_SIZE);
        l_in_bloc = l%BLOCK_SIZE;

        /* Boucle sur les blocs de la ligne l */
        uint32_t b = h1*(l_bloc%v1); // Indice du bloc dans la ligne de MCU
        for(; b < (uint32_t) nb_blocs_h*v1 - 1 - h1*(v1 - l_bloc%v1 - 1);) {
            uint16_t indice_dans_liste = (l_bloc - l_bloc%v1)*nb_blocs_h + b;
            uint16_t indice_dans_comp = 0;
            /* Boucle sur les pixels de la ligne l%8 du bloc b */
            for(uint8_t i = 0; i < BLOCK_SIZE; ++i){
                indice_dans_comp = l_in_bloc*BLOCK_SIZE + i;
                id_pixel = l*width + (b/(h1*v1))*h1*BLOCK_SIZE + ((b%(h1*v1))%h1)*BLOCK_SIZE + i;
                if (is_bw) {
                        pic->pixels[id_pixel].y = (uint8_t) blocks[l_bloc*nb_blocs_h + b][COMP_Y][l_in_bloc*BLOCK_SIZE + i];
                } else {
                        pic->pixels[id_pixel].rgb.red = (uint8_t) blocks[indice_dans_liste][RGB_R][indice_dans_comp];
                        pic->pixels[id_pixel].rgb.green = (uint8_t) blocks[indice_dans_liste][RGB_G][indice_dans_comp];
                        pic->pixels[id_pixel].rgb.blue = (uint8_t) blocks[indice_dans_liste][RGB_B][indice_dans_comp];
                }
            }

            /* Calcul de l'indice du prochain bloc selon l'upsampling */
            b += (b%h1 < h1 - 1) ? 1 : h1*(v1 - 1) + 1;
        }

        /* Traitement du dernier bloc d'une ligne pour rogner l'image si besoin */
        uint16_t indice_dernier_bloc = b;
        uint16_t indice_dans_liste = (l_bloc - l_bloc%v1)*nb_blocs_h + indice_dernier_bloc;
        uint16_t indice_dans_comp = 0;
        for(uint8_t i = 0; i < BLOCK_SIZE - (width_ext - width); i++){
            indice_dans_comp = l_in_bloc*BLOCK_SIZE + i;
            id_pixel = l*width + (indice_dernier_bloc/(h1*v1))*h1*BLOCK_SIZE + ((indice_dernier_bloc%(h1*v1))%h1)*BLOCK_SIZE + i;
            if (is_bw) {
                    pic->pixels[id_pixel].y = blocks[l_bloc*nb_blocs_h + indice_dernier_bloc][COMP_Y][indice_dans_comp];
            } else {
                pic->pixels[id_pixel].rgb.red = blocks[indice_dans_liste][RGB_R][indice_dans_comp];
                    pic->pixels[id_pixel].rgb.green = blocks[indice_dans_liste][RGB_G][indice_dans_comp];
                    pic->pixels[id_pixel].rgb.blue = blocks[indice_dans_liste][RGB_B][indice_dans_comp];
            }
        }
    }
    return pic;
}

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
    fprintf(outfile, "P%hhu\n", get_magic_number(picture->colored));
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
        if (picture->colored) {
            fwrite(&(picture->pixels[i].rgb), sizeof(struct rgb), 1, outfile);
        } else {
            fwrite(&picture->pixels[i].y, sizeof(uint8_t), 1, outfile);
        }
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

/* Retourne le nombre magique correspondant au format */
uint8_t get_magic_number(bool colored)
{
    return colored ? PPM_MAGIC_NUMBER : PGM_MAGIC_NUMBER;
}

/* Libere en memoire l'espace occupe par une image donnee */
void free_picture(struct picture *picture)
{
    free(picture->pixels);
    free(picture);
    picture = NULL;
}
