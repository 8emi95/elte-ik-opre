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

int childPid = 0;

struct person{
	char name[50];
};

struct result{
	int success;
	char not_here[1024];
};

struct a
{
 int b;
 char c[1024];
};

int main(){

int s;
 for (s = 0; s < 3; ++s){
 	
  int pipefd[2];
  int sumpipe[2];
  char sz[100];
  struct person people[50];
  struct person people_read[50];
 
  int sum; 
 // char sum_send[10];
 // char sum_read[10];
  srand(time(NULL)); //the starting value of random number generation
  int r; 
  
 /* int fd;
    printf("Fifo start!\n");
    int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); // creating named pipe file
    // S_IWGRP, S_IROTH (other jog), file permission mode
    // the file name: fifo.ftc
    // the real fifo.ftc permission is: mode & ~umask 
    if (fid==-1)
    {
	printf("Error number");
	exit(EXIT_FAILURE);
    }
    printf("Mkfifo vege, fork indul!\n");
  */
  // namepipe keszites
 
  int fd;
  int fid = mkfifo("fifo.ff", S_IRUSR|S_IWUSR);
  if (fid == -1){
	perror("Hiba nevcsonel");
	exit(EXIT_FAILURE);
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

 // pid_t child=fork();
 char str[10];
 char path[50];
 
  pid_t child=fork(); 
  // szülő

  if (child>0)
  {
   // sleep(2);
	printf("Parent started now\n");
	remove("fifo.ff");
	// konyvtar listazas
	DIR           *d;
	  struct dirent *dir;
	//  while(1){
	  d = opendir("1");
	  if (d)
	  {
		while ((dir = readdir(d)) != NULL)
		{
		  printf("%s\n", dir->d_name);
		}

		closedir(d);
	  }
//	}
  
	printf("Add file to open: \n");
	scanf("%s", str);
	
	strcpy(path,"1/");
	strcat(path,str);

	char nev[50];
	int i;
	// fájl megnyitása, tömbbe olvasás
	FILE *fp;
    fp = fopen(path, "r");
	printf("%s", path);
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
	
	/*
	int ret;
	 ret = remove(path);

   if(ret == 0) 
   {
      printf("File deleted successfully\n");
   }
   else 
   {
      printf("Error: unable to delete the file\n");
   }
	*/
	//wait();
  char sum_send[10];
	
	
	sprintf(sum_send, "%d", i);
	/*people[0].name = "apple";
	people[1].name = "banana";
	people[2].name = "cole";
	people[3].name = "datoya";
	people[4].name = "egg";
	people[5].name = "fruit";
	people[6].name = "ground";
	people[7].name = "hound";
	people[8].name = "ill";*/
	//printf("%s\n", people[0].name);
	//sum = 3;
	
	// meg kell oldani hogy az i érték is átmenjen
	//strcpy(sum_send, itoa(i));
	printf("Value of sum PARENT: %i\n", i);
	close(sumpipe[0]);
	close(pipefd[0]);
	write(sumpipe[1], sum_send, sizeof(sum_send));
	
	
	write(pipefd[1], people, sizeof(people));
	
	close(pipefd[1]);
    close(sumpipe[1]);
	childPid = child;

	printf("Write to pipe done \n");
	
	//fflush(NULL);
	fd = open("fifo.ff", O_RDONLY);
	struct result res_read;
	read(fd, &res_read, sizeof(struct result));
	printf("Read from tube PARENT: %i \nDidn't come PARENT: %s \n", res_read.success, res_read.not_here);
	close(fd);
	unlink("fifo.ff");
	
	wait();
	
/*	printf("Csonyitas eredmenye szuloben: %d!\n",fid);
	fd=open("fifo.ftc",O_RDONLY);
        struct a to_read;
	read(fd,&to_read,sizeof(struct a));
	printf("Ezt olvastam a csobol: %i %s \n",to_read.b, to_read.c);
	close(fd);
	// remove fifo.ftc
	unlink("fifo.ftc");
*/	
/*	int status;
    wait(&status);
	  printf("SIGNAL: %i \n", status);
	/*
	pause();
    printf("Signal arrived\n",SIGUSR2);
    //int status;
    wait(&status);
	  printf("SIGNAL: %i \n", status);
	*/
	
	// eredmeny fogadas
	
	
	printf("Parent process ended\n");
	fflush(NULL);
	
	//wait();
	//sleep(2);
	
//	kill(childPid, SIGTERM);
//	printf("sigterm done: %i %i\n", getppid(), getpid());
    
  }
  else 
  // gyerek
  {
   // wait();
//	printf("Child is waiting for message\n");
	//sleep(1);
	char sum_read[100];
	char not_come[1024];
	  struct result res_send;
	//int sum_read;
	close(sumpipe[1]);
	close(pipefd[1]);
	
	read(sumpipe[0], sum_read, sizeof(sum_read));
	
	
	read(pipefd[0], people_read, sizeof(people_read));
	
	
	printf("Value of sum CHILD: %s\n", (sum_read));
	printf("SOMEONE: %s", people_read[0].name); 
	strcpy(not_come,"");
	strcpy(res_send.not_here,"");
	res_send.success = rand();
	int k;
	for (k = 0; k < atoi(sum_read); ++k){
		r = rand()%100;
		if ( r < 60 ){
			strcat(not_come, people_read[k].name);
			strcat(not_come, " ");
			strcat(res_send.not_here, people_read[k].name);
			strcat(res_send.not_here, " ");
			printf("Child got: %s %i\n", people_read[k].name, r);
		}
	}
	
	// csobe rakas, küldes

	fd = open("fifo.ff", O_WRONLY);
	write(fd, &res_send, sizeof(struct result));
	close(fd);
	
/*
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	printf("now: %d-%d-%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday+1);
*/
	printf("Didn't come: %s\n", not_come);
	
	close(sumpipe[0]);
	close(pipefd[0]);
	/*
	printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGUSR1);
    sleep(3);
    kill(getppid(),SIGUSR1); 
	*/
	
	//printf("First signal has been made\n");
  //  sleep(3);
//	kill(getppid(),SIGUSR2); 
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    //sleep(3);
    printf("Child process ended\n"); 

 }
//endfor
}
  return 0;
}
