#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

char* sorEleje(char* str, FILE *f)
{
	char sor[300];
	char* ret;
	while(!feof(f))
	{
		fgets(sor, sizeof(sor), f);
		if ( strncmp(str,sor,strlen(str)) == 0)
		{	
			char *ret = sor;
			return ret;
		}
	}
	return NULL;
}

char* adatok(int azon, FILE *f)
{
	char sor[300];
	int akt;
	while (!feof(f))
	{
		char *r = sor;
		fgets(sor, sizeof(sor), f);
		akt = atoi(sor);
		if (akt == azon)
			return r;
	}
	return NULL;
}

int sorok(int azon, FILE *f)
{
	char sor[300];
	int akt;
	int c = 0;
	while(fgets(sor, sizeof(sor), f))
	{
		akt = atoi(sor);
		if (akt == azon)
			++c;
	}
	return c;
}

void buf()
{
	int x;
	if(!feof(stdin))
		while((x = getchar()) != '\n' && x != EOF);
}

void vendegTorles()
{
	printf("===================\n");
	int vendeg;
	char sor[200];
	fflush(stdin);
	buf();
	FILE *regi, *uj;
	printf("Kerem az azonositot: ");
	scanf("%d",&vendeg);
	regi = fopen("VENDEG.TXT","r");
	if (regi == NULL)
	{
		perror("megnyitas sikertelen\n");
		exit(1);
	}
	remove("VENDEG.TEMP");
	uj = fopen("VENDEG.TEMP","w");
	while(fgets(sor, sizeof(sor), regi))
	{
		if ( atoi(sor) != vendeg )	fprintf(uj,sor);
	}
	fclose(regi);
	fclose(uj);
	remove("VENDEG.TXT");
	rename("VENDEG.TEMP", "VENDEG.TXT");
	regi = fopen("LATOGATASOK.TXT","r");
	if (regi == NULL)
	{
		perror("megnyitas sikertelen\n");
		exit(1);
	}
	uj = fopen("LATOGATASOK.TEMP","w");
	while(fgets(sor, sizeof(sor), regi))
	{
		char *jel = strtok(sor,"\t");
		int rAzon = atoi(jel);
		jel = strtok(NULL, "\t");
		if (jel == NULL) break;
		int aktVendeg = atoi(jel);
		jel = strtok(NULL, "\t");
		char *nap = jel;
		if (aktVendeg != vendeg) fprintf(uj,"%d\t%d\t%s\n", rAzon, aktVendeg, nap);
	}
	fclose(regi);
	fclose(uj);
	remove("LATOGATASOK.TXT");
	rename("LATOGATASOK.TEMP", "LATOGATASOK.TXT");
}

void handler(int signumber)
{
	printf("jelzes: %i\n", signumber);
}

void rendTorles()
{
	printf("====================\n");
	fflush(stdin);
	buf();
	FILE *regi, *uj;
	int rAzon;
	char sor[300];
	printf("kerem az azonositot: ");
	scanf("%d",&rAzon);
	regi = fopen("RENDEZVENY.TXT","r");
	if (regi == NULL)
	{
		perror("megnyitas sikertelen\n");
		exit(1);
	}
	remove("RENDEZVENY.TEMP");
	uj = fopen("RENDEZVENY.TEMP","w");
	while(fgets(sor, sizeof(sor), regi))
	{
		if ( atoi(sor) != rAzon )	fprintf(uj,sor);
	}
	fclose(regi);
	fclose(uj);
	remove("RENDEZVENY.TXT");
	rename("RENDEZVENY.TEMP", "RENDEZVENY.TXT");
	regi = fopen("LATOGATASOK.TXT","r");
	if (regi == NULL)
	{
		perror("megnitas sikertelen\n");
		exit(1);
	}
	uj = fopen("LATOGATASOK.TEMP","w");
	while(fgets(sor, sizeof(sor), regi))
	{
		char *jel = strtok(sor,"\t");
		int akt = atoi(jel);
		jel = strtok(NULL, "\t");
		if (jel == NULL) break;
		int vendeg = atoi(jel);
		jel = strtok(NULL, "\t");
		char *nap = jel;
		if (akt != rAzon) fprintf(uj,"%d\t%d\t%s\n", akt, vendeg, nap);
	}
	fclose(regi);
	fclose(uj);
	remove("LATOGATASOK.TXT");
	rename("LATOGATASOK.TEMP", "LATOGATASOK.TXT");
}

