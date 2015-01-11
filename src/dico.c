#include "dico.h"

void init_dico(dictionnaire *dico) {
	dico->taille = 0;
	dico->capacite = 1024;
	dico->def = malloc(dico->capacite*sizeof(definition));
	dico->app_tailles = malloc(dico->capacite*sizeof(int));
	dico->app_capacites = malloc(dico->capacite*sizeof(int));
	dico->docs_taille = 0;
	dico->docs_capacite = 128;
	dico->docs = malloc(dico->docs_capacite*sizeof(char*));
}

void ajoute_mot_existe(dictionnaire *dico,
		int id_doc, char occs, int id_def) {
	definition* def;
	int *taille, *cap;
	def = dico->def + id_def;
	taille = dico->app_tailles   + id_def;
	cap    = dico->app_capacites + id_def;
	if (*taille == *cap) {
		def->num_doc = realloc(def->num_doc, ((*cap)*=4)*sizeof(int));
		def->occurences = realloc(def->occurences, (*cap)*sizeof(float));
	}
	def->num_doc   [*taille] = id_doc;
	def->occurences[*taille] = occs;
	(*taille)++;
}

void ajoute_mot_nouveau(dictionnaire *dico,
		int id_doc, mot* word) {
	int i, id_def;
	definition* def;
	if (dico->taille == dico->capacite) {
		dico->def
		= realloc(dico->def, (dico->capacite *= 4)*sizeof(definition));
		dico->app_tailles
		= realloc(dico->app_tailles, dico->capacite*sizeof(int));
		dico->app_capacites
		= realloc(dico->app_capacites, dico->capacite*sizeof(int));
	}
	id_def = dico->taille++;
	def = dico->def + id_def;
	def->c = strdup(word->c);
	def->checksum = word->checksum;
	dico->app_tailles  [id_def] = 1;
	dico->app_capacites[id_def] = 1;
	def->num_doc    = malloc(sizeof(int)); // ASSUME CAP = 1
	def->num_doc[0] = id_doc;
	def->occurences    = malloc(sizeof(float)); // ASSUME CAP = 1
	def->occurences[0] = word->occurences;
}

void ajoute_dico(dictionnaire *dico, listemots *mots) {
	char *nom_doc, *word, flag;
	int i, j, id_doc, id_def, checksum;
	if (dico->docs_taille == dico->docs_capacite)
		dico->docs = realloc(dico->docs,
			(dico->docs_capacite *= 4)*sizeof(char*));
	// INC DOC_TAILLE + POINTEUR TO LAST NOM_DOC :
	id_doc = dico->docs_taille++;
	nom_doc = dico->docs[id_doc] = malloc(strlen(mots->nom_doc)+1);
	strcpy(nom_doc, mots->nom_doc); // COPY
	for (i=0; i<mots->taille; i++) {
		word = mots->c[i].c;
		checksum = mots->c[i].checksum;
		flag = 0; // DOES WORD EXIST ?
		for (j=0; j<dico->taille; j++)
			if (dico->def[j].checksum == checksum // PRE CHECK
			 && !strcmp(dico->def[j].c, word)) { // WORD EXISTS
				ajoute_mot_existe(dico, id_doc,
					mots->c[i].occurences, j);
				flag = 1;
				break;
			}
		if (!flag) // WORD DOESN'T EXIST
			ajoute_mot_nouveau(dico, id_doc, mots->c+i);
	}
}

/* PROBABLEMENT INUTILE
void affiche_docs(dictionnaire* dico) {
	int i, nb_docs;
	nb_docs = dico->docs_taille;
	printf("dico->docs_taille   = %d\n"
	       "dico->docs_capacite = %d\n",
	       dico->docs_taille,
	       dico->docs_capacite);
	for (i=0; i<nb_docs; i++)
		printf("%d - %s\n", i, dico->docs[i]);
}
*/

/* PROBABLEMENT INUTILE POUR LES GROS CORPUS AUSSI */
void affiche_dico(dictionnaire* dico) {
	int i, j, k, nb_mots, nb_docs, id_doc, l, cnt;
	float occ;
	nb_mots = dico->taille;
	for (i=nb_mots-1; i>=0; i--) {
		l = strlen(dico->def[i].c);
		printf("\033[31m");
		if (l<20)
			printf("% 20s", dico->def[i].c);
		else
			for (k=0; k<20; k++)
				putchar(dico->def[i].c[k]);
		printf("\033[0m");
		nb_docs = dico->app_tailles[i];
		cnt = 1;
		for (j=0; j<nb_docs; j++) {
			id_doc = dico->def[i].num_doc[j];
			occ    = dico->def[i].occurences[j];
			l = strlen(dico->docs[id_doc]);
			putchar(' ');
			if (l<20)
				printf("%20s", dico->docs[id_doc]);
			else
				for (k=0; k<20; k++)
					putchar(dico->docs[id_doc][k]);
			printf(" \033[32m%9g\033[0m", occ);
			if (cnt%4==0 && cnt!=nb_docs) {
				putchar('\n');
				for (k=0; k<20; k++)
					putchar(' ');
			}
			cnt++;
		}
		putchar('\n');
	}
}

/* DEBUG SCORE */
void affiche_dico_bad(dictionnaire *dico) {
	int i, j;
	for (i=0; i<dico->taille; i++)
		for (j=0; j<dico->app_tailles[i]; j++)
			if (dico->def[i].occurences[j] < 0
			 || dico->def[i].occurences[j] > 1)
				printf("%20s\t%g\n",
					dico->def[i].c,
					dico->def[i].occurences[j]);
}

/* CONVERT OCCURENCES -> SCORE DANS DICO */
void frequence_dico(dictionnaire *dico) {
	int i, j, nb_mots, nb_docs;
	float freq_w_doc;
	nb_mots = dico->taille;
	for (i=0; i<nb_mots; i++) {
		freq_w_doc = 0.;
		// NB DE DOCS POUR UN MOT
		nb_docs = dico->app_tailles[i];
		for (j=0; j<nb_docs; j++) // BEGIN -> END
			freq_w_doc += dico->def[i].occurences[j];
		// FREQ_W DANS UN DOC / FREQ_W DANS LE DICO
		for (j=nb_docs-1; j>=0; j--) {// BEGIN <- END
			dico->def[i].occurences[j] /= freq_w_doc;
			if (dico->def[i].occurences[j] < 0
			 || dico->def[i].occurences[j] > 1)
				printf("%s %g %g\n",
					dico->def[i].c,
					dico->def[i].occurences[j],
					freq_w_doc);
		}
	}
}

void freedico(dictionnaire *dico) {
	int i, nb_docs, nb_mots;
	nb_docs = dico->docs_taille;
	nb_mots = dico->taille;
	//FREE INDEX DOCS
	for (i=0; i<nb_docs; i++)
		free(dico->docs[i]);
	free(dico->docs);
	free(dico->app_tailles);
	free(dico->app_capacites);
	//FREE DEFS
	for (i=0; i<nb_mots; i++) {
		free(dico->def[i].c);
		free(dico->def[i].num_doc);
		free(dico->def[i].occurences);
	}
	free(dico->def);
}
