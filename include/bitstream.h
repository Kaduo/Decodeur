/*******************************************************************************
Nom ......... : bitstream.h
Role ........ : Prototypes des fonctions de gestion du flux de données
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __BITSTREAM_H__
#define __BITSTREAM_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

struct bitstream;

extern struct bitstream *create_bitstream(const char *filename);

extern void close_bitstream(struct bitstream *stream);

extern uint8_t read_bitstream(struct bitstream *stream,
                              uint8_t nb_bits,
                              uint32_t *dest,
                              bool discard_byte_stuffing);

extern bool end_of_bitstream(struct bitstream *stream);

extern void skip_bitstream_until(struct bitstream *stream, uint8_t byte);

#endif
