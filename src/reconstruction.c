#include <stdio.h>
#include <math.h>
#include "reconstruction.h"

/* Taille d'un bloc */
const size_t TAILLE_BLOC = 8;

/* Valeur approchée de PI */
#define PI 3.14159265359

void inverse_quant(int16_t *component, uint8_t *quant_table)
{
    /* Multiplication terme à terme de la composante par la table de quantification */
    for (size_t i = 0; i < TAILLE_BLOC * TAILLE_BLOC; ++i) {
      component[i] *= quant_table[i];
    }
}

int16_t *zag_zig(int16_t *component)
{
    int16_t *inverse = malloc(sizeof(int16_t) * TAILLE_BLOC * TAILLE_BLOC);
    /* Initialisation du point courant à (0,0) et de l'orientation à droite */
    struct point current_point = {0, 0};
    enum orientation current_orientation = ORI_E;
    /* Parcours de chaque valeur dans le vecteur zigzag */
    for(size_t i = 0; i < TAILLE_BLOC * TAILLE_BLOC; ++i) {
        inverse[current_point.y*TAILLE_BLOC+current_point.x] = component[i];
        update_point(&current_point, current_orientation);
        current_orientation = next_orientation(current_orientation, &current_point);
    }
    return inverse;
}

void update_point(struct point *point, enum orientation orientation)
{
    switch(orientation) {
        case ORI_E:
            point->x += 1;
            break;
        case ORI_S:
            point->y += 1;
            break;
        case ORI_NE:
            point->x += 1;
            point->y -= 1;
            break;
        case ORI_SW:
            point->x -= 1;
            point->y += 1;
            break;
        default:
            perror("Orientation inconnue.");
    }
}

enum orientation next_orientation(enum orientation ori, const struct point *point)
{
    switch(ori) {
        case ORI_E:
            return (point->y == 3) ? ORI_NE : ORI_SW;
        case ORI_S:
            return (point->x == 3) ? ORI_SW : ORI_NE;
		    case ORI_NE:
            if(!(point->y == 0) && !(point->x == 3)) {
				          return ORI_NE;
			      } else if (point->x == 3) {
				          return ORI_S;
			      } else {
				          return ORI_E;
			      }
		    case ORI_SW:
			      if(!(point->y == 3 || point->x == 0)) {
				          return ORI_SW;
			      } else if (point->y == 3) {
				          return ORI_E;
			      } else {
				          return ORI_S;
			      }
        default:
            perror("Orientation inconnue.");
  }
	return ORI_NB;
}

float C(int16_t x){
    if (x == 0) {
        return 1/sqrt(2.);
    }
    return 0;
}

int16_t *idct(int16_t *component){
    float coefficients[63];
    float resultat_float[63];

    int16_t *resultat;
    resultat = calloc(64, sizeof(int16_t));

    for (size_t lambda = 0; lambda < 8; lambda++) {
        for (size_t mu = 0; mu < 8; mu++) {
            coefficients[mu*8 + lambda] = C(mu)*C(lambda)*component[mu*8 + lambda];
        }
    }

    for (size_t x = 0; x < 8; x++) {
        for (size_t y = 0; y < 8; y++) {
            for (size_t lambda = 0; lambda < 8; lambda++) {
                for (size_t mu = 0; mu < 8; mu++) {
                    resultat_float[y*8 + x] += coefficients[mu*8 + lambda] * cos((2*x + 1)*lambda*PI/16)
                                                                            * cos((2*y + 1)*mu*PI/16);
                }
            }
            resultat_float[y*8 + x] += 128;
            if (resultat_float[y*8 + x] < 0) {
                resultat_float[y*8 + x] = 0;
            }
            else if (resultat_float[y*8 + x] > 255) {
                resultat_float[y*8 + x] = 255;

            }
            resultat[y*8 + x] = resultat_float[y*8 + x];
        }
    }

    return resultat;
}

void reconstruct_mcu(struct mcu *mcu, const struct jpeg_desc *jpeg){
    /*bouchon*/
    return;
}
