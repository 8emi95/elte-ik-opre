#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

struct Question{
 char question[100];
 char answers[4][50];
 time_t date;
};

void readConsole(char * string,int length)
{
  fgets(string,length+1, stdin);
  if(string[strlen(string) - 1] == '\n'){
    string[strlen(string)-1]= '\0';
  }
  else{
    char ch;
    while ((ch = fgetc(stdin)) != '\n' && ch != EOF);
  }
}

void writeMenu()
{
  printf("Valassza ki, mit szeretne tenni![0-5]\n");
  printf("0 - Kilepes\n");
  printf("1 - Kerdesek es valaszok listazasa!\n");
  printf("2 - Kerdes hozzaadasa\n");
  printf("3 - Kerdes modositasa\n");
  printf("4 - Kerdes torlese\n");
  printf("5 - Kerdesek veglegesitese\n");
}

void writeQuestion(struct Question q)
{
  printf("%s\n", q.question);
  int i;
  for(i=0;i<4;i++){
    if(q.answers[i][0]!=0){
      printf("%i. valasz: %s\n",i+1, q.answers[i]);
    }
  }
  printf("%s\n", ctime(&(q.date)));
}

void listAllQuestions(struct Question * q,int db)
{
  int i;
  for(i=0;i<db;i++)
  {
    printf("%i.kerdes:\n",i+1);
    writeQuestion(q[i]);
  }
}

void listQuestions(struct Question * q,int db)
{
  int chosen=-1;
  char ans[5];
  while(chosen<0||chosen>db)
  {
  printf("Adja meg hanyadik kerdest szeretne listazni![1-%i, 0 - osszes]\n",db);
  readConsole(ans,sizeof(ans));
  chosen=atoi(ans);
  }
  if(chosen==0)
  {
    listAllQuestions(q,db);
  }
  else
  {
    printf("%i.kerdes:\n",chosen);
    chosen--;
    writeQuestion(q[chosen]);
  }
}

void addQuestion(struct Question* q, int *db)
{
  printf("Adja meg a kerdes szoveget!\n");
  readConsole(q[(*db)].question,sizeof(q[(*db)].question));
  int answer_amount=0;
  while(answer_amount<2||answer_amount>4)
  {
    char ans_a[2];
    printf("Hany valaszlehetoseg lesz?[2-4]\n");
    readConsole(ans_a,sizeof(ans_a));
    answer_amount=atoi(ans_a);
  }

  int i;
  for(i=0;i<answer_amount;i++)
  {
    printf("%i. valasz szovege: ",i+1);
    readConsole(q[(*db)].answers[i],sizeof(q[(*db)].answers[i]));
  }
  q[(*db)].date=time(NULL);
  (*db)++;
  if((*db)==sizeof(q)){
    q=(struct Question*)realloc(q,sizeof(q)+100);
  }
}

void modifyQuestion(struct Question* q, int db)
{
  listAllQuestions(q,db);
  int chosen=-1;
  char ans[5];
  while(chosen<0||chosen>db)
  {
  printf("Adja meg hanyadik kerdest szeretne modositani![1-%i, 0 - megse]\n",db);
  readConsole(ans,sizeof(ans));
  chosen=atoi(ans);
  }
  if(chosen!=0)
  {
    chosen--;
    int ansCount=0;
    while(q[chosen].answers[ansCount][0]!=0)
    {
      ansCount++;
    }
    int part=-1;
    ansCount==4 ? ansCount : (++ansCount);
    while(part<0||part>ansCount){
      printf("Adja meg, hogy a kerdest[0], vagy valamelyik valaszt[1-%i] szeretne modositani!\n",ansCount);
      readConsole(ans,sizeof(ans));
      part=atoi(ans);
    }
    if(part==0)
    {
      printf("Adja meg a kerdes szoveget\n");
      readConsole(q[chosen].question,sizeof(q[chosen].question));
    }
    else
    {
      printf("%i. valasz szovege: ",part);
      part--;
      ansCount--;
      readConsole(q[chosen].answers[part],sizeof(q[chosen].answers[part]));
      while(part<ansCount && q[chosen].answers[part][0]==0)
      {
        strcpy(q[chosen].answers[part],q[chosen].answers[part+1]);
        q[chosen].answers[part+1][0]=0;
        part++;
      }
    }
  }
}

