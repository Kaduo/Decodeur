#include <stdio.h>
#include "reconstruction.h"

int main(){
    int16_t component[64] = {0x7c, 0, 0xfee4, 0, 0, 0, 0xffec, 0, 0xfffa, 0, 0xfedb, 0, 0x6a, 0, 0xff7d, 0, 0xfeb3,
         0, 0xff8b, 0, 0x45, 0, 0x9a, 0, 0xff36, 0, 0xffa0, 0, 0x19, 0, 0x4, 0, 0x80, 0, 0x11c, 0, 0xff, 0, 0x14, 0,
          0xffa6, 0, 0x13, 0, 0x7d, 0, 0xfe1f, 0, 0xff76, 0, 0x1a, 0, 0xff59, 0, 0xf4, 0, 0xdd, 0, 0xffa8, 0, 0xffb9, 0, 0xff3c, 0};

    int16_t *resultat;
    resultat = idct(component);
    for (size_t i = 0; i < 64; i++) {
        printf("%04x ", resultat[i]);
    }
    printf("\n");
    free(resultat);
    return 0;
}
