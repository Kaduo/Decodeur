/*******************************************************************************
Nom ......... : jpeg_reader.h
Role ........ : Fonctions de lecture de l'entete JPEG
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "jpeg_reader.h"
#include "huffman.h"

/* Taille en bits d'un marqueur de section */
#define MARKER_SIZE 16
/* Taille en bits d'une longueur de section */
#define SECTION_SIZE 16
/* Nombre maximum de tables de quantification */
#define MAX_QUANT_TABLES 4
/* Nombre maximum de tables de Huffman par composante (AC/DC) */
#define MAX_HUFFMAN_TABLES 4
/* Taille en bits d'un octet */
#define BYTE_SIZE 8

/* == SECTION APP == */

/* Taille en bits de l'information de la norme (JFIF attendu) */
#define APP_NORM 5
/* Norme attendue */
static const char NORM[] = "JFIF";

/* == SECTION DQT == */

/* Taille en bits de l'information de la precision */
#define DQT_PRECISION 4
/* Taille en bits de l'information de l'indice iq */
#define DQT_INDEX_Q 4
/* Taille en octets des valeurs de la table de quantification  */
#define DQT_NB_VALUES 64
/* Taille en bits necessaire a la definition d'une table de quantification */
#define DQT_TABLE (DQT_PRECISION + DQT_INDEX_Q + DQT_NB_VALUES * BYTE_SIZE)

/* == SECTION SOF == */

/* Taille en bits de l'information de precision */
#define SOF_PRECISION 8
/* Taille en bits de l'information de la largeur ou de la hauteur de l'image */
#define SOF_IMAGE_SIZE 16
/* Taille en bits de l'informtaion du nombre de composantes */
#define SOF_NB_COMPONENTS 8
/* Taille en bits de l'information de l'identifiant de la composante */
#define SOF_IC 8
/* Taille en bits de l'information d'un facteur d'echantillonnage */
#define SOF_SAMPLING_FACTOR 4
/* Taille en bits de l'information de l'indice iq */
#define SOF_INDEX_Q 4

/* == SECTION DHT == */

/* Taille en bits de la partie inutilisee (0 sinon erreur) */
#define DHT_UNUSED 3
/* Taille en bits de l'information du type (AC ou DC) */
#define DHT_TYPE 1
/* Taille en bits de l'information de l'indice */
#define DHT_INDEX 4

/* == SECTION SOS == */

/* Taille en bits de l'information du nombre de composantes */
#define SOS_NB_COMPONENTS 8
/* Taille en bits de l'information de l'identifiant de la composante */
#define SOS_IC 8
/* Taille en bits de l'information de l'indice d'une table de Huffman */
#define SOS_INDEX_H 4
/* Taille en bits des parties inutilisees (selection et approximation) */
#define SOS_UNUSED 24

/* Structure representant une frame component */
struct frame_component {
    uint8_t ic; // Identifiant de la composante
    uint8_t **sampling_factors; // Facteurs d'echantillonnage
    uint8_t iq; // Indice de la table de quantification associee
};

/* Structure representant une scan component */
struct scan_component {
    uint8_t ic; // Identifiant de la composante
    uint8_t idc; // Indice de la table de Huffman DC associee
    uint8_t iac; // Indice de la table de Huffman AC associee
};

/* Structure representant un descripteur contenant toutes les informations lues
dans les entetes des sections JPEG */
struct jpeg_desc {
    char *filename; // Nom du fichier
    uint16_t *size; // Dimensions de l'image
    uint8_t nb_components; // Nombre de composantes
    struct frame_component *frame_components; // Composantes du frame header
    struct scan_component *scan_components; // Composantes du scan
    uint8_t nb_quantization_tables; // Nombre de tables de quantification
    uint8_t **quantization_tables; // Tables de quantification
    uint8_t *nb_huffman_tables; // Nombre de tables de Huffman par composante
    struct huff_table *huff_tables; // Tables de Huffman
    struct bitstream *stream; // Flux de donnees
};

/* Ouvre le fichier JPEG, lit tous les entetes de sections rencontres et stocke
les informations dans le descripteur retourne */
struct jpeg_desc *read_jpeg(const char *filename)
{
    /* Initialisation de la structure jpeg_desc */
    struct jpeg_desc *jpeg = malloc(sizeof(struct jpeg_desc));
    jpeg->size = callco(DIR_NB, sizeof(uint16_t));
    jpeg->nb_components = 0;
    jpeg->nb_quantization_tables = 0;
    jpeg->quantization_tables = calloc(MAX_QUANT_TABLES, sizeof(uint8_t));

    /* Recuperation du nom du fichier */
    jpeg->filename = calloc(strlen(filename) + 1, sizeof(char));
    if (!strcpy(jpeg->filename, filename)) {
        fprintf(stderr, "Le nom du fichier '%s' n'a pas pu etre recupere.\n",
                    filename);
        return EXIT_FAILURE;
    }

    /* Recuperation du flux de donnees */
    jpeg->stream = create_bitstream(filename);

    /* === LECTURE DES SECTIONS JPEG === */

