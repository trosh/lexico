#include <stdio.h>
#include <string.h>
#include <lexico.h>
#include <dico.h>
#include <matrix.h>

int main(int argc, char *argv[]) {
	FILE *f;
	char* nom_doc;
	listemots *listes_de_mots;
	dictionnaire dico;
	int i, j;
	if (argc < 2) {
		puts("usage: decoupe FICHIER [ FICHIER ... ]");
		return 1;
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
	frequence_dico(&dico);
	affiche_dico(&dico);
	printf("il y a %d docs et %d mots\n", dico.docs_taille, dico.taille);
	matrix m;
	malloc_matrix(&m,dico.taille);
	init_matrix_word(&m,&dico);
	
	
	// TOUT BE FREE
	freelistesmots(listes_de_mots, argc-1);
	freedico(&dico);
	return 0;
}
