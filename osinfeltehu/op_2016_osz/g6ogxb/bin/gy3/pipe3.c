       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h> // for pipe()
       #include <string.h>
	   #include <sys/types.h>
	   #include <signal.h>

	//
	// unnamed pipe example
	//
	
	#include <time.h>

FILE *fp, *rp;

int child_sig_received=0;
int parent_sig_received=0;
int pipe_finished=0;
void handler(int signumber){
  if(signumber==SIGUSR1)
    {
		printf("SIGUSR1\n");
      //printf("Parent Sent to child");
	  child_sig_received=1;
    }
  else if(signumber==SIGTERM)
    {
		printf("SIGTERM\n");
      //printf("Child Sent to parent ");
	  parent_sig_received=1;
    }
	else if(signumber==SIGUSR2)
	{
		printf("SIGUSR2\n");
		pipe_finished=1;
	}
  
}

struct latogato {
		char nev[50];
		char mail[50];
		struct tm signuptime;
		int razon;
};

struct platogato {
	struct latogato csoport[50];
	struct platogato *next;
	int fill; //mennyire van tele a csoport.
} *csoport;
int ncsoport = -1;

struct rendezveny {
	int free;
	int razon;
	int idopont;
	//int - 0tól 4 milliárdig terjed. a föld legnagyobb kerülete ~40000 km, ami 40 millió méter. Két integerrel közel
	//négyzetcentiméter pontosan meg lehet adni egy helyet.
	int helyx; //hosszúsági kör
	int helyy; //szélességi kör
} rend[10];
int nrend=0;
int nrazon = 0;

void create_rendezveny(){
	printf("Rendezveny krealasa;\n");
	int i = nrend;
	nrazon=nrazon+1;
	if(nrend>=10){ i = nrazon%10; printf("rendezveny %d felulirasa", i);}
	
	scanf("%d %d %d",&rend[i].helyx, &rend[i].helyy, &rend[i].idopont);
	rend[i].free=0;
	rend[i].razon=nrazon;
	nrend=i+1;
};
void delete_rendezveny(int i){
	if(i<10 && i>=0)
	{
		rend[i]=rend[nrend];
		rend[nrend].free=1;
		nrend=nrend-1;
	}
};

void list_rendezveny(){
	int i;
	for(i=0;i<10 && i<nrend;i=i+1)
	{
		if(rend[i].free==0)
		{
			printf("Rendezveny %d \n", rend[i].razon);
		}
	}
};

void create_csoport()
{
	struct platogato *next = malloc(sizeof(*next));
	printf("%d next \n" , next);
	(*next).fill=0;
	(*next).next=csoport;
	csoport = next;	
	ncsoport = ncsoport+1;
}

void csoportba_rak(struct latogato l)
{
	if((*csoport).fill==50)
	{
		create_csoport(csoport, &ncsoport);
	}
	(*csoport).csoport[(*csoport).fill]=l;
	(*csoport).fill=(*csoport).fill+1;
	fwrite(&l,sizeof(struct latogato),1,fp);
	printf(" %d. visitor", (ncsoport*50)+(*csoport).fill);
};

void list_latogatok()
{
	struct platogato *pointer = csoport;
	while(pointer!=0)
	{
		int i;
		for(i=0;i<(*pointer).fill;i=i+1)
		{
			printf("%s\n",(*pointer).csoport[i].nev);
		}
		pointer =(*pointer).next;
	}
};

void fileba_latogatok()
{
	fclose(fp);
	fp = fopen("visi.dat", "wb");
	struct platogato *pointer = csoport;
	while(pointer!=0)
	{
		int i;
		for(i=0;i<(*pointer).fill;i=i+1)
		{
			fwrite(&(*pointer).csoport[i],sizeof(struct latogato),1,fp);
			
			//printf("%s\n",(*pointer).csoport[i].nev);
		}
		pointer =(*pointer).next;
	}
	
	fclose(fp);
	fp = fopen("visi.dat", "ab+");
};

void list_csoport_rendezveny(int r)
{
	struct platogato *pointer = csoport;
	while(pointer!=0)
	{
		int i;
		for(i=0;i<(*pointer).fill;i=i+1)
		{
			if((*pointer).csoport[i].razon==r)
			{
				printf("%s\n",(*pointer).csoport[i].nev);
			}
		}
		pointer =(*pointer).next;
	}
};




void pipaba(struct platogato cs)
{
	
};

struct latogato latogato_konstrual()
{
	struct latogato l;
	printf("Get Latogato: Nev(50ch) / Mail(50ch) / Rendezveny(0..10) \n");
	scanf("%s", &l.nev);
	
