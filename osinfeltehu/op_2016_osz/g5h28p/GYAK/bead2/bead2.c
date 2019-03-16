#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h> 
#include <sys/wait.h>

//TO DO
//1. Elküldeni a rendazon es jelentkezokszamat
//2. Gyerek visszajelez hogy fogadta
//3. Elküldeni a jelentkezoket
//4. Kiirni ezeket
//5. Külön kiirni ki nem jött meg
//6. uj rendezveny

#define P1_READ     0
#define P2_WRITE    1
#define P2_READ     2
#define P1_WRITE    3

// the total number of pipe *pairs* we need
#define NUM_PIPES   2

struct Rendezveny
{
    char azon[25];
    char nev[50];
    int jelentkezokszama;
};

struct Latogato
{
    char vezeteknev[25];
    char keresztnev[25];
    char email[50];
    char rendezvenyAzon[25];
    int datum;
};

typedef struct 
{
    char vezeteknev[25];
    char keresztnev[25];
    char email[50];
} latogatok;

void insert(latogatok *p, char vezeteknev[25], char keresztnev[25],char email[50])
{
    strcpy(p->vezeteknev, vezeteknev);
    strcpy(p->keresztnev, keresztnev);
    strcpy(p->email, email);
}


int main() {

	struct Rendezveny rendezveny;

	FILE *fileptr1, *fileptr2;
    fileptr1 = fopen("rendezveny.db", "r");

    while(fscanf(fileptr1,"%s %s %d", rendezveny.azon, rendezveny.nev, &rendezveny.jelentkezokszama) != EOF)
     {

	    printf("Rendezveny: %s %s %d\n", rendezveny.azon, rendezveny.nev, rendezveny.jelentkezokszama);

	    int fd[2*NUM_PIPES];
	    int len;
	    pid_t pid;

	    // create all the descriptor pairs we need
	    for (int i=0; i<NUM_PIPES; ++i)
	    {
	        if (pipe(fd+(i*2)) < 0)
	        {
	            perror("Failed to allocate pipes");
	            exit(EXIT_FAILURE);
	        }
	    }

	    // fork() returns 0 for child process, child-pid for parent process.
	    if ((pid = fork()) < 0)
	    {
	        perror("Failed to fork process");
	        return EXIT_FAILURE;
	    }

	    // if the pid is zero, this is the child process
	    if (pid == 0)
	    {
	        // Child. Start by closing descriptors we
	        //  don't need in this process
	        close(fd[P1_READ]);
	        close(fd[P1_WRITE]);

	        // used for output
	        pid = getpid();

	        // wait for parent to send us a value
	        struct Rendezveny rendezveny_child;
	        len = read(fd[P2_READ], &rendezveny_child, sizeof(rendezveny_child));
	        if (len < 0)
	        {
	            perror("Child: Failed to read data from pipe");
	            exit(EXIT_FAILURE);
	        }
	        else if (len == 0)
	        {
	            // not an error, but certainly unexpected
	            fprintf(stderr, "Child: Read EOF from pipe");
	        }
	        else
	        {
	            // report what we received
	            printf("Rendezveny_Child: %s %s %d\n", rendezveny_child.azon, rendezveny_child.nev, rendezveny_child.jelentkezokszama);
	            // now double it and send it back
	            if (write(fd[P2_WRITE], "Megkaptam\n", sizeof("Megkaptam\n")) < 0)
	            {
	                perror("Child: Failed to write response value");
	                exit(EXIT_FAILURE);
	            }
	        }

	        // finished. close remaining descriptors.

			latogatok lat[rendezveny_child.jelentkezokszama];
			srand(time(NULL));
			
			sleep(1);
	        printf("\nA %s szamu rendezvenyre %d vendeg hivatalos:\n", rendezveny_child.azon,  rendezveny_child.jelentkezokszama);

	        for (int i = 0; i < rendezveny_child.jelentkezokszama; i++)
	        {
		        struct Latogato latogato_child;
		        len = read(fd[P2_READ], &latogato_child, sizeof(latogato_child));
		        if (len < 0)
		        {
		            perror("Child: Failed to read data from pipe");
		            exit(EXIT_FAILURE);
		        }
		        else if (len == 0)
		        {
		            // not an error, but certainly unexpected
		            fprintf(stderr, "Child: Read EOF from pipe");
		            exit(1);
		        }
		        else
		        {
		            // report what we received
		            printf("\t  %s %s %s %s\n", latogato_child.rendezvenyAzon, latogato_child.vezeteknev, latogato_child.keresztnev, latogato_child.email);
		        	insert(&lat[i], latogato_child.vezeteknev, latogato_child.keresztnev, latogato_child.email);
		        }
		    }

		    printf("\nNem megjelentek atnezese!\n");
	        for (int i = 0; i < rendezveny_child.jelentkezokszama; i++)
	    	{
	    		rand();
	    		int random_number = rand() % 100 + 1;
	    		if (random_number > 50)
	    		{
	    			if (write(fd[P2_WRITE], &lat[i], sizeof(lat[i])) < 0)
		            {
		                perror("Child: Failed to write response value");
		                exit(EXIT_FAILURE);
		            }
	    		}
	    	}

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
	    if (write(fd[P1_WRITE], &rendezveny, sizeof(rendezveny)) < 0)
	    {
	        perror("Parent: Failed to send value to child ");
	        exit(EXIT_FAILURE);
	    }

	    // now wait for a response
	    char buff[255];
	    len = read(fd[P1_READ], buff, sizeof(buff));
	    if (len < 0)
	    {
	        perror("Parent: failed to read value from pipe");
	        exit(EXIT_FAILURE);
	    }
	    else if (len == 0)
	    {
	        // not an error, but certainly unexpected
	        fprintf(stderr, "Parent(%d): Read EOF from pipe", pid);
	    }
	    else
	    {
	        // report what we received
	        printf("Parent(%d): Received: %s\n", pid, buff);
	        fileptr2 = fopen("latogato.db", "r");
	        struct Latogato latogato;
	        while(!feof(fileptr2))
	        {
	        	fscanf(fileptr2,"%s %s %s %s %d", latogato.rendezvenyAzon, latogato.vezeteknev, latogato.keresztnev, latogato.email, &latogato.datum);
	        	if (strcmp(latogato.rendezvenyAzon, rendezveny.azon) == 0)
	        	{
	        		if (write(fd[P1_WRITE], &latogato, sizeof(latogato)) < 0)
				    {
				        perror("Parent: Failed to send value to child ");
				        exit(EXIT_FAILURE);
				    }
				    //printf("%s %s %s %s\n", latogato.rendezvenyAzon, latogato.vezeteknev, latogato.keresztnev, latogato.email);
	        	}
	        }
	    }

	    latogatok NemMegjelent;

	    while(len > 0)
	    {
		    len = read(fd[P1_READ], &NemMegjelent, sizeof(NemMegjelent));
		    if (len > 1)
		    {
		    	printf("\t %s %s %s\n",NemMegjelent.vezeteknev,NemMegjelent.keresztnev,NemMegjelent.email);
			}
		}
	    // close down remaining descriptors
	    close(fd[P1_READ]);
	    close(fd[P1_WRITE]);

	    // wait for child termination
	    wait(NULL);
	}
    return EXIT_SUCCESS;
}