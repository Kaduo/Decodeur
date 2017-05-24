#include "extraction.h"

/* Extrait un MCU d'un bitstream et d'une description donnés */

int16_t *create_components(uint8_t nb_components)
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

struct mcu *extract_mcu(const struct bitstream *bitstream,
                                uint8_t nb_components_y,
                                uint8_t nb_components_cb,
                                uint8_t nb_components_cr,
                                const struct jpeg_desc *jpeg) {
    /*bouchon*/
    struct mcu *mcu = create_mcu(nb_components_y, nb_components_cb, nb_components_cr);

    for (uint8_t y = 0; y < mcu->nb_ys; y++) {
    }

    return mcu;
}

/* Extrait un type de composante d'un bitstream donné */
void extract_component(const struct bitstream *bitstream,
                                    const struct huff_table *huff_dc,
                                    const struct huff_table *huff_ac,
                                    int16_t previous_dc,
                                    int16_t *component)
{
    /* Extraction de DC */
    uint8_t magnitude = (uint8_t) next_huffman_value(huff_dc, bitstream);
    uint32_t indice = 0;
    read_bitstream(bitstream, magnitude, &indice, false);
    uint8_t signe = indice % (int32_t) pow(2, magnitude); // Attention : vaut 0 si valeur negative, 1 si positive
    indice /= pow(2, magnitude);
    component[0] = previous_dc + indice + pow(2, magnitude - 1) * (2 * signe - 1); // Petite magouille

    /* Extraction des coefficients AC */
    for (uint8_t i = 1; i < 64; i++) {
        int8_t symbole = next_huffman_value(huff_ac, bitstream);
        if (!(symbole % 16)) {
            perror("ERREUR FATALE : Symbole invalide !");
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
                uint8_t signe = indice % (int32_t) pow(2, magnitude); // Attention : vaut 0 si valeur negative, 1 si positive
                indice /= pow(2, magnitude);
                component[i] = indice + pow(2, magnitude - 1) * (2 * signe - 1); // Petite magouille
        }
    }
}

/* Extrait DC */
extern int16_t extract_dc(const struct bitstream *bitstream, const struct huff_table *huff) {
    /*bouchon*/
    int16_t dc;
    return dc;
}

/* Extrait AC */
extern int16_t *extract_ac(const struct bitstream *bitstream, const struct huff_table *huff){
    /*bouchon*/
    return NULL;
}

/*Calcule les DCs d'origine*/
extern void initial_DCs(struct mcu *mcus, size_t size){
    /*bouchon*/
    return;
}
