/*******************************************************************************
Nom ......... : component.c
Role ........ : Fonctions des composantes
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "component.h"
#include "zigzag.h"
#include "shared.h"
#include "trace.h"

/* Valeur approchée de pi */
#define PI 3.14159265359
/* Valeur minimale d'un coefficient */
#define MIN_COEFF 0.0
/* Valeur maximale d'un coefficient */
#define MAX_COEFF 255.0

/* Retourne le coefficient a partir d'un bitstream et d'une magnitude donnes */
int16_t get_coefficient(struct bitstream *stream, uint8_t magnitude, bool discard_byte_stuffing)
{
    uint32_t indice = 0;
    read_bitstream(stream, magnitude, &indice, discard_byte_stuffing);
    /* Memorisation du signe de la valeur (1 => positif, 0 => negatif) */
    uint16_t signe = indice >> (magnitude - 1);
    /* Memorisation du rang de la valeur, en partant de la plus petite valeur
    suivant son signe */
    uint16_t rang = indice & ~(1 << (magnitude - 1));
    /* Calcul du coefficient */
    return (signe ? pow(2, magnitude - 1) : (1 - pow(2, magnitude))) + rang;
}

/* Definit la valeur DC d'un tableau de coefficients donne a partir d'une
table Huffman DC, d'un bitstream et de la valeur DC du predicateur */
void get_dc(struct huff_table *dc_table,
                    struct bitstream *stream,
                    int16_t *previous_dc,
                    int16_t *coefficients)
{
    uint8_t nb_read;
    uint8_t magnitude = (uint8_t) next_huffman_value_count(dc_table, stream, &nb_read);
    trace("Magnitude DC : %02x(%hhu)\n", magnitude, nb_read);
    int16_t new_value = *previous_dc + get_coefficient(stream, magnitude, true);
    coefficients[0] = new_value;
    *previous_dc = new_value;
}

/* Definit les valeurs ACs d'un tableau de coefficients donne a partir d'une
table Huffman AC et d'un bitstream */
void get_acs(struct huff_table *ac_table,
                struct bitstream *stream,
                int16_t *coefficients)
{
    trace("Magnitudes ACs : ");
    for (size_t i = 1; i < COMPONENT_SIZE; ++i) {
        uint8_t nb_read;
        uint8_t symbole = (uint8_t) next_huffman_value_count(ac_table, stream, &nb_read);
        trace("%02x(%hhu) ", symbole, nb_read);
        /* 1er cas : code EOB */
        if (symbole == 0x00) {
            return;
        /* 2e cas : code ZRL */
        } else if (symbole == 0xF0) {
            i += 15;
        /* 3e cas : 0x?0 (symbole invalide) */
        } else if ((symbole << 4) == 0) {
            fprintf(stderr, "Symbole RLE '%04x' interdit.", symbole);
            exit(EXIT_FAILURE);
        /* 4e cas : 0xab */
        } else {
            i += (symbole >> 4);
            uint8_t magnitude = symbole & 0x0F;
            coefficients[i] = get_coefficient(stream, magnitude, true);
        }
    }
    trace("\n");
}

/* Extrait un bloc frequentiel */
int16_t *extract(struct huff_table *dc_table,
                    struct huff_table *ac_table,
                    struct bitstream *stream,
                    int16_t *previous_dc)
{
    int16_t *coefficients = calloc(COMPONENT_SIZE, sizeof(int16_t));
    get_dc(dc_table, stream, previous_dc, coefficients);
    get_acs(ac_table, stream, coefficients);
    return coefficients;
}

/* Retourne la quantification inverse d'un tableau de coefficients donne par
une table de quantification inverse donnee */
int16_t *inverse_quantization(const int16_t *coefficients,
                                uint8_t *quantization_table)
{
    int16_t *inverse = calloc(COMPONENT_SIZE, sizeof(int16_t));
    for (size_t i = 0; i < COMPONENT_SIZE; ++i) {
        inverse[i] = coefficients[i] * (int16_t) quantization_table[i];
    }
    return inverse;
}

