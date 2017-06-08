— vous aurez 10-15 minutes pour nous présenter votre projet : ce qui fonctionne, les limites,
comment vous avez conçu et testé votre code, les points importants/spécifiques de votre im-
plémentation, etc. À vous de « vendre » votre travail de la manière qui vous parait adéquate !

# Quelques chiffres :
* 289 commits
* 2500 lignes de codes
# Ce qui marche/ce qui marche pas
* Fonctionne sur toutes les images fournies
* Beaucoup de facteurs d'upsampling (tous ?) marchent
* Essai sur quelques images d'internet
* Pas de segfaults lorsque le format n'est pas supporté
* Pas de fuite de mémoire
* Pas le temps de se concentrer exclusivement sur la performance
# Organisation du travail
* Réflexion sur les structures
* Entrée/sortie des fonctions
* Répartition : alternance entre bosser seul et en groupe sur un module
* On a commencé par les prototypes + bouchons
* On a programmé "du bas vers le haut" en utilisant le jpeg2blabla et des printf pour déboguer
* Pas de problèmes pour lier les modules entre eux
* Par moment, peu de préoccupation vis-à-vis de la propreté du code, nettoyage après coup
# Choix d'architecture
* Organisation en fichiers par structure de données (component, block, mcu, picture)
* Quelques modules sont réutilisables (en dehors du cadre du projet, ou pour des variations de la norme !) : rgb.c, zigzag.c...
* Union pour les pixels (très léger gain de mémoire + permet d'encapsuler la différence B&W et couleur)
* Upsampling récursif : marche sur tous les facteurs autorisés par la norme
* blocks2pixels : marche dans tous les cas, coût : complexité du code, performance non optimale
* Pour le debug : deux options pour le make avec des print formatés plus ou moins comme le jpeg2blabla
* make en O3
* Pour les modules des profs : pas d'invention architecturelle (?)
* zag-zig : automate (marche pour toutes les tailles de bloc !)
