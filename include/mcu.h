/*******************************************************************************
Nom ......... : mcu.h
Role ........ : Prototypes des fonctions de decodage de MCU
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __MCU_H__
#define __MCU_H__

#include "jpeg_reader.h"

/* Structure representant un MCU */
struct mcu {
    size_t width;
    size_t height;
    struct block *blocks[COMP_NB];
};

/* Cree un mcu a partir d'une taille donnee */
extern struct block *create_block(size_t size);

/* Retourne le coefficient a partir d'un bitstream et d'une magnitude donnes */
extern int16_t get_coefficient(struct bitstream *stream, uint8_t magnitude);

/* Definit la valeur DC d'un bloc donne a partir d'une table Huffman DC, d'un
bitstream et de la valeur DC du predicateur */
extern void get_dc(struct block *block,
                    struct huff_table *dc_table,
                    struct bitstream *stream,
                    int16_t previous_dc);

/* Definit les valeurs ACs d'un bloc donne a partir d'une table Huffman AC et
d'un bitstream */
extern void get_acs(struct block *block,
                    struct huff_table *ac_table,
                    struct bitstream *stream);

/* Decompresse un bloc frequentiel */
extern struct block *decompress(struct huff_table *dc_table,
                                    struct huff_table *ac_table,
                                    struct bitstream *stream,
                                    int16_t previous_dc,
                                    size_t size);

/* Retourne la quantification inverse d'un bloc donne par une table donnee */
extern struct block *inverse_quantization(const struct block *block,
                                            uint8_t *quantization_table);

/* Retourne le bloc zigzag inverse d'un bloc donne */
extern struct block *inverse_zigzag(const struct block *block);

/* Retourne le coefficient de la formule de l'IDCT */
extern float coeff_idct(size_t x);

/* Retourne l'IDCT d'un bloc donne */
extern struct block *idct(const struct block *block);

/* Libere en memoire l'espace occupe par un bloc donne */
extern void free_block(struct block *block);

#endif
