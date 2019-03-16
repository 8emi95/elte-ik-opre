#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <signal.h>

typedef struct uzenet 
{ 
     long mtype;
     char mtext [ 1024 ]; 
} uzenet;

int main(int argc, char** argv)
{
	int maxsz = 0;
	int i;
	int j;

	if (argc != 2) { perror("Adjon meg 1 szamot argumentumkent! (max szerelvenyszam)\n"); exit(1); }
	
	maxsz = atoi(argv[1]);
	
	i = 0;
					j = 0;
					
					int pipefd[2]; 
				    pid_t pid;
				    char *array[10]; 
				    char sz[100], buf[100];
					bool b = false; 
					int n, m, uzenetsor, status;
						int num;
						int maxnum;
					key_t kulcs;
					kulcs = ftok("/tmp",'q'); 
					uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 

					if ( uzenetsor < 0 ) 
					{ 
				          perror("msgget"); 
				          exit(1); 
				    } 

					if (pipe(pipefd) == -1)
					{
				        perror("Hiba a pipe nyitaskor!");
				        exit(EXIT_FAILURE);
				    }

				    pid = fork();

					if (pid == -1)
					{
				        perror("Fork hiba");
				        exit(EXIT_FAILURE);
				    }

				    if (pid == 0)     // child process
				    {   	
					    sleep(1);
				        close(pipefd[1]);

								if ( (n = read(pipefd[0], sz, sizeof(sz))) > 0 )
								{
									printf("Gyerek olvasta uzenet: %s",sz);
									printf("\n");
									num = atoi(sz);
								}

								if ( (n = read(pipefd[0], sz, sizeof(sz))) > 0 )
								{
									printf("Gyerek olvasta uzenet: %s",sz);
									printf("\n");
									maxnum = atoi(sz);
								}					

				        close(pipefd[0]);

						printf("Ennyiedik vagyok: %d",num);
						printf("ossz: %d",maxnum);
						printf("Ennyien vannak utanam: %d",maxnum - num);

						int i;
						srand(time(NULL));

						sleep(1);
						int r=(rand()%100)+1;
						printf("%i\n",r);

						if (r <= 10)
						{
							printf("Sikertelen");

							sleep(1);
							int rando=(rand()%2)+1;
							printf("%i\n",rando);
							if(rando == 1)
							{
								const struct uzenet msg = {1, "Bevontattak Elcsusztam a bananom"};
								int sstatus;
								sstatus = msgsnd( uzenetsor, &msg, sizeof(msg.mtext), 0 );
								if ( sstatus < 0 )
								{
									perror("msgsnd"); 
								}
							}
							else
							{
								const struct uzenet msg = {1, "Bevontattak Lefagytam"};
								int sstatus;
								sstatus = msgsnd( uzenetsor, &msg, sizeof(msg.mtext), 0 );
								if ( sstatus < 0 )
								{
									perror("msgsnd"); 
								}
							}
						}
						else
						{
								printf("Sikeres");

								const struct uzenet msg = {1, "Beertem"};
								int sstatus;
								sstatus = msgsnd( uzenetsor, &msg, sizeof(msg.mtext), 0 );
								if ( sstatus < 0 )
								{
									perror("msgsnd"); 
								}
						}

						sleep(1);
						exit(0);
				    }
				    else   // szulo process
				    {    
				        close(pipefd[0]);
						i = 0;
						
						srand(time(NULL));

						sleep(1);
						int rnd=(rand()%maxsz)+1;
						printf("%i\n",rnd);
						char az[100];
						sprintf(az, "%d", rnd);
						
						sleep(1);
						write(pipefd[1], az, sizeof(az));
						printf("Szulo adatot irt a csobe!\n");
						fflush(NULL);

						char azon[100];
						sprintf(azon, "%d", maxsz);

						sleep(1);
						write(pipefd[1], azon, sizeof(azon));
						printf("Szulo adatot irt a csobe!\n");
						fflush(NULL);

						close(pipefd[1]);

						struct uzenet uz; 
						int status;
						
						status = msgrcv(uzenetsor, &uz, 1024, 0, 0 );
						if ( status < 0 )
						{
							perror("msgrcv"); 
						}
						printf( "Mi tortent:  %s\n", uz.mtext );

						wait();
					}

	return 0;
}