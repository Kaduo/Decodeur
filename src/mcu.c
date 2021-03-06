/*******************************************************************************
Nom ......... : mcu.c
Role ........ : Fonctions d'extraction et de traitement de la structure mcu
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdio.h>
#include <math.h>
#include "mcu.h"
#include "shared.h"
#include "trace.h"

/* Crée une structure MCU */
struct mcu *create_mcu(uint8_t nb_components_y, uint8_t nb_components_cb, uint8_t nb_components_cr)
{

    struct mcu *mcu = malloc(sizeof(struct mcu));

    mcu->components_y = calloc(nb_components_y, sizeof(int16_t *));
    if (nb_components_cb > 0) {
        mcu->components_cb = calloc(nb_components_cb, sizeof(int16_t *));
        mcu->components_cr = calloc(nb_components_cr, sizeof(int16_t *));
    } else {
        mcu->components_cb = NULL;
        mcu->components_cr = NULL;
    }
    mcu->nb_ys = nb_components_y;
    mcu->nb_cbs = nb_components_cb;
    mcu->nb_crs = nb_components_cr;

    return mcu;
}

/* Extrait une MCU complète. 'ordre_des_composantes' est un tableau indiquant
l'ordre des composantes dans la MCU. 'huff_tables' est un tableau de couples de
tables de Huffman, de forme : [[Y_dc, Y_ac], [C_dc, C_ac]] */
struct mcu *extract_mcu(struct bitstream *bitstream,
                        uint8_t nb_components_y,
                        uint8_t nb_components_cb,
                        uint8_t nb_components_cr,
                        int16_t *previous_dc_y,
                        int16_t *previous_dc_cb,
                        int16_t *previous_dc_cr,
                        enum component *ordre_des_composantes,
                        struct huff_table ***huff_tables,
                        uint8_t **quant_tables)
{
    struct mcu *mcu = create_mcu(nb_components_y, nb_components_cb, nb_components_cr);

    uint8_t nb_components = nb_components_y + nb_components_cb + nb_components_cr;
    uint8_t current_index_y = 0;
    uint8_t current_index_cb = 0;
    uint8_t current_index_cr = 0;

    for (uint8_t i = 0; i < nb_components; i++) {
        trace("### Composante n°%hhu\n\n", i);
        if (ordre_des_composantes[i] == COMP_Y) {
            mcu->components_y[current_index_y] = get_component(bitstream,
                                                        huff_tables[COMP_Y][DC],
                                                        huff_tables[COMP_Y][AC],
                                                        quant_tables[COMP_Y],
                                                        previous_dc_y);
            current_index_y++;
        } else if (ordre_des_composantes[i] == COMP_Cb) {
            mcu->components_cb[current_index_cb] = get_component(bitstream,
                                                        huff_tables[COMP_Cb][DC],
                                                        huff_tables[COMP_Cb][AC],
                                                        quant_tables[COMP_Cb],
                                                        previous_dc_cb);
            current_index_cb++;
        } else if (ordre_des_composantes[i] == COMP_Cr){
            /* Cb et Cr partagent les memes tables */
            mcu->components_cr[current_index_cr] = get_component(bitstream,
                                                        huff_tables[COMP_Cb][DC],
                                                        huff_tables[COMP_Cb][AC],
                                                        quant_tables[COMP_Cb],
                                                        previous_dc_cr);
            current_index_cr++;
        } else {
            fprintf(stderr, "Erreur dans la lecture de l'ordre des composantes.\n");
            exit(EXIT_FAILURE);
        }
    }
    return mcu;
}

/* Libère la place occupée en mémoire par une MCU */
void free_mcu(struct mcu *mcu)
{
    /* Libération des composantes */
    free(mcu->components_y);
    free(mcu->components_cb);
    free(mcu->components_cr);
    free(mcu);
    mcu = NULL;
}
