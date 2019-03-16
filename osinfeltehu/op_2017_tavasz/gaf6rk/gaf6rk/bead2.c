#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_SIZE 512
#define REND_FORMAT "%d %s\n"
#define VEND_FORMAT "%s %s %d %d \n"
#define REND_FILE "rendezveny.txt"
#define VEND_FILE "vendeg.txt"

struct Vendeg {
   char  	nev[50];
   char  	email[50];
   int  	rend_id;
   time_t  	j_ido;
};

typedef struct Vendeg VendegS ;

struct Rendezveny {
   char  	nev[50];
   int  	id;
};

typedef struct Rendezveny RendS ;

void beolvas_rend();
void beolvas_vendeg();
void rend_lebonyolitas(RendS* r);

struct Vendeg vendegek[MAX_SIZE];
struct Rendezveny rendezvenyek[MAX_SIZE];

int vendegek_szama = 0;
int rendezvenyek_szama = 0;


char result[1000];


int main(int argc, char *argv[])
{
	beolvas_rend();
	beolvas_vendeg();

	for (int i = 0; i < rendezvenyek_szama; i++)
	{
		rend_lebonyolitas(&rendezvenyek[i]);
	}

	return 0;
}


void rend_lebonyolitas(RendS* r)
{
	int pipe_file[2];
	int num = 0;

	int Xpipe = pipe(pipe_file);
	if (Xpipe < 0)
	{
		perror("pipe(pipe_file) error!");
		exit(-1);
	}

	pid_t child = fork();
	if (child < 0) 
	{
		perror ("fork() error!");
		exit(-1);
	}
	
	/* parent process */
	if (0 != child)	
	{
		char buffer[MAX_SIZE];

		printf("%s\n", "1 küldöm az esemenyt");
		write(pipe_file[1], r->nev, MAX_SIZE);

		read(pipe_file[0], buffer, MAX_SIZE);
		printf("%s: %s\n", "1 Megkaptam, megérkezett", buffer);

		printf("%s\n", "1 kuldom vendegek szamat");
		//int num = 0;
		for (int i = 0; i < vendegek_szama; i++)
		{
			if(vendegek[i].rend_id == r->id) {
				++num;
			}
		}
		printf("%d\n", num);
		write(pipe_file[1], &num, sizeof(num));

		printf("%s\n", "1 kuldom vendegeket");
		for (int i = 0; i < vendegek_szama; i++)
		{
			if(vendegek[i].rend_id == r->id) {
				write(pipe_file[1], &vendegek[i], sizeof(VendegS));
			}
		}

		int status = 0;
		//waitpid(child,&status,0); 
	}

	/* child process */
	else
	{
		char buffer[MAX_SIZE];

		read(pipe_file[0], buffer, MAX_SIZE);
		printf("%s\n", "2 megkaptam esemenyt");

		char* ok = "Megerkeztem.";
		printf("%s\n", "2 küldöm ok");
		write(pipe_file[1], ok, strlen(ok) + 1);

		//int v_szama;
		read(pipe_file[0], &num, sizeof(num));
		printf("%s: %d\n", "2 olvastam v szamat", num);


		struct Vendeg reszveveok[num];
		for(int i = 0; i < num && i < 100; i++)
		{
			struct Vendeg v;
			read(pipe_file[0], &v, sizeof(VendegS));
			printf("%s: %s\n", "2 olvastam reszvevot", v.nev);

		}

		exit(0);

	}
} 

void beolvas_rend()
{
	FILE *f;
	f = fopen(REND_FILE,"r");
	char line[MAX_SIZE];

	while(fgets(line,sizeof(line),f) != NULL)
	{
		struct Rendezveny r;

		sscanf(line, REND_FORMAT, &r.id, &r.nev);

		rendezvenyek[rendezvenyek_szama++] = r;
	}

	fclose(f);
}

void beolvas_vendeg()
{
	FILE *f;
	f = fopen(VEND_FILE,"r");
	char line[MAX_SIZE];

	while(fgets(line,sizeof(line),f) != NULL)
	{
		struct Vendeg v;

		sscanf(line, VEND_FORMAT, &v.nev, &v.email, &v.rend_id, &v.j_ido);

		vendegek[vendegek_szama++] = v;
	}

	fclose(f);
}


















