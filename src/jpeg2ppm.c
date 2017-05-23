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
    uint16_t width_ext = width + 8 - width%8;
    uint16_t height_ext = height + 8 - height%8;
    printf("Taille de l'image complétée : %d x %d\n", width_ext, height_ext);
    
    // Calcul du nombre de MCU
    uint16_t nb_mcu_h = width / (8 * h1); 
    uint16_t nb_mcu_v = height / (8 * v1); 
    uint16_t nb_mcu = nb_mcu_h * nb_mcu_v;
    printf("Nombre de MCU : %d\n", nb_mcu);
    
    // Extraction des MCU
    struct mcu **mcus = calloc(nb_mcu, sizeof(struct mcu *));
    for(uint16_t i=0; i< nb_mcu; i++){
        mcus[i] = extract_mcu(stream, jdesc);
     } //end for 
     
     
    
    
    // Libération mémoire du tableau de MCU
    for(uint16_t i=0; i< nb_mcu; i++)
    {
         free(mcus[i]);
     } // end for
    free(mcus);
    
    


    /* Nettoyage de printemps : close_jpeg ferme aussi le bitstream
     * (voir Annexe C du sujet). */
    close_jpeg(jdesc);

    return EXIT_SUCCESS;
}
