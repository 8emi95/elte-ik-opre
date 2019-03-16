#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct question {
  char *text;
  char **answers;
  unsigned char asize;
} question;

question *questions;
unsigned char qsize;

void add_item(char *arg)
{
  if(!realloc(questions, (qsize+1)*sizeof(question)))
  {
    printf("wtf, out of RAM?\n");
    return;
  }
  
  questions[qsize].text = arg;
  qsize = qsize + 1;
}

void rm_item(unsigned char id)
{
  if(id-1>qsize)
  {
    printf("\ninvalid index!");
    return;
  }
  unsigned char i;
  for(i=id; i<qsize; i++)
  {
    questions[i] = questions[i+1];
  }
  qsize--;
  if(!realloc(questions, (qsize)*sizeof(question)))
    printf("wtf, out of RAM?\n");
}

void modify_item(unsigned char *id)
{
  if(atoi(id[0])-1>qsize)
  {
    printf("\ninvalid index!");
    return;
  }
  char *modified_string = malloc(255);
  getline(&modified_string,255,stdin);
  if(atoi(id[2] == 1)
  {
	  realloc(questions[atoi(id[1])], 255);
	  strcpy(questions[atoi(id[1])].text, modified_string);
	  return;
  }
  realloc(questions[atoi(id[1])].answers[atoi(id[1])-1], 255);
  strcpy(questions[atoi(id[1])].answers[atoi(id[1])-1], modified_string);
  return;
  
}

void write_file()
{
  FILE *fd = fopen("./kerdesek.txt","w");
  prompt(0,fd);
  fclose(fd);
}

unsigned char prompt(unsigned char select, int fd)
{
  unsigned char i;
  unsigned char j;

  for (i=0; i<qsize; i++)
  {
    fprintf(fd, "\t%d: %s\n", i+1, questions[i].text);
    for(j=0; j<questions[i].asize; j++)
    {
      fprintf(fd, "%d. %s\n", j+1, questions[i].answers[j]);
    }
  }
  if(!select) return 0;
  char *selectionstr = malloc(3);
  unsigned char selection = 0;
  while(1)
  {
    printf("\n select your item:");
    getline(&selectionstr, 2, stdin);
    selection=atoi(selectionstr);
    if(selection>=0 && selection<=4)
    {
      return selection;
    }
  }
}

int main()
{
  char *line = malloc(255);
  char *command = malloc(255); // to ensure buffer safety
  char *arg = malloc(255);

  char id = -1;
  while(1)
  {
    printf(">");
    getline(&line, 255, stdin);
    command = strtok(line, " ");
    arg = strtok(0, " ");
   
    if(!strcmp(command, "add"))
    {
      add_item(arg);
      continue;
    }

    if(!strcmp(command, "rm"))
    {
      id = prompt(1,1);
      rm_item(id);
	  continue;
    }

    if(!strcmp(command, "list"))
    {
      prompt(0, 1);
	  continue;
    }

    if(!strcmp(command, "write"))
    {
      write_file();
	  continue;
    }

    if(!strcmp(command, "modify"))
    {
      id = prompt(1,1);
      modify_item(id);
	  continue;
    }
  }
}