/* Retourne le tableau zigzag inverse d'un tableau donne */
int16_t *inverse_zigzag(const int16_t *coefficients)
{
    int16_t *inverse = get_inverse(coefficients, BLOCK_SIZE);
    return inverse;
}

/* Retourne le coefficient de la formule de l'IDCT */
float coeff_idct(size_t x)
{
    return (x == 0 ? 1/sqrt(2.0) : 1);
}

/* Retourne l'IDCT d'un tableau de coefficients donne */
int16_t *idct(const int16_t *coefficients)
{
    /* Initialisation du nouveau bloc */
    int16_t *inverse = calloc(COMPONENT_SIZE, sizeof(int16_t));
    /* Initialisation des constantes */
    float offset = 128.0;
    float facteur = 1/sqrt(2.0 * (double) BLOCK_SIZE);
    /* Initialisation des resultats partiels */
    float somme = 0.0;
    float resultat = 0.0;
    /* Calcul pour chaque coefficient du bloc */
    for(size_t y = 0; y < BLOCK_SIZE; ++y) {
        for(size_t x = 0; x < BLOCK_SIZE; ++x) {
            /* Initialisation de la somme */
            somme = 0.0;
            /* Calcul de la double somme */
            for(size_t lambda = 0; lambda < BLOCK_SIZE; ++lambda) {
                for(size_t mu = 0; mu < BLOCK_SIZE; ++mu) {
                    somme += coeff_idct(lambda)
                                * coeff_idct(mu)
                                * cos((2*x+1)*lambda*PI/(2*BLOCK_SIZE))
                                * cos((2*y+1)*mu*PI/(2*BLOCK_SIZE))
                                * coefficients[mu*BLOCK_SIZE+lambda];
                }
            }
            /* Calcul du resultat */
            resultat = facteur * somme + offset;
            if (resultat < MIN_COEFF) {
                resultat = MIN_COEFF;
            } else if (resultat > MAX_COEFF) {
                resultat = MAX_COEFF;
            }
            inverse[y*BLOCK_SIZE+x] = (int16_t) round(resultat);
        }
    }
    return inverse;
}

/* Reconstruit une composante */
int16_t *get_component(struct bitstream *stream,
                        struct huff_table *dc_table,
                        struct huff_table *ac_table,
                        uint8_t *quantization_table,
                        int16_t *previous_dc)
{
    /* 1. Extraction */
    trace("#### Extracted :\n");
    int16_t *extracted = extract(dc_table,
                                  ac_table,
                                  stream,
                                  previous_dc);
    #ifdef DEBUG
    for (size_t i = 0; i < COMPONENT_SIZE; i++) {
       trace("%04x ", extracted[i]);
    }
    trace("\n\n");
    #endif

    /* 2. Quantification inverse */
    int16_t *quantization = inverse_quantization(extracted,
                                                  quantization_table);
    #ifdef DEBUG
    trace("#### Quantification inverse :\n");
    for (size_t i = 0; i < COMPONENT_SIZE; ++i) {
        trace("%04x ", quantization[i]);
    }
    trace("\n\n");
    #endif

    /* 3. Reorganisation zigzag */
    int16_t *zigzag = inverse_zigzag(quantization);
    #ifdef DEBUG
    trace("#### Zigzag inverse :\n");
    for (size_t i = 0; i < COMPONENT_SIZE; ++i) {
       trace("%04x ", zigzag[i]);
    }
    trace("\n\n");
    #endif

    /* 4. Transformee en cosinus discrete inverse (iDCT) */
    int16_t *component = idct(zigzag);
    #ifdef DEBUG
    trace("#### Composante finale :\n");
    for (size_t i = 0; i < COMPONENT_SIZE; ++i) {
        trace("%04x ", component[i]);
    }
    trace("\n\n");
    #endif

    free(extracted);
    free(quantization);
    free(zigzag);

    /* Retour de la composante */
    return component;
}
