/*******************************************************************************
Nom ......... : jpeg_reader.h
Role ........ : Fonctions de lecture de l'entete JPEG
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "jpeg_reader.h"

/* Taille en bits d'un octet */
#define BYTE_SIZE 8
/* Taille en bits d'un marqueur de section */
#define MARKER_SIZE 16
/* Taille en bits d'une longueur de section */
#define SECTION_SIZE 16

/* Structure representant un descripteur contenant toutes les informations lues
dans les entêtes des sections JPEG */
struct jpeg_desc {
    char *filename; // Nom du fichier
    struct bitstream *stream; // Flux de donnees
    uint8_t nb_quantization_tables; // Nombre de tables de quantification
    uint8_t **quantization_tables; // Tables de quantification
    uint8_t nb_huffman_tables; // Nombre de tables de Huffman
    uint8_t **huffman_tables; // Tables de Huffman
    uint16_t width, height; // Dimensions de l'image
    uint8_t nb_components; // Nombre de composantes
};

/* Ouvre le fichier JPEG, lit tous les entetes de sections rencontres et stocke
les informations dans le descripteur retourne */
struct jpeg_desc *read_jpeg(const char *filename)
{
    struct jpeg_desc *jdesc = malloc(sizeof(struct jpeg_desc));

    /* Recuperation du nom du fichier */
    jpeg_desc->filename = calloc(strlen(filename) + 1, sizeof(char));
    if(!strcpy(jpeg_desc->filename, filename)) {
        fprintf(stderr,
                    "Le nom du fichier JPEG '%s' n'a pas pu etre recupere.\n",
                    filename);
        return EXIT_FAILURE;
    }

    /* Ouverture du flux de donnees */
    jpeg_desc->stream = create_bitstream(filename);

    /* Initialisation d'une variable contenant le marqueur de la section */
    int32_t read = 0;
    /* Initialisation d'une variable contenant la longueur de la section */
    int32_t size = 0;
    /* Initialisation d'une variable contenant le nombre de bits lus */
    uint8_t nb_read = 0;

    /* Traitement du debut de l'image (marqueur SOI) */
    nb_read = read_bitstream(jpeg_desc->stream, MARKER_SIZE, &read, false);
    if (read != MARKER_SOI) {
        fprintf(stderr, "Le fichier JPEG debute par le marqueur '%04x' au lieu
                    d'un marqueur SOI '%04x'\n", reader, MARKER_SOI);
        return EXIT_FAILURE;
    }

    /* === LECTURE DES SECTIONS JPEG === */

    nb_read = read_bitstream(jpeg_desc->stream, MARKER_SIZE, &read, false);
    while (read != MARKER_EOI || nb_read != 0) {
        nb_read = read_bitstream(jpeg_desc->stream, SECTION_SIZE, &size, false);
        switch(read) {
            case MARKER_APP0:
                nb_read = read_bitstream(jpeg_desc->stream, 4, &read, false);
                break;
            case MARKER_COM:
                break;
            case

        }
        /* Lecture du contenu restant de la section */
        nb_read = read_bitstream(jpeg_desc->stream,
                                    size - MARKER_SIZE - SECTION_SIZE,
                                    &read,
                                    false);
        /* Lecture du marqueur suivant */
        nb_read = read_bitstream(jpeg_desc->stream,
                                    MARKER_SIZE,
                                    &read,
                                    false);
    }

    /* === FIN DE LECTURE DES SECTIONS JPEG === */

    /* Traitement de la fin de l'image (marqueur EOI) */
    if (marker != MARKER_EOI) {
        fprintf(stderr,
                    "Le fichier JPEG termine par le marqueur '%04x' au lieu
                    d'un marqueur EOI '%04x'\n", section_marker, MARKER_EOI);
        return EXIT_FAILURE;
    }

    return jdesc;
}

/* Ferme un descripteur prealablement ouvert, en liberant toute la memoire
necessaire */
void close_jpeg(struct jpeg_desc *jpeg)
{

}

/* Retourne le nom de fichier de l’image ouverte */
char *get_filename(const struct jpeg_desc *jpeg)
{

}
