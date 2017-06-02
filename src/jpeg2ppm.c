#include "jpeg2ppm.h"
#include <stdbool.h>
#include <string.h>

int main(int argc, char **argv)
{
    /* On recupere le nom du fichier JPEG sur la ligne de commande. */
    const char *filename = argv[1];


    FILE *pf = fopen("test.jpeg", "wb");
    uint8_t text[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    fwrite ( text, sizeof(text), 1, pf);
    /*fputc(0xAA, pf);
    fputc(0x00, pf);
    fputc(0x00, pf);
    fputc(0x01, pf);
    fputc(0xFF, pf);*/
    fclose(pf);
    
    // TEste bitstream    
    struct bitstream *stream = create_bitstream(filename);
    uint32_t lecture = 0;
    
    
    for(size_t i=0; i<50;i++){
        uint8_t nb_bits = 8;//i%8;
        uint8_t nb_lecture = 0;
            nb_lecture = read_bitstream(stream, nb_bits, &lecture, true);
            if(nb_lecture == 0) break;
            if(i%4==0) printf("\n");
            //printf("Nombre de lecture : %d\n", nb_lecture);
            printf("%x ", lecture);
            
            }
        /*
    uint8_t nb_bits = 8;
    uint8_t nb_lecture = 0;
    nb_lecture = read_bitstream(stream, nb_bits, &lecture, false);
    printf("Nombre de lecture : %d\n", nb_lecture);
    printf("On a lu : %04x\n", lecture);
    
    nb_bits = 8;
    nb_lecture = read_bitstream(stream, nb_bits, &lecture, false);
    printf("Nombre de lecture : %d\n", nb_lecture);
        printf("On a lu : %04x\n", lecture);

        nb_lecture = read_bitstream(stream, nb_bits, &lecture, false);
                printf("Nombre de lecture : %d\n", nb_lecture);
                    printf("On a lu : %04x\n", lecture);
                    
                    nb_lecture = read_bitstream(stream, nb_bits, &lecture, false);
                    printf("Nombre de lecture : %d\n", nb_lecture);
                        printf("On a lu : %04x\n", lecture);
                        if(end_of_bitstream(stream) == true) printf("End of stream true\n");

        
        
nb_lecture = read_bitstream(stream, nb_bits, &lecture, false);
    printf("Nombre de lecture : %d\n", nb_lecture);
        printf("On a lu : %04x\n", lecture);
        if(end_of_bitstream(stream) == true) printf("End of stream true\n");

nb_lecture = read_bitstream(stream, nb_bits, &lecture, false);
    printf("Nombre de lecture : %d\n", nb_lecture);
        printf("On a lu : %04x\n", lecture);
        
if(end_of_bitstream(stream) == true) printf("End of stream true\n");
    
*/            
    close_bitstream(stream);

}