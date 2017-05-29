#ifndef __RGB_H__
#define __RGB_H__

/* Structure representant un pixel RGB */
struct pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

/* Structure representant une image */
struct picture {
    size_t size;
    struct pixel *pixels;
};

#endif
