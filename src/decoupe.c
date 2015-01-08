#include <stdio.h>
#include <string.h>
#include <lexico.h>
#include <dico.h>

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
	listes_de_mots = malloc((argc-1)*sizeof(mot));
	// PARCOURIR LA LISTE DES FICHIERS
	init_dico(&dico);
	for (i=0; i<argc-1; i++) {
		f = fopen(argv[i+1], "r");
		if (f == NULL) {
			printf("mauvais argument #%d : \"%s\"\n", i, argv[i+1]);
			return 1;
		}
		fprintf(stdout, "\033[7m%s\033[0m\n", argv[i+1]);
		nom_doc = argv[i+1] - 1;
		for (j=0; argv[i+1][j]; j++)
			if (argv[i+1][j] == '/')
				nom_doc = argv[i+1] + j;
		nom_doc++;
		listes_de_mots[i] = decoupe_fichier(f, nom_doc);
		//print_mots(stderr, listes_de_mots+i);
		ajoute_dico(&dico, listes_de_mots+i);
		//fprintf(stdout, "NOMBRE DE MOTS : %d\n", listes_de_mots[i].taille);
		//fprintf(stdout, "\033[7mCLOSING FILE %s\033[0m\n", argv[i+1]);
		fclose(f);
		//fprintf(stdout, "\033[7mCLOSED FILE %s\033[0m\n", argv[i+1]);
	}
	affiche_docs(&dico);
	affiche_dico(&dico);
	frequence_dico(&dico);
	affiche_dico(&dico);
	// TOUT BE FREE
	freelistesmots(listes_de_mots, argc-1);
	freedico(&dico);
	return 0;
}
