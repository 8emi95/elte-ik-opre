#include <stdio.h>

int main()
{
	printf("Add meg a kerdest (max 120 karakter): \n");
	char kerdes[120];
	fgets(kerdes, 120, stdin);
	printf("\n");
	printf("Add meg a válaszokat (min 2, max 4): \n");
	char valasz[120];
	
	int c;
	FILE *file;
	file = fopen("kerdessor.txt", "a");
	if (file) {
		fprintf(file, kerdes);
		fprintf(file, "\n");
		int i = 0;
		fgets(valasz, 120, stdin);
		while(i != 4)
		{
			fprintf(file, valasz);
			fgets(valasz, 120, stdin);
			i = i + 1;
		}
		fprintf(file, "\n");
		fclose(file);
		printf("\n\n\n");
	}
	
	
	return 0;
}