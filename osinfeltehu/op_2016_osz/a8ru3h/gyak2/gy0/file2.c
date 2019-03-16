//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

struct nev_es_jegy {
  char[255] nev;
  int jegy;
	}

	int main(int argc,char** argv){

	  if (argc!=2){perror("You have to use program with two arguments, the file names copy_from copy_to");exit(1);}
	  int f;
	 
	  g=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);


:help



 //3. parameter the permission, if it has to be created
 //S_IRUSR=permission for reading by the owner e.g.
 if (g<0){ perror("Error at opening the file\n");exit(1);}
 
 char c;
 while (read(f,&c,sizeof(c))){ 
    //read gives back the number of bytes
    //1. parameter the file descriptor
    //2. parameter the address of variable, we read into
    //3. parameter the number of bytes we want to read in
    printf("%c",c); //we prints out the content of the file on the screen
    if (write(g,&c,sizeof(c))!=sizeof(c)) 
      {perror("There is a mistake in writing\n");exit(1);}
      //write gives back the number of written bytes
      //1. parameter the file descriptor
      //2. parameter the address of variable we want to write out
      //3. parameter the number of bytes we want to write out 
 } 
 close(f);
 close(g);
 return 0;
}
