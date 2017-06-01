#include "jpeg2ppm.h"
#include <stdbool.h>
#include <string.h>

bool est_couleur(const struct jpeg_desc *jpeg){
    return get_nb_components(jpeg) > 1;
}


/* Récupère les tables de Huffman dans l'ordre Y_dc, Y_ac, puis C_dc et C_ac (si elles existent).
Pour Antoine : ce code est irrécupérable, inutile d'essayer de le nettoyer. De même pour les fonctions suivantes.*/
extern struct huff_table ***get_huff_tables(const struct jpeg_desc *jpeg) {

    uint8_t nb_huffman_tables = get_nb_huffman_tables(jpeg, AC);

    struct huff_table ***huff_tables;
    huff_tables = malloc(nb_huffman_tables*sizeof(struct huff_table **));
    huff_tables[0] = malloc(2*sizeof(struct huff_table *));

    uint8_t id_huff_y_dc = get_scan_component_huffman_index(jpeg, DC, 0);
    uint8_t id_huff_y_ac = get_scan_component_huffman_index(jpeg, AC, 0);
    huff_tables[0][DC] = get_huffman_table(jpeg, DC, id_huff_y_dc);
    huff_tables[0][AC] = get_huffman_table(jpeg, AC, id_huff_y_ac);

    if (nb_huffman_tables > 1) {

        huff_tables[1] = malloc(2*sizeof(struct huff_table *));

        uint8_t id_huff_c_dc = get_scan_component_huffman_index(jpeg, DC, 1);
        uint8_t id_huff_c_ac = get_scan_component_huffman_index(jpeg, AC, 1);
        huff_tables[1][DC] = get_huffman_table(jpeg, DC, id_huff_c_dc);
        huff_tables[1][AC] = get_huffman_table(jpeg, AC, id_huff_c_ac);
    }

    return huff_tables;
}

/* Récupère les tables de quantification dans l'ordre Y puis C (si elle existe) */
uint8_t **get_quant_tables(const struct jpeg_desc *jpeg) {

    uint8_t nb_quant_tables = get_nb_quantization_tables(jpeg);
    uint8_t **quant_tables = malloc(nb_quant_tables*sizeof(uint8_t *));
    for (uint8_t i = 0; i < nb_quant_tables; i++) {
        quant_tables[i] = get_quantization_table(jpeg, i);
    }
    return quant_tables;
}

/* Renvoie un tableau donnant l'ordre des composantes dans les mcus */
enum component *get_components_order(const struct jpeg_desc *jpeg, uint8_t factors[COMP_NB][DIR_NB]) {

    uint8_t nb_components_y = factors[COMP_Y][DIR_H] * factors[COMP_Y][DIR_V];
    uint8_t nb_components_cb = factors[COMP_Cb][DIR_H] * factors[COMP_Cb][DIR_V];
    uint8_t nb_components_cr = factors[COMP_Cr][DIR_H] * factors[COMP_Cr][DIR_V];
    uint8_t nb_components_per_mcu = nb_components_y + nb_components_cb + nb_components_cr;

    uint8_t nb_components = get_nb_components(jpeg);

    enum component *order = malloc(nb_components_per_mcu*sizeof(enum component));

    uint8_t id_y = get_frame_component_id(jpeg, 0);
    uint8_t id_cb = id_y + 1;
    uint8_t id_cr = id_y + 2;

    if (nb_components > 1) {
        id_cb = get_frame_component_id(jpeg, 1);
        id_cr = get_frame_component_id(jpeg, 2);
    }

    uint8_t j = 0;
    for (uint8_t i = 0; i < nb_components; i++) {
        uint8_t scan_id = get_scan_component_id(jpeg, i);
        if (scan_id == id_y) {
            for (size_t k = 0; k < nb_components_y; k++) {
                order[j + k] = COMP_Y;
                j++;
            }
        }
        else if (scan_id == id_cb) {
            for (size_t k = 0; k < nb_components_cb; k++) {
                order[j + k] = COMP_Cb;
                j++;
            }
        }
        else if (scan_id == id_cr) {
            for (size_t k = 0; k < nb_components_cr; k++) {
                order[j + k] = COMP_Cr;
                j++;
            }
        }
    }

    printf("HELLO");

    return order;

}

