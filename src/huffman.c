#include "huffman.h"

struct node{
    struct node *childs[2];
    bool sheet;
    uint8_t value;
    };

struct huff_table{
    uint16_t nb_codes;
    struct node *tree;
};

struct node *create_node(bool sheet, uint8_t value){
    struct node *n = malloc(sizeof(struct node));
    n->childs[0] = NULL;
    n->childs[1] = NULL;
    n->sheet = sheet;
    n->value = value;
    return n;
} // end def


void free_tree(struct node *n){
    if(n->childs[0]) free_tree(n->childs[0]);
    if(n->childs[1]) free_tree(n->childs[0]);
    
    free(n);
} // end def

extern struct huff_table *load_huffman_table(struct bitstream *stream,
    uint16_t *nb_byte_read){
printf("\n# Fonction load huffman table\n");    
    // Init
    struct huff_table *table = calloc(sizeof(struct huff_table), 1);
    
        uint16_t nb_codes = 0;
        uint8_t nb_bits_read = 0;
        *nb_byte_read = 0;
        uint8_t nb_level_codes[16];
        uint8_t deep = 0;
        
        // Lecture du nombre de code par niveau.
        uint32_t tmp = 0;
        for(uint8_t i=0; i< 16; i++){
            nb_bits_read = read_bitstream(stream, 8, &tmp, true);
            nb_level_codes[i] = (uint8_t) tmp;
            
            // vérification de la lecture
            if(nb_bits_read !=8){
                perror("Erreur de lecture dans le bitstream lors du décodage de la table de Huffman\n");
                exit(EXIT_FAILURE);
            } else *nb_byte_read +=1;
            nb_codes += nb_level_codes[i];
            // Si on a des code sur ce niveau, on met à jour la profondeur.
            if(nb_level_codes[i]) deep = i+1;
            
            printf("%d codes au niveau %hhu\n", nb_level_codes[i], i+1);
        } // end for
        
        // Vérification sur le nombre de code.
        if(nb_codes > 256){
            fprintf(stderr, "Format de table de Huffman non compatible. La somme des codes est supérieur à 256 : %d\n", nb_codes);
            exit(EXIT_FAILURE);
        } // end if
        
        printf("## Nombre de code : %d\n", nb_codes);
        printf("Profondeur : %d\n", deep);
        
        // Lecture des codes.
        uint8_t code  = 0;
        // Boucle sur les niveaux.
        for(uint8_t l=0; l<deep; l++){
            printf("Lecture des code du niveau %d\n", l);
            for(uint8_t c=0; c < nb_level_codes[l]; c++){
                nb_bits_read = read_bitstream(stream, 8, &tmp, true);
                code = (uint8_t) tmp;
                *nb_byte_read +=1;
                printf("numéro %d, code : %d\n", c, code);
            } // end for c.
        } // end for n.
        printf("Nombre de byte lu : %d\n", *nb_byte_read);
    //exit(EXIT_FAILURE);
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

