#include <stdio.h>
#include <math.h>
#include "extraction.h"

/* Extrait un MCU d'un bitstream et d'une description donnés */

int16_t **create_components(uint8_t nb_components)
{
    int16_t **components = calloc(nb_components, sizeof(int16_t *));
    for (uint8_t i = 0; i < nb_components; i++) {
        components[i] = calloc(64, sizeof(int16_t));
    }
    return components;
}

struct mcu *create_mcu(uint8_t nb_components_y, uint8_t nb_components_cb, uint8_t nb_components_cr)
{

    struct mcu *mcu = malloc(sizeof(struct mcu));

    mcu->components_y = create_components(nb_components_y);
    mcu->components_cb = create_components(nb_components_cb);
    mcu->components_cr = create_components(nb_components_cr);
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
                        const struct jpeg_desc *jpeg)
{
    struct mcu *mcu = create_mcu(nb_components_y, nb_components_cb, nb_components_cr);

    uint8_t id_y = get_frame_component_id(jpeg, 0);
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
    }


    for (size_t i = 0; i < get_nb_components(jpeg); i++) {
        int16_t previous_dc = 0;

        if (get_scan_component_id(jpeg, i) == id_y){
            for (size_t j = 0; j < mcu->nb_ys; j++) {
                extract_component(bitstream,
                                    huff_y_dc,
                                    huff_y_ac,
                                    previous_dc,
                                    mcu->components_y[j]);
                previous_dc = mcu->components_y[j][0];
            }
        }

        else if (get_scan_component_id(jpeg, i) == id_cb){
            for (size_t j = 0; j < mcu->nb_cbs; j++) {
                extract_component(bitstream,
                                    huff_c_dc,
                                    huff_c_ac,
                                    previous_dc,
                                    mcu->components_cb[j]);
                previous_dc = mcu->components_cb[j][0];
            }
        }

        else if (get_scan_component_id(jpeg, i) == id_cr){
            for (size_t j = 0; j < mcu->nb_crs; j++) {
                extract_component(bitstream,
                                    huff_c_dc,
                                    huff_c_ac,
                                    previous_dc,
                                    mcu->components_cr[j]);
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

/* Extrait un type de composante d'un bitstream donné */
void extract_component(struct bitstream *bitstream,
                        struct huff_table *huff_dc,
                        struct huff_table *huff_ac,
                        int16_t previous_dc,
                        int16_t *component)
{
    /* Extraction de DC */
    uint8_t magnitude = (uint8_t) next_huffman_value(huff_dc, bitstream);
    uint32_t indice = 0;
    read_bitstream(bitstream, magnitude, &indice, false);
    uint8_t signe = (indice >> (magnitude - 1)); // Attention : vaut 0 si valeur negative, 1 si positive
    indice %= (uint32_t) pow(2, magnitude - 1);
    if(signe == 0) component[0] = previous_dc + indice - pow(2, magnitude) + 1;
    else component[0] = previous_dc + indice + pow(2, magnitude - 1);

    /* Extraction des coefficients AC */
    for (uint8_t i = 1; i < 64; i++) {
        int8_t symbole = next_huffman_value(huff_ac, bitstream);
        if (!(symbole % 16) && symbole != 0) {
            perror("ERREUR FATALE : symbole invalide !");
            exit(EXIT_FAILURE);
        }
        switch (symbole) {
            case 0x00:
                return;
                break;
            case 0xf0:
                i += 16;
                break;
            default:
                i += (symbole >> 4);
                uint8_t magnitude = (symbole & 0x0f);
                uint32_t indice = 0;
                read_bitstream(bitstream, magnitude, &indice, true); //Peu sûr...
                uint8_t signe = (indice >> (magnitude - 1)); // Attention : vaut 0 si valeur negative, 1 si positive
                indice %= (uint32_t) pow(2, magnitude - 1);
                if(signe == 0) component[i] = + indice - pow(2, magnitude) + 1;
                else component[i] = indice + pow(2, magnitude - 1);
        }
    }
}
