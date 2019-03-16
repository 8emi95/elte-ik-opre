#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct question {
  char *text;
  char **answers;
  unsigned char asize;
} question;

size_t TOP=255;
size_t TWO=2;

question *questions;
unsigned char qsize;

unsigned char *prompt(unsigned char select, FILE *fd)
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
    getline(&selectionstr, &TWO, stdin);
    selection=atoi(selectionstr);
    if(selection>=0 && selection<=4)
    {
      return selection;
    }
  }
}

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

void rm_item(char *id)
{
  // TODO id is a pointer now in the form 11, 12 we have to handle this like in modify!
  if(*(id-1)>qsize)
  {
    printf("\ninvalid index!");
    return;
  }
  unsigned char i;
  for(i=*id; i<qsize; i++)
  {
    questions[i] = questions[i+1];
  }
  qsize--;
  if(!realloc(questions, (qsize)*sizeof(question)))
    printf("wtf, out of RAM?\n");
}

void modify_item(unsigned char *id)
{
  if(atoi(&id[0])-1>qsize)
  {
    printf("\ninvalid index!");
    return;
  }
  char *modified_string = malloc(255);
  getline(&modified_string,&TOP,stdin);
  if(atoi(&id[1]) == 1)
  {
	  questions=realloc(questions[atoi(&id[1])].text, 255);
	  strcpy(questions[atoi(&id[1])].text, modified_string);
	  return;
  }
  questions=realloc(questions[atoi(&id[1])].answers[atoi(&id[1])-1], 255);
  strcpy(questions[atoi(&id[1])].answers[atoi(&id[1])-1], modified_string);
  return;
  
}

void write_file()
{
  FILE *fd = fopen("./kerdesek.txt","w");
  prompt(0,fd);
  fclose(fd);
}


int main()
{
  char *line = malloc(255);
  char *command = malloc(255); // to ensure buffer safety
  char *arg = malloc(255);

  char *id = malloc(3);
  while(1)
  {
    printf(">");
    getline(&line, (size_t*)255, stdin);
    command = strtok(line, " ");
    arg = strtok(0, " ");
   
    if(!strcmp(command, "add"))
    {
      add_item(arg);
      continue;
    }

    if(!strcmp(command, "rm"))
    {
      id = prompt(1, stdout);
      rm_item(id);
	  continue;
    }

    if(!strcmp(command, "list"))
    {
      prompt(0, stdout);
	  continue;
    }

    if(!strcmp(command, "write"))
    {
      write_file();
	  continue;
    }

    if(!strcmp(command, "modify"))
    {
      id = prompt(1, stdout);
      modify_item(id);
	  continue;
    }
  }
}