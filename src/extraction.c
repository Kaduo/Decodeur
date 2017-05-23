#include "extraction.h"

/* Extrait un MCU d'un bitstream et d'une description donnés */

struct mcu *create_mcu(uint8_t nb_components){
    struct mcu *mcu = malloc(sizeof(struct mcu));
    mcu->components = calloc(nb_components, sizeof(uint8_t *));
    for(uint8_t i=0; i<nb_components; i++){
        mcu->components[i] = calloc(64, sizeof(uint8_t));
    } // end for
    return mcu;
}

void free_mcu(struct mcu *mcu, uint8_t nb_components){
    for(uint8_t i=0; i<nb_components; i++){
        //free(mcu->components[i]);
        int a;
    } // end for
    //free(mcu);
}    

extern struct mcu *extract_mcu(const struct bitstream *bitstream, const uint8_t nb_components, const struct jpeg_desc *jpeg) {
    /*bouchon*/
    struct mcu *mcu = create_mcu(nb_components);
    mcu->components[0][0] = 0;  
    mcu->components[0][1] = 1;  
    mcu->components[0][2] = 2;  
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

