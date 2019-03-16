
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct {
  struct tm created;  
  unsigned char marked;   
  char text[200];
  char answers[4][200];   
  unsigned char sent;
} question_t;

question_t questions[30];
unsigned int size=0;


unsigned int getint(char* out) 
{
   unsigned int result;
   printf("%s: ", out);
   scanf(" %d", &result);
   return (result);
}

void getstr(char* out, char* res)
{
   char c;
   unsigned int i=0;   

   printf("%s: ", out);
   scanf("\n");
   do 
   {
      c=fgetc(stdin);
      if (c!='\n') 
      {
        if (i<200) 
        {
           res[i]=c;
           res[i+1]='\0';
           i++;
        }
      }
   } while ((c!='\n')&&(i<200));
}

void readData() 
{
   FILE* in;
   unsigned int i;

   for (i=0; i<30; i++) {
     questions[i].text[0]='\0';
   }
   i = 0;
   in=fopen("data.bin", "rb");
   if (in!=0) 
   {
     size = 0;   
     while (feof(in)==0) 
     {
       fread(&questions[i], sizeof(question_t), 1, in); 
       if (feof(in)==0) size++;
       i++;
     }   
     fclose(in);
   }
}

void writeData() 
{
   FILE* out=0;
   unsigned int i=0;

   out=fopen("data.bin", "wb");  
   if (out!=0) 
   {
      for (i=0; i<30; i++) 
      {
        if (questions[i].text[0]!= '\0') fwrite(&questions[i], sizeof(question_t), 1, out);
      }
      fclose(out);
   }
}

void printQuestion(unsigned int i, question_t* question, unsigned char preview, int* results) 
{
   if (preview == 0) 
   {
      printf("\nid: %d\n", i);   
      printf("created: %.4d-%.2d-%.2d-%.2d:%.2d:%.2d\n", question->created.tm_year + 1900, question->created.tm_mon, 
        question->created.tm_mday, question->created.tm_hour, question->created.tm_min, question->created.tm_sec);
      printf("marked: %s\n", (question->marked == 1) ? "true" : "false");
      printf("\n");   
      printf("question: %s\n", question->text);   
      printf("\n");   
      for (i=0; i<4; i++) if (question->answers[i][0]) printf("answer %.1d: %s\n", i+1, question->answers[i]);   
   } 
   else 
   {
      printf("\n%s\n", question->text);   
      printf("\n");   
      for (i=0; i<4; i++) if (question->answers[i][0]) printf("%.1d) %s\n", i+1, question->answers[i]);   
      if (results != NULL) 
      {
        printf("\nanswers:\n");   
        for (i=0; i<4; i++) if (question->answers[i][0]) printf("%.1d) %d\n", i+1, results[i]);   
      }
   }
   printf("\n");   
}

