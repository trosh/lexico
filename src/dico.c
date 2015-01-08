#include "dico.h"

void init_dico(dictionnaire *dico) {
	dico->taille = 0;
	dico->capacite = 1024;
	dico->def = malloc(dico->capacite*sizeof(definition));
	dico->docs_taille = 0;
	dico->docs_capacite = 128;
	dico->docs = malloc(dico->docs_capacite*sizeof(char*));
}

void ajoute_dico(dictionnaire *dico, listemots *mots) {
	char *nom_doc, *mot;
	definition *def;
	apparition *app;
	int i, j;
	char flag;
	if (dico->docs_taille == dico->docs_capacite)
		dico->docs = realloc(dico->docs,
			(dico->docs_capacite *= 2)*sizeof(char*));
	// INC DOC_TAILLE + POINTEUR TO LAST NOM_DOC :
	nom_doc = dico->docs[dico->docs_taille++];
	nom_doc = malloc(strlen(mots->nom_doc));
	strcpy(nom_doc, mots->nom_doc); // COPY
	for (i=0; i<mots->taille; i++) {
		mot = mots->c[i].c;
		flag = 0; // WORD EXISTS ?
		for (j=0; j<dico->taille; j++)
			if (!strcmp(dico->def[j].mot, mot)) {
				flag = 1; // WORD EXISTS !!
				def = dico->def + j;
				if (def->app_taille == def->app_capacite)
					def->app = realloc(def->app,
						(def->app_capacite *= 2)*sizeof(apparition));
				app = def->app + def->app_taille++;
				break;
			}
		if (!flag) { // WORD DOESN'T EXIST
			if (dico->taille == dico->capacite)
				dico->def = realloc(dico->def,
					(dico->capacite *= 2)*sizeof(definition));
			def = dico->def + dico->taille++;
			def->mot = malloc(strlen(mot));
			strcpy(def->mot, mot);
			def->app_taille = 1;
			def->app_capacite = 8;
			def->app = malloc(def->app_capacite*sizeof(apparition));
			app = def->app;
		}
		app->num_doc = dico->docs_taille-1;
		app->occurences = mots->c[i].occurences;
	}
}
