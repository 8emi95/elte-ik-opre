#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>


void sig_kuldo(char *pid,char* prognev)
{
	int fd;
	char s[1024],path[32];
	char *p;
	char name[1024];
	//minden folyamat a /proc könyvtárban hagy "nyomot"
	//a könyvtáron belül a pid könyvtárban vannak az adott pid-ü
	//folyamat jellemzõi, például a status fájl tartalmazza a nevet.
	sprintf(path,"/proc/%s/status",pid);
	fd = open(path,O_RDONLY,0);
	read(fd,s,sizeof(s)); // beolvasunk a statusból
	close(fd);
	printf("A pid erteke: %s\n",pid);	
	p = strstr(s,"Name:");
	//printf("A keresett resz: %s\n",p);
	sscanf(p,"Name:\t%s\n",name);	// a p keresett szovegbol beolvasunk nev-be
	printf("A program neve: %s\n",name);
	if( strcmp(name,prognev)==0  ){
	    kill(atoi(pid),SIGTERM);
	    printf("A kill(SIGTERM) elkuldve: %d azonositoju folyamatnak!\n",atoi(pid));
	}
}



int main(int argc, char **argv)
{
	DIR *dir;
	struct dirent *akt;

	if(argc<2){
	    printf("Kell egy parameter, hogy kinek küldjek jelet!");
	    return 1;
	}

	dir = opendir("/proc");
	while((akt = readdir(dir)) != NULL) 
	{
  	   if(isdigit(*akt->d_name))
	       sig_kuldo(akt->d_name,argv[1]); // a nev a PID
	}
	closedir(dir);
	
	return 0;
}
