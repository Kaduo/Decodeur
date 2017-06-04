#include "huffman.h"

struct huff_table{
    uint16_t nb_codes;
};

extern struct huff_table *load_huffman_table(struct bitstream *stream,
    uint16_t *nb_byte_read){
    
} // end def

extern int8_t next_huffman_value(struct huff_table *table,
struct bitstream *stream){

} // end def


extern int8_t next_huffman_value_count(struct huff_table *table,
struct bitstream *stream,
uint8_t *nb_bits_read){

} // end def

extern void free_huffman_table(struct huff_table *table){

} // end def

