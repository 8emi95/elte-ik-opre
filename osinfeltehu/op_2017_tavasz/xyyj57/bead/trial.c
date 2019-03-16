#include <stdio.h>

void kerdes_veglegesites()
{
	//szam megadasa
	//adott szamu kerdes vegleges.txt fajlba irasa
}

void kerdessor_megadasa()
{
	//kerdoiv.txt
	//szamok megadasa
	//
}

void kerdes_torlese()
{
	//szam megadasa
	//torles a fajlbol
}

void kerdes_modositas(char *num)
{
	char temp[3];
	FILE *file;
	file = fopen("kerdessor.txt", "r+");
	if(file)
	{
		while(fgets(temp, 3, file) != NULL)
		{
			if((strstr(temp, *num)) != NULL)
			{
				printf("MATCH FOUND");
			}
		}
	}
}

void kerdesek_szamat_novel()
{
	int ks;
	FILE *file;
	file = fopen("kerdessor.txt", "r+");
	fscanf(file, "%d", &ks);
	ks = ks + 1;
	fprintf(file, "%d", ks);
	
}

void kerdesek_kilistaz(int *ks) 
{ 
	int c;
	FILE *file;
	file = fopen("kerdessor.txt", "r");
	if (file) {
		while ((c = getc(file)) != EOF)
			putchar(c);
		fclose(file);
		printf("\n\n\n");
	}
}

void kerdest_hozzafuz(int *ks)
{
	//számmal együtt
	printf("Add meg a kerdest (max 120 karakter): \n");
	char kerdes[150];
	fgets(kerdes, 150, stdin);
	fgets(kerdes, 150, stdin);
	printf("\n");
	printf(kerdes);

	printf("Add meg a válaszokat (min 2, max 4): \n");
	char valasz[120];
	
	int c;
	FILE *file;
	file = fopen("kerdessor.txt", "a");
	if (file) {
		*ks = *ks + 1;
		kerdesek_szamat_novel();
		fprintf(file, "#%d# ", *ks);
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

}

int main()
{	//FIXME
	int *kerdesek_szama;
	FILE *file;
	file = fopen("kerdessor.txt", "r");
	if(file != NULL)
	{
		fscanf(file, "%d", &kerdesek_szama);
		fclose(file);
		printf("Kerdesek szama: %d", *kerdesek_szama);
		fprintf(file, "\n");
		fprintf(file, "\n");
	}
	else 
	{
		kerdesek_szama = 0;
		file = fopen("kerdessor.txt", "w");
		fprintf(file, "%d", *kerdesek_szama);
		fprintf(file, "\n");
		printf("Kerdesek szama: %d \n", *kerdesek_szama);
		fprintf(file, "\n");
		fclose(file);
	}
	printf("\n\n"),
	printf("1) Kerdessor megadasa \n");
	printf("2) Uj kerdes felvetele \n");
	printf("3) Kerdes modositasa \n");
	printf("4) Kerdes torlese\n");
	printf("5) Kerdesek listazasa\n");
	printf("6) Kerdes veglegesitese\n");
	printf("7) Kilepes \n");
	printf("\n");
	printf("\n");
   
	char str[3];
//MENU kezelés
	while(1)
	{
		scanf("%s", str);
		int tmp = atoi(str);
		switch(tmp) {
			case 1: printf("Kerdessor megadasa \n"); break;
			case 2: printf("Uj kerdes felvetele \n");
case 3: printf("Kerdes modositasa \n Kerdesszama: ");char *num;scanf("%s", num); kerdes_modositas(num); break;
					kerdest_hozzafuz(kerdesek_szama); break;
			case 5: printf("Kerdesek listazasa \n\n\n"); 
					kerdesek_kilistaz(kerdesek_szama); break;
			case 7: printf("Kilepes \n"); return 0; break;
			default: printf("Ilyen menupont nincs \n");
		}
	}
	
	return 0;
}