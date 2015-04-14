#include <stdio.h>
#include <string.h>
#include <lexico.h>
#include <dico.h>
#include <sets.h>
#include <matrix.h>
#include <mpi.h>


int main(int argc, char *argv[]) {
	FILE *f;
	char* nom_doc;
	listemots *listes_de_mots;
	dictionnaire dico;
	set docs, words;
	int i, j,k, disp,rank,size,ND,NW,nb_combinaisons,nb_calcul,rank_i;
	int i_debut,i_fin,j_debut,j_fin;
	int tailles[2]; // tailles[0]=NW et tailles[1]ND
	int indice_doc[4], indice_word[4];
	matrix matrix_words,matrix_docs;
	
//INIT
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	if(rank == 0)
	{
		
		if (argc < 2) {
			puts("usage: decoupe FICHIER [ FICHIER ... ]");
			return 1;
		}
		disp = 0;
		if (!strcmp(argv[1], "-d")) {
			disp = 1;
			argv++;
			argc--;
		}
		listes_de_mots = malloc((argc-1)*sizeof(listemots));
		// PARCOURIR LA LISTE DES FICHIERS
		init_dico(&dico);
		for (i=0; i<argc-1; i++) {
			f = fopen(argv[i+1], "r");
			if (f == NULL) {
				printf("mauvais argument #%d : \"%s\"\n", i, argv[i+1]);
				return 1;
			}
			fprintf(stdout, "\033[7m%s\033[0m", argv[i+1]);
			/* cherche si le nom de fichier contien un '/' pour avoir
			 * au final le nom réduit du fichier */
			nom_doc = argv[i+1] - 1; /* AU CAS OU IL N'Y A PAS DE '/' */
			for (j=0; argv[i+1][j]; j++)
				if (argv[i+1][j] == '/')
					nom_doc = argv[i+1] + j; /* POINTEUR VERS LE '/' */
			nom_doc++; /* POINTEUR VERS LE PREMIER CHARACTERE NON '/' */
			listes_de_mots[i] = decoupe_fichier(f, nom_doc);
			fprintf(stdout, "    NB DE MOTS : %d\n", listes_de_mots[i].taille);
			ajoute_dico(&dico, listes_de_mots+i);
			fclose(f);
		}
		freelistesmots(listes_de_mots, argc-1);
		frequence_dico(&dico);
		if (disp) affiche_dico(&dico);
		printf("il y a %d docs et %d mots\n", dico.docs_taille, dico.taille);
	//MATRIX
		malloc_matrix(&matrix_words, dico.taille);
		malloc_matrix(&matrix_docs, dico.docs_taille);
		init_matrix(&matrix_words);
		//init_matrix(&matrix_docs);
	//SETS
		docs = build_docs(&dico);
		//disp_set(&docs);
		words = build_words(&dico);
		
	printf("ENVOI DOCS & WORDS\n");
	//ENVOI DOCS & WORDS
		int tailles[2]; // tableau contenant NW et ND
		tailles[0] = NW = dico.taille; //NW
		tailles[1]= ND =dico.docs_taille; //ND
		freedico(&dico);
		//envoi des tailles
		for(i=1; i<size; i++)
		{
			printf("envoi taille à %d\n",i);
			MPI_Send(&tailles, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
		//envoi de docs
		for(i=1; i<size; i++)
		{
			printf("envoi docs à %d\n",i);
			MPI_Send(docs.contenu, NW*ND, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
		}
		//envoi de words
		for(i=1; i<size; i++)
		{
			printf("envoi words à %d\n",i);
			MPI_Send(words.contenu, NW*ND, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
		}
		
	printf("CALCUL DE LA REPARTITION DE TRAVAIL\n");	
	//CALCUL DE LA REPARTITION DE TRAVAIL
		//doc
		rank_i=i=k=0;
		nb_combinaisons=ND*(ND+1)/2;
		nb_calcul=nb_combinaisons/size;
		
		//indice rang 0
		int indice_temp[4]; //temp pour rang 0
		indice_temp[0]=i_debut=0;
		indice_temp[1]=j_debut=0;
		indice_temp[2]=i_fin=0;
		indice_temp[3]=j_fin=0;
		
		while( k<nb_calcul )
		{
				j_fin++;
				if(j_fin==ND)
				{
					i_fin++;
					j_fin=i;
				}
				k++;
		}
		indice_temp[2]=i_fin;
		indice_temp[3]=j_fin;
		
		printf("Proc rang %d:\ndebut = %d,%d\nfin = %d,%d\n",rank,indice_temp[0],indice_temp[1],indice_temp[2],indice_temp[3]);
		
		i_debut=i_fin;
		j_debut=j_fin + 1;
		if(j_debut==ND)
		{
			i_debut++;
			j_debut=i_debut;
		}
		
		while( k<rank_i*nb_calcul )
		{
			if(k+1 == (rank_i+1)*nb_calcul)
			{
				printf("RANK %d\n",rank_i);
				indice_doc[0]=i_debut;
				indice_doc[1]=i_fin;
				indice_doc[2]=j_debut;
				indice_doc[3]=j_fin;
				MPI_Send(indice_doc, 4, MPI_INT, rank_i, 0, MPI_COMM_WORLD);
				rank_i++;
				
				i_debut=i_fin;
				j_debut=j_fin + 1;
				if(j_debut==ND)
				{
					i_debut++;
					j_debut=i_debut;
				}
				
				if(rank_i == size)
				{
					indice_doc[0]=i_debut;
					indice_doc[1]=i_fin;
					indice_doc[2]=ND-1;
					indice_doc[3]=ND-1;
					MPI_Send(indice_doc, 4, MPI_INT, rank_i, 0, MPI_COMM_WORLD);
				}
			}
			else
			{
				j_fin++;
				if(j_fin==ND)
				{
					i_fin++;
					j_fin=i;
				}
				k++;
			}
		}
		indice_doc[0]=indice_temp[0];
		indice_doc[1]=indice_temp[1];
		indice_doc[2]=indice_temp[2];
		indice_doc[3]=indice_temp[3];
		
	}
	else
	{
	//RECEPTION DOCS & WORDS
		//reception tailles
		MPI_Recv(&tailles, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("recep taille de %d\n",rank);
		docs.nb_lignes = NW =tailles[1]; //ND
		docs.nb_colonnes = ND =tailles[0]; //NW
		printf("rank %d: ND=%d, NW=%d\n",rank,ND,NW);
		//allocs docs
		docs.c = malloc(docs.nb_lignes*sizeof(float*));
		docs.contenu = malloc(NW*ND*sizeof(float));
		//reception docs
		MPI_Recv(docs.contenu, tailles[0]*tailles[1], MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("recep doc de %d\n",rank);
		for (i=0; i<docs.nb_lignes; i++)
		{
			docs.c[i] = docs.contenu + i*docs.nb_colonnes;
		}
		
		//alloc words
		words.c = malloc(docs.nb_colonnes*sizeof(float*));
		words.contenu = malloc(docs.nb_lignes*docs.nb_colonnes*sizeof(float));
		//reception words
		MPI_Recv(words.contenu, tailles[0]*tailles[1], MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("recep words de %d\n",rank);
		for (i=0; i<docs.nb_colonnes; i++)
		{
			words.c[i] = words.contenu + i*docs.nb_lignes;
		}
		
	//RECEPTIONS INDICES
		MPI_Recv(indice_doc, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Proc rang %d:\ndebut = %d,%d\nfin = %d,%d\n",rank,indice_doc[0],indice_doc[1],indice_doc[2],indice_doc[3]);
		
	}
	


if(rank == 0)
{
//ALGO CALCUL
	for (i=0; i<4; i++) {
		matrix_docs  = dist_polia(&docs,&matrix_words,indice_doc); // Nd*Nd
		
		//disp_matrix(&matrix_docs);
		free(matrix_words.contenu);
		free(matrix_words.mat);
		puts("words");
		
		 
		
		matrix_words = dist_polia(&words,&matrix_docs,indice_word); // Nw*Nw
		
		//disp_matrix(&matrix_words);
		free(matrix_docs.contenu);
		free(matrix_docs.mat);
		puts("docs");
	}
	matrix_docs  = dist_polia(&docs,&matrix_words,indice_doc);
	printf("SCORE final %lg\n",matrix_docs.mat[0][0]);
	//disp_matrix(&matrix_words);
}

// TOUT BE FREE
	freeset(&docs);
	freeset(&words);
	
//FINALIZE
	MPI_Finalize();
	
	return 0;
}
