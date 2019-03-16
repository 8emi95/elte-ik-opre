#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h> 
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int childPid = 0;

struct person{
	char name[50];
};

struct result{
	int success;
	char not_here[1024];
};


int main(){

remove("fifo.ff");

 while(1){

  int pipefd[2];
  int sumpipe[2];
  char sz[100];
  struct person people[50];
  struct person people_read[50];
 
  int sum; 
  srand(time(NULL)); //the starting value of random number generation
  int r; 

  // namepipe keszites 
  int fd;
  int fid = mkfifo("fifo.ff", S_IRUSR|S_IWUSR);
  if (fid == -1){
	exit(EXIT_FAILURE);
	remove("fifo.ff");
  }
 
 if (pipe(pipefd) == -1) 
 {
   perror("Hiba a pipe nyitaskor!");
   exit(EXIT_FAILURE);
 }
 
 if (pipe(sumpipe) == -1) 
 {
   perror("Hiba a pipe nyitaskor!");
   exit(EXIT_FAILURE);
 }
	
 char str[10];
 char path[50];
 
  pid_t child=fork(); 
  // szülő
  if (child>0)
  {
	printf("Parent started now\n ---------------------------- \n");
	// konyvtar listazas
	printf("Listing event names: \n");
	
	DIR           *d;
	  struct dirent *dir;
	  d = opendir("1");
	  if (d)
	  {
		while ((dir = readdir(d)) != NULL)
		{
		  printf("%s\n", dir->d_name);
		}
		closedir(d);
	  }

	printf(" ---------------------------- \n");
	printf("Add event to open: \n");
	scanf("%s", str);
	
	strcpy(path,"1/");
	strcat(path,str);

	char nev[50];
	int i;
	// fájl megnyitása, tömbbe olvasás
	FILE *fp;
    fp = fopen(path, "r");
    if (fp == 0){
        printf("File not found\n");
    } else {
		i = 0;
        while (!feof(fp)){
        fscanf(fp, "%s\n", nev);
		strcpy(people[i].name,nev);
		++i;
       }
    }
    fclose(fp);
		
   remove(path);
	
    char sum_send[10];	
	sprintf(sum_send, "%d", i);

	close(sumpipe[0]);
	close(pipefd[0]);
	write(sumpipe[1], sum_send, sizeof(sum_send));	
	write(pipefd[1], people, sizeof(people));
	
	close(pipefd[1]);
    close(sumpipe[1]);
	childPid = child;

	// eredmeny fogadas	
	fd = open("fifo.ff", O_RDONLY);
	struct result res_read;
	read(fd, &res_read, sizeof(struct result));
	printf(" ---------------------------- \nChild sent success to PARENT: %i \nChild sent who didn't come to PARENT: %s \n ---------------------------- \n", res_read.success, res_read.not_here);
	close(fd);
	unlink("fifo.ff");
	
	wait();
	
	printf("Parent process ended\n ---------------------------- \n\n");
	fflush(NULL);	
  }
  else 
  // gyerek
  {
    char sum_read[100];
	char not_come[1024];
	struct result res_send;
	
	close(sumpipe[1]);
	close(pipefd[1]);
	
	read(sumpipe[0], sum_read, sizeof(sum_read));	
	read(pipefd[0], people_read, sizeof(people_read));
	
	printf("Child arrived, waiting for guests!\n ---------------------------- \nThe list of guests are:\n");

	// adatok feldolgozása
	int k;
	for (k = 0; k < atoi(sum_read); ++k){
		printf("%s\n", people_read[k].name);	
	}
	
	printf(" ---------------------------- \n");
	
	strcpy(not_come,"");
	strcpy(res_send.not_here,"");
	res_send.success = rand();

	for (k = 0; k < atoi(sum_read); ++k){
		r = rand()%101;
		if ( r < 10 ){
			strcat(not_come, people_read[k].name);
			strcat(not_come, " ");
			strcat(res_send.not_here, people_read[k].name);
			strcat(res_send.not_here, " ");

		}
	}
	
	// csobe rakas, küldes
	fd = open("fifo.ff", O_WRONLY);
	write(fd, &res_send, sizeof(struct result));
	close(fd);
	
	printf("Didn't come: %s\n ---------------------------- \n", not_come);
	
	close(sumpipe[0]);
	close(pipefd[0]);
	
    printf("Child process ended\n ---------------------------- \n"); 

 }
//endwhile
}
  return 0;
}
