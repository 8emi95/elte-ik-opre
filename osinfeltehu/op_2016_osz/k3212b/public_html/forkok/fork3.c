#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <wait.h>
#include <stdlib.h>

#define MAX_COUNT 5

void cprocess();
void process();

pid_t cpid;

  char names[10][50];
  int i = 0;
  char* id;
  FILE *f;

  int fd[2];
  char buff[300];

int main()
{
  

  int Pipe = pipe(fd);
  if (Pipe < 0)
  {
    perror("Pipe error!");
    exit(-1);
  }

  cpid = fork();

  if (cpid == 0)
  {
    cprocess();
  }
  else
  {
    process();
  };
wait(NULL);
  return 0;
};

  void cprocess()
{
  read(fd[0], buff, strlen(id));
	printf("A partner megkapta arendezveny azonositojat:\n%s\n\n", buff);
		
};

void process()
{
  id = "2";
  f = fopen(id, "r");
  if (f == NULL)
  {
    perror("File opening error\n");
    exit(1);
  }
  char line[50];
  while (fgets(line, sizeof(line), f))
  {
    strcpy(names[i], line);
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    ++i;
  }
  printf("Sending id to child: %s \n", id);
      write(fd[1], id, strlen(id));
};