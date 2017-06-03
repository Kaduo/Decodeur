#include "jpeg2ppm.h"
#include <stdbool.h>
#include <string.h>

int main(int argc, char **argv)
{
    /* On recupere le nom du fichier JPEG sur la ligne de commande. */
    const char *filename = argv[1];


    FILE *pf = fopen("test.jpeg", "wb");
    uint8_t data[] = {
    0xaa, 1, 0x55, 0x55, 4, 5, 6, 0xff, 7, 8, 0xff, 0x00, 9};
    fwrite(data, sizeof(data), 1, pf);
    fclose(pf);
    
    // TEste bitstream    
    struct bitstream *stream = create_bitstream(filename);
uint32_t out=0;
size_t nb_bits = 8;
for(size_t i=0; i<20; i++){
    read_bitstream(stream, nb_bits, &out,  true);
    printf("%x ", out);
    if(i%4==3) printf("\n");
    if(end_of_bitstream(stream)) printf("End of stream\n");
    }    
    close_bitstream(stream);

}