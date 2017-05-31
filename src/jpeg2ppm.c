#include "jpeg2ppm.h"
#include <stdbool.h>
#include <string.h>

int main(int argc, char **argv)
{
    /* On recupere le nom du fichier JPEG sur la ligne de commande. */
    const char *filename = argv[1];


    FILE *pf = fopen("test.jpeg", "wb");
    fputc(0xAA, pf);
    fputc(0x34, pf);
    fputc(0x56, pf);
    fputc(0xab, pf);
    fputc(0xff, pf);
    fclose(pf);
    
    // TEste bitstream    
    struct bitstream *stream = create_bitstream(filename);
    uint32_t lecture = 0;
    uint8_t nb_bits = 40;
    uint8_t nb_lecture = 0;
    nb_lecture = read_bitstream(stream, nb_bits, &lecture, false);
    
    printf("Nombre de lecture : %d\n", nb_lecture);
    printf("On a lu : %04x\n", lecture);
    
    close_bitstream(stream);

}