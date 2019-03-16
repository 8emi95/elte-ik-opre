//Read and print out the file given in the parameter
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h> //O_RDONLY,
#include <unistd.h> //close
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
 
struct Clients
{
    char name[50];
    char adress[100];
    char email[30];
    char telNum[20];
    int priority;
    time_t t;

};
 
struct Clients clients[100];
struct Clients sortedClients[100]; 
int clientNumber = 0;
struct tm * ti;

int pipe1[2];
int pipe2[2];
 
 int r;

void loadData()
{
    FILE * f;

    f=fopen("input.txt","rb");
    if (f!=NULL)
    {
        clientNumber = fread (clients, sizeof(struct Clients),100,f);
        printf("clientNumber: %d \n", clientNumber);
        fclose(f);
    }  
}

void saveData()
{
    FILE * f;

    f=fopen("input.txt","wb");
    if (f==NULL){perror("File opening error\n"); exit(1);}

    fwrite (clients, sizeof(struct Clients),clientNumber,f);
    fclose(f);
}

void listClients()
{   
    
    int i;
    for(i = 0; i < clientNumber; i++){
        ti = localtime ( &clients[i].t );
        printf("-------------------------------------------------------------------------------\n");
        printf("|%s|%s|%s|%s|%d| Syst: %d %d %d %d:%d:%d \n",clients[i].name,clients[i].adress,clients[i].email,clients[i].telNum,clients[i].priority,ti->tm_mday, ti->tm_mon + 1, ti->tm_year + 1900,ti->tm_hour, ti->tm_min, ti->tm_sec);
        printf("-------------------------------------------------------------------------------\n");
    }
}


char* minEnt(char* str)
{
    int wordLength;
    wordLength=strlen(str)-1;
    if(str[wordLength]=='\n')
        str[wordLength] = '\0';
    return str;
}




void addClient()
{
    int ret;
    char *ptr;    
    char  word[100];
    
 
   
    // ide kell majd még egy ellenőrzés hogy nincs e maxona  tömb
    
    printf("Kerem adja meg a nevet:");
    fgets(word, 50, stdin);
    strncpy(clients[clientNumber].name,minEnt(word),50);

    printf("Kerem adja meg a cimet:");
    fgets(word, 100, stdin);
    strncpy(clients[clientNumber].adress,minEnt(word),100);

    printf("Kerem adja meg az emailcimet:");
    fgets(word, 30, stdin);
    strncpy(clients[clientNumber].email,minEnt(word),30);

    printf("Kerem adja meg a telefonszamat:");
    fgets(word, 20, stdin);
    strncpy(clients[clientNumber].telNum,minEnt(word),20);

    printf("Kerem adja meg hogy ker e priroitasi szart, 0 a nem 1 az igen:");
    // ellenőrzés h egyes vagy 0
    fgets(word, 10, stdin);

    ret =strtol(minEnt(word),&ptr,10);
    clients[clientNumber].priority=ret;

    clients[clientNumber].t =time(NULL);
 // ti= localtime ( &clients[clientNumber].t);

    clientNumber++;
}
 
 int searchByName()
 {
   int i;
   char name[50];
   int wordLength;

   printf("Kerem Adjon meg egy nevet ami alapjan keresne!\n");
  fgets(name, 50, stdin);
    wordLength=strlen(name)-1;
    if(name[wordLength]=='\n')
        name[wordLength] = '\0';

  

   for(i=0;i<clientNumber;i++)
   {
     if(!strcmp(clients[i].name, name))
     {   ti = localtime ( &clients[i].t );
      printf("|%s|%s|%s|%s|%d| Syst: %d %d %d %d:%d:%d \n",clients[i].name,clients[i].adress,clients[i].email,clients[i].telNum,clients[i].priority,ti->tm_mday, ti->tm_mon + 1, ti->tm_year + 1900,ti->tm_hour, ti->tm_min, ti->tm_sec);return i;}
     
   }
   printf("Nincs ilyen nevu kliens. \n");
   return -1;
 }
 



