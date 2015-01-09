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
                       int id_doc,
                       char occs,
                       int id_def) {
	definition* def;
	int *taille, *cap;
	def = dico->def + id_def;
	taille = dico->app_tailles   + id_def;
	cap    = dico->app_capacites + id_def;
	//printf("%d-1\n",*cap);
	if (*taille == *cap) {
		def->num_doc = realloc(def->num_doc, ((*cap)*=4)*sizeof(int));
		def->occurences = realloc(def->occurences, (*cap)*sizeof(float));
	}
	//printf("%d-2\n",*cap);
	/**/
	def->num_doc   [*taille] = id_doc;
	def->occurences[*taille] = occs;
	(*taille)++;
}

void ajoute_mot_nouveau(dictionnaire *dico,
                        int id_doc,
                        mot* word) {
	definition* def;
	int id_def;
	if (dico->taille == dico->capacite) {
		dico->def = realloc(dico->def,
			(dico->capacite *= 4)*sizeof(definition));
		dico->app_tailles = realloc(dico->app_tailles, dico->capacite*sizeof(int));
		dico->app_capacites = realloc(dico->app_capacites, dico->capacite*sizeof(int));
	}
	id_def = dico->taille++;
	def = dico->def + id_def;
	//def->c = "";
	def->c = strdup(word->c);
	dico->app_tailles  [id_def] = 1;
	dico->app_capacites[id_def] = 1;
	def->num_doc    = malloc(sizeof(int)); // ASSUME CAP = 1
	def->num_doc[0] = id_doc;
	def->occurences    = malloc(sizeof(float)); // ASSUME CAP = 1
	def->occurences[0] = word->occurences;
}

void ajoute_dico(dictionnaire *dico, listemots *mots) {
	char *nom_doc, *word, flag;
	int i, j, id_doc, id_def;
	if (dico->docs_taille == dico->docs_capacite)
		dico->docs = realloc(dico->docs,
			(dico->docs_capacite *= 4)*sizeof(char*));
	// INC DOC_TAILLE + POINTEUR TO LAST NOM_DOC :
	id_doc = dico->docs_taille++;
	nom_doc = dico->docs[id_doc] = malloc(strlen(mots->nom_doc)+1);
	strcpy(nom_doc, mots->nom_doc); // COPY
	for (i=0; i<mots->taille; i++) {
		word = mots->c[i].c;
		//printf("%20s", word);
		//if (i%4==3) putchar('\n');
		flag = 0; // DOES WORD EXIST ?
		for (j=0; j<dico->taille; j++)
			if (!strcmp(dico->def[j].c, word)) { // WORD EXISTS
				ajoute_mot_existe(dico, id_doc,
					mots->c[i].occurences, j);
				flag = 1;
				break;
			}
		if (!flag) // WORD DOESN'T EXIST
			ajoute_mot_nouveau(dico, id_doc, mots->c+i);
	}
}

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

void affiche_dico(dictionnaire* dico) {
	int i, j, nb_mots, nb_docs, id_doc, l;
	float occ;
	nb_mots = dico->taille;
	for (i=nb_mots-1; i>=0; i--) {
		printf("% 25s:", dico->def[i].c);
		nb_docs = dico->app_tailles[i];
		for (j=0; j<nb_docs; j++) {
			id_doc = dico->def[i].num_doc[j];
			occ    = dico->def[i].occurences[j];
			printf("\t%s %lg", dico->docs[id_doc], occ);
			//printf("% 4d %d", id_doc, occ);
		}
		putchar('\n');
	}
}

void freedico(dictionnaire *dico) {
	int i, nb_docs, nb_mots;
	nb_docs = dico->docs_taille;
	nb_mots = dico->taille;
//FREE index Docs
	for (i=0; i<nb_docs; i++)
		free(dico->docs[i]);
	free(dico->docs);
	free(dico->app_tailles);
	free(dico->app_capacites);
//FREE des Def
	for (i=0; i<nb_mots; i++) {
		free(dico->def[i].c);
		free(dico->def[i].num_doc);
		free(dico->def[i].occurences);
	}
	free(dico->def);
}


void frequence_dico(dictionnaire *dico) {
	int i, j, nb_mots, nb_docs;
	float freq_w_doc;
	nb_mots = dico->taille;
	//calcul du nombre de mots au total
	for (i=0; i<nb_mots; i++) {
		//printf("dico->app_tailles[i]=%d\n",dico->app_tailles[i]);
		freq_w_doc = 0.;
		nb_docs = dico->app_tailles[i];	//nombre differents de docs pour 1 mot en particulier
		for (j=0; j<nb_docs; j++)
			freq_w_doc += dico->def[i].occurences[j];
		for (j=nb_docs-1; j>=0; j--) // (mini optimisation) freq_w dans un doc / freq_w dans le dico
			dico->def[i].occurences[j] /= freq_w_doc;
	}
}

