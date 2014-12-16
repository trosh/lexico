#include "dico.h"

void init_dico(dictionnaire *dico){
	
}
void ajoute_dico(dictionnaire *dico, listemots *liste_mots){
	dico->taille=0;
	dico->capacite=1024;
	dico->def=malloc(dico->capacite*sizeof(definition));
}
