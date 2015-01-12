#include "matrix.h"

void malloc_matrix(matrix *m, int taille) {
	int i;
	m->taille = taille;
	m->mat = malloc(taille*sizeof(float*));
	// WOWOW PAS DE CALLOC INUTILE
	//m->contenu = calloc(taille*taille, sizeof(float));
	m->contenu = malloc(taille*taille*sizeof(float));
	// sorte de index 2d -> index 1d? j'aime bien
	for (i=0; i<taille; i++)
		m->mat[i] = m->contenu + taille*i;
}

// FILL MAT WITH 1. ; EXCEPT DIAG = 0.
void init_matrix(matrix *m) {
	int i, j;
	for (i=0; i<m->taille; i++)
		for (j=0; j<m->taille; j++)
			m->mat[i][j] = 0.9999999;
	for (j=0; j<m->taille; j++)
		m->mat[j][j] = 0.00000001;
}

float setDist(float *s1, float *s2, int s_size, matrix *dist_mat) {
	int e1, e2;
	float d_min, d_avg, score_min, d, s;
	d_avg = 0.0;
	// CALCUL DE LA MOYENNE
	// (POUR CHAQUE ELEMENT E1 DE S1) :
	for (e1=0; e1<s_size; e1++) {
		d_min = FLT_MAX;
		score_min = 0.;
		// Calcul du min
		for (e2=e1; e2<s_size; e2++) {
			d = dist_mat->mat[e1][e2]; // w_id ou d_id
			s = s1[e1] * s2[e2];
			if (d < d_min) {// && s> 1e-8) {
				d_min = d;
				score_min = s;
			}
		}
		//printf("d_min=%lg\n",d_min);
		if (score_min > 1e-8)
			d_avg += d_min / score_min;
	}
	d_avg /= s_size;
	return d_avg;
}

float setDistSym(float *s1, float *s2,
		int s_size, matrix *dist_mat) {
	return setDist(s1, s2, s_size, dist_mat)
	     + setDist(s2, s1, s_size, dist_mat);
}

matrix dist_polia(set *s, matrix *dist_mat) {
	matrix Result;
	int i, j, t;
	t = s->nb_lignes;
	malloc_matrix(&Result, t);
	for (i=0; i<t; i++) {
		for (j=i; j<t; j++) {
			Result.mat[j][i] =
			Result.mat[i][j] =
			setDistSym(s->c[i], s->c[j], s->nb_colonnes, dist_mat);
		}
	}
	return Result;
}

void disp_matrix(matrix *m) {
	int i, j;
	char num[10];
	for (i=0; i<m->taille; i++) {
		for (j=0; j<m->taille; j++) {
			sprintf(num, "%.0f", 232+m->mat[i][j]*23);
			printf("\033[48;5;%sm ", num);
			//printf("%g ", m->mat[i][j]);
		}
		puts("\033[0m");
	}
}

void free_matrix(matrix *m) {
	free(m->contenu);
	free(m->mat);
}
