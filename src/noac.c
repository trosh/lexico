#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unac.h>

#define LINESIZE 4096
// J'ARRIVE PAS A FAIRE MARCHER DES GROSSES LIGNES
// AVEC > 0 ACCENTS ET UN PETIT LINESIZE MAIS AU
// MOINS LA CA MARCHE DONC ESPERONS QUE Y'A PAS DE
// TROP GROSSE LIGNE OU QUE QUELQU'UN D'INTELLIGENT
// FIXE CA :)

int main(int argc, char* argv[]) {
	char *out = NULL, *fnout, *line;
	size_t out_length = 0;
	FILE *fin, *fout;
	int i;
	if (argc < 2) {
		fprintf(stderr,
		        "usage: noac files/[...] [...]\n"
		        "\tcopie le(s) fichier(s) sans accents dans\n"
		        "\tnoacfiles/[...]\n");
		return 1;
	}
	for (i=1; i<argc; i++) {
		fin = fopen(argv[i], "r");
		if (fin == NULL) {
			fprintf(stderr, "mauvais entree %s\n", argv[i]);
			return 1;
		}
		if ( unac_string("UTF-8", argv[i], strlen(argv[i]),
		                 &out, &out_length) ) {
			printf("unac_string error on %s\n", argv[i]);
			return 1;
		}
		fnout = malloc(100);
		sprintf(fnout, "noac%s", out);
		printf("output filename: %s\n", fnout);
		free(out);
		out = NULL;
		out_length = 0;
		fout = fopen(fnout, "w");
		if (fout == NULL) {
			fprintf(stderr, "mauvaise sortie %s\n", fnout);
			return 1;
		}
		free(fnout);
		line = malloc(LINESIZE);
		while (fgets(line, LINESIZE, fin)) {
			if ( unac_string("UTF-8", line, strlen(line),
			                 &out, &out_length) ) {
				printf("unac_string error on %s\n", line);
				printf("length: %d\n", strlen(line));
				return 1;
			}
			fputs(out, fout);
			free(out);
			out = NULL;
			out_length = 0;
		}
		fclose(fout);
		free(line);
		fclose(fin);
	}
	return 0;
}