char *get_outfile_name(const char *filename, bool colored)
{
    char *outfile = malloc(sizeof(char) * strlen(filename) + 1);
    /* Recherche la derniere occurence du caractere point '.' */
    char find = '.';
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
	/* Si y'a pas au moins un argument en ligne de commandes, on
	 * boude. */
	fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);
	return EXIT_FAILURE;
    }
    /* On recupere le nom du fichier JPEG sur la ligne de commande. */
    const char *filename = argv[1];

    /* On cree un jpeg_desc qui permettra de lire ce fichier. */
    struct jpeg_desc *jdesc = read_jpeg(filename);

    /* On recupere le flux des donnees brutes a partir du descripteur. */
    struct bitstream *stream = get_bitstream(jdesc);

    // Nombre de composant
    uint8_t nb_components = get_nb_components(jdesc);
    printf("Nombre de composante : %hhu\n", nb_components);


    /*******
    *  MCU
    ******/

    // Calcul du nombre de MCU

    uint8_t sampling_factors[3][2] = {{0}}; //Contient les sampling factors h1,v1,h2,...


    for (size_t i = 0; i < nb_components; ++i) {
        sampling_factors[i][0] = get_frame_component_sampling_factor(jdesc, DIR_H, i);
        sampling_factors[i][1] = get_frame_component_sampling_factor(jdesc, DIR_V, i);
    }

    // H1 V1
    printf("H1 : %hhu, V1 : %hhu\n", sampling_factors[0][0], sampling_factors[0][1]);

    //Nombre de composantes par MCU
    uint8_t nb_components_y = sampling_factors[0][0]*sampling_factors[0][1];
    uint8_t nb_components_cb = sampling_factors[1][0]*sampling_factors[1][1];
    uint8_t nb_components_cr = sampling_factors[2][0]*sampling_factors[2][1];

    // Taille de l'image.
    uint16_t width = get_image_size(jdesc, DIR_H);
    uint16_t height = get_image_size(jdesc, DIR_V);
    printf("Taille de l'image : %d x %d\n", width, height);
    // Taille de l'image complétée.
    uint16_t width_ext = 0;
    uint16_t height_ext = 0;
    if(width%8){
        width_ext = width + 8 - width % 8;
    }
    else{
        width_ext = width;
    }
    if(height%8){
        height_ext = height + 8 - height % 8;
    }
    else{
        height_ext = height;
    }
    printf("Taille de l'image complétée : %d x %d\n", width_ext, height_ext);

    // Calcul du nombre de MCU
    uint16_t nb_mcus_h = width_ext / (8 * sampling_factors[0][0]);
    uint16_t nb_mcus_v = height_ext / (8 * sampling_factors[0][1]);
    uint16_t nb_mcus = nb_mcus_h * nb_mcus_v;
    printf("Nombre de MCU : %d\n", nb_mcus);

    // Création du tableau de mcus
    struct mcu **mcus = calloc(nb_mcus, sizeof(struct mcu *));


    // Récupération des tables et des informations utiles à l'extraction des mcus
    uint8_t **quant_tables = get_quant_tables(jdesc);
    struct huff_table ***huff_tables = get_huff_tables(jdesc);
    enum component *ordre_des_composantes = get_components_order(jdesc, sampling_factors);

    // Debug
    printf("Table de quantification index 0 : \n");
    for(uint8_t i=0; i<64; i++){
        printf("%hhu ", quant_tables[0][i]);
    } // end for
    printf("\n");

    // On extrait les mcus
    int16_t previous_dc_y = 0;
    int16_t previous_dc_cb = 0;
    int16_t previous_dc_cr = 0;
    for (size_t i = 0; i < nb_mcus; ++i) {
        printf("\n\n======================\n");
        printf("\nMCU %d :", i);
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

    }

    printf("\nPremière composante Y :\n");
    for (size_t i = 0; i < 64; i++) {
        printf("%"PRId16 " ", mcus[0]->components_y[0][i]);
    }
    printf("\n");

    /* Reconstruction des blocs */

    uint16_t nb_blocks = nb_components_y*nb_mcus;
    block *liste_blocks = malloc(nb_blocks * sizeof(block));
    block *blocks_temp = NULL;
    for (uint16_t i = 0; i < nb_mcus; i++) {
        blocks_temp = extract_blocks(mcus[i], sampling_factors);
        for (size_t j = 0; j < nb_components_y; j++) {
            liste_blocks[i*nb_components_y+j] = blocks_temp[0];
        }
        free(blocks_temp);
        blocks_temp = NULL;
    }

    if (est_couleur(jdesc)) {
        for (size_t i = 0; i < nb_blocks; i++) {
            convert_to_rgb(liste_blocks[i]);
        }
    }

    /*printf("\nComposante R si couleur, niveau de gris sinon\n");
    for (size_t i = 0; i < 64; i++) {
        printf("%"PRId16"\n", liste_blocks[0][0][i]);
    }
    printf("\n");*/


    /*******
    * Conversion en pixel *
    *******/

    struct picture *pic = blocks2pixels(liste_blocks,
                                        width,
                                        height,
                                        width_ext,
                                        height_ext,
                                        sampling_factors[COMP_Y][DIR_H], // H1
                                        sampling_factors[COMP_Y][DIR_V]);
    printf("\n");
    for (size_t i = 0; i < 64; i++) {
        printf("%d ", pic->pixels[i]->y);
    }
    printf("\n");

    /*******
    * Création de l'image PPM ou PGM *
    *****/

    write_ppm(pic, get_outfile_name(filename, pic->colored));

    // Libération mémoire du tableau de MCU
    for(uint16_t i=0; i< nb_mcus; i++){
         free_mcu(mcus[i]);
     } // end for
    free(mcus);

    /* Nettoyage de printemps : close_jpeg ferme aussi le bitstream
     * (voir Annexe C du sujet). */
    close_jpeg(jdesc);

    return EXIT_SUCCESS;
}
