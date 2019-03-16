#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h>
#include <sys/time.h>
#include <sys/msg.h> 
#include <sys/types.h> 
#include <string.h>  

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

void handler_sigusr1(int signumber){
  printf("Uzemelteto jelzest kapott, hogy a metro beert!\n\n",signumber);
}

void handler_sigusr2(int signumber){
  printf("Uzemelteto jelzest kapott, hogy a metrot bevontattak!\n\n",signumber);
}

// sendig a message
int kuld_BMF( int uzenetsor, int train_id, int number_all_trains)
{ 
    char int_str[80];
    struct uzenet uz = { 5, "" };
     
    sprintf(int_str, "%d", train_id);
    strcpy(uz.mtext, int_str);
    strcat(uz.mtext, ",");
    sprintf(int_str, "%d", number_all_trains);
    strcat(uz.mtext, int_str);
      
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 

int kuld_metro( int uzenetsor, int siker)
{ 
    char int_str[80];
   int status;
   if (siker)
   {
      struct uzenet uz = { 5, "Nem volt problema!\n" };
      status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 );  
   }
   else
   {
      struct uzenet uz = { 5, "Bevontatas oka: elcsuszas bananhelyon!\n" };
      status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 );  
   }
      
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
} 
     
int fogad_BMF( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "BMF: A metro uzenetet kuldott:  %s--------------------\n", uz.mtext ); 
     return 0; 
} 
     
int fogad_metro( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 

     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
     {
         //printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext );
     }
          
      //Szerelveny tovabbi teendoi!
      
      char *str_split;
      int train_id;
      int number_all_trains;
      
      str_split = strtok(uz.mtext, ",");
      train_id = atoi(str_split);
      //printf("train_id: %i\n", train_id);
      str_split = strtok(NULL, ",");
      number_all_trains = atoi(str_split);
      //printf("number_all_trains: %i\n", number_all_trains);
      
      int train_tripped_on_a_banana;
        
        srand(time(NULL)); // the starting value of random number generation
          int p = rand() % 101; // szam 0-100 kozott
          if (p < 11) {
            train_tripped_on_a_banana = 1;
          } else {
            train_tripped_on_a_banana = 0;
          }
         
         printf("-------------------------\nEn a(z) %i. metro vagyok!\nUtanam meg %i db lesz!\n",train_id,number_all_trains-train_id);
         if (train_tripped_on_a_banana)
         {
             printf("Elcsusztam a bananhelyon!\n\n");
             kill(getppid(), SIGUSR2);
             kuld_metro(uzenetsor, 0);
         }
         else
         {
             printf("Teljesitettem az uzemet!\n\n");
             kill(getppid(), SIGUSR1);
             kuld_metro(uzenetsor, 1);
         }
       
     return 0; 
} 

int generate_train_id(int number_all_trains)
{
    int train_id;
    
    if(number_all_trains == 1)
    {
        train_id = 1;
    }
    else
    {
        srand(time(NULL)); //the starting value of random number generation
        train_id = rand()%number_all_trains; //number between 0-99
        train_id = train_id + 1;
    }
    
    //printf("Random szam: %i\n", train_id);
    return train_id;
}

int main(int argc,char ** argv)
{
    int uzenetsor, status, number_all_trains;
    key_t kulcs;
    
    signal(SIGUSR1,handler_sigusr1); 
    signal(SIGUSR2,handler_sigusr2);
    
    if (argc<2)
    {
        printf("Keves argumentum van megadva! (%i)\n", argc);
        return 1;
    }
    else
    {
        if(atoi(argv[1]) < 1)
        {
            printf("Helytelen szerelvenyszam van megadva!\n\n");
            return 1;
        }
        else
        {
           number_all_trains = atoi(argv[1]);
        }
    }
    
    kulcs = ftok(argv[0],1); 
     //printf ("A kulcs: %d\n",kulcs);
     uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
     if ( uzenetsor < 0 ) { 
          perror("msgget"); 
          return 1; 
     } 
 
    pid_t  child=fork(); //forks make a copy of variables
    if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
    if (child>0) //BMF
    {
        int train_id = generate_train_id(atoi(argv[1]));   
        kuld_BMF(uzenetsor, train_id, number_all_trains);
        sleep(2);
        fogad_BMF(uzenetsor);
        wait( NULL ); 
          // After terminating child process, the message queue is deleted. 
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
          if ( status < 0 ) 
               perror("msgctl"); 
          return 0; 
    }
    else //A szerelvény!
    {
        return fogad_metro( uzenetsor ); 
        // The child process receives a message. 
    }
    
    return 0;
}