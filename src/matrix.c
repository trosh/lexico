#include "matrix.h"


void malloc_matrix(matrix* m, int taille) {
	int i;
	m->mat = malloc(taille*sizeof(float*));
	m->contenu = calloc(taille*taille, sizeof(float));
	// sorte de index 2d -> index 1d? j'aime bien
	for (i=0; i<taille; i++)
		m->mat[i] = m->contenu + taille*i;
}

// FILL MAT WITH 1. ; EXCEPT DIAG = 0.
void init_matrix_word(matrix* m, dictionnaire* dico) {
	int i, j;
	for (i=0; i<dico->taille; i++)
		for (j=0; j<dico->taille; j++)
			m->mat[i][j] = 1.; // SEGFAULT POUR TROP DE CONTENU
	for (j=0; j<dico->taille; j++)
		m->mat[j][j] = 0.;
}


float setDist(float* s1, float* s2, int s_size, matrix dist_mat) {
	int e1, e2;
	float d_min, d_avg, score_min, d;
	d_avg = 0.0;
	// Calcul de l'average
	// (pour chaque element e1 du set s1) :
	for (e1=0; e1<s_size; e1++) {
		d_min = FLT_MAX;
		score_min = 0.;
		// Calcul du min
		for (e2=0; e2<s_size; e2++) {
			d = dist_mat.mat[e1][e2]; // w_id ou d_id
			if (d < d_min) {
				d_min = d;
				score_min = s1[e1] * s2[e2];
			}
		}
		d_avg += d_min / score_min;
	}
	d_avg /= s_size;
	return d_avg;
}

float setDistSym(float* s1, float* s2,int s_size, matrix dist_mat) {
	return setDist(s1, s2, s_size, dist_mat) + setDist(s2, s1, s_size, dist_mat);
}
 
matrix dist_polia(set s, matrix dist_mat) {
	matrix Result;
	int i,j;
	for(i=0; i< s.nb_lignes; i++) {
		for(j=0; j< s.nb_lignes; j++) {  // possibilité d'optimiser et ne remplir qu'une matrice triangulaire avec for j = i...
			Result.mat[i][j] = setDist(s.c[i], s.c[j], s.nb_lignes, dist_mat);
		}
	}
	
	free(dist_mat.contenu);
	return Result;
}

