#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define clear() printf("\033[H\033[J")

void menu();
void hozzadas();
void listazas();
void modositas();
void vendTorles();
void ujRendezveny();
void rendTorles();

int main(int argc, char *argv[])
{
	printf("King of Stands nyilvantartas!\n\n");
	menu();

	return 0;
}

void menu() {

	int i;
	
	FILE *fp;

	
	

	
	printf("Menu:\n");
	printf("1. Uj vendeg jelentkezes.\n");
	printf("2. Vendegek listazasa.\n");
	printf("3. Vendeg adatainak modositasa.\n");
	printf("4. Vendeg adatainak torlese.\n");
	printf("-------------------------------------------\n");
	printf("5. Uj rendezveny inditasa.\n");
	printf("6. Rendezveny torlese.\n");
	printf("7. Kilepes\n\n");
	printf("Valassz menupontot: ");
	scanf("%d", &i);

	switch (i){

		case 1:
			clear();
			hozzadas();
			menu();
			break;

		case 2:
			clear();
			listazas();
			menu();
			break;

		case 3:
			clear();
			modositas();
			menu();
			break;

		case 4:
			clear();
			vendTorles();
			menu();
			break;

		case 5:
			clear();
			ujRendezveny();
			menu();
			break;

		case 6:
			clear();
			rendTorles();
			menu();
			break;

		case 7:
			exit(0);
			break;
		

		default:
			printf("Helytelen menupont lett megadva! Probald ujra!\n\n");
			menu();
			break;
	}






	/*printf("1. Uj rendezveny inditasa.\n");
	printf("2. Rendezvenyek listazasa.\n");
	printf("3. Rendezveny torlese.\n");
	printf("\n");
	printf("Add meg a menupontot: ");
	scanf("%d", &i);
	//printf("You have entered %d.\n",  i);
	switch (i)
	{
	case 1:
		printf("Uj rendezveny neve: ");
		scanf("%79s", nev);
		printf("Uj rendezveny azonositoja: ");
		scanf("%79s", azon);
		snprintf(filename, sizeof(filename), "%s.txt", azon);
		fp = fopen(filename, "w+");
		fprintf(fp, "%s\n", nev);
		fclose(fp);
		break;


	default:
		printf("Helytelen menupont lett megadva! Probald ujra!\n\n");
		menu();
		break;
	}*/

}

void hozzadas() {

	int ch = 0;
	int lines = 0;

	char nev[80];
	char email[80];
	char azon[80];
	char filename[64];
	FILE *fp, *fp2;
	time_t rawtime;
	struct tm * timeinfo;

	char buf[512];
	int n = 0;


	printf("\nUj vendeg jelentkezes:\n");
	printf("Vendeg neve: ");
	scanf("%79s", nev);
	printf("Vendeg email cime: ");
	scanf("%79s", email);
	printf("Rendezveny azonositoja: ");
	scanf("%79s", azon);

	fp2 = fopen("rendezvenyek.txt", "r");
	//ch = getc(fileptr1);

	while (fgets(buf, 512, fp2) != NULL) {

		int len = strlen(buf);
		char a[len];
		char b[len];

		sscanf(buf, " %[^;] , %s ", a, b);

		//printf("%s", a);
		if ((strstr(a, azon)) != NULL) {
			n++;
			break;
		}
		
		

	}

	fclose(fp2);

	if (n > 0) {
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		fp = fopen("vendegek.txt", "a+");
		fprintf(fp, "%s;%s;%s;%s", nev, email, azon, asctime(timeinfo));
		fclose(fp);

		fp = fopen("vendegek.txt", "r");
		while (!feof(fp))
		{
			ch = fgetc(fp);
			if (ch == '\n')
			{
				lines++;
			}
		}
		fclose(fp);
		printf("Sikeres jelentkezes, te vagy a(z) %d. jelentkezo!\n\n", lines);

	}
	else {

		printf("Hiba, nincs ilyen azonositoju rendezveny!\n");
	}



}

void listazas() {

	FILE *fp;
	int ch = 0;
	int lines = 0;

	fp = fopen("vendegek.txt", "r");
	while (!feof(fp))
	{
		ch = fgetc(fp);
		if (ch != ';') {
			putchar(ch);
		}
		else {
			printf(" - ");
		}
		
	}
	fclose(fp);
	printf("\n");




}

