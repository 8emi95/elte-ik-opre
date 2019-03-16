#include "all.h"

int status;
pid_t child1, child2;

void handler(int signumber){
  // printf("[ STATION ]: Signal with number %i has arrived\n",signumber);
  
  char cim[100];
  int receive_address=open("vissza", O_RDONLY);
  read(receive_address, cim, sizeof(cim));
  close(receive_address);
  printf("\nAllomas: Megkaptam a cimet: %s\n", cim);
  
  sleep(1);
  
  printf("\nAllomas: Elkuldom a cimet a kocsinak\n");
  int tel=child1;
  int send_address=open("oda", O_WRONLY);
  write(send_address, cim, sizeof(cim));
  write(send_address, &tel, sizeof(int));
  close(send_address);
  
  printf("\nAllomas: Elkuldtem a cimet a kocsinak\n");
  
  // printf("\nAz utas bejelentkezett\n\n");
}

void doJob(char * address, int n)
{
	srand(time(NULL));
	// szulotol -> gyereknek
	int f = mkfifo("oda", 0744);
    if (f < 0)
    {
        perror("Error a oda_cso letrehozasanal!");
        exit(1);
    }

    // gyerektol -> szulonek
    int g = mkfifo("vissza", 0744);
    if (g < 0)
    {
        perror("Error a vissza_cso letrehozasanal!");
        exit(1);
    }
	
	/*General variables and data structures*/
    // int rv;
    
    /* create signal */
    signal(SIGUSR1,handler);

    int status;
	// int pipefd[2]; // unnamed pipe file descriptor array
    // char buffer[5][255];  // char array for reading from pipe
    // int  pidIDs[3]; //0: parent 1:Child1 2:Child2
	
	
	// if (pipe(pipefd) == -1) 
    // {
	   // perror("Hiba a pipe nyitaskor!");
	   // exit(EXIT_FAILURE);
    // }
	
    /* creating parent-child processes */
    child1 = fork();

    if (child1 == -1) {perror("Error: Fork -- CLIENT\n");exit(EXIT_FAILURE);}
    /* parent process */
    if (0 != child1)    
    {   
        //printf("[ STATION ]: This is the parent process!\n");
        //printf("[ STATION ]: I am the parent my pid is %d\n", getpid());
        
        child2 = fork();
        if (child2 == -1) {perror("Error: Fork -- CAR\n");exit(EXIT_FAILURE);}
        if(child2 == 0) // car
		{
            //printf("[ CAR ]: I am the SECOND child. My PID: %d, and my parent's PID: %d\n", getpid(), getppid());
            
            /*Here DO what CAR needs to do!! */
			// char cim[100];
			char * cim;
			int tel;
			
			int receive_address=open("oda", O_RDONLY);
			read(receive_address, cim, sizeof(cim)); 
			printf("Kocsi: megkaptam a cimet: %s", cim);
            read(receive_address, &tel, sizeof(int)); 
            close(receive_address);
			
			printf("es a szamot: %d\n", tel);
			
            printf("[ CAR ]: FINISHED\n");
        }

        /*Here DO what PARENT needs to do!! */
      
        wait(&status);
        /*  OR
            waitpid(child1, &status, 0);
            waitpid(child2, &status, 0);
        */
	    printf("[ STATION ]: Parent has finished!\n");	
    }
    /* child process */
    else if( child1 == 0){  // taxi client
		printf("[ CLIENT ]: I am the FIRST child. My PID: %d, and my parnet's PID: %d\n", getpid(), getppid());
           
		/*Here DO what CLIENT needs to do!! */
		
		// jelzem az, hogy irtam a csobe
		kill(getppid(), SIGUSR1);
		// beirom a csobe az uzenetet	
		char s[100];
		strcpy(s, address);
		int send_address= open("vissza", O_WRONLY);
        write(send_address, s, sizeof(s));
		close(send_address);
        printf("Utas: Elkuldtem a cimem...\n");
        close(send_address);

		printf("[ CLIENT ]: FINISHED\n");
    }
	unlink("oda"); unlink("vissza");
}

int main(int argc, char ** argv)
{
	if(argc < 2)
	{
		printf("\n\nAdjon meg egy cimet!\n\n");
		exit(1);
	}
	else
	{
		char * cim;
		int i;
		for(i = 1; i < argc; ++i)
		{
			strcat(cim, argv[i]);
			strcat(cim, " ");
		}
		doJob(cim, 2);
	}
	
	return 0;
	
}
