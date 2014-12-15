#ifndef DICO_H
#define DICO_H
#include "lexico.h"

/*typedef struct {
	char *c;        // CONTENU
	int occurences; // NOMBRE D'OCCURENCES DE CE MOT
} mot;

typedef struct {
	mot *c;     // CONTENU
	int taille; // NOMBRE DE MOTS
} listemots;

// FREE LES MALLOC QUE DECOUPE_FICHIER A FAIT
void freelistemots(listemots*);
// OU S'IL Y EN A UNE LISTE :
void freelistesmots(listemots*, int taille);
inline int est_une_lettre_valable(char);
inline char convert_minuscule(char);
// CREE UNE LISTE DE MOTS ET LEURS OCCURENCES A PARTIR D'UN FICHIER
listemots decoupe_fichier(FILE*);
// AFFICHE UNE SUSDITE LISTE
void print_mots(listemots);*/

/*
 * UN DICTIONNAIRE EST UN ENSEMBLE DE MOT 
 */
typedef struct {
	definition *def; //MOT ET SA LISTE DE SYNONYME
	int taille;		//NBR DE MOT DANS LE DICO
}dictionaire;
/*
 * UNE DEFINITION EST UN MOT DU DICTIONNAIRE ET EST DEFINIE PAR:
 * 				-UN STRING
 * 				-UN IDENTIFIANT
 * 				-UNE LISTE D'APPARITIONS
 */
typedef struct {
	char *c; //CONTENU
	int id;	//IDENTIFIANT
	apparition *synonyme;	//LISTE DE SYNONYME ASSOCIE A CE MOT
} definition;

/*
 * UNE APPARITION REPRESENTE UN SYNONYME D'UN MOT DU DICTIONNAIRE ET EST DEFINIE PAR:
 * 				-LE NOM D'UN DOCUMENT
 * 				-LE NOMBRE DOCCURENCE DANS CE DOC
 */
typedef struct {
	char *doc;	//NOM DU DOC
	int occurences;	//OCCURENCE DU MOT DANS LE DOC
} appariton;



#endif //DICO_H
