#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>  //fork
#include <sys/wait.h>
#include <sys/types.h>

void incrementInt(int* in)
{
   *in -= 1;
}

int main(int argc, char *argv[])
{
	srand(time(NULL)); //the starting value of random number generation
	int r; //number between 0-99
	int status;
	int hanyadik;
	int max = *argv[0];

	pid_t child=fork(); //forks make a copy of variables
	if (child<0){
		perror("The fork calling was not succesful\n"); exit(1);

	}
	else if (child > 0) { // parent
		waitpid(child,&status,0);
		printf("Esely: %d",r);
        if(r<10){
            printf("Bevontattak");
        }
        else{
            printf("Beert");
        }
        printf("The end of parent process\n");
	}
	else { // child

        printf("Hanyadik: ");
		scanf ("%d", &hanyadik);
		printf("Hanyadik: %d", hanyadik);

		incrementInt(&max);

		printf("Lesz meg: %d", max);

        r==rand()%100;


	}


	return 0;
}