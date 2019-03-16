#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 


struct messgequeue { 
  long mtype;
      char mtext [ 1024 ]; 
      
      
};
       

int send( int mqueue ) 
{ 
  const struct messgequeue m = { 3, "55 , 95 , 67" }; 
   int status; 
               
    status = msgsnd( mqueue, &m, strlen ( m.mtext ) + 1 , 0 ); 


      if ( status < 0 ) 
        perror("msgsnd error"); 
            return 0; 
}

int receive( int mqueue ) 
{ 
struct messgequeue  m; 
int status; 

   
status = msgrcv(mqueue, &m, 1024, 3, 0 ); 
        
if ( status < 0 ) 
perror("msgsnd error"); 
else
 
printf("\n The coordinates are: %s \n", m.mtext);
return 0; 
} 


int main (int mqueue, char* argv[]) { 
  pid_t child; 
  int messgequeue , status; 
  key_t key;
     
                    
  
                               
  key = ftok(argv[0],1); 
  printf ("The key: %d\n",key);
    messgequeue  = msgget( key, 0600 | IPC_CREAT ); 
     if ( messgequeue < 0 ) { 
     perror("msgget error"); 
         return 1; 
}    


child = fork(); 
  if ( child > 0 ) { 
   send( messgequeue );   
    wait( NULL );
    
    status = msgctl(messgequeue, IPC_RMID, NULL ); 
         if ( status < 0 )
  perror("msgctl error"); 
    return 0; 
      } else if ( child == 0 ) { 
        return receive( messgequeue ); 
     
      } else { 
      perror("fork error"); 
      return 1;                                                   
    }
                                                                          
   return 0; 
} 