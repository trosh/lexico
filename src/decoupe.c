#include <stdio.h>
#include <lexico.h>

int main(int argc, char *argv[]) {
	FILE *f;
	listemots *listes_de_mots;
	int i, j;
	if (argc < 2) {
		puts("usage: decoupe FICHIER [ FICHIER ... ]");
		return 1;
	}
	listes_de_mots = malloc((argc-1)*sizeof(mot*));
	// PARCOURIR LA LISTE DES FICHIERS
	for (i=0; i<argc-1; i++) {
		f = fopen(argv[i+1], "r");
		if (f == NULL) {
			printf("mauvais argument #%d : \"%s\"\n", i, argv[i+1]);
			return 1;
		}
		fprintf(stdout, "\033[7m%s\033[0m\n", argv[i+1]);
		listes_de_mots[i] = decoupe_fichier(f);
		print_mots(stderr, listes_de_mots[i]);
		fprintf(stdout, "NOMBRE DE MOTS : %d\n", listes_de_mots[i].taille);
		fprintf(stdout, "\033[7mCLOSING FILE %s\033[0m\n", argv[i+1]);
		fclose(f);
		fprintf(stderr, "\033[7mCLOSED FILE %s\033[0m\n", argv[i+1]);
		freelistemots(listes_de_mots+i);
	}
	// TOUT BE FREE
	freelistesmots(listes_de_mots, argc-1);
	return 0;
}