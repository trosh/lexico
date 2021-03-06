#include "matrix.h"

void malloc_matrix(matrix *m, int taille) {
	int i;
	m->taille = taille;
	m->mat = malloc(taille*sizeof(float*));
	m->contenu = malloc(taille*taille*sizeof(float));
	for (i=0; i<taille; i++)
		m->mat[i] = m->contenu + taille*i;
}

// FILL MAT WITH 1. ; EXCEPT DIAG = 0.
void init_matrix(matrix *m) {
	int i, j;
#ifdef DAT
#warning COMPILING FOR DATFILES
	FILE * datfile;
	int rept;
	puts("\033[37;41mMEASURING init_matrix (def DAT)\033[0m");
	datfile = fopen("dat/init_matrix", "a");
	if (datfile == NULL) {
		fputs("cannot open dat/init_matrix\n", stderr);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	double t = MPI_Wtime();
	for (rept=0; rept<100; rept++) {
#endif
#pragma omp parallel for collapse(2) schedule(static)
	for (i=0; i<m->taille; i++)
		for (j=0; j<m->taille; j++)
			m->mat[i][j] = 1.;
	/* CANNOT NOWAIT */
#pragma omp parallel for schedule(static)
	for (j=0; j<m->taille; j++)
		m->mat[j][j] = 0.;
#ifdef DAT
	}
	t = (MPI_Wtime()-t)/rept;
	fprintf(datfile, "%d\t%lg\n", omp_get_max_threads(), t);
	fclose(datfile);
#endif
}

float setDist(float *s1, float *s2, int s_size, matrix *dist_mat) {
	int e1, e2, nb_elem=0, rank;
	float d_min, d_avg, score_min, d, s;
#ifdef DAT
#warning COMPILING FOR DATFILES
	FILE * datfile;
	int rept;
	puts("\033[37;41mMEASURING setDist (def DAT)\033[0m");
	datfile = fopen("dat/setDist", "a");
	if (datfile == NULL) {
		fputs("cannot open dat/setDist\n", stderr);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	double t = MPI_Wtime();
	for (rept=0; rept<50; rept++) {
#endif
	d_avg = 0.0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// CALCUL DE LA MOYENNE
	// (POUR CHAQUE ELEMENT E1 DE S1) :
#pragma omp parallel for schedule(static) private(e1,e2,d_min,score_min,d,s) reduction(+:d_avg,nb_elem)
	for (e1=0; e1<s_size; e1++) {
		if (s1[e1] != 0) { //si le score est nul alors il n'existe pas dans le doc
			nb_elem++;
			d_min = FLT_MAX;
			score_min = 0.;
			// Calcul du min
			for (e2=e1; e2<s_size; e2++)
				if (s2[e2] != 0) { //si le score est nul alors il n'existe pas dans le doc
					d = dist_mat->mat[e1][e2]; // w_id ou d_id
					s = s1[e1] * s2[e2];
					if (d < d_min && s > 1e-8) {
						d_min = d;
						score_min = s;
					}
				}
			//printf("d_min=%lg\n",d_min);
			if (score_min > 1e-8) // != 0 totalement arbitraire
				d_avg += d_min / score_min;
		}
	}
#ifdef DAT
	}
	t = (MPI_Wtime()-t)/rept;
	fprintf(datfile, "%d\t%lg\n", omp_get_max_threads(), t);
	fclose(datfile);
#endif
	//d_avg /= s_size; //il faut diviser par le nombre de mot dans le docs, pas le nombre total de mots.
	//printf("%d\n",nb_elem);
	d_avg /= nb_elem;
	return d_avg;
}

float setDistSym(float *s1, float *s2,
		int s_size, matrix *dist_mat) {
	return setDist(s1, s2, s_size, dist_mat);
	     + setDist(s2, s1, s_size, dist_mat);
}

matrix dist_polia(set *s, matrix *dist_mat, int *indice,int rank) {
	matrix Result;
	int i, j,t ;
	t = s->nb_lignes;
	malloc_matrix(&Result, t);
	i = indice[0];
	//printf("s->nb_colonnes = %d\n", s->nb_colonnes);
	//PROCESS SPANS MORE THAN 1 LINE
	if (indice[0] != indice[1]) {
		for (j=indice[2]; j<t; j++) {
			Result.mat[j][i] =
			Result.mat[i][j] =
			setDistSym(s->c[i], s->c[j], s->nb_colonnes, dist_mat);
		}
			
		for (i=indice[0]+1; i<indice[1]; i++)
			for (j=i; j<t; j++) {
				Result.mat[j][i] =
				Result.mat[i][j] =
				setDistSym(s->c[i], s->c[j], s->nb_colonnes, dist_mat);
			}
	
		for (j=i; j<=indice[3]; j++) {
			Result.mat[j][i] =
			Result.mat[i][j] =
			setDistSym(s->c[i], s->c[j], s->nb_colonnes, dist_mat);
		}
		
	}
//PROCESS SPANS 1 LINE
	else
		for (j=indice[2]; j<=indice[3]; j++) {
			Result.mat[j][i] =
			Result.mat[i][j] =
			setDistSym(s->c[i], s->c[j], s->nb_colonnes, dist_mat);
		}
	return Result;
}

void disp_matrix(matrix *m) {
	int i, j;
	char num[10];
	for (i=0; i<m->taille; i++) {
		for (j=0; j<m->taille; j++) {
			sprintf(num, "%.0f", 232+m->mat[i][j]/10);
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

void communication(matrix* mat, int rank, int size,int my_start) {
	int i,adresse_start[size];
	MPI_Request requete[size*2];
	
	for(i=0; i<size; i++) {
		if(i != rank) {
			MPI_Irecv(&adresse_start[i],1, MPI_INT, i,0, MPI_COMM_WORLD, &requete[2*i]);
			MPI_Isend(&my_start,1, MPI_INT, i,0, MPI_COMM_WORLD, &requete[2*i+1]);
		}
	}
	for(i=0; i<size; i++) {
		if(i != rank) {
			MPI_Waitall(2,&requete[2*i],MPI_STATUS_IGNORE);
			MPI_Irecv(&mat->contenu[adresse_start[i]],1, MPI_FLOAT, i,0, MPI_COMM_WORLD, &requete[2*i]);
			MPI_Isend(&mat->contenu[my_start],1, MPI_FLOAT, i,0, MPI_COMM_WORLD, &requete[2*i+1]);
		}
	}
	for(i=0; i<size; i++) {
		if(i != rank) {
			MPI_Waitall(2,&requete[2*i],MPI_STATUS_IGNORE);
		}
	}
}
