#include <stdlib.h> //le kell torolni az adat.txt-t es ujra letrehozni 
#include <stdio.h>
#include <sys/types.h> //fork-hoz
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> //lock


int main()
{
  int f;
  char szo1[]=" Szulo szulo szulo szulo szulo szulo szulo szulo szul";
  int hossz1=strlen(szo1);
  char szo2[]=" Gyerek gyerek gyerek gyerek gyerek gyerek gyerek gyerek gyerek";
  int hossz2=strlen(szo2);
  int i;
  struct flock lock_data;
  lock_data.l_start=0;
  lock_data.l_len=100000;
  lock_data.l_whence=SEEK_SET;
  int rc=0; //lock eredmenye
  f=open("adat.txt",O_RDWR|O_CREAT|O_TRUNC);
  pid_t child=fork();
  if (child<0){ //hiba
    perror("hiba");
  }
  else 
  {
    if (child>0)
    { //szulo
    //  while (1)
      { 
       // printf("szulo %i ",getpid());
          lock_data.l_pid=getpid();    
          lock_data.l_type=F_WRLCK;
          rc=fcntl(f,F_SETLKW,&lock_data); //F_setlkw - var, amig megteheti .    F_SETLK nem var
          if (rc!=(-1))
          {        
          
           for (i=0;i<hossz1;i++)
           {
             write(f,&szo1[i],1);
             usleep(20);          
           }  
           write(f,"\n",1);
           lock_data.l_type=F_UNLCK;
           fcntl(f,F_SETLK,&lock_data);
          }
         ;
      }
    }
    else 
    {
    //  while (1)
      {//printf("gyerek %i ",getpid());
         lock_data.l_pid=getpid();
         lock_data.l_type=F_WRLCK;
         rc=fcntl(f,F_SETLKW,&lock_data);
         if (rc!=(-1))
         {       
            for (i=0;i<hossz2;i++)
            {
              write(f,&szo2[i],1);
              usleep(20);
            }  
            write(f,"\n",1);
            lock_data.l_type=F_UNLCK;
            fcntl(f,F_SETLK,&lock_data);      };
        }
      }
    }
    close(f);
}
