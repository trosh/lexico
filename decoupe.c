#include "lexico.h"

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
		if ((f = fopen(argv[i+1], "r")) == NULL) {
			printf("mauvais argument #%d : \"%s\"\n", i, argv[i+1]);
			return 1;
		}
		listes_de_mots[i] = decoupe_fichier(f);
		print_mots(listes_de_mots[i]);
		fprintf(stderr, "NOMBRE DE MOTS : %d\n",
			listes_de_mots[i].taille);
		puts("CLOSING FILE ...");
		fclose(f);
		puts("CLOSED FILE !");
	}
	// TOUT BE FREE
	freelistesmots(listes_de_mots, argc-1);
	return 0;
}
