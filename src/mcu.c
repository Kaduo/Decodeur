#include <stdio.h>
#include <math.h>
#include "mcu.h"
#include "component.h"
#include "jpeg_reader.h"

/* Taille d'un bloc */
const size_t TAILLE_BLOC = 8;

struct mcu *create_mcu(uint8_t nb_components_y, uint8_t nb_components_cb, uint8_t nb_components_cr)
{

    struct mcu *mcu = malloc(sizeof(struct mcu));

    mcu->components_y = NULL;
    mcu->components_cb = NULL;
    mcu->components_cr = NULL;
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

    // Le code ci-dessous sert à récupérer les tables de quantification et les tables de Huffman
    // Il faut le mettre dans une autre fonction.
    /* uint8_t id_y = get_frame_component_id(jpeg, 0);
    uint8_t id_huff_y_dc = get_scan_component_huffman_index(jpeg, DC, 0);
    uint8_t id_huff_y_ac = get_scan_component_huffman_index(jpeg, AC, 0);
    struct huff_table *huff_y_dc = get_huffman_table(jpeg, DC, id_huff_y_dc);
    struct huff_table *huff_y_ac = get_huffman_table(jpeg, AC, id_huff_y_ac);


    uint8_t id_cb = 0;
    uint8_t id_cr = 0;
    uint8_t id_huff_c_dc = 0;
    uint8_t id_huff_c_ac = 0;

    struct huff_table *huff_c_dc = NULL;
    struct huff_table *huff_c_ac = NULL;

    if (mcu->nb_cbs > 0){
        id_cb = get_frame_component_id(jpeg, 1);
        id_cr = get_frame_component_id(jpeg, 2);
        id_huff_c_dc = get_scan_component_huffman_index(jpeg, DC, 1);
        id_huff_c_ac = get_scan_component_huffman_index(jpeg, AC, 1);
        huff_c_dc = get_huffman_table(jpeg, DC, id_huff_c_dc);
        huff_c_ac = get_huffman_table(jpeg, DC, id_huff_c_ac);
    } */

    uint8_t nb_components = nb_components_y + nb_components_cb + nb_components_cr;

    for (size_t i = 0; i < nb_components; i++) {
        int16_t previous_dc = 0;

        if (ordre_des_composantes[i] == COMP_Y){
            for (size_t j = 0; j < mcu->nb_ys; j++) {
                mcu->components_y[j] = get_component(bitstream,
                                                        huff_tables[0][0],
                                                        huff_tables[0][1],
                                                        quant_tables[0],
                                                        previous_dc,
                                                        TAILLE_BLOC);
                previous_dc = mcu->components_y[j][0];
            }
        }

        else if (ordre_des_composantes[i] == COMP_Cb){
            for (size_t j = 0; j < mcu->nb_cbs; j++) {
                mcu->components_cb[j] = get_component(bitstream,
                                                        huff_tables[1][0],
                                                        huff_tables[1][1],
                                                        quant_tables[1],
                                                        previous_dc,
                                                        TAILLE_BLOC);
                previous_dc = mcu->components_cb[j][0];
            }
        }

        else if (ordre_des_composantes[i] == COMP_Cr){
            for (size_t j = 0; j < mcu->nb_crs; j++) {
                mcu->components_cr[j] = get_component(bitstream,
                                                        huff_tables[1][0],
                                                        huff_tables[1][1],
                                                        quant_tables[1],
                                                        previous_dc,
                                                        TAILLE_BLOC);
                previous_dc = mcu->components_cr[j][0];
            }
        }

        else{
            perror("ERRREUR FATALE : Si vous voyez ce message, c'est que j'ai rien compris au schmilblick");
            exit(EXIT_FAILURE);
        }
    }

    return mcu;
}
