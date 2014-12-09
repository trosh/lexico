#include <stdio.h>

int main(int argc, char* argv[]) {
	int i, haspipe;
	char line[200], title[50], foutname[60];
	FILE *fin, *fout; // I/O
	if (argc != 2) {
		fprintf(stderr, "usage: splitwiki FILE\n");
		return 1;
	}
	fout = NULL;
	if ((fin = fopen(argv[1], "r"))==NULL) {
		fprintf(stderr, "Mauvais FILE\n");
		return 1;
	}
	while (fgets(line, 200, fin)) {
		if (!strncmp(line, "[[", 2)) {
			haspipe = 0;
			for (i=2; line[i]!=']'; i++) {
				if (line[i]=='|') { // PAS UN ARTICLE
					haspipe = 1;
					break;
				}
				title[i-2] = line[i];
			}
			if (haspipe) continue; // PAS UN ARTICLE
			if (fout != NULL)
				fclose(fout);
			title[i-2] = '\0';
			puts(title);
			sprintf(foutname, "files/%s.txt", title);
			fout = fopen(foutname, "w"); // OUTPUT FILE
		}
		if (fout!=NULL) // S'IL Y A UN TITRE SUR LA PREMIERE
		                // LIGNE CA DEVRAIT TOUJOURS ETRE VRAI
			fputs(line, fout);
	}
	fclose(fout);
	fclose(fin);
	return 0;
}