    /* Variable qui stockera le contenu lu */
    int32_t read = 0;
    /* Variable qui stockera le nombre de bits lus */
    uint8_t nb_read = 0;
    /* Variable qui stockera la longueur de la section */
    int32_t size = 0;
    /* Variable qui stockera les bits restant a lire dans une section */
    int32_t unread = 0;
    /* Variable qui stockera le nombre de tables de quantification */
    uint8_t nb_dqt = 0;
    /* Variable qui stockera l'indice iq */
    uint8_t iq = 0;

    /* Traitement du debut de l'image (marqueur SOI) */
    read_bitstream(jpeg->stream, MARKER_SIZE, &read, false);
    if (read != MARKER_SOI) {
        fprintf(stderr, "Le fichier JPEG debute par le marqueur '%04x' au lieu
                    d'un marqueur SOI '%04x'.\n", read, MARKER_SOI);
        return EXIT_FAILURE;
    }

    /* Lecture du marqueur */
    nb_read = read_bitstream(jpeg->stream, MARKER_SIZE, &read, false);
    /* Boucle de lecture a deux conditions d'arret : soit le marqueur est EOI,
    soit il n'y a plus de bits a lire */
    while (read != MARKER_EOI || nb_read != 0) {
        /* Lecture de la longueur de la section */
        read_bitstream(jpeg->stream, SECTION_SIZE, &size, false);
        /* Comparaison du marqueur lu avec les marqueurs connus */
        switch(read) {
            /* Application Data */
            case MARKER_APP0:

                break;
            /* Commentaire */
            case MARKER_COM:
                /* Lecture de tous les bits de la section */
                unread = size * BYTE_SIZE - MARKER_SIZE - SECTION_SIZE;
                read_bitstream(jpeg->stream, unread, &read, false);
                break;
            /* Define Quantization Table */
            case MARKER_DQT:
                /* Calcul du nombre de tables de quantification */
                unread = size * BYTE_SIZE - MARKER_SIZE - SECTION_SIZE;
                nb_dqt = unread / DQT_TABLE;
                /* Lecture de chaque table de quantification */
                for (uint8_t i = jpeg->nb_quantization_tables; i < nb_dqt; ++i) {
                    /* Incrementation du nombre de tables dans la structure */
                    jpeg->nb_quantization_tables++;
                    read_bitstream(jpeg->stream, DQT_PRECISION, &read, false);
                    read_bitstream(jpeg->stream, DQT_INDEX_Q, &iq, false);
                    jpeg->quantization_tables[iq] = calloc(DQT_NB_VALUES,
                                                            sizeof(uint8_t));
                    for (uint8_t i = 0; i < DQT_NB_VALUES; ++i) {
                        read_bitstream(jpeg->stream, BYTE_SIZE, &read, false);
                        jpeg->quantization_tables[iq][i] = (uint8_t) read;
                    }
                }
                break;
            /* Start Of Frame */
            case MARKER_SOF:
                read_bitstream(jpeg->stream, SOF_PRECISION, &read, false);
                read_bitstream(jpeg->stream, SOF_IMAGE_SIZE, &read, false);
                jpeg->size[DIR_H] = (uint16_t) read;
                read_bitstream(jpeg->stream, SOF_IMAGE_SIZE, &read, false);
                jpeg->size[DIR_V] = (uint16_t) read;
                nb_read = read_bitstream(jpeg->stream,
                                            SOF_NB_COMPONENTS,
                                            jpeg->nb_components,
                                            false);
                jpeg->components = calloc(jpeg->nb_components,
                                                sizeof(struct component));
                for (uint8_t i = 0; i < jpeg->nb_components; ++i) {
                    nb_read = read_bitstream(jpeg->stream,
                                                SOF_INDEX,
                                                jpeg->components[i]->id,
                                                false);
                    jpeg->components[i]->sampling_factors = calloc(DIR_NB,
                                                            sizeof(uint8_t*));
                    jpeg->components[i]->sampling_factors[DIR_H] =
                                                    calloc(1, sizeof(uint8_t));
                    nb_read = read_bitstream(jpeg->stream,
                            SOF_NB_COMPONENTS,
                            jpeg->components[i]->sampling_factors[DIR_H],
                            false);
                    jpeg->components[i]->sampling_factors[DIR_V] =
                                                    calloc(1, sizeof(uint8_t));
                    nb_read = read_bitstream(jpeg->stream,
                            SOF_NB_COMPONENTS,
                            jpeg->components[i]->sampling_factors[DIR_H],
                            false);
                    nb_read = read_bitstream(jpeg->stream,
                                                SOF_INDEX,
                                                jpeg->components[i]->iq,
                                                false);
                }
                break;
            case MARKER_DHT:
                nb_read = read_bitstream(jpeg->stream,
                                            DHT_UNUSED,
                                            &read,
                                            false);
                if (read != 0) {
                    fprintf(stderr, "Marqueur DHT : les trois premiers bits
                            ne valent pas 0\n.");
                    return EXIT_FAILURE;
                }
                while (size - nb_bytes_read > 0) {
                    nb_read = read_bitstream(jpeg->stream,
                                                DHT_TYPE,
                                                &huff_type,
                                                false);
                    nb_read = read_bitstream(jpeg->stream,
                                                DHT_INDEX,
                                                &huff_index,
                                                false);

                    huff_tables[huff_type][huff_index] = load_huffman_table(
                                                            stream,
                                                            &nb_byte_read);
                }
                break;
            case MARKER_SOS:
                nb_read = read_bitstream(jpeg->stream,
                                            SOS_NB_COMPONENTS,
                                            &read,
                                            false);
                for (int32_t i = 0; i < read; ++i) {
                    nb_read = read_bitstream(jpeg->stream,
                                                SOS_NB_COMPONENTS,
                                                &(jpeg->scan_components[i]->ic),
                                                false);
                    nb_read = read_bitstream(jpeg->stream,
                                                SOS_INDEX,
                                                &(jpeg->scan_components[i]->idc),
                                                false);
                    nb_read = read_bitstream(jpeg->stream,
                                                SOS_INDEX,
                                                &(jpeg->scan_components[i]->iac),
                                                false);
                }
                nb_read = read_bitstream(jpeg->stream,
                                            SOS_UNUSED,
                                            &read,
                                            false);

                break;
            default:
                fprintf(stderr, "Marqueur lu '%04x' inconnu.\n", read);
                return EXIT_FAILURE;
        }
        /* Lecture du marqueur suivant */
        nb_read = read_bitstream(jpeg->stream,
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

    return jpeg;
}

/* Ferme un descripteur prealablement ouvert, en liberant toute la memoire
necessaire */
void close_jpeg(struct jpeg_desc *jpeg)
{

}

/* Retourne le nom de fichier de l’image ouverte */
char *get_filename(const struct jpeg_desc *jpeg)
{
    return jpeg->filename;
}

/* Retourne l’adresse d’un bitstream permettant de lire les donnees brutes
d’image encodées dans le flux */
struct bitstream *get_bitstream(const struct jpeg_desc *jpeg)
{
    return jpeg->stream;
}

/* Retourne le nombre de tables de quantifications */
uint8_t get_nb_quantization_tables(const struct jpeg_desc *jpeg)
{
    return jpeg->nb_quantization_tables;
}

/* Retourne l’adresse memoire de la i_eme table de quantification */
uint8_t *get_quantization_table(const struct jpeg_desc *jpeg, uint8_t index)
{
    if (index > jpeg->nb_quantization_tables) {
        fprintf(stderr, "Table de quantification n°%hhu demandee (max : %hhu)\n",
                    index, jpeg->nb_quantization_tables);
        return EXIT_FAILURE;
    }
    return jpeg->quantization_tables[index];
}

/* Retourne le nombre de tables de Huffman de la composante acdc (DC ou AC) */
uint8_t get_nb_huffman_tables(const struct jpeg_desc *jpeg,
                                     enum acdc acdc)
{
    return jpeg->nb_huffman_tables[acdc];
}

/* Retourne la index table de Huffman de la composante DC ou AC */
struct huff_table *get_huffman_table(const struct jpeg_desc *jpeg,
                                            enum acdc acdc, uint8_t index)
{
    return jpeg->huff_tables[acdc][index];
}

/* Retourne la taille de l’image (nombre de pixels) dans la direction dir
(horizontale ou verticale) */
uint16_t get_image_size(struct jpeg_desc *jpeg, enum direction dir)
{
    return jpeg->size[dir];
}

/* Retourne le nombre de composantes */
uint8_t get_nb_components(const struct jpeg_desc *jpeg)
{
    return jpeg->nb_components;
}

/* Retourne l’identifiant de la frame_comp_index composante definie dans le frame
header */
uint8_t get_frame_component_id(const struct jpeg_desc *jpeg,
                                      uint8_t frame_comp_index)
{
    return jpeg->components[frame_comp_index]->id_frame;
}

/* Retourne le facteur de sous-echantillonnage dans la direction dir de la
frame_comp_index composante de couleur */
uint8_t get_frame_component_sampling_factor(const struct jpeg_desc *jpeg,
                                                   enum direction dir,
                                                   uint8_t frame_comp_index)
{
    return jpeg->components[frame_comp_index]->sampling_factors[dir];
}

/* Retourne l’index de la table de quantification de la frame_comp_index
composante de couleur */
uint8_t get_frame_component_quant_index(const struct jpeg_desc *jpeg,
                                               uint8_t frame_comp_index)
{
    return jpeg->components[frame_comp_index]->index_q;
}

/* Retourne l’identifiant de la scan_comp_index composante lue dans le scan */
uint8_t get_scan_component_id(const struct jpeg_desc *jpeg,
                                     uint8_t scan_comp_index)
{
    return jpeg->scan_components[scan_comp_index]->id;
}

/* Retourne l’index de la table de Huffman (DC ou AC) associee a la
scan_comp_index composante lue dans le scan */
uint8_t get_scan_component_huffman_index(const struct jpeg_desc *jpeg,
                                                enum acdc,
                                                uint8_t scan_comp_index)
{
    return jpeg->scan_components[scan_comp_index]->huff_index[acdc];
}
