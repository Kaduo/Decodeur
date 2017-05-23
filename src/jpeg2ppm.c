#include <stdlib.h>
#include <stdio.h>

#include "jpeg_reader.h"
#include "bitstream.h"

#include "extraction.h"
#include "reconstruction.h"
#include "ppm.h"
#include "rgb.h"
#include "upsampling.h"

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

    // H1 V1
    uint8_t h1 = get_frame_component_sampling_factor(jdesc, DIR_H, 0);
    uint8_t v1 = get_frame_component_sampling_factor(jdesc, DIR_V, 0);
    printf("H1 : %hhu, V1 : %hhu\n", h1, v1);

    // Taille de l'image.
    uint16_t width = get_image_size(jdesc, DIR_H);
    uint16_t height = get_image_size(jdesc, DIR_V);
    printf("Taille de l'image : %d x %d\n", width, height);
    // Taille de l'image complétée.
    uint16_t width_ext;
    uint16_t height_ext;
    if(width%8){
        width_ext = width + 8 - width%8;
    }
    else{
        width_ext = width;
    }
    if(height%8){
        width_ext = width + 8 - width%8;
    }
    else{
        height_ext = height;
    }
    printf("Taille de l'image complétée : %d x %d\n", width_ext, height_ext);

    // Calcul du nombre de MCU
    uint16_t nb_mcus_h = width / (8 * h1); 
    uint16_t nb_mcus_v = height / (8 * v1); 
    uint16_t nb_mcus = nb_mcus_h * nb_mcus_v;
    printf("Nombre de MCU : %d\n", nb_mcus);
    
    // Extraction des MCU
    struct mcu **mcus = calloc(nb_mcus, sizeof(struct mcu *));
    for(uint16_t i=0; i< nb_mcus; i++){
        mcus[i] = extract_mcu(stream, nb_components, jdesc);
    } // end for
    
    // For debug : print mcu 1, Y ! OK!
    printf("Debug : mcu 1, composante Y : \n");
    for(uint8_t j=0; j<64;j++){
            printf("%hhu ", mcus[0]->components[0][j]);
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
       
       // Pour tout les MCUS
     for(uint16_t i=0; i< nb_mcus; i++){
         // Pour tout les composants.
         for(uint8_t j=0; j<nb_components; j++){
             // Si j=0, on est sur la composante Y. Il faut la table 0.
             uint8_t id_table = 0;
             if(j!=0){
                 id_table = 1;
             } // end if
             printf("dans la boucle : i = %d, j= %hhu, id_table = %hhu\n", i, j, id_table);
             //**********************************
             // ICI erreur de segmentation!! Erreur de segmentation (core dumped)                                                                                                                                    
             inverse_quant(mcus[i]->components[j], quant_tables[id_table]);
             printf("i = %d, j= %hhu\n", i, j);
         } // end for components
     } // end for MCUS
     
     
     
    
    

    // Libération mémoire du tableau de MCU
    for(uint16_t i=0; i< nb_mcus; i++){
         free_mcu(mcus[i], nb_components);
     } // end for
    free(mcus);




    /* Nettoyage de printemps : close_jpeg ferme aussi le bitstream
     * (voir Annexe C du sujet). */
    close_jpeg(jdesc);

    return EXIT_SUCCESS;
}
