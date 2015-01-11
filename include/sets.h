#ifndef SETS_H
#define SETS_H

#include <dico.h>

typedef struct {
	float **c;
	float *contenu;
	int s1;
	int s2;
} set;

set build_docs(dictionnaire *dico);
set build_words(dictionnaire *dico);
void disp_set(set *docs);
void freeset(set *s);
void affiche_set(set* s);

#endif // SETS_H