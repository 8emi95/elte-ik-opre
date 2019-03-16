#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

// some macros to make the code more understandable
//  regarding which pipe to use to a read/write operation
//
//  Parent: reads from P1_READ, writes on P1_WRITE
//  Child:  reads from P2_READ, writes on P2_WRITE
#define P1_READ     0
#define P2_WRITE    1
#define P2_READ     2
#define P1_WRITE    3

// the total number of pipe *pairs* we need
#define NUM_PIPES   2

// 
#define KERDES_HOSSZA 150
#define VALASZ_HOSSZA 120

#define NUM_QS_FROM_PARENT 3
#define NUM_MAX_ANS_FROM_CHILD_BY_QS 20
#define MAX_NUM_ANS_OF_QS 4

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
	char kerdes[KERDES_HOSSZA];
	fgets(kerdes, KERDES_HOSSZA, stdin);
	fgets(kerdes, KERDES_HOSSZA, stdin);
	printf("\n");
	printf(kerdes);

	printf("Add meg a válaszokat (min 2, max 4) enter leutessel kihagyhatod a valaszt: \n");
	char valasz[VALASZ_HOSSZA];
	
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
			fgets(valasz, VALASZ_HOSSZA, stdin);
			if (enters == 2)
			{
				while(*valasz == '\n')
				{
					fgets(valasz, VALASZ_HOSSZA, stdin);
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
			if (valasz != "\n") {
				fprintf(file, ";");
			}
			else {
				printf("valasz == \n");
			}
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
	char kerdes[KERDES_HOSSZA];
	fgets(kerdes, KERDES_HOSSZA, stdin);
	fgets(kerdes, KERDES_HOSSZA, stdin);
	printf("\n");

	printf("Add meg a válaszokat (min 2, max 4) enter leutessel kihagyhatod a valaszt: \n");
	char valasz[VALASZ_HOSSZA];

	FILE *file;
	file = fopen("kerdessor.txt", "a+");
	if (file) {
		fprintf(file, "%i.) ", kerdes_eleje);
		fprintf(file, "%s\n", kerdes);
		
		int i = 1;
		int enters = 0;
		while(i != 5)
		{
			fgets(valasz, VALASZ_HOSSZA, stdin);
			if (enters == 2)
			{
				while(*valasz == '\n')
				{
					fgets(valasz, VALASZ_HOSSZA, stdin);
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
			if (valasz != "\n") {
				fprintf(file, ";");
			}
			else {
				printf("valasz == \n");
				
			}
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

/*
 * Generates random unsigned integer number
 * between two unsigned integer number
 * returns a random unsigned integer number 
 * from the specified interval given by parameters
 * param: @min minimum of the interval
 * param  @max maximum of the interval
 */
unsigned int rand_interval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}

int kerdesek_szama() {
	int kerdesek_szama;
	FILE *fp;
	fp = fopen("kerdesek_szama.txt", "r+");
	fscanf(fp, "%i", &kerdesek_szama);
	fclose(fp);
	
	return kerdesek_szama;
}

/*
 * This struct is contains the answers
 * for each question what was sent from the parent
 * Max ans per qs is 20
 */
struct Valaszok {
	int val1[NUM_MAX_ANS_FROM_CHILD_BY_QS];
	int val2[NUM_MAX_ANS_FROM_CHILD_BY_QS];
	int val3[NUM_MAX_ANS_FROM_CHILD_BY_QS];
	int val_hossz[3];
};


// kerdeshez tartozo valaszok szama
int valaszok_szama(int kerdes_szama) 
{
	int cntr = 0;
	FILE *fp1;
    char c = 1;
    int kerdes_eleje = kerdes_szama;
    char kerdes_vege = '#';

    fp1 = fopen("kerdessor.txt", "r");

    while (c != EOF)
	{
      c = getc(fp1);
      if (c != kerdes_eleje + '0')
      {
        //printf("Nem kell: %c", c);
      }
      else	//ha megegyezik a kerdes szamaval
      {
					char temp = c;
					c = getc(fp1);
					if(c != '.')
					{
						//printf("valami megint nem kell: %c", c);
					}
					else
					{
						while (c != kerdes_vege)    //megszámolni hány db üres a;;;b;###
						{
                            while(c == '\n') {
                                c = getc(fp1);
                            }
                            char elozo_c = c;
							c = getc(fp1);
                            while(c == '\n') {
                                c = getc(fp1);
                            }
                            if (c == ';' && elozo_c == ';') {
                                ++cntr;
                            }
						}
						c = getc(fp1);
						c = getc(fp1);
					}
			}
    }
    fclose(fp1);
    return 4 - cntr;
}

int konzultacio() {
	int ksz = kerdesek_szama();
	int fd[2*NUM_PIPES];
  int val = 0, len, i;
  pid_t pid;
	struct Valaszok gyerek_valaszai;	//a gyerek ebbe teszi bele az adott kérdéshez válogatott válaszok számait
	int szulo_altal_valasztott_kerdesekhez_tartozo_valaszok_szama[NUM_QS_FROM_PARENT];	//a választott kérdésekhez tartozó válaszok száma db

	int gyerek_kapja[NUM_QS_FROM_PARENT];
	int szulo_adja[NUM_QS_FROM_PARENT];
		
	struct Valaszok szulo_kapja;
	struct Valaszok gyerek_adja;
  // create all the descriptor pairs we need
  for (i=0; i<NUM_PIPES; ++i) {
    if (pipe(fd+(i*2)) < 0) {
      perror("Failed to allocate pipes");
      exit(EXIT_FAILURE);
    }
  }

  // fork() returns 0 for child process, child-pid for parent process.
  if ((pid = fork()) < 0) {
    perror("Failed to fork process");
    return EXIT_FAILURE;
  }

  // if the pid is zero, this is the child process
  if (pid == 0) {
		// Child. Start by closing descriptors we
    //  don't need in this process
    close(fd[P1_READ]);
    close(fd[P1_WRITE]);
    // used for output
    pid = getpid();
		
    // wait for parent to send us a value
    read(fd[P2_READ], &gyerek_kapja, sizeof(gyerek_kapja));
    if (gyerek_kapja < 0) {
      perror("Child: Failed to read data from pipe");
      exit(EXIT_FAILURE);
    }
    else if (gyerek_kapja == 0) {
      // not an error, but certainly unexpected
      fprintf(stderr, "Child: Read EOF from pipe");
    }
    else {
      // report what we received
			printf("Child(%d): Received %d, %d, %d\n", pid, gyerek_kapja[0], gyerek_kapja[1], gyerek_kapja[2]);

      // now send back
	  int rnd_valasz;
	  int rnd_valaszok_szama;
	  int i;
	  int j;
	  for (i = 0; i < 3; ++i) {
		rnd_valaszok_szama = rand_interval(10,20);
		gyerek_adja.val_hossz[i] = rnd_valaszok_szama;
		for (j = 0; j != rnd_valaszok_szama; ++j) {
			rnd_valasz = rand_interval(1, gyerek_kapja[i]);
			if(i == 0) {
				gyerek_adja.val1[j] = rnd_valasz;
			}
			else if (i == 1) {
				gyerek_adja.val2[j] = rnd_valasz;
			}
			else {
				gyerek_adja.val3[j] = rnd_valasz;
			}
		}
	  }

      printf("Child(%d): Sending data to Parent\n %d - %d,\n %d - %d,\n %d - %d, ... back\n", pid, gyerek_adja.val1[0], gyerek_adja.val1[1],
				gyerek_adja.val2[0], gyerek_adja.val2[1], gyerek_adja.val3[0], gyerek_adja.val3[1]);
      if (write(fd[P2_WRITE], &gyerek_adja, sizeof(gyerek_adja)) < 0) {
        perror("Child: Failed to write response value");
        exit(EXIT_FAILURE);
      }
    }

		// finished. close remaining descriptors.
    close(fd[P2_READ]);
    close(fd[P2_WRITE]);

    return EXIT_SUCCESS;
  }

    // Parent. close unneeded descriptors
    close(fd[P2_READ]);
    close(fd[P2_WRITE]);

    // used for output
    pid = getpid();

    // send a value to the child
		int rnd_kerdes1, rnd_kerdes2, rnd_kerdes3;
		rnd_kerdes1 = rand_interval(1, ksz);
		rnd_kerdes2 = rand_interval(1, ksz);
		rnd_kerdes3 = rand_interval(1, ksz);
		
		szulo_adja[0] = valaszok_szama(rnd_kerdes1);
		szulo_adja[1] = valaszok_szama(rnd_kerdes2);
		szulo_adja[2] = valaszok_szama(rnd_kerdes3);

    printf("Parent(%d): Sending %d, %d, %d to child\n", pid, szulo_adja[0], szulo_adja[1], szulo_adja[2]);

    if (write(fd[P1_WRITE], &szulo_adja, sizeof(szulo_adja)) != sizeof(szulo_adja))
    {
        perror("Parent: Failed to send value to child ");
        exit(EXIT_FAILURE);
    }

    // now wait for a response
    read(fd[P1_READ], &szulo_kapja, sizeof(szulo_kapja));
    if (szulo_kapja.val1[0] < 0)
    {
        perror("Parent: failed to read value from pipe");
        exit(EXIT_FAILURE);
    }
    else if (szulo_kapja.val1[0] == 0)
    {
        // not an error, but certainly unexpected
        fprintf(stderr, "Parent(%d): Read EOF from pipe", pid);
    }
    else
    {
        // report what we received
        printf("Parent(%d): Received the data\n", pid);

		// do the printing of the consultation
		printf("\n Konzultacio eredmenye: \n");
		int k, l;
		//for (k = 0; k < 3; ++k) {
			printf("1. Kerdesre: ");
			for(l = 0; l != szulo_kapja.val_hossz[0]; ++l) {
				printf("%d, ", szulo_kapja.val1[l]);
			}
			printf("\n");
			printf("2. Kerdesre: ");
			for(l = 0; l != szulo_kapja.val_hossz[1]; ++l) {
				printf("%d, ", szulo_kapja.val2[l]);
			}
			printf("\n");
			printf("3. Kerdesre: ");
			for(l = 0; l != szulo_kapja.val_hossz[2]; ++l) {
				printf("%d, ", szulo_kapja.val3[l]);
			}
			printf("\n");
		//}
    }

    // close down remaining descriptors
    close(fd[P1_READ]);
    close(fd[P1_WRITE]);

    // wait for child termination
    wait(NULL);

    return EXIT_SUCCESS;
}

int main()
{

	printf("KERDOIV KEZELO \n\n");
	printf("1) Kerdessor megadasa \n");
	printf("2) Kerdes felvetele \n");
	printf("3) Kerdes modositasa \n");
	printf("4) Kerdes torlese\n");
	printf("5) Kerdesek listazasa\n");
	printf("6) Kerdes veglegesitese\n");
	printf("7) Konzultacio\n");
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
			case 7: printf("Konzultacio\n"); int knz = konzultacio(); break;
			case 8: printf("Kilepes\n"); return 0; break;
			default: printf("Ilyen menupont nincs\n");
		}
	}
	return 0;
}