#ifndef __UPSAMPLING_H__
#define __UPSAMPLING_H__

#include <stdint.h>
#include <stdbool.h>
#include "bitstream.h"
#include "extraction.h"

const size_t TAILLE_BLOC = 64;

/* Structure représentant un bloc */
struct block {
    uint8_t y[TAILLE_BLOC];
    uint8_t cb[TAILLE_BLOC];
    uint8_t cr[TAILLE_BLOC];
}

/* Suréchantillonne le MCU donné */
extern struct block *upsampling(const struct mcu *mcu);

#endif
