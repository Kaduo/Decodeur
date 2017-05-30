/*******************************************************************************
Nom ......... : component.c
Role ........ : Fonctions des composantes
Auteurs .... : A. He - M. Nebra - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "component.h"
#include "zigzag.h"

/* Valeur minimale d'un coefficient */
const float MIN_COEFF = 0.0;
/* Valeur maximale d'un coefficient */
const float MAX_COEFF = 255.0;

/* Cree un bloc a partir d'une largeur et hauteur donnes */
struct block *create_block(size_t size)
{
    struct block *block = malloc(sizeof(struct block));
    block->size = size;
    block->coefficients = NULL;
    return block;
}

/* Retourne le coefficient a partir d'un bitstream et d'une magnitude donnes */
int16_t get_coefficient(struct bitstream *stream, uint8_t magnitude)
{
    uint32_t indice = 0;
    read_bitstream(stream, magnitude, &indice, true);
    /* Memorisation du signe de la valeur (1 => positif, 0 => negatif) */
    uint8_t signe = indice >> (magnitude - 1);
    /* Memorisation du rang de la valeur, en partant de la plus petite valeur
    suivant son signe */
    uint8_t rang = indice & ~(1 << (magnitude - 1));
    /* Calcul du coefficient */
    return (signe ? pow(2, magnitude - 1) : (1 - pow(2, magnitude))) + rang;
}

/* Definit la valeur DC d'un tableau de coefficients donne a partir d'une
table Huffman DC, d'un bitstream et de la valeur DC du predicateur */
void get_dc(struct huff_table *dc_table,
                    struct bitstream *stream,
                    int16_t previous_dc,
                    int16_t *coefficients)
{
    uint8_t magnitude = (uint8_t) next_huffman_value(dc_table, stream);
    coefficients[0] = previous_dc - get_coefficient(stream, magnitude);
}

/* Definit les valeurs ACs d'un tableau de coefficients donne a partir d'une
table Huffman AC et d'un bitstream */
void get_acs(struct huff_table *ac_table,
                        struct bitstream *stream,
                        int16_t *coefficients,
                        size_t length)
{
    for (size_t i = 1; i < length; ++i) {
        uint8_t symbole = (uint8_t) next_huffman_value(ac_table, stream);
        /* 1er cas : code EOB */
        if (symbole == 0x00) {
            return;
        } else if (symbole == 0xF0) {
            i += 16;
        } else if ((symbole << 4) == 0) {
            fprintf(stderr, "Symbole RLE '%d' interdit.", symbole);
            exit(EXIT_FAILURE);
        } else {
            i += (symbole >> 4);
            uint8_t magnitude = symbole & 0x0F;
            coefficients[i] = get_coefficient(stream, magnitude);
        }
    }
}

/* extracte un bloc frequentiel */
int16_t *extract(struct huff_table *dc_table,
                        struct huff_table *ac_table,
                        struct bitstream *stream,
                        int16_t previous_dc,
                        size_t length)
{
    int16_t *coefficients = calloc(length, sizeof(int16_t));
    get_dc(dc_table, stream, previous_dc, coefficients);
    get_acs(ac_table, stream, coefficients, length);
    return coefficients;
}

/* Retourne la quantification inverse d'un tableau de coefficients donne par
une table de quantification inverse donnee */
int16_t *inverse_quantization(const int16_t *coefficients,
                                uint8_t *quantization_table,
                                size_t length)
{
    int16_t *inverse = calloc(length, sizeof(int16_t));
    for (size_t i = 0; i < length; ++i) {
        inverse[i] = coefficients[i] * (int16_t) quantization_table[i];
    }
    return inverse;
}

/* Retourne le tableau zigzag inverse d'un tableau donne */
int16_t *inverse_zigzag(const int16_t *coefficients, size_t size)
{
    int16_t *inverse = calloc(size * size, sizeof(int16_t));
    inverse = get_inverse(coefficients, size);
    return inverse;
}

/* Retourne le coefficient de la formule de l'IDCT */
float coeff_idct(size_t x)
{
    return (x == 0 ? 1/sqrt(2.0) : 1);
}

/* Retourne l'IDCT d'un tableau de coefficients donne */
int16_t *idct(const int16_t *coefficients, size_t size)
{
    /* Initialisation du nouveau bloc */
    int16_t *inverse = calloc(size * size, sizeof(int16_t));
    /* Initialisation des constantes */
    float pi = acos(-1);
    float offset = 128.0;
    float facteur = 1/sqrt(2.0 * (double) size);
    /* Initialisation des resultats partiels */
    float somme = 0.0;
    float resultat = 0.0;
    /* Calcul pour chaque coefficient du bloc */
    for(size_t y = 0; y < size; ++y) {
        for(size_t x = 0; x < size; ++x) {
            /* Initialisation de la somme */
            somme = 0.0;
            /* Calcul de la double somme */
            for(size_t lambda = 0; lambda < size; ++lambda) {
                for(size_t mu = 0; mu < size; ++mu) {
                    somme += coeff_idct(lambda)
                                * coeff_idct(mu)
                                * cos((2*x+1)*lambda*pi/(2*size))
                                * cos((2*y+1)*mu*pi/(2*size))
                                * coefficients[mu*size+lambda];
                }
            }
            /* Calcul du resultat */
            resultat = facteur * somme + offset;
            if (resultat < MIN_COEFF) {
                resultat = MIN_COEFF;
            } else if (resultat > MAX_COEFF) {
                resultat = MAX_COEFF;
            }
            inverse[y*size+x] = (int16_t) round(resultat);
        }
    }
    return inverse;
}

/* Reconstruit une composante */
int16_t *get_component(struct bitstream *stream,
                        struct huff_table *dc_table,
                        struct huff_table *ac_table,
                        uint8_t *quantization_table,
                        int16_t previous_dc,
                        size_t size)
{
    /* 1. Extraction - extraction */
    int16_t *extracted = extract(dc_table,
                                  ac_table,
                                  stream,
                                  previous_dc,
                                  size*size);
    /* 2. Quantification inverse */
    int16_t *quantization = inverse_quantization(extracted,
                                                  quantization_table,
                                                  size*size);
    free(extracted);
    /* 3. Reorganisation zigzag */
    int16_t *zigzag = inverse_zigzag(quantization, size);
    free(quantization);
    /* 4. Transformee en cosinus discrete inverse (iDCT) */
    int16_t *component = idct(zigzag, size);
    free(zigzag);
    /* Retour de la composante */
    return component;
}
