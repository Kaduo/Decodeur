#include "bitstream.h"

struct bitstream{
                              char *filename;
};

struct bitstream *create_bitstream(const char *filename){
                              struct bitstream *stream = malloc(sizeof(struct bitstream));
                              return stream;
} // end def

void close_bitstream(struct bitstream *stream){
                              free(stream);
} // end def

uint8_t read_bitstream(struct bitstream *stream,
                              uint8_t nb_bits,
                              uint32_t *dest,
                              bool discard_byte_stuffing){
                              
                              return 0;
} //end def

bool end_of_bitstream(struct bitstream *stream){
                              return false;
} // end def

/* Optionnel! */
//extern void skip_bitstream_until(struct bitstream *stream, uint8_t byte);

