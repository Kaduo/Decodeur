#include <stdio.h>
#include "reconstruction.h"

int main(){
    int16_t component[64] = {0x1b9, 0x8, 0xfff8, 0, 0, 0, 0, 0, 0x40, 0xffee, 0xfffd, 0, 0, 0, 0, 0, 0xfff1, 0xc, 0x6, 0, 0, 0, 0, 0, 0x3, 0, 0xfff8, 0x6, 0, 0, 0, 0, 0xfffc, 0xfffc, 0, 0, 0, 0, 0, 0, 0x5, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    int16_t *resultat;
    resultat = idct(component);
    for (size_t i = 0; i < 64; i++) {
        printf("%04x ", resultat[i]);
    }
    printf("\n");
    free(resultat);
    return 0;
}
