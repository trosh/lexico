#ifndef DICO_H
#define DICO_H
#include "lexico.h"

/*
 * UNE APPARITION D'UN MOT DU DICTIONNAIRE EST DEFINIE PAR:
 *   - LE NOM D'UN DOCUMENT DANS LEQUEL IL APPARAIT
 *   - LE NOMBRE DOCCURENCE DANS CE DOC
 */
typedef struct {
	int num_doc;       // NUMERO DU DOC
	double occurences; // OCCURENCE DU MOT DANS LE DOC
} apparition;

/*
 * UNE DEFINITION EST UN MOT DU DICTIONNAIRE ET EST DEFINIE PAR:
 *   - UN STRING
 *   - UN IDENTIFIANT
 *   - UNE LISTE D'APPARITIONS
 */
typedef struct {
	char *c;         // CONTENU
	apparition *app; // LISTE DES DOCS OU CE MOT APPARAIT
	int app_taille;
	int app_capacite;
} definition;

/*
 * UN DICTIONNAIRE EST UN ENSEMBLE DE MOT 
 */
typedef struct {
	definition *def; // LISTE DE (MOT ET SA LISTE D'APPARITIONS)
	int taille;      // NOMBRE DE MOTS DANS LE DICO
	int capacite;    // CAPACITE ALLOUEE POUR NOTRE TABLEAU DE DEFINITION
	char **docs;     // LISTE DES DOCS (NUM_DOC -> NOM_DOC)
	int docs_taille;
	int docs_capacite;
} dictionnaire;

void init_dico     (dictionnaire *dico);
void ajoute_dico   (dictionnaire *dico, listemots *liste_mots);
void affiche_dico  (dictionnaire *dico);
void affiche_docs  (dictionnaire *dico);
void freedico      (dictionnaire *dico);
void frequence_dico(dictionnaire *dico); // CALCULE LE SCORE DE CHAQUE MOT

#endif //DICO_H
