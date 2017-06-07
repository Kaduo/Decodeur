#include "bitstream.h"

struct bitstream{
    const char *filename;
    FILE *pfile;
    uint8_t byte;
    uint8_t byte_pos;
    uint8_t next;
    bool next_available;
    bool end_of_stream;
};

struct bitstream *create_bitstream(const char *filename){

    struct bitstream *stream = malloc(sizeof(struct bitstream));
    stream->byte = 0;
    stream->byte_pos = 0;
    stream->next = 0;
    stream->next_available = true;
    stream->end_of_stream = false;
    stream->filename = filename;

    // Ouvertture du fichier
    stream->pfile = fopen(filename, "rb");
    if (stream->pfile == NULL){
        perror("File not found\n");
    exit(EXIT_FAILURE);
    } // end if error

    // Chargement des  deux buffers.
    // 1
    size_t nb_reads = fread(&stream->byte, sizeof(uint8_t), 1, stream->pfile);
    // 2
    nb_reads += fread(&stream->next, sizeof(uint8_t), 1, stream->pfile);
    if(nb_reads != 2){
        perror("Erreur lors de la lecture des deux premiers octets.");
        exit(EXIT_FAILURE);
    } // end if error
    return stream;
} // end def

void close_bitstream(struct bitstream *stream){

    //Fermetture du fichier
    if(fclose(stream->pfile)) perror("Erreur à la fermeture de l'image jpeg\n");
    free(stream);
} // end def

void fill_next(struct bitstream *stream){
    int16_t result = fgetc(stream->pfile);
    if( result == -1) {
        stream->next_available = false;
     } else{
         stream->next = result;
         stream->next_available = true;
     } // end else
} // end def

void del_byte_stuffing(struct bitstream *stream){
    if(stream->byte == 0xFF && stream->next == 0x00){
        trace("\nBitstream : Byte stuffing\n");
        fill_next(stream);
    } // end if
} // end def

uint8_t read_bitstream(struct bitstream *stream,
    uint8_t nb_bits,
    uint32_t *dest,
    bool discard_byte_stuffing){

    *dest = 0;
    // Si fin de fichier.
    if(end_of_bitstream(stream)) return 0;

    // Si pas de bit demandé.
    if(nb_bits == 0) return 0;

    uint8_t nb_bits_read = 0;

   // Lecture en mode rapide pour lecture d'octet alligné.
    while(nb_bits >= 8 && stream->byte_pos == 0){
        *dest <<= 8;
        *dest |= stream->byte;
        nb_bits-=8;
        nb_bits_read += 8;

        // Si on peut encore lire le fichier.
        if(stream->next_available){
            if(discard_byte_stuffing) del_byte_stuffing(stream);
            stream->byte = stream->next;
            fill_next(stream);
        } else { // On ne peut plus approvisionner byte, fin du stream.
            stream->end_of_stream = true;
        } // end else
   } // end fast read

    // Lecture bit à bit.
    for(uint8_t i=0; i < nb_bits; i++){
        // On lit un bit.
        *dest <<= 1;
        *dest |= (uint8_t)(stream->byte << stream->byte_pos) >> 7;
        nb_bits_read++;
        stream->byte_pos++;

        // Si on a vidé byte.
        if(stream->byte_pos == 8){
            // Si on peut encore lire le fichier.
            if(stream->next_available){
                if(discard_byte_stuffing) del_byte_stuffing(stream);
                stream->byte = stream->next;
                fill_next(stream);
                stream->byte_pos = 0;
            } else {
                stream->end_of_stream = true;
                return nb_bits_read;
            } // end else
        } // end if
    } // end for
    return nb_bits_read;
} // end def


// La fonction end_of_bitstream retourne true si le flux a été entièrement parcouru, false s’il reste des bits à lire.
bool end_of_bitstream(struct bitstream *stream){
    return stream->end_of_stream;
} // end def



/* Optionnel! */
extern void skip_bitstream_until(struct bitstream *stream, uint8_t byte){
    stream->byte_pos = 0;
    while(stream->next != byte && stream->next_available == true){
        fill_next(stream);
    } // end while
    if( stream->next_available){
        fill_next(stream);
        stream->byte = stream->next;
        if( stream->next_available){
            fill_next(stream);
        } // end if
    } else {
        stream->end_of_stream = true;
    } // end if

} // end def
