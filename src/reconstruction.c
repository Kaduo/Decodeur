#include <stdlib.h>
#include <stdio.h>
#include "reconstruction.h"

/* Taille d'un bloc */
const size_t TAILLE_BLOC = 8;

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
    for(int i = 0; i < TAILLE_BLOC * TAILLE_BLOC; ++i) {
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

void idct(int16_t *component){
    /*bouchon*/
    return;
}

void reconstruct_mcu(struct mcu *mcu, const struct jpeg_desc *jpeg){
    /*bouchon*/
    return;
}
