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

void init_matrix_word(matrix* m,dictionnaire* dico) {
	int i,j;
	for(i=0;i<dico->taille;i++){
		for(j=0;j<dico->taille;j++){
			m->mat[i][j]=index_to_score(i,j);
		}
	}
}
