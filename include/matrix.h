#ifndef MATRIX_H
#define MATRIX_H

/*
 * MATRICE CARRE A VALEUR FLOAT
 */
typedef struct {
	int taille;
	float **mat;
	float  *contenu;
} matrix;

//alloue l'espace memoire necessaire et init ses elem a 0
void malloc_matrix(matrix* mat,int taille);
void init_matrix_word(matrix* mat);
void dist_polia();
void setDist();
void setDistSym();
void dist_polia();

#endif //MATRIX_H
