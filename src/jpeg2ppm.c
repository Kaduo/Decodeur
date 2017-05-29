/*******************************************************
Nom ......... : jpeg2ppm.c
Role ........ : Point d'entree du programme
                Decode une image JPEG dans un format PPM
Auteurs .... : A. He - M. Nebra - B. Potet (Ensimag 1A 2016/2017 - G6)

Pour compiler : make
Pour exécuter : ./jpeg2ppm jpeg
********************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "jpeg_reader.h"
#include "bitstream.h"

int main(int argc, char **argv)
{
    /* Verification du bon nombre d'arguments (la commande et le fichier) */
    if (argc != 2) {
        fprintf(stderr, "Utilisation : %s fichier.jpeg\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Recuperation du nom du fichier JPEG */
    const char *filename = argv[1];
    /* Creation du descripteur qui permettra de lire ce fichier */
    struct jpeg_desc *jdesc = read_jpeg(filename);
    /* Recuperation du flux des donnees brutes a partir du descripteur */
    struct bitstream *stream = get_bitstream(jdesc);

    
    /* Fermeture du flux de données */
    close_jpeg(jdesc);

    return EXIT_SUCCESS;
}
