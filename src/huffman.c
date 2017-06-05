#include "huffman.h"

struct huff_table{
    uint16_t nb_codes;
};

extern struct huff_table *load_huffman_table(struct bitstream *stream,
    uint16_t *nb_byte_read){
printf("Fonction load huffman table\n");    
    // Init
    struct huff_table *table = calloc(sizeof(struct huff_table), 1);
    
        uint16_t nb_codes = 0;
        *nb_byte_read = 0;
        uint8_t nb_level_codes[16];
        
        // Lecture du nombre de code par niveau.
        for(uint8_t i=0; i< 16; i++){
            *nb_byte_read += read_bitstream(stream, 8, (uint32_t*) &nb_level_codes[i], true);
            nb_codes += nb_level_codes[i];
            printf("%d codes au niveau %hhu\n", nb_level_codes[i], i+1);
        } // end for
        printf("Nombre de code : %d\n", nb_codes);
    exit(EXIT_FAILURE);
        return table;        
    
} // end def

extern int8_t next_huffman_value(struct huff_table *table,
struct bitstream *stream){

    printf("Fonction next_huffman_value\n");
    exit(EXIT_FAILURE);
    stream = 0;
    table = 0;
    return 0;

} // end def


extern int8_t next_huffman_value_count(struct huff_table *table,
struct bitstream *stream,
uint8_t *nb_bits_read){

printf("Fonction next_huffman_value_count\n");
exit(EXIT_FAILURE);
    stream = 0;
    table = 0;
    nb_bits_read = 0;
    return 0;

} // end def

extern void free_huffman_table(struct huff_table *table){
    free(table);
} // end def

