#ifndef DICO_H
#define DICO_H
#include "lexico.h"

/*
 * UNE APPARITION D'UN MOT DU DICTIONNAIRE EST DEFINIE PAR:
 *   - LE NOM D'UN DOCUMENT DANS LEQUEL IL APPARAIT
 *   - LE NOMBRE DOCCURENCE DANS CE DOC
 */
typedef struct {
	int num_doc;    //NUMERO DU DOC
	int occurences; //OCCURENCE DU MOT DANS LE DOC
} apparition;

/*
 * UNE DEFINITION EST UN MOT DU DICTIONNAIRE ET EST DEFINIE PAR:
 *   -UN STRING
 *   -UN IDENTIFIANT
 *   -UNE LISTE D'APPARITIONS
 */
typedef struct {
	char *mot;       // CONTENU
	int id;          // IDENTIFIANT
	apparition *app; // LISTE DES DOCS OU CE MOT APPARAIT
} definition;

/*
 * UN DICTIONNAIRE EST UN ENSEMBLE DE MOT 
 */
typedef struct {
	definition *def; // MOT ET SA LISTE DE SYNONYME
	int taille;      // NBR DE MOT DANS LE DICO
	int capacite;    // CAPACITE ALLOUEE POUR NOTRE TABLEAU DE DEFINITION
} dictionnaire;

void init_dico(dictionnaire *dico);
void ajoute_dico(dictionnaire *dico, listemots *liste_mots, int dico_courant);

#endif //DICO_H
