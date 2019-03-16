#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h> 
#include <mqueue.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// Posix Message queue sample
// mq_open, mq_send,mq_receive, mq_unlink
// -lrt -vel fordit!


int compare_strings(char a[], char b[])
{
   int c = 0;
 
   while (a[c] == b[c]) {
      if (a[c] == '\0' || b[c] == '\0')
         break;
      c++;
   }
 
   if (a[c] == '\0' && b[c] == '\0')
      return 1;
   else
      return 0;
}

void beert(int signumber){
    if(10==signumber)
    {
        printf("Sikeresen beert a szerelveny.\n");
    }
    else
    {
        printf("Be lett vontatva.\n");
    }
}

int main (int argc, char* argv[]) { 

    int max = atoi(argv[1]);
    int jaratok = max;
    int mentesito = 0;
    srand(time(NULL));
    
    int pid;
    key_t kulcs;
    int oszt_mem_id;
    char *s;
	// a parancs nevevel es az 1 verzio szammal kulcs generalas
    kulcs=ftok(argv[0],1);
    // osztott memoria letrehozasa, irasra olvasasra, 500 bajt mrettel
    oszt_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);
    // csatlakozunk az osztott memoriahoz, 
    // a 2. parameter akkor kell, ha sajat cimhez akarjuk illeszteni
    // a 3. paraméter lehet SHM_RDONLY, ekkor csak read van  
    s = shmat(oszt_mem_id,NULL,0);
    
    int shmid;
    int *array;
    int count = 5;
    int i = 0;
    int SizeMem;
    key_t key = kulcs;
    
    kulcs=ftok(argv[0],1);
    
    //SizeMem = sizeof(*array)*count;

    shmid = shmget(key, count*sizeof(int), IPC_CREAT);

    array = (int *)shmat(shmid, 0, 0);

    
    

    struct sigaction partnerArrive;
    partnerArrive.sa_handler = beert;
    sigemptyset(&partnerArrive.sa_mask);
    partnerArrive.sa_flags = 0;
    sigaction(SIGUSR1,&partnerArrive,NULL);
    sigaction(SIGUSR2,&partnerArrive,NULL);
    
    
    pid_t init_pid = getpid();
     pid_t child,ujchild; 
     int uzenetsor, status;
	char* mqname="/zarthelyi";
    char* mqname_2="/zarthelyi_2";
	struct mq_attr attr;
	struct mq_attr attr_2;
	mqd_t mq1,mq2;
	attr.mq_maxmsg=5; //MAXMSGS
	attr.mq_msgsize=50; //MSGSIZE
    
	attr_2.mq_maxmsg=5; //MAXMSGS
	attr_2.mq_msgsize=50; //MSGSIZE 
	//
	mq_unlink(mqname); // delete if exist
    mq_unlink(mqname_2); // delete if exist
    
	mq1=mq_open(mqname, O_CREAT|O_RDWR,0600,&attr);
    mq2=mq_open(mqname_2, O_CREAT|O_RDWR,0600,&attr_2);
    
     if ( mq1 < 0 ) { 
          printf("mq_open error: %d \n",errno); 
          return 1; 
     }
     if ( mq2 < 0 ) { 
          printf("mq_open error: %d \n",errno); 
          return 1; 
     } 
     char buffer[100];	// mq_send, mq_receive uses char array
	 printf("Buffer length: %d\n",strlen(buffer));
     
     int inditott_szerelvenyek[max];
     int k=0;
     for(k=0;k<max;++k)
     {
         inditott_szerelvenyek[k]=0;
     }
     
     for(k=0;k<max;++k)
     {
        if(init_pid==getpid())
        {
           child = fork();
           if((child)<0) {
				printf("forking error");
				exit(1);
			}
        } 
        if ( child > 0 ) { 
            
            sigset_t signalsetting;
            sigfillset(&signalsetting);
            sigdelset(&signalsetting,SIGUSR1);
            sigdelset(&signalsetting,SIGUSR2);
            printf("Parent process starts\n");
        
            char str[30];
            int hanyadik = rand()%max;
            while(inditott_szerelvenyek[hanyadik]==1)
            {
                hanyadik = rand()%max;
            }
            inditott_szerelvenyek[hanyadik]=1;
            sprintf(str, "%d", hanyadik+1);
            int db=mq_send(mq1,str,2, 4 );  //Parent sends a message. 
            sprintf(str, "%d", max-(hanyadik+1));
            db=mq_send(mq1,str,2, 4 );  //Parent sends a message.
            
            sigsuspend(&signalsetting);
            
            db=mq_receive(mq2,buffer, 54,NULL);
            printf("A szerelvennyel a kovetkezo tortenik: ");
            int bhej;
            if(compare_strings(buffer,"1") || compare_strings(buffer,"2"))
            {
                printf("a buffer tartalma: %s\n",buffer);
                if(compare_strings(buffer,"1"))
                {
                    printf("elcsuszott egy bananhejon, uj inditasa.\n");
                }
                else
                {
                    printf("zord hideg aldozata lett, uj inditasa.\n");
                }
                ujchild = fork();
                if(ujchild>0) //továbbra is parent
                {
                    sprintf(str,"%s","On egy potszerelveny lesz.\n");
                    db=mq_send(mq1,str,30, 4 );
                    sprintf(str, "%d", hanyadik+1);
                    db=mq_send(mq1,str,2, 4 );
                    ++mentesito;
                }
                else //potlo child
                {
                    printf("----------------------\nChild substitute process starts\n");
                    db=mq_receive(mq1,buffer, 54,NULL);
                    printf("En a kovetkezo uzenetet kaptam: \n",buffer);
                    db=mq_receive(mq1,buffer, 54,NULL);
                    printf("En a(z) : %s szerelvenyt mentesitem\n",buffer);
                    printf("Child substitute process ends\n");
                    mq_close(mq1);
                    mq_close(mq2);
                    exit(0);
                }
            }
            else
            {
                printf(" %s ",buffer);
            }
            
            if(k==max-1)
            {
                int status;
                wait(&status);
                mq_close(mq1);
                mq_close(mq2);
                mq_unlink(mqname);
                printf("Ma %d mentesito jaratot kellett inditani.",mentesito);
                printf("Parent process ended\n");
                
                //atlag kiszamitasa
                int z;
                int szum=0;
                for(z=0;z<max;z++)
                {
                    szum+=array[z];
                }
                printf("Az idok atlaga: %d\n",szum/max);
                shmdt((void *) array);
                shmctl(oszt_mem_id,IPC_RMID,NULL);
            }
        }
        else
        { // child
            char str[30];
            sleep(1);
            printf("----------------------\nChild process starts\n");
            
            int db=mq_receive(mq1,buffer, 54,NULL);
            printf("En a(z) : %s szerelveny vagyok\n",buffer, db,strerror(errno));

            db=mq_receive(mq1,buffer, 50, NULL);
            printf("Meg ennyien jonnek utana:%s\n", buffer,db);
            
            int beert_e = rand()%10;
            sprintf(str, "%d", beert_e);
            
            if(beert_e>0)
            {
                kill(getppid(),SIGUSR1);
                printf("Beertem, siker.\n");
            }
            else
            {
                kill(getppid(),SIGUSR2);
                printf("Sajnos bevontattak.\n");
            }
            
            
            int bananhej = rand()%2;
            if(beert_e==0 && bananhej)
            {
                sprintf(str,"%d",1);
                array[k] = -1;
            }
            else if(beert_e==0)
            {
                sprintf(str,"%d",2);
                array[k] = -1;
            } 
            else
            {
                sprintf(str, "%s", "Minden rendben volt.\n");
                //writing to shared memory:
                array[k] = ((rand()%21)+10);
            }
            db=mq_send(mq2,str,30, 1 );  //Child sends a message.

            printf("Child process ends.\n");
            mq_close(mq1);
            mq_close(mq2);
            mq_unlink(mqname_2);
            sleep(1);
            shmdt((void *) array);
            exit(0);
        }
     }
     return 0; 
} 
