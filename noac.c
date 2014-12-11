#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unac.h>

int main(int argc, char* argv[]) {
	char *out = NULL, *fnout, *line;
	size_t out_length = 0;
	FILE *fin, *fout;
	int i;
	if (argc < 2) {
		fprintf(stderr,
		        "usage: noac file/[...].txt [...]\n"
		        "\tcopie le(s) fichier(s) sans accents dans\n"
		        "\tnoacfiles/[...].txt\n");
		return 1;
	}
	for (i=1; i<argc; i++) {
		if ((fin=fopen(argv[i], "r"))==NULL) {
			fprintf(stderr, "mauvais entree %s\n", argv[i]);
			return 1;
		}
		if ( unac_string("UTF-8", argv[i], strlen(argv[i]),
						 &out, &out_length) ) {
			printf("unac_string error on %s\n", argv[i]);
			return 1;
		}
		fnout = malloc(50);
		sprintf(fnout, "noac%s", out);
		printf("fnout: %s\n", fnout);
		free(out);
		out = NULL;
		out_length = 0;
		if ((fout=fopen(fnout, "w"))==NULL) {
			fprintf(stderr, "mauvaise sortie %s\n", fnout);
			return 1;
		}
		line = malloc(200);
		while (fgets(line, 200, fin)) {
			if ( unac_string("UTF-8", line, strlen(line),
			                 &out, &out_length) ) {
				printf("unac_string error on %s\n", line);
				return 1;
			}
			fputs(out, fout);
			free(out);
			out = NULL;
			out_length = 0;
		}
	}
	return 0;
}
