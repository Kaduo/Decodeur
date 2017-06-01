#include "bitstream.h"

struct bitstream{
                              const char *filename;
                              FILE *pfile;
                              uint32_t buffer;
                              uint8_t bits_in_buffer;
                              uint8_t cur_byte;
                              uint8_t bits_in_cur_byte;
                              
                              bool end_of_stream;
};

struct bitstream *create_bitstream(const char *filename){
                              
                              struct bitstream *stream = malloc(sizeof(struct bitstream));
                              stream->buffer = 0;
                              stream->bits_in_buffer = 32;
                              stream->cur_byte = 0;
                              stream->bits_in_cur_byte = 8;
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

void load_cur_byte(struct bitstream *stream){
                              int16_t byte = fgetc(stream->pfile);
                              if(byte == -1){
                                                            stream->cur_byte = 0;
                                                            stream->bits_in_cur_byte = 0;
                              } // end if
                              else{
                                                            stream->cur_byte = byte;
                                                            stream->bits_in_cur_byte = 8;
                              } // end else
} // end def

// Rempli le buffer à partir de cur_byte.
void fill_buffer(struct bitstream *stream, uint8_t nb_bits){
                                                            while(nb_bits >0){
                                                                                          // Si on est arrivé à la fin du fichier (cur_byte est vide.
                                                                                          if(stream->bits_in_cur_byte == 0){
                                                                                                                        stream->bits_in_buffer -= nb_bits;
                                                                                                                        nb_bits = 0;
                                                                                          } // end if
                                                                                          
                                                                                          // Si on demande l'ensemble des bits de cur_byte:
                                                                                                      else if(nb_bits >= stream->bits_in_cur_byte){
                                                                                                                                    stream->buffer |= stream->cur_byte << (nb_bits - stream->bits_in_cur_byte);
                                                                                                                                    nb_bits -= stream->bits_in_cur_byte;
                                                                                                                                    load_cur_byte(stream);
                                                                                                      } // end if
                                                                                                      
                                                                                                      // On a besoin que de quelques bits de cur_byte:
                                                                                                      else{
                                                                                                                                    //if(nb_bits >= 3) printf("cas 3, nb_bits : %d, bits in byte : %d\n", nb_bits, stream->bits_in_cur_byte);
                                                                                                                                    uint8_t data = stream->cur_byte << (8-stream->bits_in_cur_byte);
                                                                                                                                    data >>= 8-nb_bits;
                                                                                                                                    //printf("byte : %04x, data : %04x\n", stream->cur_byte, data);
                                                                                                                                    stream->buffer |= data;
                                                                                                                                    stream->bits_in_cur_byte -= nb_bits;
                                                                                                                                    nb_bits=0;
                                                                                                      } // end else
                                                            } // end while
} //end def


uint8_t read_bitstream(struct bitstream *stream,
                              uint8_t nb_bits,
                              uint32_t *dest,
                              bool discard_byte_stuffing){
                              
                              //if(nb_bits >= 8) printf("nb_bits : %d\n", nb_bits); 
                              
                              
                              // Si fin de fichier.
                              if(end_of_bitstream(stream)){
                                                            *dest = 0;
                                                            return 0;
                              } // end if
                              
                              // On regarde combien de bits on peut lire au max.
                              if( nb_bits> stream->bits_in_buffer) nb_bits = stream->bits_in_buffer;
                              // On les lit, et on les décale si on ne lit pas tout.
                              *dest = stream->buffer >> (32-nb_bits);
                              
                              // Mise à jour du buffer.
                              // Supression des bits lu.
                              stream->buffer <<= nb_bits;
                              if (nb_bits == 32) stream->buffer = 0;
                              
                              // Si on à épuisé les données dans cur_byte, on déplace le pointeur du buffer.
                              if (stream->bits_in_cur_byte == 0){
                                                            stream->bits_in_buffer -= nb_bits;
                                                            // Si bits_in_buffer est null, le fichier et cur byte sont vide également. 
                                                            if (stream->bits_in_buffer == 0) stream->end_of_stream = true;
                              } // end if
                              else{ // On peut re-remplire le buffer
                                                            fill_buffer(stream, nb_bits);
                              } // end else

                              // discard byte stuffing
                              if(
                                                            discard_byte_stuffing &&
                                                            *dest == 0xFF &&
                                                            nb_bits == 8 &&
                                                            (stream->buffer & 0xFF000000) == 0){
                                                                                          perror("applique byte stuffing\n");
                                                            uint32_t tmp=0; 
                                                            read_bitstream(stream, 8, &tmp, false);
                              }
                              
                              return nb_bits;
} // end def

// La fonction end_of_bitstream retourne true si le flux a été entièrement parcouru, false s’il reste des bits à lire. 
bool end_of_bitstream(struct bitstream *stream){
                              return stream->end_of_stream;
} // end def

/* Optionnel! */
//extern void skip_bitstream_until(struct bitstream *stream, uint8_t byte);

