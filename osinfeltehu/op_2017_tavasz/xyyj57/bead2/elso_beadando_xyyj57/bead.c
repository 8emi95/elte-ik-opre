#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>

void kimasol_filebol_from_to(char *fname, char *to, char *kezdete, char *vege)
{
  FILE *fp;
	char temp[512];
  char linec[512];
	char *kezd_tmp = strcat(kezdete, ".)");
	printf("KEZD_TMP: %s\n", kezd_tmp);

	if((fp = fopen(fname, "r")) == NULL) {
		printf("Open Error");
        exit(1);
	}

    FILE *tofp;

    if((tofp = fopen(to, "a")) == NULL) {
		printf("Open Error");
        exit(1);
	}

//mukodes
	while(fgets(temp, 512, fp) != NULL) {
		if((strstr(temp, kezd_tmp)) != NULL) {	//'1.)' ha van valahol 1-es a file-ban akkor is egyezik
            printf("valahol egyezik");
            strcpy(linec, temp);
            fprintf(tofp, linec);
			while(fgets(temp, 512, fp) != NULL)
            {
                strcpy(linec, temp);
                fprintf (tofp, linec);
                if((strstr(temp, vege)) != NULL) {
                    break;
                }
            }
		}
    }

    if(fp) {
		fclose(fp);
	}
    if(tofp) {
		fclose(tofp);
	}
}

void kerdessor_megadasa()
{
	char *i;
	printf("\n");
	while(1)
	{
		scanf("%s", i);
		if(*i == '0')
		{
			break;
		}
		kimasol_filebol_from_to("kerdessor.txt","kerdoiv.txt", i,"###");
		//printf("Input: %s\n", i);
	}
	printf("end");
}

void kerdes_felvetele()			// FIXME Datummal egyutt
{
	int kerdesek_szama;
	FILE *fp_old;
	FILE *fp_new;
	fp_old = fopen("kerdesek_szama.txt", "r+");
	fp_new = fopen("new.txt", "w+");
	
	fscanf(fp_old, "%d", &kerdesek_szama);
	kerdesek_szama = kerdesek_szama + 1;
	fprintf(fp_new, "%d", kerdesek_szama);
	fclose(fp_old);
	fclose(fp_new);
	remove("kerdesek_szama.txt");
	rename("new.txt", "kerdesek_szama.txt");
	
	printf("Add meg a kerdest (max 120 karakter): \n");
	char kerdes[150];
	fgets(kerdes, 150, stdin);
	fgets(kerdes, 150, stdin);
	printf("\n");
	printf(kerdes);

	printf("Add meg a válaszokat (min 2, max 4) enter leutessel kihagyhatod a valaszt: \n");
	char valasz[120];
	
	int c;
	FILE *file;
	file = fopen("kerdessor.txt", "a+");
	if (file) {
		fprintf(file, "%i.) ", kerdesek_szama);
		fprintf(file, "%s\n", kerdes);
		//ido lekerese
		char ido[32];
		time_t t;
		time(&t);
		strftime(ido, sizeof(ido), "(%Y,%m,%d,%H,%M,%S)", localtime(&t));
		fprintf(file, "(%s)\n", ido);
		
		int i = 1;
		int enters = 0;
		while(i != 5)
		{
			fgets(valasz, 120, stdin);
			if (enters == 2)
			{
				while(*valasz == '\n')
				{
					fgets(valasz, 120, stdin);
				}
			}
			if(*valasz == '\n')
			{
				++enters;
				printf("%i. valasz kihagyva\n", i);
			}
			else
			{
				printf("%i. valasz %s\n", i, valasz);
			}
			fprintf(file, valasz);
			i = i + 1;
		}

		fprintf(file, "###\n\n\n");
		fclose(file);
	}
}

