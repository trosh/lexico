#include "matrix.h"

void malloc_matrix(matrix* m, int taille) {
	int i;
	m->mat = malloc(taille*sizeof(float*));
	m->contenu = calloc(taille*taille, sizeof(float));
	// sorte de index 2d -> index 1d? j'aime bien
	for (i=0; i<taille; i++)
		m->mat[i] = m->contenu + taille*i;
}

// Rempli la matrice de mot avec 1. sauf sur la diag 0.
void init_matrix_word(matrix* m, dictionnaire* dico) {
	int i, j;
	for (i=0; i<dico->taille; i++)
		for (j=0; j<dico->taille; j++)
			m->mat[i][j] = 1.;
	for (j=0; j<dico->taille; j++)
		m->mat[j][j] = 0.;
}

float setDist(int s1, int s2, matrix dist_mat) {
	int e1, e2;
	float d_min, d_avg, score_min, d;
	d_avg = 0.0;
	// Calcul de l'average
	// (pour chaque element e1 du set s1) :
	for (e1=0; e1<dist_mat.taille; e1++) {
		d_min = FLT_MAX;
		score_min = 0.;
		// Calcul du min
		for (e2=0; e2<dist_mat.taille; e2++) {
			d = dist_mat.mat[e1][e2]; // w_id ou d_id
			if (d < d_min) {
				d_min = d;
				score_min = dist_mat.mat[s1][e1] * dist_mat.mat[s2][e2];
			}
		}
		d_avg += d_min / score_min;
	}
	d_avg /= dist_mat.taille;
	return d_avg;
}
