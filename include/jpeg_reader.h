/*******************************************************************************
Nom ......... : jpeg_reader.h
Role ........ : Prototypes des fonctions de lecture de l'entete JPEG
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __JPEG_DESC_H__
#define __JPEG_DESC_H__

#include <stdint.h>
#include <stdbool.h>

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

/* Enumeration des marqueurs JPEG */
enum marker {
    MARKER_SOI = 0xffd8,
    MARKER_EOI = 0xffd9,
    MARKER_APP0 = 0xffe0,
    MARKER_COM = 0xfffe,
    MARKER_DQT = 0xffdb,
    MARKER_SOF = 0xffc0
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

// access to stream, placed just at the beginning of the scan raw data
extern struct bitstream *get_bitstream(const struct jpeg_desc *jpeg);

// from DQT
extern uint8_t get_nb_quantization_tables(const struct jpeg_desc *jpeg);
extern uint8_t *get_quantization_table(const struct jpeg_desc *jpeg,
                                       uint8_t index);

// from DHT
extern uint8_t get_nb_huffman_tables(const struct jpeg_desc *jpeg,
                                     enum acdc acdc);
extern struct huff_table *get_huffman_table(const struct jpeg_desc *jpeg,
                                            enum acdc acdc, uint8_t index);

// from Frame Header SOF0
extern uint16_t get_image_size(struct jpeg_desc *jpeg, enum direction dir);
extern uint8_t get_nb_components(const struct jpeg_desc *jpeg);

extern uint8_t get_frame_component_id(const struct jpeg_desc *jpeg,
                                      uint8_t frame_comp_index);
extern uint8_t get_frame_component_sampling_factor(const struct jpeg_desc *jpeg,
                                                   enum direction dir,
                                                   uint8_t frame_comp_index);
extern uint8_t get_frame_component_quant_index(const struct jpeg_desc *jpeg,
                                               uint8_t frame_comp_index);

// from Scan Header SOS
extern uint8_t get_scan_component_id(const struct jpeg_desc *jpeg,
                                     uint8_t scan_comp_index);
extern uint8_t get_scan_component_huffman_index(const struct jpeg_desc *jpeg,
                                                enum acdc,
                                                uint8_t scan_comp_index);

#endif
