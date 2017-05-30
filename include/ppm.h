#ifndef __PPM_H__
#define __PPM_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* Cree un fichier PPM a partir d'une image (tableau uint32_t) */
extern void create_image_ppm(const struct block *blocks,
                        uint32_t width,
                        uint32_t height,
                        char *filename);

/* Ecrit l'entete d'un fichier PPM */
extern void write_ppm_header(uint32_t width, uint32_t height, char *filename);

/* Ecrit les données d'un fichier PPM */
extern void write_ppm_data(const struct block *blocks,
                      uint32_t width,
                      uint32_t height,
                      char *filename);

/* Ouvre un fichier dans un mode donne */
extern void open_file(FILE **file, char *filename, char *mode);

/* Ferme un fichier */
extern void close_file(FILE **file, char *filename);

#endif
