#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>	// open-hez, param�terekhez kell
#include <sys/stat.h>  // open-hez kell
#include <string.h>
#include <unistd.h>	// read utasítás kivanja
//
struct csapat
{
	int alapitas;
	char* nev;
};
//

int main()
{
 char *s="Almafa";
// 
 struct csapat x;
 printf("Az int m�rete: %i\n",sizeof(int));
 printf("A csapat struct m�rete: %i\n",sizeof(struct csapat));
 int fd=open("ir1.log",O_RDONLY);
 while (read(fd,&x,sizeof(struct csapat)))
 {
 	//read(fd,&y,sizeof(struct csapat));
 	printf("A beolvasott évszám: %d\n",x.alapitas);
 	printf("A csapat neve: %d \n",x.nev);
 }
 close(fd);
 return 0;
}
