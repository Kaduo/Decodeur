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
    uint8_t h1 = get_frame_component_sampling_factor(jdesc, DIR_H, 0);
    uint8_t v1 = get_frame_component_sampling_factor(jdesc, DIR_V, 0);
    printf("H1 : %hhu, V1 : %hhu\n", h1, v1);
    uint8_t nb_mcu = h1 * v1; 
    printf("Nombre de MCU : %hhu\n", nb_mcu);
    // Extraction des MCU
    
    struct mcu *mcus = calloc(nb_mcu, sizeof(struct mcu));
    //extract_mcu(const struct bitstream *bitstream, const struct jpeg_desc *jpeg);
    
    


    /* Nettoyage de printemps : close_jpeg ferme aussi le bitstream
     * (voir Annexe C du sujet). */
    close_jpeg(jdesc);

    return EXIT_SUCCESS;
}
