/*******************************************************************************
Nom ......... : jpeg_reader.h
Role ........ : Fonctions de lecture de l'entete JPEG
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "jpeg_reader.h"
#include "shared.h"

/* Taille en bits d'un octet */
#define BYTE_SIZE 8
/* Taille en bits d'un marqueur de section */
#define MARKER_SIZE 16
/* Taille en bits d'une longueur de section */
#define SECTION_SIZE 16

/* == Section DQT == */
/* Nombre maximum de tables de quantification */
#define MAX_QUANTIZATION_TABLES 4
/* Taille en bits de l'information de la precision */
#define DQT_PRECISION 4
/* Taille en bits de l'information de l'indice q */
#define DQT_INDEX_Q 4
/* Nombre de valeurs dans une table de quantification  */
#define DQT_NB_VALUES 64

/* == SECTION SOF == */
/* Taille en bits de l'information de precision */
#define SOF_PRECISION 8
/* Taille en bits de l'information de la largeur ou de la hauteur de l'image */
#define SOF_IMAGE_SIZE 16
/* Taille en bits de l'informtaion du nombre de composantes */
#define SOF_NB_COMPONENTS 8
/* Taille en bits de l'information d'un facteur d'echantillonnage */
#define SOF_SAMPLING_FACTOR 4

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
    uint8_t ***components; // Composantes
    uint8_t ***sampling_factors; // Facteurs d'echantillonnage
};

/* Ouvre le fichier JPEG, lit tous les entetes de sections rencontres et stocke
les informations dans le descripteur retourne */
struct jpeg_desc *read_jpeg(const char *filename)
{
    /* Allocations memoires */
    struct jpeg_desc *jdesc = malloc(sizeof(struct jpeg_desc));
    jpeg_desc->quantization_tables = calloc(MAX_QUANTIZATION_TABLES,
                                                sizeof(uint8_t*));

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

    /* Contenu lu */
    int32_t read = 0;
    /* Nombre de bits lus */
    uint8_t nb_read = 0;
    /* Taille de la section en cours de lecture */
    int32_t size = 0;
    /* Nombre de tables de quantification au sein d'une section DQT */
    uint8_t nb_tables_dqt = 0;
    /* Indices des tables (q : quantification / c : composante) */
    uint8_t iq = 0, ic = 0;

    /* Traitement du debut de l'image (marqueur SOI) */
    nb_read = read_bitstream(jpeg_desc->stream, MARKER_SIZE, &read, false);
    if (read != MARKER_SOI) {
        fprintf(stderr, "Le fichier JPEG debute par le marqueur '%04x' au lieu
                    d'un marqueur SOI '%04x'.\n", reader, MARKER_SOI);
        return EXIT_FAILURE;
    }

    /* === LECTURE DES SECTIONS JPEG === */

    nb_read = read_bitstream(jpeg_desc->stream, MARKER_SIZE, &read, false);
    while (read != MARKER_EOI || nb_read != 0) {
        nb_read = read_bitstream(jpeg_desc->stream, SECTION_SIZE, &size, false);
        size *= BYTE_SIZE;
        switch(read) {
            case MARKER_APP0:
                nb_read = read_bitstream(jpeg_desc->stream, 32, &read, false);
                char norm[4];
                sprintf(norm, "%ld", read);
                if (strcmp(norm, "JFIF")) {
                    fprintf(stderr, "Le fichier JPEG ne suit pas la norme JFIF.");
                    return EXIT_FAILURE;
                }
                break;
            case MARKER_COM;
                break;
            case MARKER_DQT:
                nb_tables_dqt = (size - MARKER_SIZE - SECTION_SIZE)
                                    / DQT_TABLE_SIZE;
                jpeg_desc->nb_quantization_tables += nb_tables_dqt;
                for (uint8_t i = 0; i < nb_tables_dqt; ++i) {
                    nb_read = read_bitstream(jpeg_desc->stream,
                                                DQT_PRECISION,
                                                &read,
                                                false);
                    nb_read = read_bitstream(jpeg_desc->stream,
                                                DQT_PRECISION,
                                                &iq,
                                                false);
                    jpeg_desc->quantization_tables[iq] = calloc(DQT_NB_VALUES
                                                            * sizeof(uint8_t));
                    for (uint8_t i = 0; i < DQT_NB_VALUES; ++i) {
                        nb_read = read_bitstream(jpeg_desc->stream,
                                                    sizeof(uint8_t),
                                                    &read,
                                                    false);
                        jpeg_desc->quantization_tables[iq][i] = (uint8_t) read;
                    }
                }
                break;
            case MARKER_SOF:
                nb_read = read_bitstream(jpeg_desc->stream,
                                            SOF_PRECISION,
                                            &read,
                                            false);
                nb_read = read_bitstream(jpeg_desc->stream,
                                            SOF_IMAGE_SIZE,
                                            jpeg_desc->width,
                                            false);
                nb_read = read_bitstream(jpeg_desc->stream,
                                            SOF_IMAGE_SIZE,
                                            jpeg_desc->heigth,
                                            false);
                nb_read = read_bitstream(jpeg_desc->stream,
                                            SOF_NB_COMPONENTS,
                                            jpeg_desc->nb_components,
                                            false);
                jpeg_desc->sampling_factors = calloc(jpeg_desc->nb_components,
                                                        sizeof(uint8_t**));
                for (uint8_t i = 0; i < jpeg_desc->nb_components; ++i) {
                    jpeg_desc->sampling_factors[i] = calloc(DIR_NB,
                                                                sizeof(uint8_t*));
                    jpeg_desc->sampling_factors[i][DIR_H] = calloc(1, sizeof(uint8_t));
                    nb_read = read_bitstream(jpeg_desc->stream,
                                                SOF_NB_COMPONENTS,
                                                jpeg_desc->sampling_factors[i][DIR_H],
                                                false);
                    jpeg_desc->sampling_factors[i][DIR_V] = calloc(1, sizeof(uint8_t));
                    nb_read = read_bitstream(jpeg_desc->stream,
                                                SOF_NB_COMPONENTS,
                                                jpeg_desc->sampling_factors[i][DIR_V],
                                                false);
                }
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
                    d'un marqueur EOI '%04x'.\n", section_marker, MARKER_EOI);
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
