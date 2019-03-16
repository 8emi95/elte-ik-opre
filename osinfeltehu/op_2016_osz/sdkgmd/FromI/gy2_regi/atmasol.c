#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
void beolvas(char* fnev){
 printf("Kerek egy fajlnevet:");
 if (scanf("%s",fnev)==EOF){
   perror("Nem adott meg fajlnevet!\n");exit(0);}; 
}
void megnyit(char * fnev,int * fajlleiro, int mire){  
  if (mire == O_RDONLY){ 
    if (access(fnev,F_OK)!=0){perror("Nem letezik a fajl!\n");exit(0);}
  }
  (*fajlleiro)=open(fnev,mire);
  if (errno!=0){perror("Hibas megnyitas\n");exit(0);};
  
}
void atmasol(int f,int g){
  char c;
  while (read(f,&c,1)){
   if (write(g,&c,1)!=1){perror("Hibas iras\n");exit(0);};
  }
  close(f);
  close(g);
}

int main(){
 int f,g;
 char  inputnev[80],outputnev[80];
 beolvas(inputnev);
 beolvas(outputnev);
 megnyit(inputnev,&f,O_RDONLY);
 megnyit(outputnev,&g,O_RDONLY|O_WRONLY|O_CREAT);

 atmasol(f,g);
 printf("Kesz...");
}