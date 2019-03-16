#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    char* fileName = "data.txt"; /* should check that argc > 1 */
    FILE* file = fopen(fileName, "r"); /* should check the result */
    char line[256];
	int bufferSize = 100;
	char* lines[bufferSize];
	int i;
	for(i=0; i<bufferSize; i++) lines[i] = (char*) malloc(bufferSize * sizeof(char));
	
	int linesNumber = 0;

    while (fgets(line, sizeof(line), file)) {
        printf("%s\n", line);
		stpcpy(lines[linesNumber], line);
		linesNumber++;
    }
	
	printf("-----\n");
	
	for(i=0; i<linesNumber; i++) printf("%s\n", lines[i]); 

    fclose(file);
	
	for(i=0; i<bufferSize; i++) free(lines[i]);
	
    return 0;
}