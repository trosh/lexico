#ifndef DICO_H
#define DICO_H
#include "lexico.h"

/*
 * UNE APPARITION D'UN MOT DU DICTIONNAIRE EST DEFINIE PAR:
 *   - LE NOM D'UN DOCUMENT DANS LEQUEL IL APPARAIT
 *   - LE NOMBRE DOCCURENCE DANS CE DOC
 */
/*
typedef struct {
	int num_doc;     // NUMERO DU DOC
	char occurences; // OCCURENCE DU MOT DANS LE DOC
} apparition;
*/
/*
 * UNE DEFINITION EST UN MOT DU DICTIONNAIRE ET EST DEFINIE PAR:
 *   - UN STRING
 *   - UN IDENTIFIANT
 *   - UNE LISTE D'APPARITIONS
 */
typedef struct {
	char *c;         // CONTENU
	//apparition *app; // LISTE DES DOCS OU CE MOT APPARAIT
	int *num_doc;
	float *occurences;
} definition;

/*
 * UN DICTIONNAIRE EST UN ENSEMBLE DE MOT 
 */
typedef struct {
	definition *def; // LISTE DE (MOT ET SA LISTE D'APPARITIONS)
	int *app_tailles;
	int *app_capacites;
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
float index_to_score(int w_id,int d_id); // RENVOIT LE SCORE D'UN MOT EN DONNANT l'ID DU MOT ET DU DOC RENVOIT 0 SINON

#endif //DICO_H
