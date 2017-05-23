#include "extraction.h"

/* Extrait un MCU d'un bitstream et d'une description donnés */
extern struct mcu *extract_mcu(const struct bitstream *bitstream, const struct jpeg_desc *jpeg) {
    /*bouchon*/
    struct mcu *mcu;
    return mcu;
}

/* Extrait un type de composante d'un bitstream donné */
extern int16_t *extract_component(const struct bitstream *bitstream,
                                    const struct huff_table *y_dc,
                                    const struct huff_table *y_ac,
                                    const struct huff_table *c_dc,
                                    const struct huff_table *c_ac,
                                    enum component comp) {
    /*bouchon*/
    return NULL;
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

/* Libère un MCU */
extern void free_mcu(const struct mcu *mcu){
    /*bouchon*/
    return;
}
