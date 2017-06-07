/*******************************************************************************
Nom ......... : jpeg2ppm.c
Role ........ : Fonctions du point d'entrée du programme
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdbool.h>
#include <string.h>
#include "jpeg2ppm.h"
#include "bitstream.h"
#include "jpeg_reader.h"
#include "picture.h"
#include "mcu.h"
#include "shared.h"
#include "trace.h"

/* Indique si l'image est en couleur */
bool est_couleur(const struct jpeg_desc *jpeg)
{
    return get_nb_components(jpeg) > 1;
}

/* Récupère les tables de Huffman dans l'ordre Y_dc, Y_ac, puis C_dc et C_ac (si elles existent) */
struct huff_table ***get_huff_tables(const struct jpeg_desc *jpeg)
{
    uint8_t nb_huffman_tables = get_nb_huffman_tables(jpeg, AC);

    struct huff_table ***huff_tables;
    huff_tables = malloc(nb_huffman_tables * sizeof(struct huff_table**));

    /* Récupération des tables de Huffman de la composante Y */
    huff_tables[COMP_Y] = malloc(2 * sizeof(struct huff_table*));
    uint8_t id_huff_y_dc = get_scan_component_huffman_index(jpeg, DC, COMP_Y);
    uint8_t id_huff_y_ac = get_scan_component_huffman_index(jpeg, AC, COMP_Y);
    huff_tables[COMP_Y][DC] = get_huffman_table(jpeg, DC, id_huff_y_dc);
    huff_tables[COMP_Y][AC] = get_huffman_table(jpeg, AC, id_huff_y_ac);

    /* Si l'image est en couleur, récupération de l'autre table */
    if (nb_huffman_tables > 1) {
        huff_tables[COMP_Cb] = malloc(2 * sizeof(struct huff_table*));
        uint8_t id_huff_c_dc = get_scan_component_huffman_index(jpeg, DC, COMP_Cb);
        uint8_t id_huff_c_ac = get_scan_component_huffman_index(jpeg, AC, COMP_Cb);
        huff_tables[COMP_Cb][DC] = get_huffman_table(jpeg, DC, id_huff_c_dc);
        huff_tables[COMP_Cb][AC] = get_huffman_table(jpeg, AC, id_huff_c_ac);
    }

    return huff_tables;
}

/* Récupère les tables de quantification dans l'ordre Y puis C (si elle existe) */
uint8_t **get_quant_tables(const struct jpeg_desc *jpeg)
{
    uint8_t nb_quant_tables = get_nb_quantization_tables(jpeg);
    uint8_t **quant_tables = malloc(nb_quant_tables * sizeof(uint8_t*));
    for (uint8_t i = 0; i < nb_quant_tables; ++i) {
        quant_tables[i] = get_quantization_table(jpeg, i);
    }
    return quant_tables;
}

/* Renvoie un tableau donnant l'ordre des composantes dans les MCUs */
enum component *get_components_order(const struct jpeg_desc *jpeg, uint8_t factors[COMP_NB][DIR_NB])
{
    uint8_t nb_components_y = factors[COMP_Y][DIR_H] * factors[COMP_Y][DIR_V];
    uint8_t nb_components_cb = factors[COMP_Cb][DIR_H] * factors[COMP_Cb][DIR_V];
    uint8_t nb_components_cr = factors[COMP_Cr][DIR_H] * factors[COMP_Cr][DIR_V];
    uint8_t nb_components_per_mcu = nb_components_y + nb_components_cb + nb_components_cr;
    trace("Nb. de composantes par MCU : %d\n", nb_components_per_mcu);

    uint8_t nb_components = get_nb_components(jpeg);

    enum component *order = malloc(nb_components_per_mcu * sizeof(enum component));

    uint8_t id_y = get_frame_component_id(jpeg, COMP_Y);
    uint8_t id_cb;
    uint8_t id_cr;

    if (nb_components > 1) {
        id_cb = get_frame_component_id(jpeg, COMP_Cb);
        id_cr = get_frame_component_id(jpeg, COMP_Cr);
    }

