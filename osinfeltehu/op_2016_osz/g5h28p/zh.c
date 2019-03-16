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

struct Parameterek
{
    char *nev;
    int feladat;
    int sorszam;
};

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(int argc, char **argv) {

	signal(SIGTERM,handler);
	struct Parameterek par;

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
	        struct Parameterek par_child;
	        len = read(fd[P2_READ], &par_child, sizeof(par_child));
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
	            printf("Rendezveny_Child(%d):  nev: %s; feladat: %d; sorszam: %d\n",pid, par_child.nev, par_child.feladat, par_child.sorszam);
	            // now double it and send it back
	            srand(time(NULL));
	             pid = getpid();
		        rand();
	    		int random_number = rand() % 100 + 1;
	    		printf("Sikerul_e: %d\n", random_number);
	    		if (random_number > 50)
	    		{
	    			printf("%s\n","Sikerul megoldani a feladatot");
		        	write(fd[P2_WRITE], &random_number, sizeof(random_number));
		        }
		        else
		        {
		        	printf("%s\n","Nem sikerul megoldani a feladatot");
		        	write(fd[P2_WRITE], &random_number, sizeof(random_number));
		        }
		        kill(getppid(),SIGTERM);
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
	    printf("%s\n", argv[0]);
	    par.nev=argv[1];
	    srand(time(NULL));
        rand();
		int random_number1 = rand() % 100 + 1;
		par.feladat = random_number1;
		rand();
		int random_number2 = rand() % 100 + 1;
		par.sorszam = random_number2;
	    // send a value to the child
	    if (write(fd[P1_WRITE], &par, sizeof(par)) < 0)
	    {
	        perror("Parent: Failed to send value to child ");
	        exit(EXIT_FAILURE);
	    }

	    // now wait for a response
	    int sikerult_e;
	    len = read(fd[P1_READ], &sikerult_e, sizeof(sikerult_e));
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
	        if (sikerult_e > 50)
	     	   printf("Parent(%d): Received: sikerul megoldania.\n", pid);
	      	else
	       		printf("Parent(%d): Received: nem sikerul megoldania.\n", pid);
	       		
	    }
	    // close down remaining descriptors
	    close(fd[P1_READ]);
	    close(fd[P1_WRITE]);

	    // wait for child termination
	    wait(NULL);

    return EXIT_SUCCESS;
}