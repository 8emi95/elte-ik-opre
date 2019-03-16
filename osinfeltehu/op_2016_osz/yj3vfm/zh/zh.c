       #include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
	   #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	   #include <signal.h>
	//
	// unnamed pipe example
	//
	void handler(int signumber){
  
	}

       int main(int argc, char *argv[])
       {
		   
		   
		   key_t kulcs;
			int oszt_mem_id;
			char *s;
			// a parancs nevevel es az 1 verzio szammal kulcs generalas
			kulcs=ftok(argv[0],1);
			// osztott memoria letrehozasa, irasra olvasasra, 500 bajt mrettel
			oszt_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);  
			s = shmat(oszt_mem_id,NULL,0);
		   
		   
		   
		   int bukottak=0;
		   char szeret[100][100];
		   char buko[100][100];
		   //printf("Count:%d\n",argc);
		   signal(SIGUSR1,handler);
		    char siker[100];
		   int pipefd[2]; // unnamed pipe file descriptor array
           pid_t pid;
           char sz[100];  // char array for reading from pipe
			char sz2[100];
           if (pipe(pipefd) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
		   int i;
		   for(i=1;i<argc;i++){
           pid = fork();	// creating parent-child processes
           if (pid == -1) 
	   {
               perror("Fork hiba");
               exit(EXIT_FAILURE);
           }

           if (pid == 0) 
	   {		    	// child process
	       sleep(1);	// sleeping a few seconds, not necessary
		  
		   srand ( getpid());
               //close(pipefd[1]);  //Usually we close the unused write end
               read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
			   read(pipefd[0],&i,sizeof(i));
			   read(pipefd[0],sz2,sizeof(sz2));
               printf("Gyerek:Jatekos:%s,Feladat:%s,Azon:%d\n",sz,sz2,i);
			   int r = rand()%100+1;
			   if(r<50){
				   strcpy(siker,"Sikerult\n");
			   }else{
				   strcpy(siker,"Nem Sikerult\n"); 
			   }
			   printf("Gyerek:%s",siker);
			   write(pipefd[1], siker,sizeof(siker));
			   strcat(sz,":Elso");
			   //strcpy(o[i],"Jatekos:);
				// elengedjuk az osztott memoriat
				strcat(s,sz);
			   kill(getppid(),SIGUSR1);
			   
               //close(pipefd[0]); // finally we close the used read end
			   exit(1);
           } 
           else 
           {    // szulo process 
				stpcpy(sz,argv[i]);
               //close(pipefd[0]); //Usually we close unused read end
               write(pipefd[1], sz,sizeof(sz));
			   write(pipefd[1], &i,sizeof(i));
			   strcpy(sz2,"Feladat1");
			   write(pipefd[1], sz2,sizeof(sz2));

               //close(pipefd[1]); // Closing write descriptor 
			   pause();
			   read(pipefd[0],siker,sizeof(siker));
               printf("Szulo:%s\n",siker);
			   if(strcmp(siker,"Nem Sikerult\n")==0){
				   strcpy(buko[bukottak],sz);
				   bukottak++;
			   }
			   strcat(szeret[i],s);
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait();		// waiting for child process (not necessary)
				// try it without wait()
		   }
	   }
	   //printf("%d\n",bukottak);
	   int vege=bukottak;
	   int j;
	   for(j=0;j<bukottak;j++){
		   
		   
		   pid = fork();	// creating parent-child processes
           if (pid == -1) 
	   {
               perror("Fork hiba");
               exit(EXIT_FAILURE);
           }

           if (pid == 0) 
	   {		    	// child process
	       sleep(1);	// sleeping a few seconds, not necessary
		  
		   srand ( getpid());
               //close(pipefd[1]);  //Usually we close the unused write end
			   read(pipefd[0],sz,sizeof(sz));
			   read(pipefd[0],sz2,sizeof(sz2));
               printf("Gyerek:Jatekos:%s,Potfeladat:%s\n",sz,sz2);
			   int r = rand()%100;
			   if(r<80){
				   strcpy(siker,"Sikerult\n");
			   }else{
				   strcpy(siker,"Nem Sikerult\n"); 
			   }
			   printf("Gyerek:Pot:%s\n",siker);
			   write(pipefd[1], siker,sizeof(siker));
			   kill(getppid(),SIGUSR1);
			   exit(1);
           } 
           else 
           {    // szulo process 
			   strcpy(sz2,"Feladat1");
			   write(pipefd[1], buko[j],sizeof(buko[j]));
			   write(pipefd[1], sz2,sizeof(sz2));
			   pause();
			   read(pipefd[0],siker,sizeof(siker));
			   if(strcmp(siker,"Sikerult\n")==0){
				   strcpy(buko[bukottak],sz);
				   vege--;
			   }
               fflush(NULL); 	// flushes all write buffers (not necessary)
               wait(NULL);		// waiting for child process (not necessary)
				// try it without wait()
		   }
	   }
	   printf("Sikerult:%d,Bukottak:%d\n",argc-1-vege,vege);
	   
	   
	   exit(EXIT_SUCCESS);	// force exit, not necessary
       }

 
