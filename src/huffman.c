/*******************************************************************************
Nom ......... : huffman.c
Role ........ : Fonctions de gestion des tables de Huffman
Auteurs .... : A. He - M. Barbe - B. Potet (Ensimag 1A 2016/2017 - G6)
*******************************************************************************/

#include "huffman.h"
#include "trace.h"

/* Structure représentant un noeud d'un arbre */
struct node {
    struct node *childs[2];
    bool sheet;
    uint8_t value;
};

/* Structure représentant une table de Huffman */
struct huff_table{
    uint16_t nb_codes;
    struct node *tree;
    uint8_t deep;
};

/* Crée un noeud */
struct node *create_node(bool sheet, uint8_t value)
{
    struct node *n = malloc(sizeof(struct node));
    n->childs[0] = NULL;
    n->childs[1] = NULL;
    n->sheet = sheet;
    n->value = value;
    return n;
}

/* Libère un arbre à partir de sa racine */
void free_tree(struct node *n)
{
    if(n->childs[0]) free_tree(n->childs[0]);
    if(n->childs[1]) free_tree(n->childs[1]);
    free(n);
    n = NULL;
}

/* Charge une table de Huffman */
struct huff_table *load_huffman_table(struct bitstream *stream,
                                        uint16_t *nb_byte_read)
{
    trace("## Fonction load huffman table\n");

    struct huff_table *table = calloc(sizeof(struct huff_table), 1);

    uint16_t nb_codes = 0;
    uint8_t nb_bits_read = 0;
    *nb_byte_read = 0;
    uint8_t nb_level_codes[16];
    uint8_t deep = 0;

    /* Lecture du nombre de code par niveau */
    uint32_t tmp = 0;
    for(uint8_t i = 0; i < 16; ++i) {
        nb_bits_read = read_bitstream(stream, 8, &tmp, true);
        nb_level_codes[i] = (uint8_t) tmp;

        /* Vérification de la lecture */
        if(nb_bits_read != 8) {
            fprintf(stderr, "Erreur de lecture dans le bitstream lors du décodage de la table de Huffman.\n");
            exit(EXIT_FAILURE);
        } else {
            (*nb_byte_read)++;
        }
        nb_codes += nb_level_codes[i];

        /* Si on a des codes sur ce niveau, on met à jour la profondeur */
        if(nb_level_codes[i]) {
            deep = i+1;
        }

        trace("%d codes au niveau %d\n", nb_level_codes[i], i+1);
    }

    /* Vérification sur le nombre de code */
    if (nb_codes > 256) {
        fprintf(stderr, "Format de table de Huffman non compatible. La somme des codes est supérieur à 256 : %d\n", nb_codes);
        exit(EXIT_FAILURE);
    }

    trace("### Nombre de codes : %d\n", nb_codes);
    trace("### Profondeur : %d\n", deep);

    /* == CONSTRUCTION DE L'ARBRE == */

    /* Création de la racine */
    struct node *root = create_node(false, 0xFF);
    /* Liste des noeuds à traiter pour le niveau en cours */
    struct node **free_nodes = malloc(sizeof(struct node *) << deep);
    free_nodes[0] = root;
    uint16_t nb_free_slots = 2;
    /* Liste des noeuds à traiter par le prochain niveau */
    struct node **next_free_nodes = malloc(sizeof(struct node *) << deep);
    uint16_t nb_next_free_slots = 0;

    /* Boucle sur les niveaux */
    for(uint8_t l = 1; l <= deep; ++l) {
        trace("### Complétion du niveau %d\n", l);
        trace("Nombre de slots libre : %d\n", nb_free_slots);

        /* Boucle sur les noeuds du niveau */
        for(uint16_t i = 0; i < nb_free_slots; ++i) {
            /* Si on a écrit tout les code Huffman, on sort des boucles */
            if (l == deep && nb_level_codes[l-1] <= i) {
                break;
            }

            trace("* Noeud : %d / Slot %d\n", i/2, i%2);

            /* On regarde si des codes sont encore disponibles pour ce niveau */
            if( nb_level_codes[l-1] > i) {
                /* On ajoute une feuille, on lit le symbole dans le bitstream */
                nb_bits_read = read_bitstream(stream, 8, &tmp, true);
                if(nb_bits_read != 8) {
                    fprintf(stderr, "Erreur de lecture du fichier lors de l'extraction des codes de Huffman.\n");
                    exit(EXIT_FAILURE);
                }
                (*nb_byte_read)++;

                /* On crée un fils pour le noeud en cours */
                free_nodes[i/2]->childs[i%2] = create_node(true, (uint8_t) tmp);
            } else { // Sinon, ajout d'un fils de type noeud
                struct node *new = create_node(false, 0xFF);
                free_nodes[i/2]->childs[i%2] = new;
                /* On l'ajoute à la liste des noeuds à parcourir pour le prochain niveau */
                next_free_nodes[nb_next_free_slots/2] = new;
                nb_next_free_slots += 2;
            }
        }

        /* On inverse les listes */
        struct node **sav = free_nodes;
        free_nodes = next_free_nodes;
        next_free_nodes = sav;
        nb_free_slots = nb_next_free_slots;
        nb_next_free_slots = 0;
    }

    trace("Nombre d'octets lus : %d\n", *nb_byte_read);

    /* Libération des des deux listes */
    free(free_nodes);
    free(next_free_nodes);

    table->tree = root;
    table->deep = deep;
    return table;
}

/* Décode et renvoie le symbole Huffman suivant */
int8_t next_huffman_value(struct huff_table *table,
struct bitstream *stream){

    uint8_t nb_bits_read;
    return next_huffman_value_count(table, stream, &nb_bits_read);
} // end def


extern int8_t next_huffman_value_count(struct huff_table *table,
struct bitstream *stream,
uint8_t *nb_bits_read){


    uint32_t bit = 0;
    *nb_bits_read = 0;
    struct node *n= table->tree;

    while (*nb_bits_read < table->deep){
        // Lecture du bit.
    if(! read_bitstream(stream, 1, &bit, true)){
        fprintf(stderr, "Erreur de lecture du fichier lors du décodage d'un code Huffman.\n");
        exit(EXIT_FAILURE);}

        (*nb_bits_read)++;

        if(n->childs[bit] == 0) fprintf(stderr, "Erreur de fils\n");
        n = n->childs[bit];

        // On regarde si le noeud est contient un symbole.
        if(n->sheet){
        trace("\nSymbole trouvé : %d\n", n->value);
            return n->value;
        } // end if

    } // end while
    fprintf(stderr, "Erreur de décodage du symbole Huffman, nombre de bits lu : %d, deep : %d\n", *nb_bits_read, table->deep);
    exit(EXIT_FAILURE);
} // end def

extern void free_huffman_table(struct huff_table *table){

    if(table){
        free_tree(table->tree);
        free(table);
        table = NULL;
    } // end if
} // end def
