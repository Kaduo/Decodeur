/*******************************************************************************
Nom ......... : jpeg2ppm.h
Role ........ : Prototypes des fonctions du point d'entrée du programme
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#ifndef __JPEG2PPM_H__
#define __JPEG2PPM_H__

#include <stdlib.h>
#include <stdio.h>
#include "jpeg_reader.h"

/* Indique si l'image est en couleur */
bool est_couleur(const struct jpeg_desc *jpeg);

/* Récupère les tables de Huffman dans l'ordre Y_dc, Y_ac, puis C_dc et C_ac (si elles existent) */
struct huff_table ***get_huff_tables(const struct jpeg_desc *jpeg);

/* Récupère les tables de quantification dans l'ordre Y puis C (si elle existe) */
uint8_t **get_quant_tables(const struct jpeg_desc *jpeg);

/* Renvoie un tableau donnant l'ordre des composantes dans les MCUs */
enum component *get_components_order(const struct jpeg_desc *jpeg, uint8_t factors[COMP_NB][DIR_NB]);

/* Récupère le nom du fichier de sortie */
char *get_outfile_name(const char *filename, bool colored);

#endif
