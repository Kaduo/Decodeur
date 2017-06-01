/*******************************************************************************
Nom ......... : component.h
Role ........ : Prototypes des fonctions des composantes
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "jpeg_reader.h"
#include "huffman.h"
#include "bitstream.h"

/* Retourne le coefficient a partir d'un bitstream et d'une magnitude donnes */
extern int16_t get_coefficient(struct bitstream *stream, uint8_t magnitude, bool discard_byte_stuffing);

/* Definit la valeur DC d'un tableau de coefficients donne a partir d'une
table Huffman DC, d'un bitstream et de la valeur DC du predicateur */
extern void get_dc(struct huff_table *dc_table,
                    struct bitstream *stream,
                    int16_t *previous_dc,
                    int16_t *coefficients);

/* Definit les valeurs ACs d'un tableau de coefficients donne a partir d'une
table Huffman AC et d'un bitstream */
extern void get_acs(struct huff_table *ac_table,
                        struct bitstream *stream,
                        int16_t *coefficients,
                        size_t length);

/* Extrait une composante */
extern int16_t *extract(struct huff_table *dc_table,
                                    struct huff_table *ac_table,
                                    struct bitstream *stream,
                                    int16_t *previous_dc,
                                    size_t length);

/* Retourne la quantification inverse d'un tableau de coefficients donne par
une table de quantification inverse donnee */
extern int16_t *inverse_quantization(const int16_t *coefficients,
                                            uint8_t *quantization_table,
                                            size_t length);

/* Retourne le tableau zigzag inverse d'un tableau donne */
extern int16_t *inverse_zigzag(const int16_t *coefficients, size_t size);

/* Retourne le coefficient de la formule de l'IDCT */
extern float coeff_idct(size_t x);

/* Retourne l'IDCT d'un tableau de coefficients donne */
extern int16_t *idct(const int16_t *coefficients, size_t size);

/* Reconstruit une composante */
extern int16_t *get_component(struct bitstream *stream,
                        struct huff_table *dc_table,
                        struct huff_table *ac_table,
                        uint8_t *quantization_table,
                        int16_t *previous_dc,
                        size_t size);

#endif