int main(int argc, char* argv[]) {
  unsigned int i, q;
   
  srand(time(NULL));
   
  readData();
  
  if (argc > 1) {
    int in=0;
    int out=0;
    char buffer[4096];
    int f=0;
    int n=0;
    int results[4]={0, 0, 0, 0};
    int p=0;

    if (strcmp(argv[1], "client")==0) 
    {
      unlink("client");
      f = mkfifo("client", S_IRUSR | S_IWUSR);
  
      if (f==0) 
      {
        printf("connecting to server...\n");    
   
        out=open("server", O_WRONLY | O_NONBLOCK);
        if (out!=-1) 
        {
          int persons=(rand()%10)+10;
          question_t question;
         
          sprintf(buffer, "%s", argv[2]);
          write(out, buffer, 4096);
      
          printf("connected\n");
          in=open("client", O_RDONLY);
  
          for (i=0; i<3; i++) 
          {
            read(in, &question, sizeof(question_t));
            printf("question received\n");  
            
            p=persons;
            while (p>0) 
            {
              int answer=rand()%4;
              if (question.answers[answer]) 
              {
                p--;
                results[answer]++;
              }    
            }
            
            printQuestion(i+1, &question, 1, results);

            write(out, results, sizeof(results));  
            printf("answers sent\n");  
          }
          
          printf("disconnected\n");      
        } else printf("server is unreachable!\n");  
      }
  }
    else if (strcmp(argv[1], "server")==0)
    {
      for (i=0; i<size; i++) if (questions[i].marked) n++;
        
      if (n >= 3) 
      {
        printf("running as server\n");
        
        unlink("server");
        f=mkfifo("server", S_IRUSR | S_IWUSR);

        if (f==0) 
        {
  
          printf("waiting for client ...\n");
          
          in = open("server", O_RDONLY);
          if (in!=-1) 
          {
            read(in, buffer, 4096);

            printf("client \"%s\" connected\n", buffer);
            out = open("client", O_WRONLY | O_NONBLOCK);
      
            for (i=0; i<size; i++) questions[i].sent=0; 
  
            for (i=0; i<3; i++) 
            {
              int question=-1;             
              while (question==-1) 
              {
                q=rand()%size;
                if ((questions[q].marked) && (!questions[q].sent)) question=q;
              }
              
              write(out, &questions[question], sizeof(question_t));
              printf("question sent\n");

              read(in, &results, sizeof(results));
              printf("answers received\n");

              printQuestion(i+1, &questions[question], 1, results);
                
              questions[question].sent=1;      
            }
      
            printf("client disconnected\n");
            close(out);
          }           

          close(in);
        }     
      }
      else
      {
        printf("\nnot enough questions!\n");
      }
    }
  } else {
    while (1) {
      printf("1 - insert question\n");
      printf("2 - modify question\n");
      printf("3 - delete question\n");
      printf("4 - modify answer\n");
      printf("5 - delete answer\n");
      printf("6 - list all questions\n");
      printf("7 - mark/unmark question\n");
      printf("8 - preview\n");
      printf("x - exit\n");
        
      char menu=' ';
      printf("enter menu: ");
      scanf(" %c", &menu);  

      switch (menu) 
      {
      case '1': 
      {        
        time_t t=time(NULL);
        questions[size].created=*localtime(&t);
        getstr("question text ? ", questions[size].text);               
        writeData();
        readData();
        break;  
      }
      case '2':
      {
        i=getint("question number ? ");

        if ((i>0)&&(i<=size)) 
        {
          printQuestion(i, &questions[i-1], 0, NULL);
          getstr("question text ? ", questions[i-1].text);               
          writeData();
        }   
        break;  
      }
      case '3': 
      {
        i=getint("question number ? ");
         
        if ((i>0)&&(i<=size)) 
        {
          questions[i-1].text[0]='\0';
          writeData();
          readData();
        }
        break;  
      }
      case '4' : {
        i=getint("question number ? ");
         
        if ((i>0)&&(i<=size)) 
        {
          unsigned int j=0;
          printQuestion(i, &questions[i-1], 0, NULL);
              
          j=getint("answer number ? ");
              
          if ((j>0)&&(j<=4)) 
          {
            getstr("answer text ? ", questions[i-1].answers[j-1]);
            writeData();
            printQuestion(i, &questions[i-1], 0, NULL);
          }
        }
        break;  
      }
      case '5' : {
        i=getint("question number ? ");
         
        if ((i>0)&&(i<=size)) 
        {
          unsigned int j=0;
          printQuestion(i, &questions[i-1], 0, NULL);
              
          j=getint("answer number ? ");
              
          if ((j>0)&&(j<=4)) 
          {
            questions[i-1].answers[j-1][0]='\0';
            writeData();
            printQuestion(i, &questions[i-1], 0, NULL);
          }
        }
        break;  
      }
      case '6': 
      {
        unsigned int n=0;
        for (i=0; i<size; i++) if (questions[i].text[0]) printQuestion(++n, &questions[i], 0, NULL);            
        if (n == 0) printf("\nno entries!\n\n");
        break;  
      }
      case '7':
      {
        i=getint("question number ? ");
         
        if ((i>0)&&(i<=size)) 
        {
          unsigned int n=0;
          unsigned int j=0;              

          for (j=0; j<4; j++) if (questions[i-1].answers[j][0]) n++;                   

          if (n>=2) 
          {
            questions[i-1].marked=1-questions[i-1].marked;
            printQuestion(i, &questions[i-1], 0, NULL);
            writeData();
          } 
          else printf("\nnot enough answers!\n\n");
        }
        break;
      }
      case '8': 
      {
        unsigned int n=0;

        printf("\npreview\n\n");
        for (i=0; i<size; i++) if ((questions[i].text[0]) && (questions[i].marked==1)) printQuestion(++n, &questions[i], 1, NULL);            
        if (n == 0) printf("\nno entries!\n\n");
        break;  
      }
      case 'x': 
      {
        writeData();
        return;
      }
      }
    }
  }
   
   return 0;
} 
