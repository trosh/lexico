#ifndef SETS_H
#define SETS_H
#include <dico.h>

typedef struct {
	float **c;
	float *contenu;
	int nb_lignes;	//nb lignes
	int nb_colonnes; //nb colonnes
} set;

set build_docs(dictionnaire *dico);
set build_words(dictionnaire *dico);
void disp_set(set *docs);
void freeset(set *s);
void affiche_set(set* s);

#endif // SETS_H
