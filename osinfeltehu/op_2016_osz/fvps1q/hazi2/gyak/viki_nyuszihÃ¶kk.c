#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 
#include <mqueue.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/stat.h> 

void handler(int signumber)
{
  signal(SIGTERM,SIG_DFL);
}

int main () { 
     
	 const char * fajlnev = "madar.txt"; 	
	 
	 FILE * f;
	 f = fopen(fajlnev,"a+");
	 int a = 0;
	 char t[1000];
	 char madar[1000];
	
	
	
	int cso_esemeny_hely[2]; 
	int cso_esemeny_hely2[2]; 
    char cso_madar[100];
	char cso_id[100];
	
	pid_t child; 
	

	signal(SIGTERM,handler);
	 
	 pid_t gyerek;
	 
	 pipe(cso_esemeny_hely);
	 pipe(cso_esemeny_hely2);
	 
     gyerek= fork(); 
     
	 if ( gyerek > 0 ) { 
	 
			pause();
		
			close(cso_esemeny_hely[1]);
			read(cso_esemeny_hely[0],cso_madar,sizeof(cso_madar));
			close(cso_esemeny_hely[0]);

			close(cso_esemeny_hely2[1]);
			read(cso_esemeny_hely2[0],cso_id,sizeof(cso_id));
			close(cso_esemeny_hely2[0]);

			printf("Szülő: A választott madár:%s", cso_madar);
			printf("Szülő: A választó id-ja:%i", cso_id);

		int status;
		wait(&status);
		
		
     } 
	 else if 
	 ( gyerek == 0 ) { 
	 int i=1; int z=0; 
	
			 while(fgets(t,sizeof(char)*1000,f) && z==0)
			{
				if (i % 2==1) {
				printf("Eljutok ide1\n");
				printf("%s\n",t);
				a++;
				srand(time(NULL)+a);
				int r2 = rand() % 2;
					if(r2 == 1)
					{
						strcpy(madar,t);
						z++;
					}
			} i++;
			}
			fclose(f);
			if (z==0)
				{
				fgets(t,sizeof(char)*1000,f);
				strcpy(madar,t);
				}		
	
			
			close(cso_esemeny_hely[0]);
			write(cso_esemeny_hely[1],madar,strlen(madar));
			close(cso_esemeny_hely[1]);
			
			
			
			int b=getpid();
			printf("%i", b);
			
			char pid[100];
		
			sprintf(pid,"%i",b);
			printf("%s", pid);
			
			close(cso_esemeny_hely2[0]);
			write(cso_esemeny_hely2[1],pid,strlen(pid));
			close(cso_esemeny_hely2[1]);
		
			
			kill(getppid(),SIGTERM);
			
	 }	
	else { 
          perror("fork"); 
          return 1; 
     }
	 
	 
     return 0; 
} 
