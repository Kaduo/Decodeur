/*******************************************************************************
Nom ......... : mcu.c
Role ........ : Fonctions d'extraction et de traitement de la structure mcu
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdio.h>
#include <math.h>
#include "mcu.h"

struct mcu *create_mcu(uint8_t nb_components_y, uint8_t nb_components_cb, uint8_t nb_components_cr)
{

    struct mcu *mcu = malloc(sizeof(struct mcu));

    mcu->components_y = malloc(nb_components_y*sizeof(int16_t *));
    if (nb_components_cb > 0) {
        mcu->components_cb = malloc(nb_components_cb*sizeof(int16_t *));
        mcu->components_cr = malloc(nb_components_cr*sizeof(int16_t *));
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

void free_components(int16_t **components, uint8_t size_components)
{
    for (uint8_t i = 0; i < size_components; i++) {
        free(components[i]);
    }
    free(components);
    components = NULL;
}

void free_mcu(struct mcu *mcu)
{
    free_components(mcu->components_y, mcu->nb_ys);
    free_components(mcu->components_cb, mcu->nb_cbs);
    free_components(mcu->components_cr, mcu->nb_crs);
    free(mcu);
    mcu = NULL;
}

/*
Extrait une mcu complète (s'occupe de l'allocation mémoire).

ordre_des_composantes est un tableau indiquant l'ordre des composantes dans la mcu.

huff_tables est un tableau de couples de tables de Huffman, de forme : [[Y_dc, Y_ac], [C_dc, C_ac]]
*/
struct mcu *extract_mcu(struct bitstream *bitstream,
                        uint8_t nb_components_y,
                        uint8_t nb_components_cb,
                        uint8_t nb_components_cr,
                        enum component *ordre_des_composantes,
                        struct huff_table ***huff_tables,
                        uint8_t **quant_tables)
{
    struct mcu *mcu = create_mcu(nb_components_y, nb_components_cb, nb_components_cr);


    uint8_t nb_components = nb_components_y + nb_components_cb + nb_components_cr;
    uint8_t current_index_y = 0;
    uint8_t current_index_cb = 0;
    uint8_t current_index_cr = 0;
    int16_t previous_dc_y = 0;
    int16_t previous_dc_cb = 0;
    int16_t previous_dc_cr = 0;

    for (size_t i = 0; i < nb_components; i++) {

        if (ordre_des_composantes[i] == COMP_Y){
            mcu->components_y[current_index_y] = get_component(bitstream,
                                                        huff_tables[0][0],
                                                        huff_tables[0][1],
                                                        quant_tables[0],
                                                        previous_dc_y,
                                                        (size_t) 8);

            previous_dc_y = mcu->components_y[current_index_y][0];
            current_index_y++;
        }

        else if (ordre_des_composantes[i] == COMP_Cb){
            mcu->components_cb[current_index_cb] = get_component(bitstream,
                                                        huff_tables[1][0],
                                                        huff_tables[1][1],
                                                        quant_tables[1],
                                                        previous_dc_cb,
                                                        (size_t) 8);

            previous_dc_cb = mcu->components_y[current_index_cb][0];
            current_index_cb++;
        }

        else if (ordre_des_composantes[i] == COMP_Cr){
                mcu->components_cr[current_index_cr] = get_component(bitstream,
                                                        huff_tables[1][0],
                                                        huff_tables[1][1],
                                                        quant_tables[1],
                                                        previous_dc_cr,
                                                        (size_t) 8);

                previous_dc_cr = mcu->components_y[current_index_cr][0];
                current_index_cr++;
        }

        else{
            perror("ERRREUR FATALE : Si vous voyez ce message, c'est que j'ai rien compris au schmilblick");
            exit(EXIT_FAILURE);
        }
    }

    return mcu;
}
