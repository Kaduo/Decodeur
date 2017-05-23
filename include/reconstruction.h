#ifndef __RECONSTRUCTION_H__
#define __RECONSTRUCTION_H__

#include <stdint.h>
#include <stdbool.h>
#include "bitstream.h"
#include "extraction.h"

/* Orientation dans le zigzag */
enum orientation {
  ORI_E = 0, // Vers la droite
  ORI_S = 1, // Vers le bas
  ORI_NE = 2, // Vers le haut droit
  ORI_SW = 3, // Vers le bas gauche
  /* Sentinelle */
  ORI_NB
};

/* Structure de point ordonné */
struct point {
  int8_t x;
  int8_t y;
};

/* Quantification inverse : multiplie la composante par la table de quantification donnée */
extern void inverse_quant(int16_t *component, uint8_t *quant_table);

/* Zig-zag inverse de la composante */
extern void zag_zig(int16_t *component);

/* Met à jour le point courant */
extern void update_point(struct point *point, enum orientation orientation);

/* Renvoie l'orientation suivante */
enum orientation next_orientation(enum orientation ori, const struct point *point);

/* Applique la transformée en cosinus discrète inverse sur la composante */
extern void idct(int16_t *component);

/* Transforme un MCU fréquentiel en spatial */
extern void reconstruct_mcu(struct mcu *mcu, const struct jpeg_desc *jpeg);

#endif