void modositas() {

	int seged = 0;
	char nev[80];
	char nev2[80];
	char buf[512];
	char *p;
	time_t rawtime;
	struct tm * timeinfo;

	FILE *fileptr1, *fileptr2;
	char filename[] = "vendegek.txt";
	char ch;
	int delete_line = 1;
	int temp = 1;


	printf("Add meg a modositando vendeg nevet: ");
	scanf("%79s", nev);

	printf("Add meg az uj nevet: ");
	scanf("%79s", nev2);


	fileptr1 = fopen(filename, "r");
	//ch = getc(fileptr1);

	int len = strlen(buf);
	char a[80];
	char b[80];
	char c[80];
	char d[200];

	while (fgets(buf, 512, fileptr1) != NULL) {

		

		sscanf(buf, "%[^;] ; %[^;] ; %[^;]", a, b, c);
		//printf("%s", buf);
		//printf("%s-%s-%s-%s\n\n", a, b, c);

		if ((strstr(a, nev)) != NULL) {
			
			break;
		}

		delete_line++;

	}

	//printf( "%s %s %s ,%s, %s\n", nev2, a, b, c, d);
	//printf( "%d", delete_line);

	//rewind
	rewind(fileptr1);

	fileptr2 = fopen("tmp.txt", "w+");
	char line[1024];
	int lineCount = 0;
	
	while (fgets(line, sizeof(line), fileptr1) != NULL)
	{
		if (lineCount != (delete_line-1))
		{
			fprintf(fileptr2, "%s", line);
		}
		else {
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			fprintf(fileptr2, "%s;%s;%s;%s", nev2, b, c, asctime(timeinfo));
		}

		lineCount++;
	}

	fclose(fileptr1);
	fclose(fileptr2);
	remove(filename);
	rename("tmp.txt", filename);

}

void vendTorles() {

	int seged = 0;
	char nev[80];
	char buf[512];
	char *p;

	FILE *fileptr1, *fileptr2;
	char filename[] = "vendegek.txt";
	char ch;
	int delete_line = 1;
	int temp = 1;


	printf("Add meg a torlendo vendeg nevet: ");
	scanf("%79s", nev);


	fileptr1 = fopen(filename, "r");
	//ch = getc(fileptr1);

	while (fgets(buf, 512, fileptr1) != NULL) {
		
		int len = strlen(buf);
		char a[len];
		char b[len];
		char c[len];

		sscanf(buf, " %[^;] , %[^;] , %s" , a, b, c);

		
		if ((strstr(a, nev)) != NULL) {

			break;
		}
		delete_line++;
		
	}

	//rewind
	rewind(fileptr1);

	fileptr2 = fopen("tmp.txt", "w+");
	char line[1024];
	int lineCount = 0;
	while (fgets(line, sizeof(line), fileptr1) != NULL)
	{
		if (lineCount != delete_line - 1)
		{
			fprintf(fileptr2, "%s", line);
		}

		lineCount++;
	}
	
	fclose(fileptr1);
	fclose(fileptr2);
	remove(filename);
	rename("tmp.txt", filename);



}

void ujRendezveny() {

	FILE *fp;
	char nev[80];
	char azon[80];

	printf("\nUj rendezveny inditasa:\n");
	printf("Rendezveny azonositoja: ");
	scanf("%79s", azon);
	printf("Rendezveny neve: ");
	scanf("%79s", nev);
	fp = fopen("rendezvenyek.txt", "a+");
	fprintf(fp, "%s;%s\n", azon, nev);
	fclose(fp);

}

void rendTorles() {
	
	int seged = 0;
	char nev[80];
	char buf[512];
	char *p;

	FILE *fileptr1, *fileptr2;
	char filename[] = "rendezvenyek.txt";
	char ch;
	int delete_line = 1;
	int temp = 1;

	
	printf("Add meg a torlendo rendezveny nevet: ");
	scanf("%79s", nev);


	fileptr1 = fopen(filename, "r");
	ch = getc(fileptr1);

	while (fgets(buf, 512, fileptr1) != NULL) {
		if ((strstr(buf, nev)) != NULL) {
			
			break;
		}
		delete_line++;
	}
	//rewind
	rewind(fileptr1);
	
	fileptr2 = fopen("tmp.txt", "w+");
	char line[1024]; 
	int lineCount = 0;
	while (fgets(line, sizeof(line), fileptr1) != NULL)
	{
		if (lineCount != delete_line -1)
		{
			fprintf(fileptr2, "%s", line);
		}

		lineCount++;
	}

	fclose(fileptr1);
	fclose(fileptr2);
	remove(filename);
	rename("tmp.txt", filename);



}

