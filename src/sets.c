#include <sets.h>

set build_docs(dictionnaire *dico) {
	int i, j, k, nb_docs, nb_mots;
	set docs;
	docs.nb_lignes = nb_docs = dico->docs_taille; //ND
	docs.nb_colonnes = nb_mots = dico->taille; //NW
	docs.c = malloc(nb_docs*sizeof(float*));
	docs.contenu = calloc(nb_docs*nb_mots,sizeof(float)); //mise a zero des scores
	for (i=0; i<nb_docs; i++) {
		docs.c[i] = docs.contenu + i*nb_mots;
		for (j=0; j<nb_mots; j++)
			for (k=0; k<dico->app_tailles[j]; k++)
				if (dico->def[j].num_doc[k] == i) {
					docs.c[i][j] = dico->def[j].occurences[k];
					break;
				}
	}
	return docs;
}

set build_words(dictionnaire *dico) {
	int i, j, nb_mots, nb_docs;
	set words;
	words.nb_lignes = nb_mots = dico->taille;
	words.nb_colonnes = nb_docs = dico->docs_taille;
	words.c = malloc(nb_mots*sizeof(float*));
	words.contenu = calloc(nb_docs*nb_mots,sizeof(float));
	for (i=0; i<nb_mots; i++) {
		words.c[i] = words.contenu + i*nb_docs;
		for (j=0; j<dico->app_tailles[i]; j++){
			words.c[i][dico->def[i].num_doc[j]] = dico->def[i].occurences[j];
		}
	}
	return words;
}

void disp_set(set *docs) {
	int i, j;
	char num[10];
	for (i=0; i<docs->nb_lignes; i++) {
		for (j=0; j<docs->nb_colonnes && j<140; j++) {
			sprintf(num, "%.0f", 232+docs->c[i][j]*23);
			printf("\033[48;5;%sm ", num);
		}
		puts("\033[0m");
	}
}

void freeset(set *s) {
	int i;
	free(s->contenu);
	free(s->c);
}

void affiche_set(set* s) {
	int i,j;
	for (i=0; i<s->nb_lignes; i++) {
		for (j=0; j<s->nb_colonnes; j++)
			printf("%lg|",s->c[i][j]);
		printf("\n");
	}
}

//wesh tu me ctrl-c ctrl-v toi MTN !!!?
/* haha deso frere
float index_to_score(dictionnaire* dico, int w_id, int d_id) {
	int i, j, nb_mots, nb_docs, id_doc;
	nb_mots = dico->taille;
	nb_docs = dico->docs_taille;
	if (w_id >= nb_mots || w_id < 0) {
		printf("w_ID hors bornes [0,%d]\n", nb_mots);
		return 0.;
	}
	else if (d_id >= nb_docs || d_id < 0) {
		printf("d_ID hors bornes [0,%d]\n", nb_docs);
		return 0.;
	}
	else {
		for (i=0; i<nb_docs; i++)
			if (dico->def[w_id].num_doc[i] == d_id)
				return dico->def[w_id].occurences[d_id];
		//printf("le mot %d n'apparait pas dans le doc %d\n",w_id, d_id);
		return 0.;
	}
}
*/
