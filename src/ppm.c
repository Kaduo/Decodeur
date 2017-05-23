/*
 * Fichier : ppm.c
 * Description : Fonctions de creation d'un fichier PPM binaire
 */

#include <stdlib.h>
#include <string.h>
#include "ppm.h"

/* Nombre magique du format PPM binaire */
const char BINARY_PPM_MAGIC_NUMBER = '6';
/* Valeur maximale d'une couleur */
const uint8_t MAX_COLOR_VALUE = 255;

/* Cree un fichier PPM a partir d'une image (tableau uint32_t) */
void create_image_ppm(const struct block *blocks,
                        uint32_t width,
                        uint32_t height,
                        char *filename)
{
    /* Ecriture de l'entete */
    write_ppm_header(width, height, filename);
    /* Ecriture des donnees */
    write_ppm_data(blocks, width, height, filename);
}

/* Ecrit l'entete d'un fichier PPM */
void write_ppm_header(uint32_t width, uint32_t height, char *filename)
{
    FILE* outfile = NULL;
    /* Ouverture du fichier (mode 'w' : fichier vide ou cree) */
    open_file(&outfile, filename, "w");
    /* Ecriture de l'entete dans le fichier */
    fprintf(outfile, "P%c\n", BINARY_PPM_MAGIC_NUMBER);
    fprintf(outfile, "%u %u\n", width, height);
    fprintf(outfile, "%hhu\n", MAX_COLOR_VALUE);
    /* Fermeture du fichier */
    close_file(&outfile, filename);
}

/* Ecrit les données d'un fichier PPM */
void write_ppm_data(const struct block *blocks,
                      uint32_t width,
                      uint32_t height,
                      char *filename)
{
    FILE* outfile = NULL;
    /* Ouverture du fichier (mode 'ab' : donnees au format binaire ajoutees
    en fin de fichier */
    open_file(&outfile, filename, "ab");

    /* TODO : RECONSTRUCTION DES BLOCS ET ECRITURE DES DONNEES */

    /* Fermeture du fichier */
    close_file(&outfile, filename);
}

/* Ouvre un fichier dans un mode donne */
void open_file(FILE **file, char *filename, char *mode)
{
    /* Ouverture du fichier : si succes, alors fin de la fonction, sinon,
    affichage d'un message d'erreur et exit */
    if ((*file = fopen(filename, mode)) == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s.\n", filename);
        exit(EXIT_FAILURE);
    }
}

/* Ferme un fichier */
void close_file(FILE **file, char *filename)
{
    /* Fermeture du fichier : si succes, alors fin de la fonction, sinon,
    affichage d'un message d'erreur et exit */
    if (fclose(*file) != 0) {
        fprintf(stderr, "Impossible de fermer le fichier %s.\n", filename);
        exit(EXIT_FAILURE);
    }
}
