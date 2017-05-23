#include <math.h>
#include "reconstruction.h"

#define PI 3.14159265

void inverse_quant(int16_t *component, uint8_t *quant_table){
    /*bouchon*/
    return;
}

void zag_zig(int16_t *component){
    /*bouchon*/
    return;
}

float C(int16_t ksi){
    if (ksi == 0) {
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
