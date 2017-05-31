#include "jpeg2ppm.h"
#include <stdbool.h>
#include <string.h>

int main(int argc, char **argv)
{
    /* On recupere le nom du fichier JPEG sur la ligne de commande. */
    const char *filename = argv[1];


    FILE *pf = fopen("test.jpeg", "wb");
    fputc(0b10101010, pf);
    fclose(pf);
    
    // TEste bitstream    
    struct bitstream *stream = create_bitstream(filename);
    
    close_bitstream(stream);

}