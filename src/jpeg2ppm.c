#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "jpeg_reader.h"
#include "bitstream.h"

#include "extraction.h"
#include "reconstruction.h"
#include "ppm.h"
#include "rgb.h"
#include "upsampling.h"

bool est_couleur(const struct jpeg_desc *jpeg){
    return get_nb_components(jpeg) > 1;
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
        width_ext = width + 8 - width % 8;
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

    // Extraction des MCU
    struct mcu **mcus = calloc(nb_mcus, sizeof(struct mcu *));
    for(uint16_t i=0; i< nb_mcus; i++){
        mcus[i] = extract_mcu(stream, nb_components_y, nb_components_cb, nb_components_cr, jdesc);
    } // end for

    // Pour debug : Affiche un composant mcu 1, Y ! OK!
    printf("Debug : mcu 0, composante Y : \n");
    for(uint8_t j=0; j<64;j++){
            printf("%d ", mcus[0]->components_y[0][j]);
         } //end for
         printf("\n");

     /************
     *   Quantification inverse  *
     *********/

     uint8_t nb_quant_tables = get_nb_quantization_tables(jdesc);
     printf("Nombre de table de quantification : %hhu\n", nb_quant_tables);
     uint8_t **quant_tables = calloc(nb_quant_tables, sizeof(uint8_t *));
     for(uint8_t j=0; j < nb_quant_tables; j++){
         quant_tables[j] = get_quantization_table(jdesc, j);
     } // end for

     // Debug
     printf("Table de quantification index 0 : \n");
     for(uint8_t i=0; i<64; i++){
         printf("%hhu ", quant_tables[0][i]);
     } // end for
     printf("\n");


     // Boucle sur les MCUs
     for(uint16_t m = 0; m < nb_mcus; ++m) {
         /* Quantification inverse sur les composantes Y */
         for(uint8_t y = 0; y < nb_components_y; ++y) {
             inverse_quant(mcus[m]->components_y[y], quant_tables[0]);
         }
         if(est_couleur(jdesc)) {
             /* Quantification inverse sur les composantes Cb */
             for(uint8_t cb = 0; cb < nb_components_cb; ++cb) {
                 inverse_quant(mcus[m]->components_cb[cb], quant_tables[1]);
             }
             /* Quantification inverse sur les composantes Cr */
             for(uint8_t cr = 0; cr < nb_components_cr; ++cr) {
                 inverse_quant(mcus[m]->components_cr[cr], quant_tables[1]);
             }
         }
     }

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
