#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "question.h"

void readdata(struct question *questions, int *k) {

	printf("MLSZ: Kerdobiztos kuldese a kozpontba... \n");

	// Adatok betöltése a fájlból
	FILE *f;
	char line[512];
	int i;

	f = fopen("vegleges.txt", "r");

	printf("Adatok beolvasasa...\n");

	while (!feof(f)) {

		if (fgets(line, sizeof line, f) != NULL) {

			strcpy(questions[*k].question, line);

			fgets(line, sizeof line, f);
			questions[*k].qnum = atoi(line);

			for (i = 0; i < questions[*k].qnum; ++i) {
				fgets(line, sizeof line, f);
				strcpy(questions[*k].answers[i], line);
			}

			fgets(line, sizeof line, f);
			strcpy(questions[*k].date, line);

			*k = *k + 1;

		}

	}

	fclose(f);

	printf("Adatok sikeresen beolvasva.\n");

}