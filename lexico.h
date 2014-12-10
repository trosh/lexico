#ifndef LEXICO_H
#define LEXICO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
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
void print_mots(listemots);

#endif //LEXICO_H
