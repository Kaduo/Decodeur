#include "bitstream.h"

struct bitstream{
                              const char *filename;
                              FILE *pfile;
                              uint32_t buffer;
                              uint8_t cur_byte;
                              uint8_t cur_byte_pos;
                              
                              bool end_of_stream;
};

struct bitstream *create_bitstream(const char *filename){
                              
                              struct bitstream *stream = malloc(sizeof(struct bitstream));
                              stream->buffer = 0;
                              stream->cur_byte = 0;
                              stream->cur_byte_pos = 0;
                              stream->end_of_stream = false;
                              stream->filename = filename;
                              
                              // Ouvertture du fichier
                              stream->pfile = fopen(filename, "rb");
                              	if (stream->pfile == NULL){
                              	                              perror("File not found\n");
                              	exit(EXIT_FAILURE);
                              	 } // end if error 
                              	 
                              	 // Chargement des 32 premiers bits.
                              	 uint32_t byte = 0;
                              	 // 1
                              	 size_t nb_reads = fread(&byte, sizeof(uint8_t), 1, stream->pfile);
                              	 stream->buffer = byte <<24;
                              	 // 2
                              	 byte = 0;
                              	 nb_reads += fread(&byte, sizeof(uint8_t), 1, stream->pfile);
                              	 byte <<= 16;
                              	 stream->buffer |= byte;
                              	 // 3
                              	 byte = 0;
                              	 nb_reads += fread(&byte, sizeof(uint8_t), 1, stream->pfile);
                              	                               	 byte <<= 8;
                              	                               	 stream->buffer |= byte;
                              	                               	 // 4
                              	                               	 byte = 0;
                              	                               	 nb_reads += fread(&byte, sizeof(uint8_t), 1, stream->pfile);
                              	                               	 stream->buffer |= byte;
                              	                               	 
                              	                               	 // 5, cur_byte
                              	                               	 nb_reads += fread(&stream->cur_byte, sizeof(uint8_t), 1, stream->pfile);
                              	                               	 
                              	 if(nb_reads != 5){
                              	                               perror("Erreur lors de la lecture des cinq premiers octets.");
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

