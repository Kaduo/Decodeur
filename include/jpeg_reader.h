/*******************************************************************************
Nom ......... : jpeg_reader.h
Role ........ : Prototypes des fonctions de lecture de l'entete JPEG
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __JPEG_DESC_H__
#define __JPEG_DESC_H__

#include <stdint.h>
#include <stdbool.h>
#include "trace.h"

#include "bitstream.h"

/* Enumeration des composantes de couleur Y, Cb et Cr */
enum component {
    COMP_Y,
    COMP_Cb,
    COMP_Cr,
    /* Sentinelle */
    COMP_NB
};

/* Enumeration des directions horizontale (H) et verticale (V) */
enum direction {
    DIR_H,
    DIR_V,
    /* Sentinelle */
    DIR_NB
};

/* Enumeration des composantes fréquentielles DC et AC */
enum acdc {
    DC = 0,
    AC = 1,
    /* Sentinelle */
    ACDC_NB
};

/* Structure representant un descripteur contenant toutes les informations lues
dans les entêtes des sections JPEG */
struct jpeg_desc;

/* Ouvre le fichier JPEG, lit tous les entetes de sections rencontres et stocke
les informations dans le descripteur retourne */
extern struct jpeg_desc *read_jpeg(const char *filename);

/* Ferme un descripteur prealablement ouvert, en liberant toute la memoire
necessaire */
extern void close_jpeg(struct jpeg_desc *jpeg);

/* Retourne le nom de fichier de l’image ouverte */
extern char *get_filename(const struct jpeg_desc *jpeg);

/* Retourne l’adresse d’un bitstream permettant de lire les donnees brutes
d’image encodées dans le flux */
extern struct bitstream *get_bitstream(const struct jpeg_desc *jpeg);

/* Retourne le nombre de tables de quantifications */
extern uint8_t get_nb_quantization_tables(const struct jpeg_desc *jpeg);

/* Retourne l’adresse memoire de la i_eme table de quantification */
extern uint8_t *get_quantization_table(const struct jpeg_desc *jpeg,
                                       uint8_t index);

/* Retourne le nombre de tables de Huffman de la composante acdc (DC ou AC) */
extern uint8_t get_nb_huffman_tables(const struct jpeg_desc *jpeg,
                                     enum acdc acdc);

/* Retourne la i_eme table de Huffman de la composante DC ou AC */
extern struct huff_table *get_huffman_table(const struct jpeg_desc *jpeg,
                                            enum acdc acdc, uint8_t index);

/* Retourne la taille de l’image (nombre de pixels) dans la direction dir
(horizontale ou verticale) */
extern uint16_t get_image_size(struct jpeg_desc *jpeg, enum direction dir);

/* Retourne le nombre de composantes */
extern uint8_t get_nb_components(const struct jpeg_desc *jpeg);

/* Retourne l’identifiant de la frame_comp_index composante definie dans le frame
header */
extern uint8_t get_frame_component_id(const struct jpeg_desc *jpeg,
                                      uint8_t frame_comp_index);

/* Retourne le facteur de sous-echantillonnage dans la direction dir de la
frame_comp_index composante de couleur */
extern uint8_t get_frame_component_sampling_factor(const struct jpeg_desc *jpeg,
                                                   enum direction dir,
                                                   uint8_t frame_comp_index);

/* Retourne l’index de la table de quantification de la frame_comp_index
composante de couleur */
extern uint8_t get_frame_component_quant_index(const struct jpeg_desc *jpeg,
                                               uint8_t frame_comp_index);

/* Retourne l’identifiant de la scan_comp_index composante lue dans le scan */
extern uint8_t get_scan_component_id(const struct jpeg_desc *jpeg,
                                     uint8_t scan_comp_index);

/* Retourne l’index de la table de Huffman (DC ou AC) associee a la
scan_comp_index composante lue dans le scan */
extern uint8_t get_scan_component_huffman_index(const struct jpeg_desc *jpeg,
                                                enum acdc acdc,
                                                uint8_t scan_comp_index);

#endif
