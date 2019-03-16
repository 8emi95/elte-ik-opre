#include <stdlib.h> //le kell torolni az adat.txt-t es ujra letrehozni 
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
  int f;
  char szo1[]=" Szulo szulo szulo szulo szulo szulo szulo szulo szulo";
  int hossz1=strlen(szo1);
  char szo2[]=" Gyerek gyerek gyerek gyerek gyerek gyerek gyerek gyerek gyerek";
  int hossz2=strlen(szo2);
  int i;
  int rc=0; //lock eredmenye
  f=open("adat.txt",O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
  pid_t child=fork();
  if (child<0){ //hiba
    perror("hiba");
  }
  else 
  {
    if (child>0)
    { //szulo
       // printf("szulo %i ",getpid());
	  usleep(10);
          rc=flock(f,LOCK_EX); //LOCK_SH, LOCK_UN
          if (rc==0) // OK az flock
          {        
           for (i=0;i<hossz1;i++)
           {
             write(f,&szo1[i],1);
             usleep(10);          
           }  
           write(f,"\n",1);
           flock(f,LOCK_UN);
          }
         ;
    }
    else 
    {
         rc=flock(f,LOCK_EX);
         if (rc==0)// flock ok
         {       
            for (i=0;i<hossz2;i++)
            {
              write(f,&szo2[i],1);
              usleep(10);
            }  
            write(f,"\n",1);
            flock(f,LOCK_UN);      };
        }
    }
    close(f);
}
