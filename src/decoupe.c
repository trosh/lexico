#include <stdio.h>
#include <string.h>
#include <lexico.h>
#include <dico.h>
#include <sets.h>
#include <matrix.h>

int main(int argc, char *argv[]) {
	FILE *f;
	char* nom_doc;
	listemots *listes_de_mots;
	dictionnaire dico;
	set docs, words;
	int i, j, disp;
	matrix matrix_words,matrix_docs;
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
	affiche_dico_bad(&dico);
	printf("il y a %d docs et %d mots\n", dico.docs_taille, dico.taille);
//MATRIX
	malloc_matrix(&matrix_words, dico.taille);
	malloc_matrix(&matrix_docs, dico.docs_taille);
	init_matrix_word(&matrix_words, &dico);
	init_matrix_word(&matrix_docs, &dico);
//SETS
	docs = build_docs(&dico);
	words = build_words(&dico); 
	putchar('\n');
	puts("AFFICHAGE DE DOCS (140 MOTS MAX)");
	disp_set(&docs);
	disp_set(&words);
	
	for(i=0;i<10;i++){
		
	}
	//printf("\nAFFICHAGE DE WORDS\n");
	//disp_set(&words);
	//disp_set(&docs);
	//disp_set(&words);
		freedico(&dico);
// TOUT BE FREE
	freeset(&docs);
	freeset(&words);
	return 0;
}
