#include "bitstream.h"

struct bitstream{
                              const char *filename;
                              FILE *pfile;
                              int32_t buffer;
                              bool end_of_stream;
};

struct bitstream *create_bitstream(const char *filename){
                              
                              struct bitstream *stream = malloc(sizeof(struct bitstream));
                              stream->buffer = 0;
                              stream->end_of_stream = false;
                              stream->filename = filename;
                              
                              // Ouvertture du fichier
                              stream->pfile = fopen(filename, "rb");
                              	if (stream->pfile == NULL){
                              	                              perror("File not found\n");
                              	exit(EXIT_FAILURE);
                              	 } // end if error 
                              
                              
                              
                              return stream;
} // end def

void close_bitstream(struct bitstream *stream){
                              
                              //Fermetture du fichier
                              if(fclose(stream->pfile)) perror("Erreur à la fermeture de l'image jpeg\n");
                              free(stream);
} // end def

uint8_t read_bitstream(struct bitstream *stream,
                              uint8_t nb_bits,
                              uint32_t *dest,
                              bool discard_byte_stuffing){
                              stream->buffer = 1;
                              return 0;
} //end def

// La fonction end_of_bitstream retourne true si le flux a été entièrement parcouru, false s’il reste des bits à lire. 
bool end_of_bitstream(struct bitstream *stream){
                              return stream->end_of_stream;
} // end def

/* Optionnel! */
//extern void skip_bitstream_until(struct bitstream *stream, uint8_t byte);

