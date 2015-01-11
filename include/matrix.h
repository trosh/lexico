#ifndef MATRIX_H
#define MATRIX_H
#include "dico.h"
#include <float.h>

//#define FLT_MAX 3.40282347E+38F

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
//<<<<<<< HEAD
float setDist(int id_s1,int  id_s2, matrix dist_mat);
float setDistSym(int s1,int  s2, matrix dist_mat);
//=======
float setDist(int s1,int  s2, matrix dist_mat);
//>>>>>>> e4d2e6d2cbb4eed763ace51ece3ab90793159fc6
void dist_polia();

#endif //MATRIX_H
