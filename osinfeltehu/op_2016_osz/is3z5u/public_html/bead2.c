#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define InputFile "backup.txt"


int main(int argc, char *argv[])
{
/*
    printf(argv[0]);
  	printf("\n");
    printf(argv[1]);
    printf("\n");
*/    
	
	int MyPipeFiel[2];
	char buffer[300];

	int MyPipe = pipe(MyPipeFiel);
	if (MyPipe < 0)
	{
		perror("pipe(MyPipeFiel) error!");
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
		/* printf("main send --> %d=getpid() -- %d=child\n", getpid(), child); */
		
		write(MyPipeFiel[1], argv[1], strlen(argv[1]) + 1);

		wait(NULL);

		read(MyPipeFiel[0], buffer, 300);
		printf("%s", buffer);
	}
	/* child process */
	else
	{
		/* printf("child send -> %d=getpid() -- %d=getppid()\n", getpid(), getppid()); */
	
		read(MyPipeFiel[0], buffer, strlen(argv[1]) + 1);
		printf("A gyermekfolyamat megkapta a kovetkezo rendezveny azonositojat:\n%s\n\n", buffer);
		
		/*****************************************************/
		FILE * fPointer;
    	fPointer = fopen(InputFile, "r");
		
    	int j = 0;
    	char name[300];
		char mail[300];
		char rendezveny_azon[100];
    	time_t time;

    	int Sum = 0;
		int NemJottekSzama = 0;

		char x[1024];
    	while (fscanf(fPointer, " %1023s", x) == 1) {
			switch(j%4){
            case 0:
                memcpy(name, x, strlen(x)+1);
                j++;
                break;
            case 1:
                memcpy(mail, x, strlen(x)+1);
                j++;
                break;
            case 2:
                memcpy(rendezveny_azon, x, strlen(x)+1);
                j++;
                break;
            case 3:
                time = atoi(x);
				if (!strcmp(buffer, rendezveny_azon))
				{
					/* printf("\nname: %s, r_azon: %s", name, rendezveny_azon); */
					Sum = Sum + 1;
					if ( (rand() % 100) < 10 )
					{
						NemJottekSzama = NemJottekSzama + 1;
					}
				}
                j++;  
                break;
        	}
    	}
		char str[20];
		char result[1000];
		strcpy(result, "Rendezveny sikeressege: ");
		sprintf(str, "%d", rand()%4+1);
    	strcat(result, str);
		strcat(result, "\nA ");
		sprintf(str, "%d", Sum);
		strcat(result, str);
		strcat(result, " fobol ");
		sprintf(str, "%d", NemJottekSzama);
		strcat(result, str);
		strcat(result, " nem jelent meg.\n");

		write(MyPipeFiel[1], result, strlen(result) + 1);
		/*****************************************************/
	}


	return 0;
}























