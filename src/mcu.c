#include <stdio.h>
#include <math.h>
#include "component.h"


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

struct mcu *extract_mcu(struct bitstream *bitstream,
                        uint8_t nb_components_y,
                        uint8_t nb_components_cb,
                        uint8_t nb_components_cr,
                        struct huff_table *huff_tables,
                        const struct jpeg_desc *jpeg)
{
    struct mcu *mcu = create_mcu(nb_components_y, nb_components_cb, nb_components_cr);

    /* uint8_t id_y = get_frame_component_id(jpeg, 0);
    uint8_t id_huff_y_dc = get_scan_component_huffman_index(jpeg, DC, 0);
    uint8_t id_huff_y_ac = get_scan_component_huffman_index(jpeg, AC, 0);
    struct huff_table *huff_y_dc = get_huffman_table(jpeg, DC, id_huff_y_dc);
    struct huff_table *huff_y_ac = get_huffman_table(jpeg, AC, id_huff_y_ac); */


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
    }


    for (size_t i = 0; i < get_nb_components(jpeg); i++) {
        int16_t previous_dc = 0;

        if (get_scan_component_id(jpeg, i) == id_y){
            for (size_t j = 0; j < mcu->nb_ys; j++) {
                mcu->components_y[j] = get_component(bitstream,
                                                        huff_y_dc,
                                                        huff_y_ac,
                                                        previous_dc);
                previous_dc = mcu->components_y[j][0];
            }
        }

        else if (get_scan_component_id(jpeg, i) == id_cb){
            for (size_t j = 0; j < mcu->nb_cbs; j++) {
                mcu->components_cb[j] = get_component(bitstream,
                                                        huff_c_dc,
                                                        huff_c_ac,
                                                        previous_dc);
                previous_dc = mcu->components_cb[j][0];
            }
        }

        else if (get_scan_component_id(jpeg, i) == id_cr){
            for (size_t j = 0; j < mcu->nb_crs; j++) {
                mcu->components_cr[j] = get_component(bitstream,
                                                        huff_c_dc,
                                                        huff_c_ac,
                                                        previous_dc);
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
