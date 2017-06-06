/*******************************************************************************
Nom ......... : mcu.c
Role ........ : Fonctions d'extraction et de traitement de la structure mcu
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdio.h>
#include <math.h>
#include "mcu.h"
#include "shared.h"

struct mcu *create_mcu(uint8_t nb_components_y, uint8_t nb_components_cb, uint8_t nb_components_cr)
{

    struct mcu *mcu = malloc(sizeof(struct mcu));

    mcu->components_y = calloc(nb_components_y, sizeof(int16_t *));
    if (nb_components_cb > 0) {
        mcu->components_cb = calloc(nb_components_cb, sizeof(int16_t *));
        mcu->components_cr = calloc(nb_components_cr, sizeof(int16_t *));
    }
    else {
        mcu->components_cb = NULL;
        mcu->components_cr = NULL;
    }
    mcu->nb_ys = nb_components_y;
    mcu->nb_cbs = nb_components_cb;
    mcu->nb_crs = nb_components_cr;

    return mcu;
}

/*void free_components(int16_t **components, uint8_t size_components)
{
    for (uint8_t i = 0; i < size_components; i++) {
        free(components[i]);
    }
    free(components);
    components = NULL;
}

void free_mcu(struct mcu *mcu)
{
    // On libère les comp_Y dans les blocks, car on fait une copie.
    //free_components(mcu->components_y, mcu->nb_ys);
    free_components(mcu->components_cb, mcu->nb_cbs);
    free_components(mcu->components_cr, mcu->nb_crs);
    free(mcu);
    mcu = NULL;
}*/

/*
Extrait une mcu complète (s'occupe de l'allocation mémoire).

ordre_des_composantes est un tableau indiquant l'ordre des composantes dans la mcu.

huff_tables est un tableau de couples de tables de Huffman, de forme : [[Y_dc, Y_ac], [C_dc, C_ac]]
*/
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
        if (ordre_des_composantes[i] == COMP_Y) {
            mcu->components_y[current_index_y] = get_component(bitstream,
                                                        huff_tables[COMP_Y][DC],
                                                        huff_tables[COMP_Y][AC],
                                                        quant_tables[COMP_Y],
                                                        previous_dc_y,
                                                        BLOCK_SIZE);

            current_index_y++;

        } else if (ordre_des_composantes[i] == COMP_Cb) {
            mcu->components_cb[current_index_cb] = get_component(bitstream,
                                                        huff_tables[1][DC],
                                                        huff_tables[1][AC],
                                                        quant_tables[1],
                                                        previous_dc_cb,
                                                        BLOCK_SIZE);

            current_index_cb++;

        } else if (ordre_des_composantes[i] == COMP_Cr){
            /* Cb et Cr partagent les memes tables */
            mcu->components_cr[current_index_cr] = get_component(bitstream,
                                                        huff_tables[1][DC],
                                                        huff_tables[1][AC],
                                                        quant_tables[1],
                                                        previous_dc_cr,
                                                        BLOCK_SIZE);

            current_index_cr++;

        } else {
            perror("ERRREUR FATALE : Si vous voyez ce message, c'est que j'ai rien compris au schmilblick");
            exit(EXIT_FAILURE);
        }
    }
    return mcu;
}
