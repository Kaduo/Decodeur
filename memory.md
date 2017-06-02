# MCU
## Tableau de MCU : mcus
Allocation dans le main. : Ok

On doit le libérer dans le main : Ok

## MCU
### Create
Dans la fonction extract_mcu on appel create_mcu:


struct mcu *mcu = malloc(sizeof(struct mcu));

    mcu->components_y = malloc(nb_components_y*sizeof(int16_t *));
    if (nb_components_cb > 0) {
        mcu->components_cb = malloc(nb_components_cb*sizeof(int16_t *));
        mcu->components_cr = malloc(nb_components_cr*sizeof(int16_t *    }

## Suppression
Il faut libérer dans le main.
On garde la composante Y qui passe dans les blocs.
Dans la libération d'un MCU, on libère les composantes CB et CR.

# Composantes.
## Création
Extract_mcu appel 
get_component
Qui appel 
idct:
int16_t *inverse = calloc(size * size, sizeof(int16_t));

### Dans get_component.
*extracted  crée par extract, Livéré quand?

*zigzag = inverse_zigzag(quantization, size);

*quantization = inverse_quantization(extracted

*component = idct(zigzag, size);

 free(extracted);
      free(quantization);
      free(zigzag);
      /* Retour de la composante */
       

## Suppression

Quand on supprime un MCU on supprime 


# Bloc
## Liste de blocks
## Allocation 

Petite liste de blocksdans extract block :
block *blocks = malloc(nb_blocks * sizeof(block *));


## Block
Dans extract block:
blocks[i] = create_block();