#ifndef SETS_H
#define SETS_H
#include <dico.h>

typedef struct {
	float **c;
	float *contenu;
	int nb_lignes;   //nb lignes
	int nb_colonnes; //nb colonnes
} set;

set build_docs (dictionnaire*);
set build_words(dictionnaire*);
void disp_set   (set*);
void freeset    (set*);
void affiche_set(set*);
void disp_set   (set*);

#endif // SETS_H
