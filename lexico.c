#include "lexico.h"

void freelistemots(listemots *mots) {
	int i;
	for (i=0; i<mots->taille; i++) {
		puts(mots->c[i].c);
		free(mots->c[i].c); // FREE CHAQUE MOT
	}
	free(mots->c);          // FREE TABLEAU DE MOTS
}

// TOUT BE FREE
void freelistesmots(listemots *listes_de_mots, int taille) {
	int i;
	for (i=0; i<taille; i++)
		freelistemots(listes_de_mots+i); // FREE CONTENU DE LA LISTE
	free(listes_de_mots); // FREE LISTE DE LISTES
}

inline int est_une_lettre_valable(char c) {
	if ((c >= 'A' && c <= 'Z')  // EST UNE LETTRE MAJUSCULE
	 || (c >= 'a' && c <= 'z')  // EST UNE LETTRE MINUSCULE
	 ||  c == '-' || c == '\'') // EST UN HYPHEN / QUOTE
		return 1; // VRAI (EST UNE LETTRE VALABLE)
	return 0;     // FAUX (N'EST PAS UNE LETTRE VALABLE)
}

inline char convert_minuscule(char c) {
	if (c>='A' && c<= 'Z')
		return c - ('A'-'a'); 
	return c;
}

listemots decoupe_fichier(FILE *fichier) {
	char zone_de_travail[100], c, flag;
	int i, taille_du_mot, capacite = 1024;
	listemots mots;
	mot *last_mot;
	mots.c = malloc(capacite*sizeof(mot)); // DANGER
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
			if (mots.taille==capacite-1)
				mots.c = realloc(mots.c, (capacite*=2)*sizeof(mot));
			last_mot = mots.c + mots.taille;
			last_mot->occurences = 1;
			last_mot->c = malloc((taille_du_mot+1)*sizeof(char)); // DANGER
			strncat(last_mot->c, zone_de_travail, taille_du_mot+1);
			// PAS BESOIN DE VIDER ZONE_DE_TRAVAIL !
			mots.taille++;
			taille_du_mot = 0;
		}
	}
	return mots;
}

void print_mots(listemots mots) {
	int i;
	for (i=0; i<mots.taille; i++) {
		printf("% 20s % 3d", mots.c[i].c, mots.c[i].occurences);
		if (i%4 == 0) puts("");
		else printf("\t");
	}
	puts("");
}