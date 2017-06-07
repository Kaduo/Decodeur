/*******************************************************************************
Nom ......... : bitstream.c
Role ........ : Fonctions de gestion du flux de données
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include <stdio.h>
#include "trace.h"
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

struct bitstream *create_bitstream(const char *filename)
{
    struct bitstream *stream = malloc(sizeof(struct bitstream));
    stream->byte = 0;
    stream->byte_pos = 0;
    stream->next = 0;
    stream->next_available = true;
    stream->end_of_stream = false;
    stream->filename = filename;

    /* Ouverture du fichier */
    stream->pfile = fopen(filename, "rb");
    if (stream->pfile == NULL) {
        fprintf(stderr, "Fichier non trouvé !\n");
        exit(EXIT_FAILURE);
    }

    /* Chargement des  deux buffers */

    /* Premier buffer */
    size_t nb_reads = fread(&stream->byte, sizeof(uint8_t), 1, stream->pfile);
    /* Deuxième buffer */
    nb_reads += fread(&stream->next, sizeof(uint8_t), 1, stream->pfile);
    if(nb_reads != 2) {
        fprintf(stderr, "Erreur lors de la lecture des deux premiers octets.\n");
        exit(EXIT_FAILURE);
    }
    return stream;
}

void close_bitstream(struct bitstream *stream)
{
    /* Fermeture du fichier */
    if (fclose(stream->pfile)) {
        fprintf(stderr, "Erreur à la fermeture de l'image jpeg.\n");
        exit(EXIT_FAILURE);
    }
    free(stream);
}

void fill_next(struct bitstream *stream)
{
    int16_t result = fgetc(stream->pfile);
    if (result == -1) {
        stream->next_available = false;
     } else {
         stream->next = result;
         stream->next_available = true;
     }
}

void del_byte_stuffing(struct bitstream *stream){
    if (stream->byte == 0xff && stream->next == 0x00) {
        trace("Saut d'un octet de bourrage.\n");
        fill_next(stream);
    }
}

uint8_t read_bitstream(struct bitstream *stream,
                        uint8_t nb_bits,
                        uint32_t *dest,
                        bool discard_byte_stuffing)
{
    *dest = 0;

    /* Si fin de fichier, fin de fonction */
    if(end_of_bitstream(stream)) {
        return 0;
    }

    /* Si on ne demande pas de bit, fin de fonction */
    if (nb_bits == 0) {
        return 0;
    }

    uint8_t nb_bits_read = 0;

    /* Lecture en mode rapide pour lecture d'octet aligné */
    while (nb_bits >= 8 && stream->byte_pos == 0) {
        *dest <<= 8;
        *dest |= stream->byte;
        nb_bits-=8;
        nb_bits_read += 8;

        /* Si on peut encore lire le fichier, remplissage du buffer */
        if (stream->next_available){
            if(discard_byte_stuffing) del_byte_stuffing(stream);
            stream->byte = stream->next;
            fill_next(stream);
        } else { // On ne peut plus approvisionner octet, fin du stream
            stream->end_of_stream = true;
        }
    }

    /* Lecture bit à bit */
    for (uint8_t i = 0; i < nb_bits; ++i) {
        /* Lecture d'un bit */
        *dest <<= 1;
        *dest |= (uint8_t)(stream->byte << stream->byte_pos) >> 7;
        nb_bits_read++;
        stream->byte_pos++;

        /* Cas où un octet a été vidé */
        if (stream->byte_pos == 8) {
            /* Si on peut encore lire le fichier... */
            if(stream->next_available){
                if(discard_byte_stuffing) del_byte_stuffing(stream);
                stream->byte = stream->next;
                fill_next(stream);
                stream->byte_pos = 0;
            } else {
                stream->end_of_stream = true;
                return nb_bits_read;
            }
        }
    }
    return nb_bits_read;
}

bool end_of_bitstream(struct bitstream *stream)
{
    return stream->end_of_stream;
}

void skip_bitstream_until(struct bitstream *stream, uint8_t byte)
{
    stream->byte_pos = 0;
    while(stream->next != byte && stream->next_available == true){
        fill_next(stream);
    }
    if( stream->next_available){
        fill_next(stream);
        stream->byte = stream->next;
        if( stream->next_available){
            fill_next(stream);
        }
    } else {
        stream->end_of_stream = true;
    }
}