    uint8_t j = 0; // Position de la composante lue
    /* Itération sur le type de composante */
    for (uint8_t i = 0; i < nb_components; ++i) {
        uint8_t scan_id = get_scan_component_id(jpeg, i);
        if (scan_id == id_y) {
            for (uint8_t k = 0; k < nb_components_y; ++k) {
                order[j] = COMP_Y;
                j++;
            }
        } else if (scan_id == id_cb) {
            for (uint8_t k = 0; k < nb_components_cb; ++k) {
                order[j] = COMP_Cb;
                j++;
            }
        } else if (scan_id == id_cr) {
            for (uint8_t k = 0; k < nb_components_cr; ++k) {
                order[j] = COMP_Cr;
                j++;
            }
        }
        else {
            fprintf(stderr, "Composante non reconnue.\n");
        }
    }

    return order;
}

/* Récupère le nom du fichier de sortie */
char *get_outfile_name(const char *filename, bool colored)
{
    char *outfile = calloc(strlen(filename) + 1, sizeof(char));
    /* Recherche la derniere occurence du caractere point '.' */
    const char find = '.';
    const char *last = strchr(filename, find);
    /* Si le caractere a ete trouve, on genere un nom, sinon NULL */
    if (last) {
        /* Recuperation de l'index du caractere trouve */
        size_t index = last - filename;
        /* Copie du filename dans l'outfile jusqu'au caractere trouve */
        strncpy(outfile, filename, index + 1);
        /* Concatenation de l'extension appropriee */
        strcat(outfile, colored ? "ppm" : "pgm");
        return outfile;
    }
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
    	/* Si y'a pas au moins un argument en ligne de commandes, on boude */
    	fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);
    	return EXIT_FAILURE;
    }

    /* On recupere le nom du fichier JPEG sur la ligne de commande. */
    const char *filename = argv[1];
    trace("# Décodage du fichier %s\n", filename);

    /* On cree un jpeg_desc qui permettra de lire ce fichier. */
    struct jpeg_desc *jdesc = read_jpeg(filename);

    /* On recupere le flux des donnees brutes a partir du descripteur. */
    struct bitstream *stream = get_bitstream(jdesc);

    uint8_t nb_components = get_nb_components(jdesc);
    trace("Nombre de composantes : %hhu\n\n", nb_components);

    /* == EXTRACTION DES MCUs == */

    /* Récupération des facteurs d'échantillonnage */
    uint8_t sampling_factors[COMP_NB][DIR_NB] = {{0}}; // Contient les sampling factors h1,v1,h2,...
    for (size_t i = 0; i < nb_components; i++) {
        sampling_factors[i][DIR_H] = get_frame_component_sampling_factor(jdesc, DIR_H, i);
        sampling_factors[i][DIR_V] = get_frame_component_sampling_factor(jdesc, DIR_V, i);
    }

    trace("Sampling factors :\n");
    trace("\tH1 : %hhu, V1 : %hhu\n", sampling_factors[COMP_Y][DIR_H], sampling_factors[COMP_Y][DIR_V]);
    trace("\tH2 : %hhu, V3 : %hhu\n", sampling_factors[COMP_Cb][DIR_H], sampling_factors[COMP_Cb][DIR_V]);
    trace("\tH3 : %hhu, V3 : %hhu\n\n", sampling_factors[COMP_Cr][DIR_H], sampling_factors[COMP_Cr][DIR_V]);

    /* Calcul du nombre de composantes par MCUs */
    uint8_t nb_components_y = sampling_factors[COMP_Y][DIR_H] * sampling_factors[COMP_Y][DIR_V];
    uint8_t nb_components_cb = sampling_factors[COMP_Cb][DIR_H] * sampling_factors[COMP_Cb][DIR_V];
    uint8_t nb_components_cr = sampling_factors[COMP_Cr][DIR_H] * sampling_factors[COMP_Cr][DIR_V];

    /* Calcul de la taille de l'image */
    uint16_t width = get_image_size(jdesc, DIR_H);
    uint16_t height = get_image_size(jdesc, DIR_V);
    trace("Taille de l'image : %dx%d\n", width, height);

    /* Taille de l'image complétée */
    uint16_t width_ext = 0;
    uint16_t height_ext = 0;
    if (width%(BLOCK_SIZE*sampling_factors[COMP_Y][DIR_H])) {
        width_ext = width + (BLOCK_SIZE*sampling_factors[COMP_Y][DIR_H]) - width % (BLOCK_SIZE*sampling_factors[COMP_Y][DIR_H]);
    } else {
        width_ext = width;
    }

    if (height%(BLOCK_SIZE*sampling_factors[COMP_Y][DIR_V])) {
        height_ext = height + (BLOCK_SIZE*sampling_factors[COMP_Y][DIR_V]) - height % (BLOCK_SIZE*sampling_factors[COMP_Y][DIR_V]);
    } else {
        height_ext = height;
    }

    trace("Taille de l'image complétée : %dx%d\n\n", width_ext, height_ext);

    /* Calcul du nombre de MCUs */
    uint32_t nb_mcus_h = width_ext / (BLOCK_SIZE * sampling_factors[COMP_Y][DIR_H]);
    uint32_t nb_mcus_v = height_ext / (BLOCK_SIZE * sampling_factors[COMP_Y][DIR_V]);
    uint32_t nb_mcus = nb_mcus_h * nb_mcus_v;
    trace("Nombre de MCUs : %d\n\n", nb_mcus);

    /* Création du tableau de MCUs */
    struct mcu **mcus = malloc(nb_mcus * sizeof(struct mcu*));

    /* Récupération des tables et des informations utiles à l'extraction des MCUs */
    uint8_t **quant_tables = get_quant_tables(jdesc);
    struct huff_table ***huff_tables = get_huff_tables(jdesc);
    enum component *ordre_des_composantes = get_components_order(jdesc, sampling_factors);

    /* Extraction des MCUs */
    trace("# Extraction des MCUs\n");
    int16_t previous_dc_y = 0;
    int16_t previous_dc_cb = 0;
    int16_t previous_dc_cr = 0;
    for (uint32_t i = 0; i < nb_mcus; ++i) {
        trace("## MCU %d :\n", i);
        mcus[i] = extract_mcu(stream,
                                nb_components_y,
                                nb_components_cb,
                                nb_components_cr,
                                &previous_dc_y,
                                &previous_dc_cb,
                                &previous_dc_cr,
                                ordre_des_composantes,
                                huff_tables,
                                quant_tables);
        trace("\n");
    }
    trace("\n");

    /* Libération de l'ordre des composantes */
    free(ordre_des_composantes);
    ordre_des_composantes = NULL;

    /* Libération des tables de quantification et de Huffman */
    free(huff_tables[COMP_Y]);
    if (est_couleur(jdesc)) {
        free(huff_tables[COMP_Cb]);
    }
    free(huff_tables);
    huff_tables = NULL;
    free(quant_tables);
    quant_tables = NULL;

    /* Reconstruction des blocs */
    trace("# Reconstruction des blocs\n");
    uint32_t nb_blocks = nb_components_y * nb_mcus;
    block *liste_blocks = malloc(nb_blocks * sizeof(block));
    block *blocks_temp = NULL; // Liste des blocs extraits d'une MCU
    for (uint32_t i = 0; i < nb_mcus; ++i) {
        blocks_temp = extract_blocks(mcus[i], sampling_factors);
        for (uint8_t j = 0; j < nb_components_y; ++j) {
            liste_blocks[i*nb_components_y+j] = blocks_temp[j];
        }
        free(blocks_temp);
        blocks_temp = NULL;
    }

    /* Libération mémoire du tableau de MCUs */
    for(uint32_t i = 0; i < nb_mcus; ++i) {
         free_mcu(mcus[i]);
     }
    free(mcus);
    mcus = NULL;

    /* == CREATION DE L'IMAGE == */

    /* Conversion en RGB si l'image est en couleur */
    if (est_couleur(jdesc)) {
        for (uint32_t i = 0; i < nb_blocks; ++i) {
            convert_to_rgb(liste_blocks[i]);
        }
    }

    trace("# Construction de l'image en pixels\n");
    struct picture *pic = blocks2pixels(liste_blocks,
                                        width,
                                        height,
                                        width_ext,
                                        sampling_factors[COMP_Y][DIR_H],
                                        sampling_factors[COMP_Y][DIR_V]);

    /* Libération mémoire des blocs */
    for(uint32_t b = 0; b < nb_blocks; ++b) {
        free_block(liste_blocks[b], est_couleur(jdesc));
    }
    free(liste_blocks);
    liste_blocks = NULL;

    trace("# Écriture de l'image\n");
    char *outfile = get_outfile_name(filename, pic->colored);
    write_ppm(pic, outfile);
    free_picture(pic);
    free(outfile);

    /* Nettoyage de printemps : close_jpeg ferme aussi le bitstream */
    close_jpeg(jdesc);

    return EXIT_SUCCESS;
}
