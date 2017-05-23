#ifndef __UPSAMPLING_H__
#define __UPSAMPLING_H__

#include <stdint.h>
#include <stdbool.h>
#include "bitstream.h"
#include "extraction.h"

/* Structure représentant un bloc */
struct block {
    uint8_t y[64];
    uint8_t cb[64];
    uint8_t cr[64];
};

/* Suréchantillonne le MCU donné */
extern struct block *upsampling(const struct mcu *mcu, uint8_t *sampling_factors);

#endif