void jelentkezok(int rAzon)
{
	printf("jelentkezettek:\n\n");
	FILE *lat, *vendeg;
	lat = fopen("LATOGATASOK.TXT","r");
	if (lat == NULL)
	{
		perror("megnyitas sikertelen\n");
		exit(1);
	}
	char sor[100];
	while (!feof(lat))
	{
		fgets(sor, sizeof(sor), lat);
		char *jel = strtok(sor, "\t");
		int aktes = atoi(jel);
		jel = strtok(NULL, "\t");
		if (jel == NULL) break;
		int aktv = atoi(jel);
		if ( aktes == rAzon )
		{
			vendeg = fopen("VENDEG.TXT","r");
			if (vendeg == NULL)
			{
				perror("megnyitas sikertelen\n");
				fclose(lat);
				exit(1);
			}
			printf( adatok(aktv, vendeg) );
			fclose(vendeg);
		}
	}
	printf("\n");
	fclose(lat);
}

void strVeg(char *szov)
{
	char *c = szov + strlen(szov) - 1;
	*c = '\0';
}		

int main()
{
	signal(SIGRTMIN, handler);
	int all;
	int cso[2];
	char sz[300];
	char adatok[4][300];
	if (pipe(cso) == -1)
	{
		perror("nincs csitott csovezetek");
    	exit(EXIT_FAILURE);
	}
	pid_t gyerek = fork();
	if (gyerek < 0)
	{
		perror("gyerekfoyamat nem indult\n");
		exit(1);
	}
	if (gyerek > 0)
	{
		printf("sikeres sulofolyamat\n");
		FILE *rend = fopen("RENDEZVENY.TXT", "r");
		if (rend == NULL)
		{
			perror("megnyitas sikertelen");
			exit(1);
		} 
		char sor[300];
		fgets(sor, sizeof(sor), rend);
		char *rendek = strtok(sor, " \t");
		int i=0;
		while(rendek != NULL)
		{
			strcpy(adatok[i], rendek);
			rendek = strtok(NULL, " \t");
			++i;
		}
		printf("helyszin: %s\n", adatok[3]);
		fclose(rend);
		write(cso[1], adatok[3], strlen(adatok[3])-1);
		pause();
		printf("jelzes megerkezett\n");
		write(cso[1], adatok[0], strlen(adatok[0]));
		printf("rendezveny latogatoinak kuldese\n");
		pause();

		int siker;
		read(cso[0], &siker, sizeof(siker));
		printf("siker: %i \%\n", siker);

		waitpid(gyerek, &all, 0);
		printf("gyerekfolyamat vege\n");
		close(cso[1]);
		close(cso[0]);
	}
	else
	{
		printf("gyerek elindult\n");
		read(cso[0], sz, sizeof(adatok[3]-1));
		printf("celhelyszin: %s\n", sz);
		printf("erkezes\n");
		kill(getppid(), SIGRTMIN);
		read(cso[0], sz, sizeof(adatok[0]));
		printf("resztvevok megerkeztek\n");
		int rAzon = atoi(sz);
		jelentkezok(rAzon);
		FILE *lat = fopen("LATOGATASOK.TXT", "r");
		int rvk = sorok(rAzon, lat);
		fclose(lat);
		printf("varhato letszam: %i\n", rvk);
		printf("varakozas\n");
		srand(time(NULL));
		int siker = rand() % 101;
		write(cso[1], &siker, sizeof(siker));
		printf("siker kuldese\n");
		kill(getppid(), SIGRTMIN);

		close(cso[0]);
		close(cso[1]);
	}
	return 0;
}
