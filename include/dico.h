#ifndef DICO_H
#define DICO_H
#include "lexico.h"

/*
 * UNE DEFINITION EST UN MOT DU DICTIONNAIRE ET EST DEFINIE PAR:
 *   - UN STRING
 *   - UN IDENTIFIANT
 *   - UNE LISTE D'APPARITIONS
 */
typedef struct {
	char *c; // CONTENU
	int checksum; // C[0]+C[1]+...
	int *num_doc;
	float *occurences;
} definition;

/*
 * UN DICTIONNAIRE EST UN ENSEMBLE DE MOT 
 */
typedef struct {
	definition *def;  // LISTE DE (MOT ET SA LISTE D'APPARITIONS)
	// APP_TAILLE[I] == LEN(DEF[I].NUM_DOC) == LEN(DEF[I].OCCURENCES)
	int *app_tailles;
	int *app_capacites; // CAPACITES CORRESPONDANTES
	int taille;      // NOMBRE DE MOTS DANS LE DICO
	int capacite;    // CAPACITE ALLOUEE POUR NOTRE TABLEAU DE DEFINITION
	char **docs;     // LISTE DES DOCS (NUM_DOC -> NOM_DOC)
	int docs_taille;
	int docs_capacite;
} dictionnaire;

void init_dico      (dictionnaire *dico);
void ajoute_dico    (dictionnaire *dico, listemots *liste_mots);
//void affiche_docs   (dictionnaire *dico); // TODO : DECIDE S'IL FAUT GARDER
void affiche_dico   (dictionnaire *dico);
void affiche_dico_bad(dictionnaire *dico);
void freedico       (dictionnaire *dico);
void frequence_dico (dictionnaire *dico); // CALCULE LE SCORE DE CHAQUE MOT

#endif //DICO_H
