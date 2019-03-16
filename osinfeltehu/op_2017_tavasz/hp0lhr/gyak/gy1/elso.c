/*
struct
szöveg
szám
feltölt szó+számfájlba ír
visszaolvas
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <string.h>
#include <errno.h> //perror, errno

struct MyStruct{
	char* Word;
	int Number;
};

int main(int argc,char** argv){
 
 if (argc!=2){perror("You have to use program with an argument; the file name");exit(1);}
 int From,To;
 struct MyStruct Ms;
 
 int g;
 g=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
 if (g<0){ perror("Error at opening the file\n");exit(1);}
 
 char StringBuffer[128];
 printf("Add meg a szot: ");
 scanf("%s", StringBuffer);
 printf("\n");
 Ms.Word=(char*)malloc(128*sizeof(char));
 strcpy(Ms.Word, StringBuffer);
 
 printf("Add meg a szamot: ");
 scanf("%d", &Ms.Number);
 printf("\n");
 
 if (write(g,&Ms.Word,sizeof(Ms.Word))!=sizeof(Ms.Word)) 
      {perror("There is a mistake in writing\n");exit(1);}
 
 if (write(g,&Ms.Number,sizeof(Ms.Number))!=sizeof(Ms.Number)) 
      {perror("There is a mistake in writing\n");exit(1);}
 
 close(g);
 
 int f;
 f=open(argv[1],O_RDONLY); 
 if (f<0){ perror("Error at opening the file\n");exit(1);}
 
 read(f,StringBuffer,sizeof(StingBuffer)) 
 printf("%s",StringBuffer); //we prints out the content of the file on the screen
 
 read(f,&g,sizeof(g)) 
 printf("%d",&g); //we prints out the content of the file on the screen
 
 close(f);

 free(Ms.Word);
 return 0;
}