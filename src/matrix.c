#include "matrix.h"


void malloc_matrix(matrix* m,int taille) {
	int i;
	m->mat = malloc(taille*sizeof(float*));
	m->contenu = malloc(taille*taille*sizeof(float));
	for(i=0;i<taille;i++)
		m->mat[i]=&(m->contenu[taille*i]);
	for(i=0;i<taille*taille;i++)
		m->contenu[i]=0.;
}
