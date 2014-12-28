#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int i, j;
	for (i=1; i<argc; i++) {
		printf("%d - %s (%d)\n", i, argv[i], strlen(argv[i]));
		for (j=0; argv[i][j]; j++)
			printf("% 9d %c\n", argv[i][j], argv[i][j]);
	}
	return 0;
}
