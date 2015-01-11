#ifndef MATRIX_H
#define MATRIX_H
#include "dico.h"
#include <float.h>

#define FLT_MAX 3.40282347E+38F

/*
 * MATRICE CARRE A VALEUR FLOAT
 */
typedef struct {
	int taille;
	float **mat;
	float  *contenu;
} matrix;

//alloue l'espace memoire necessaire et init ses elem a 0
void malloc_matrix(matrix* mat, int taille);
void init_matrix_word(matrix* m, dictionnaire* dico);
void dist_polia();
float setDist(int, int, matrix);
void setDistSym();
void dist_polia();

#endif //MATRIX_H
