#ifndef MATRIX_H
#define MATRIX_H
#include "dico.h"
#include "sets.h"
#include <float.h>
#include <mpi.h>
#include <omp.h>

/* MATRICE CARRE A VALEUR FLOAT */
typedef struct {
	int taille;
	float **mat;
	float  *contenu;
} matrix;

//alloue l'espace memoire necessaire et init ses elem a 0
void malloc_matrix(matrix* mat, int taille);
void init_matrix(matrix* m);
float setDistSym(float* s1, float* s2,int s_size, matrix *dist_mat);
float setDist(float* s1, float* s2, int s_size, matrix *dist_mat);
matrix dist_polia(set *s, matrix *dist_mat, int *indice,int r);
void disp_matrix(matrix*);
void free_matrix(matrix*);

#endif //MATRIX_H