void removeQuestion(struct Question* q, int *db)
{
  listAllQuestions(q,(*db));
  int chosen=-1;
  char ans[5];
  while(chosen<0||chosen>(*db))
  {
  printf("Adja meg hanyadik kerdest szeretne torolni![1-%i, 0 - megse]\n",(*db));
  readConsole(ans,sizeof(ans));
  chosen=atoi(ans);
  }

  chosen--;
  while(chosen<(*db)){
    strcpy(q[chosen].question,q[chosen+1].question);
    int i;
    for(i=0;i<4;i++){
      strcpy(q[chosen].answers[i],q[chosen+1].answers[i]);
    }
    q[chosen].date=q[chosen+1].date;
    chosen++;
  }
  (*db)--;
}

void finalizeQuestions(struct Question* q, int db)
{
  int final_amount=-1;
  char ans[30];
  while(final_amount<0||final_amount>db)
  {
    printf("Adja meg  hany kerdesbol alljon a kerdoiv![1-%i, 0 - megse]\n",db);
    readConsole(ans,sizeof(ans));
    final_amount=atoi(ans);
  }
  if(final_amount!=0)
  {
    int * finals;
    finals=(int *)malloc(final_amount*sizeof(int));
    int i;
    listAllQuestions(q,db);
    for(i=0;i<final_amount;i++)
    {
      int chosen=-1;
      while(chosen<0||chosen>db)
      {
        printf("Adja meg a kerdoiv %i. kerdeset![1-%i]\n",i+1,db);
        readConsole(ans,sizeof(ans));
        chosen=atoi(ans);
        chosen--;
        int j=0;
        while(j<i && chosen!=finals[j]){
          j++;
        }
        if(j!=i){
          chosen=-1;
          printf("Ez a kerdes mar szerepelt! ");
        }
      }
      finals[i]=chosen;
    }
    int f;
    printf("Adja meg a fajl nevet, ahova a vegleges kerdoivet menteni szeretne!\n");
    readConsole(ans, sizeof(ans));
    f=open(ans,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
    if (f<0){ perror("Error at opening the file\n");exit(1);}
    for(i=0;i<final_amount;i++){
        write(f, &q[finals[i]], sizeof(q[finals[i]]));
    }
    close(f);
    free(finals);
  }
}

int main(int argc,char** argv){
  struct Question *q;
  q=(struct Question*)malloc(100*sizeof(struct Question));
  int db=0;
  char choice[2];
  char filename[30];
  int f;
  printf("Szeretne megnyitni korabban mentett kerdeseket? (I/N)\n");
  readConsole(choice, sizeof(choice));
  if(choice[0]=='I'){
    printf("Adja meg a fajl nevet!\n");
    readConsole(filename, sizeof(filename));
    f=open(filename,O_RDONLY); 
    if (f<0){ perror("Error at opening the file\n");exit(1);}
    while(read(f, &q[db], sizeof(q[db]))){
      db++;
      if(db==sizeof(q)){
        q=(struct Question*)realloc(q,sizeof(q)+100);
      }
    }
    close(f);
  }
  
  while(choice[0]!='0')
  {
    writeMenu();
    readConsole(choice, sizeof(choice));
    switch(*choice) {
    case ('1')  :
      listQuestions(q,db);
      break;
    
    case ('2')  :
      addQuestion(q,&db);
      break;
    
    case ('3')  :
      modifyQuestion(q,db);
      break;
    
    case ('4')  :
      removeQuestion(q,&db);
      break;
    
    case ('5')  :
      finalizeQuestions(q,db);
      break;
    }
  }

  printf("Szeretne menteni a modositasokat? (I/N)\n");
  readConsole(choice, sizeof(choice));
  if(choice[0]=='I'){
    if(filename[1]==0)
    {
      printf("Adja meg a fajl nevet, ahova a kerdeseket menteni szeretne!\n");
      readConsole(filename, sizeof(filename));
    }
    f=open(filename,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
    if (f<0){ perror("Error at opening the file\n");exit(1);}
    write(f, q, db*sizeof(*q));
    close(f);
  }
  free(q);
  return 0;
}


