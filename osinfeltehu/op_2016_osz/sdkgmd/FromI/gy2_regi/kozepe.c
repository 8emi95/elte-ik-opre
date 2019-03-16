#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
void megnyitas(char *fnev,int *fajlleiro){
 (*fajlleiro)=open(fnev,O_RDONLY);
 if (errno){perror("Megnyitasi hiba");exit(0);};
}
void kozepreallit(int fajlleiro){
 struct stat statusz;
 int pozicio;
 if (fstat(fajlleiro,&statusz)){perror("Hibas statusz olvasas");exit(0);}
 pozicio=(int)((statusz.st_size)/2);
 printf("%i\n",pozicio);
 if (!lseek(fajlleiro,pozicio,SEEK_SET)){perror("Hibas pozicio beallitas");exit(0);};
}
void olvas_ir(int fajlleiro){
 char c;
 if (!read(fajlleiro,&c,1)){perror("Hibas olvasas");exit(0);}
 printf("A kozepso karakter: %c \n",c);
 }
 int main(int argc,char * arg){
 int f;
 megnyitas("szoveg.txt",&f);
 kozepreallit(f);
 olvas_ir(f);
 close(f);
 }