void kerdes_modositasa()
{
	  //Kerdest torol
		FILE *fp1, *fp2;
    char c = 1;
    int kerdes_eleje;
    char kerdes_vege = '#';

    fp1 = fopen("kerdessor.txt", "r");
    
    printf(" \n Enter kerdes szama (csak szamot adj meg): ");
    scanf("%i", &kerdes_eleje);

    fp2 = fopen("copy.txt", "w");

    while (c != EOF)
		{
      c = getc(fp1);
      if (c != kerdes_eleje + '0')
      {
				putc(c, fp2);
      }
      else	//ha megegyezik a kerdes szamaval
      {
					char temp = c;
					c = getc(fp1);
					if(c != '.')
					{
						putc(temp, fp2);
						putc(c, fp2);
					}
					else
					{
						while (c != kerdes_vege)
						{
							c = getc(fp1);
						}
						c = getc(fp1);
						c = getc(fp1);
					}
			}
    }

    fclose(fp1);
    fclose(fp2);

    remove("kerdessor.txt");
    rename("copy.txt", "kerdessor.txt");
		
		//Uj kerdest beker
	printf("Add meg a kerdest (max 120 karakter): \n");
	char kerdes[150];
	fgets(kerdes, 150, stdin);
	fgets(kerdes, 150, stdin);
	printf("\n");

	printf("Add meg a válaszokat (min 2, max 4) enter leutessel kihagyhatod a valaszt: \n");
	char valasz[120];

	FILE *file;
	file = fopen("kerdessor.txt", "a+");
	if (file) {
		fprintf(file, "%i.) ", kerdes_eleje);
		fprintf(file, "%s\n", kerdes);
		
		int i = 1;
		int enters = 0;
		while(i != 5)
		{
			fgets(valasz, 120, stdin);
			if (enters == 2)
			{
				while(*valasz == '\n')
				{
					fgets(valasz, 120, stdin);
				}
			}
			if(*valasz == '\n')
			{
				++enters;
				printf("%i. valasz kihagyva\n", i);
			}
			else
			{
				printf("%i. valasz %s\n", i, valasz);
			}
			fprintf(file, valasz);
			i = i + 1;
		}

		fprintf(file, "###\n\n\n");
		fclose(file);
	}
}

void kerdes_torlese()
{
	//Kerdest torol
		FILE *fp1, *fp2;
    char c = 1;
    int kerdes_eleje;
    char kerdes_vege = '#';

    fp1 = fopen("kerdessor.txt", "r");
    
    printf(" \n Enter kerdes szama (csak szamot adj meg): ");
    scanf("%i", &kerdes_eleje);

    fp2 = fopen("copy.txt", "w");

    while (c != EOF)
		{
      c = getc(fp1);
      if (c != kerdes_eleje + '0')
      {
				putc(c, fp2);
      }
      else	//ha megegyezik a kerdes szamaval
      {
					char temp = c;
					c = getc(fp1);
					if(c != '.')
					{
						putc(temp, fp2);
						putc(c, fp2);
					}
					else
					{
						while (c != kerdes_vege)
						{
							c = getc(fp1);
						}
						c = getc(fp1);
						c = getc(fp1);
					}
			}
    }

    fclose(fp1);
    fclose(fp2);

    remove("kerdessor.txt");
    rename("copy.txt", "kerdessor.txt");
}

void kerdesek_listazasa()
{
int c;
	FILE *kerdessor;
	kerdessor = fopen("kerdessor.txt", "r");
	if (kerdessor) {
		while ((c = getc(kerdessor)) != EOF)
			putchar(c);
		fclose(kerdessor);
		printf("\n\n\n");
	}
}

void kerdes_veglegesitese()
{
	char *ker_szama;
	printf("Add meg a kerdes szamat: ");
	scanf("%s", ker_szama);
	kimasol_filebol_from_to("kerdessor.txt", "vegleges.txt", ker_szama, "###");
}


void 

int main()
{

	printf("KERDOIV KEZELO \n\n");
	printf("1) Kerdessor megadasa \n");
	printf("2) Kerdes felvetele \n");
	printf("3) Kerdes modositasa \n");
	printf("4) Kerdes torlese\n");
	printf("5) Kerdesek listazasa\n");
	printf("6) Kerdes veglegesitese\n");
	printf("7) Kerdes konzultacio\n");
	printf("8) Kilepes \n");
	printf("\n");
	printf("\n");
  
	char str[3];
//MENU kezelés
	while(1)
	{
		scanf("%s", str);
		int tmp = atoi(str);
		switch(tmp) {
			case 1: printf("Kerdessor megadasa\n"); kerdessor_megadasa(); break;
			case 2: printf("Kerdes felvetele\n"); kerdes_felvetele(); break;
			case 3: printf("Kerdes modositasa\n"); kerdes_modositasa(); break;
			case 4: printf("Kerdes torlese\n"); kerdes_torlese(); break;
			case 5: printf("Kerdesek listazasa\n\n"); kerdesek_listazasa(); break;
			case 6: printf("Kerdes veglegesitese\n"); kerdes_veglegesitese(); break;					
			case 7: printf("Konzultacio\n"); break;
			case 8: printf("Kilepes\n"); return 0; break;
			default: printf("Ilyen menupont nincs\n");
		}
	}
	return 0;
}