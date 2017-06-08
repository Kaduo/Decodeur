/*******************************************************************************
Nom ......... : huffman.h
Role ........ : Prototypes des fonctions de gestion des tables de Huffman
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <stdint.h>
#include <stdbool.h>
#include "bitstream.h"

/* Structure représentant une table de Huffman */
struct huff_table;

/* Charge une table de Huffman */
extern struct huff_table *load_huffman_table(struct bitstream *stream,
                                             uint16_t *nb_byte_read);

/* Décode et renvoie le symbole Huffman suivant */
extern int8_t next_huffman_value(struct huff_table *table,
                                 struct bitstream *stream);

/* Décode et renvoie le symbole Huffman suivant en comptant le nombre de bits lus */
extern int8_t next_huffman_value_count(struct huff_table *table,
                                       struct bitstream *stream,
                                       uint8_t *nb_bits_read);

/* Libère la place mémoire occupée par une table de Huffman */
extern void free_huffman_table(struct huff_table *table);

#endif
