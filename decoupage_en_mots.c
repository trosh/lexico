#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char *c;        // CONTENU
	int occurences; // NOMBRE D'OCCURENCES DE CE MOT
} mot;

typedef struct {
	mot *c;     // CONTENU
	int taille; // NOMBRE DE MOTS
} listemots;

int est_une_lettre_valable(char c) {
	if ((c >= 'A' && c <= 'Z') // EST UNE LETTRE MAJUSCULE
	 || (c >= 'a' && c <= 'z') // EST UNE LETTRE MINUSCULE
	 ||  c == '-')             // EST UN HYPHEN
		return 1; // VRAI (EST UNE LETTRE VALABLE)
	return 0;     // FAUX (N'EST PAS UNE LETTRE VALABLE)
}

char convert_minuscule(char c) {
	if (c>='A' && c<= 'Z')
		return c - ('A'-'a'); 
	return c;
}

listemots decoupe_fichier(FILE *fichier) {
	char zone_de_travail[100], c, flag;
	int i, taille_du_mot, nb_mots_total = 1000;
	listemots mots;
	mot *last_mot;
	mots.c = malloc(mots.taille*sizeof(mot));
	mots.taille = 0;
	taille_du_mot = 0;
	while ((c=fgetc(fichier))!=EOF) {
		if (est_une_lettre_valable(c)) {
			c = convert_minuscule(c);
			zone_de_travail[taille_du_mot] = c;
			taille_du_mot++;
		}
		else if (taille_du_mot>0) { // FIN DU MOT
			zone_de_travail[taille_du_mot] = '\0';
			flag = 0; // LE MOT A-T-IL ETE TROUVE ?
			for (i=0; i<mots.taille; i++)
				if (!strcmp(mots.c[i].c, zone_de_travail)) {
					mots.c[i].occurences++;
					taille_du_mot = 0;
					flag = 1;
					break;
				}
			if (flag) continue;
			// VERIF TAILLE DE MOTS, DOUBLE TAILLE SI NECESSAIRE
			if (mots.taille==nb_mots_total)
				mots.c = realloc(mots.c, nb_mots_total*=2);
			last_mot = mots.c + mots.taille;
			last_mot->occurences = 1;
			last_mot->c = malloc((taille_du_mot+1)*sizeof(char));
			strncat(last_mot->c, zone_de_travail, taille_du_mot+1);
			// PAS BESOIN DE VIDER ZONE_DE_TRAVAIL !
			mots.taille++;
			taille_du_mot = 0;
		}
	}
	printf("NOMBRE DE MOTS : %d\n", mots.taille);
	return mots;
}

void print_mots(listemots mots) {
	int i;
	for (i=0; i<mots.taille; i++)
		printf("%s %d\t", mots.c[i].c, mots.c[i].occurences);
	puts("");
}

int main(int argc, char *argv[]) {
	FILE *f;
	listemots *listes_de_mots;
	int i, j;
	if (argc < 2) {
		puts("usage: decoupage_en_mots FICHIER [ FICHIER ... ]");
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
		fclose(f);
	}
	// TOUT BE FREE
	for (i=0; i<argc-1; i++) {
		for (j=0; j<listes_de_mots[i].taille; j++)
			free(listes_de_mots[i].c[j].c);
		free(listes_de_mots[i].c);
	}
	free(listes_de_mots);
	return 0;
}
