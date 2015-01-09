#include <lexico.h>

void freelistemots(listemots *mots) {
	int i;
	for (i=0; i<mots->taille; i++)
		free(mots->c[i].c); // FREE CHAQUE MOT
	free(mots->c);          // FREE TABLEAU DE MOTS
	free(mots->nom_doc);    // FREE NOM DOC
}

// TOUT BE FREE
void freelistesmots(listemots *listes_de_mots, int taille) {
	int i;
	for (i=0; i<taille; i++)
		freelistemots(listes_de_mots+i); // FREE CONTENU DE LA LISTE
	//free(listes_de_mots); // FREE LISTE DE LISTES
	//je pense que tu essayes de free un tableau vide car dans ta boucle tu a deja free le contenu
}

inline int est_une_lettre_valable(char c) {
	if ((c >= 'A' && c <= 'Z')  // EST UNE LETTRE MAJUSCULE
	 || (c >= 'a' && c <= 'z')  // EST UNE LETTRE MINUSCULE
	 ||  c == '-' || c == '\'') // EST UN HYPHEN / QUOTE
		return 1; // VRAI (EST UNE LETTRE VALABLE)
	return 0;     // FAUX (N'EST PAS UNE LETTRE VALABLE)
}

inline int recheck_mot_valable(char* str, int size) {
	if (str[0] == '-'  || str[size-1] == '-'
	 || str[0] == '\'' || str[size-1] == '\'')
		return 0; // FAUX (LETTRE SPECIALE EN DEBUT/FIN)
	return 1;     // VRAI (ELSE)
}

inline char convert_minuscule(char c) {
	if (c>='A' && c<= 'Z')
		return c - ('A'-'a'); 
	return c;
}

listemots decoupe_fichier(FILE *fichier, char* nom_doc) {
	char zone_de_travail[100], c, flag;
	int i, taille_du_mot, capacite = 1024;
	listemots mots;
	mot *last_mot;
	mots.c = malloc(capacite*sizeof(mot)); // DANGER
	mots.nom_doc = malloc(strlen(nom_doc)+1);
	strcpy(mots.nom_doc, nom_doc);
	mots.taille = 0;
	taille_du_mot = 0;
	while ((c=fgetc(fichier))!=EOF) { // DANGER!!! PAS FORCEMENT ASCII
		if (est_une_lettre_valable(c)) {
			c = convert_minuscule(c);
			zone_de_travail[taille_du_mot] = c;
			taille_du_mot++;
		}
		else if (taille_du_mot>0) { // FIN DU MOT
			if(!recheck_mot_valable(zone_de_travail, taille_du_mot)) {
				taille_du_mot = 0;
				continue;
			}
			zone_de_travail[taille_du_mot] = '\0';
			flag = 0; // LE MOT A-T-IL ETE TROUVE ?
			for (i=0; i<mots.taille; i++)
				if (!strcmp(mots.c[i].c, zone_de_travail)) {
					mots.c[i].occurences++;
					taille_du_mot = 0;
					flag = 1; // LE MOT A ETE TROUVE
					break;
				}
			if (flag) continue;
			// VERIF TAILLE DE MOTS, DOUBLE TAILLE SI NECESSAIRE
			if (mots.taille==capacite-1)
				mots.c = realloc(mots.c, (capacite*=2)*sizeof(mot));
			last_mot = mots.c + mots.taille;
			last_mot->occurences = 1;
			last_mot->c = strndup(zone_de_travail, taille_du_mot+1);
			// PAS BESOIN DE VIDER ZONE_DE_TRAVAIL !
			mots.taille++;
			taille_du_mot = 0;
		}
	}
	return mots;
}

void print_mots(FILE* flux, listemots *mots) {
	int i;
	fprintf(flux, "%s\n", mots->nom_doc);
	for (i=0; i<mots->taille; i++) {
		fprintf(flux, "% 15s% 4d", mots->c[i].c, mots->c[i].occurences);
		if ((i+1)%4 == 0) fputc('\n', flux);
	}
	fputc('\n', flux);
}