	scanf("%s", &l.mail);
	do{
	scanf("%d", &l.razon);
	}while(l.razon<0);
	time_t t;
	time(&t);
	l.signuptime = (*localtime( &t));
	
	printf("%d , %d, %d", l.signuptime.tm_sec, l.signuptime.tm_min, l.signuptime.tm_hour);
	return l;
};

void cleanup()
{
	struct platogato *pointer = csoport;
	while(pointer!=0)
	{
		struct platogato *next = (*pointer).next;
		free(pointer);
		pointer = next;
	}
}

int rendezveny_aktualis(int r)
{
	int l = 0;
	int i = 0;
	while(l==0 && i < nrend)
	{
		if(rend[i].razon==r)
		{
			l=1;
		}
		else
		{
			i=i+1;	
		}
	}
	return l;
}

int main(int argc, char *argv[])
{
	int i;
	int pipefd[2]; // unnamed pipe file descriptor array
	pid_t pid;
	char sz[100];  // char array for reading from pipe

	if (pipe(pipefd) == -1) 
	{
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
	}

	create_csoport();
	//ha valamit elrontok
	//rp = fopen("rend.dat", "wb");
	//fclose(rp);
	//akkor kipucolom a rend.datot
	
	rp = fopen("rend.dat", "rb");
	for(i=0;i<10;i=i+1)
	{
		rend[i].free=1;
	}
	i = 0;
	while(fread(&rend[i], sizeof(struct rendezveny),1, rp) && i < 10)
	{
		if(rend[i].free==0)
		{
			nrazon=rend[i].razon;
			printf("Rend %d\n" , nrazon);
			nrend=nrend+1;
			i=i+1;
		}
	}
	//itt olvassuk be az akt rendezvényeket.
	fclose(rp);
	
	signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP),
    signal(SIGUSR1,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
	signal(SIGUSR2,handler);
	
	//rp = fopen("rend.dat", "wb");
	//ebbe fogjuk kiírni.
	
	
	fp = fopen("visi.dat", "ab+");
	//itt beolvassuk az aktuális látogatókat.
	struct latogato l;
	for(i=0;i<10 && fread(&l, sizeof(struct latogato),1, fp);i=i+1)
	{
		if(rendezveny_aktualis(l.razon))
		{
			csoportba_rak(l);
		}
	}
	list_latogatok();
	
	int option = 1;
	while(option>0)
	{
		printf("Rendezveny Konstrualas? (1)\nJelentkezes? (2)\nRendezveny Listazas (3)\nJelentkezo Listazas (4)\nCsoport_kiir (5)\nRendezveny Futtatas (6)\n>");
		scanf("%d", &option);
		
		if(option==1)
		{
			printf("rendezvenykrealas\n");
			list_rendezveny();
			create_rendezveny();
		}
		else if(option==2)
		{
			csoportba_rak(latogato_konstrual());
		}
		else if(option==3)
		{
			list_rendezveny();
		}
		else if(option==4)
		{
			list_latogatok();
		}
		else if(option==5)
		{
			int r;
			printf("Melyik Rendezvenyhez?");
			list_rendezveny();
			scanf("%d",&r);
			list_csoport_rendezveny(r);
		}
		else if(option==6)
		{
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
			int r;
			printf("Melyik Rendezvenyhez?");
			list_rendezveny();
			do{
			scanf("%d",&r);
			if(rend[r].free==1)
			{
				printf("Wrong Rendezveny");
			}
			}while(rend[r].free==1 && nrend>0);
			if(nrend==0)
			{
				continue;
			}
			
			pid = fork();	// creating parent-child processes
			if (pid <0 ) 
			{
			   perror("Fork hiba");
			   exit(EXIT_FAILURE);
			}

			if (pid == 0) 
			{		    	// child process
		
			cleanup(); //takaritsd ki a szobadat!!!
			while(child_sig_received==0)
			{
				sleep(0.1);	// sleeping a few seconds, very necessary
			}
			child_sig_received=0;
			struct rendezveny akt;
			read(pipefd[0],&akt.razon,sizeof(akt.razon));
			read(pipefd[0],&akt.helyx,sizeof(akt.helyx));
			read(pipefd[0],&akt.helyy,sizeof(akt.helyy));
			//printf("Rendezveny [ID:%i] Szervezesre került", akt.razon);
			
			struct platogato pakk;
			sleep(1);
			//visszajelezni parentnek;
			kill(getppid(),SIGTERM);
			
			srand(time(0));
			int count=0;
			//printf("pipee %d %d \n", pipe_finished, child_sig_received);
			pause();
			while(pipe_finished==0)
			{
				while(child_sig_received==0 && pipe_finished==0)
				{
					//sleep(1);	// sleeping a few seconds, very necessary
					//printf("pipee %d %d \n", pipe_finished, child_sig_received);
				
				}
				if(pipe_finished>0)
				{
					printf("END PIPE");
					kill(getppid(),SIGTERM);
					break;
				}
				//printf("Sigusr1received\n");
				sleep(1);
				child_sig_received=0;
				//printf("Sigusr1received\n");
				size_t fark = read(pipefd[0],&pakk,sizeof(struct platogato));
				printf("%d\n", fark);
				int i = 0;
				
				int arrived;
				printf("Sigusr1received %d\n", pakk.fill);
				//processzáljuk a megkapott csoportot.
				for(i=0;i<pakk.fill;i=i+1)
				{
					arrived = rand()%10;
					if(arrived!=9)
					
					{
						count=count+1;
						//printf("name: %s\n",pakk.csoport[i].nev);
						printf("%s megerkezett \n", pakk.csoport[i].nev);
					}
					else printf("%s nem erkezett meg \n", pakk.csoport[i].nev);
				}
				kill(getppid(),SIGTERM);
				sleep(1);
				if(pipe_finished>0)
				{
					printf("END PIPE");
					kill(getppid(),SIGTERM);
					break;
				}
				
				printf("Sigusr1received\n");
				
			}
			
			
			
			
			//takaritas
			   close(pipefd[0]); // finally we close the used read end
			   close(pipefd[1]); // finally we close the used write end
			   exit(EXIT_SUCCESS); //végzett a gyerek, gyerek exit.
			} 
			else 
			{    // szulo process 
			   printf("Szulo indul!\n");
			   
			   write(pipefd[1], &(rend[r].razon), sizeof(rend[r].razon));
			   write(pipefd[1], &(rend[r].helyx), sizeof(rend[r].helyx));
			   write(pipefd[1], &(rend[r].helyy), sizeof(rend[r].helyy));
			   kill(pid,SIGUSR1);
			   sleep(1);
			   while(parent_sig_received==0)
			   {
				   //sleep(1);
			   }
			   parent_sig_received=0;
			   printf("parent sig received");
			   struct platogato pack;
			   pack.next=0;
			   struct platogato *pointer;
			   
			   pointer = csoport;
			   while(pointer!=0)
				{
					int i;
					for(i=0;i<(*pointer).fill;i=i+1)
					{
						if((*pointer).csoport[i].razon==rend[r].razon)
						{
							//printf("%s\n",(*pointer).csoport[i].nev);
							
							if(pack.fill==50)
							{
								write(pipefd[1], &pack, sizeof(struct platogato));
								kill(pid,SIGUSR1);
								sleep(3);
								while(parent_sig_received==0)
								{
									sleep(3);
								}
								parent_sig_received=0;
								sleep(1);
								pack.fill=0;
							}
							pack.csoport[pack.fill]=(*pointer).csoport[i];
							pack.fill=pack.fill+1;
							printf("packing...\n");
						}
					}
					pointer =(*pointer).next;
				}
				
				if(pack.fill>0)
				{
					printf("sending final\n");
					write(pipefd[1], &pack, sizeof(struct platogato));
					kill(pid,SIGUSR1);
					sleep(3);
					while(parent_sig_received==0)
					{
						//sleep(1);
					}
					parent_sig_received=0;
				}
				
				
				kill(pid,SIGUSR2);
				sleep(1);
				while(parent_sig_received==0)
				{
					sleep(1);
				}
				parent_sig_received=0;
			   
			   
			   
			   fflush(NULL); 	// flushes all write buffers (not necessary)
			   //wait();		// waiting for child process (not necessary)
			   // try it without wait()
			
				//rend[r].free=1;
			}
			
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
		}
	}


	
	
	
	//mentes
	list_latogatok();
	
	//fileba_latogatok();
	
	//cleanup
	for(i=0;i<10;i=i+1)
	{
		if(rend[i].free==0);
		{
			//fwrite(&rend[i],sizeof(struct rendezveny), 1, rp);
		}
	}
	
	
	cleanup();
	
	
	fclose(fp);
	fclose(rp);
	close(pipefd[1]); // Closing write descriptor 
	close(pipefd[0]); // Closing write descriptor 
	exit(EXIT_SUCCESS);	// force exit, not necessary
}

 