void deleteClient(int n)
{
  
 
    if((-1 < n) && (n < clientNumber-1))
    {
        memcpy(&clients[n], &clients[n+1], sizeof(struct Clients) * (clientNumber - n - 1));
        clientNumber--;
    }
    else if(n==clientNumber-1)
    {
        clientNumber--;
    }
    
  
}





void searchByPriority()
{
   char *ptr;
   int ret;
   char input[30];
   int i;
   int k=0;
   printf("Kerem adja meg hogy milyen prioritasu bejelenteseket keres, 1es a surgos, 0as a nem surgos.\n");
   fgets(input, 50, stdin);

        ret =strtol(minEnt(input),&ptr,20);

   if(ret==0)
   {
    for (i=0;i<clientNumber;i++)
     {
       if(clients[i].priority==0)
       {
         k=1;  
         ti = localtime ( &clients[i].t );
         printf("-------------------------------------------------------------------------------\n");
         printf("|%s|%s|%s|%s|%d| Syst: %d %d %d %d:%d:%d \n",clients[i].name,clients[i].adress,clients[i].email,clients[i].telNum,clients[i].priority,ti->tm_mday, ti->tm_mon + 1, ti->tm_year + 1900,ti->tm_hour,  ti->tm_min, ti->tm_sec);
         printf("-------------------------------------------------------------------------------\n");
       }
     }
     if(!k)
     {
         printf("Nincsen ilyen prioritasu bejelentes.");
     }
     
   }
   else
   {
     for (i=0;i<clientNumber;i++)
     {
       if(clients[i].priority==1)
       {  
           k=1;  
          ti = localtime ( &clients[i].t );  
         printf("-------------------------------------------------------------------------------\n"); 
         printf("|%s|%s|%s|%s|%d| Syst: %d %d %d %d:%d:%d \n",clients[i].name,clients[i].adress,clients[i].email,clients[i].telNum,clients[i].priority,ti->tm_mday, ti->tm_mon + 1, ti->tm_year + 1900,ti->tm_hour, ti->tm_min, ti->tm_sec);
         printf("-------------------------------------------------------------------------------\n");
       }
     } 
      if(!k)
     {
         printf("Nincsen ilyen prioritasu bejelentes.");
     }
     
   }
}

void editClient(n)
{
   char  word[100];
    int number;
    int ret;
    char *ptr;    
    
    if((-1 < n) && (n<clientNumber))
    {
    // ide kell majd még egy ellenőrzés hogy nincs e maxona  tömb
    
        printf("Kerem adja meg a nevet:");
        fgets(word, 50, stdin);
        strncpy(clients[n].name,minEnt(word),50);

        printf("Kerem adja meg a cimet:");
        fgets(word, 100, stdin);
        strncpy(clients[n].adress,minEnt(word),100);

        printf("Kerem adja meg az emailcimet:");
        fgets(word, 30, stdin);
        strncpy(clients[n].email,minEnt(word),30);

        printf("Kerem adja meg a telefonszamat:");
        fgets(word, 20, stdin);
        strncpy(clients[n].telNum,minEnt(word),20);

        printf("Kerem adja meg hogy ker e priroitasi szart, 0 a nem 1 az igen:");
        // ellenőrzés h egyes vagy 0
        fgets(word, 10, stdin);

        ret =strtol(minEnt(word),&ptr,10);
        clients[n].priority=ret;

        clients[n].t =time(NULL);
   
    }
}

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

void sortedList()
{
	int k= 0;
	int i;
	for(i=0;i<clientNumber;i++)
	{
		if(clients[i].priority==1)
		{
			sortedClients[k]=clients[i];
			k++;
		}
	}
	for(i=0;i<clientNumber;i++)
	{
		if(clients[i].priority==0)
		{
			sortedClients[k]=clients[i];
			k++;
		}
	}
}

void createPipe(){
    printf("piepokat hozok letre \n");
    if (pipe(pipe1) == -1)
    {
        perror("Error while opening pipe!");
        exit(EXIT_FAILURE);
    }
    
    if (pipe(pipe2) == -1)
    {
        perror("Error while opening pipe!");
        exit(EXIT_FAILURE);
    }
}


void masodikbeadando()
{	
    int status;
	printf("elinsul a 2. beadando\n");
	 // random szám a várakozáshoz
	 createPipe();//létrehozzuk a 2 csövet
	 char sz[200]; // a szöveg mennyisége ami átmegy a csövön
	signal(SIGTERM,handler);
    printf("signal utanra eljutottam \n");
	pid_t pid1, pid2;
	pid1 = fork();
	
	int count=0;
	if(pid1== -1)
    {
        perror("The fork calling was not succesful\n");
        exit(1);
    }
	if(pid1 == 0) //idomar
    {
		//trigger company
		kill(getppid(),SIGTERM);
		
		//get message 
		struct Clients actual;
		read(pipe1[0],&actual , sizeof(actual));
		printf("|%s|%s|%s|%s|%d| \n",actual.name,actual.adress,actual.email,actual.telNum,actual.priority);
		sleep(3);
		printf("Catcher - I'm here!\n");
		
		kill(pid2, SIGTERM);
		waitpid(pid2, &status, 0);
		write(pipe2[1],&random ,sizeof(random));
		close(pipe2[1]);
		printf("Idmomár - time sent!\n");
		
	}
	else {
		pid2 = fork();
		if(pid2== -1)
		{
        perror("The fork calling was not succesful\n");
        exit(1);
		}
		if(pid2 == 0) //madar
        {
			pause();
			printf("madárka elindult\n");
			time_t to;
			srand(time(&to));
				r = rand() % 5 + 1;
			kill(pid1, SIGTERM);
		}
		else { // parent
			
			pause();
			printf("PARENT: SIGNAL ARRIVED\n");
		    printf("Szulo indul!\n");
			sleep(1);
			close(pipe1[0]);
			close(pipe2[1]);
			printf("Company - I send the next address!\n");
			//close(pipe1[0]);	
			write(pipe1[1],&sortedClients[count] , sizeof(sortedClients[count]));
			count++;
            waitpid(pid1, &status, 0);
			close(pipe1[1]);
			read(pipe2[0],&random ,sizeof(random));
			close(pipe2[1]);
			printf("Company - I read the time!\n");
			fflush(NULL);
			
			printf("|%s|%s|%s|%s|%d| szamla: %d \n",sortedClients[count].name,sortedClients[count].adress,sortedClients[count].email,sortedClients[count].telNum,sortedClients[count].priority,random);
			
		}
	   }
}

int console()
{
    char input[30];
    int ret;
    char *ptr;    
    int z=1;
    while(z=1)
    {   
        printf("\n");
        printf("Nyomj '1'          az uj felhasznalohoz.\n");
        printf("Nyomj '2'     felhasznalo modositasahoz.\n");
        printf("Nyomj '3'        felhasznalo torlesehez.\n");
        printf("Nyomj '4'      nev szerinti keresesehez.\n");
        printf("Nyomj '5' prioritas szerinti kereseshez.\n");
        printf("Nyomj '6'       a kliensek listazahasoz.\n");
        printf("Nyomj '0'  a programbol valo kilepeshez.\n");
        printf("Nyomj '9'               a szamlahazshoz.\n");
        printf("\n");
        
        
        fgets(input, 50, stdin);

        ret =strtol(minEnt(input),&ptr,10);
        if(ret==0)
        {
            exit(0);
        } 
        else if(ret==1)
        {
            addClient();
            saveData();
        }
        else if(ret==2)
        {
            editClient(searchByName());
            saveData();
        }
        else if(ret==3)
        {
            deleteClient(searchByName());
            saveData();
        }
        else if(ret==4)
        {
            searchByName();
          
        }
          else if(ret==5)
        {
            searchByPriority();
          
        }
          else if(ret==6)
        {
            listClients();
          
        }
		else if(ret==9)
        {
            masodikbeadando();
          
        }
        
    }
    
} 
/////////////////////////////////////////////////////////////////////////////////////2. beadando




int main(int argc,char** argv)
{
   
    loadData();
    console();
   
}









