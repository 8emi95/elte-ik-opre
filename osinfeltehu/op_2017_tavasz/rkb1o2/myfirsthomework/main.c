#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>



void showFile()
{ int game;
  game = open("question.txt",O_RDONLY);
  if (game < 0){ perror("Error at opening the file\n");exit(1);}

  printf( " HELLO! Welcome to the Hungarian Ball Games :\n" );
    char c;
  while (read(game,&c,sizeof(c))){
    printf("%c",c);
  }
  close(game);
}

void addQestion()
{ int game;
  game = open("question.txt", O_RDWR|O_APPEND);
  if (game <0){ perror("Error at opening the file\n");exit(1);}
  char *rs;
  rs=malloc(30);
    char *firstanswer;
  firstanswer=malloc(30);
  char *secondanswer;
  secondanswer=malloc(30);
  char *thirdanswer;
  thirdanswer=malloc(30);
  char *fourthanswer;
  fourthanswer=malloc(30);
  int nu =0;

  char c;

  while (read(game,&c,sizeof(c))){

  if (c == '\n')
        {
            nu++;
        }

    }
  int numbersf=nu/6+1;

   FILE *f;
   f = fopen("question.txt", "a");

   printf("Please, Ask your question\n");
   fgets(rs,30,stdin);
   printf("You wrote: %d. %s", numbersf, rs);
   fprintf(f,"%d. %s", numbersf, rs);

   printf("Please, write your 1st anwer\n");
   fgets(firstanswer,30,stdin);
   printf("You wrote: %d.a. %s", numbersf, firstanswer);
   fprintf(f,"%d.a. %s", numbersf, firstanswer);

   printf("Please, write your 2nd answer\n");
   fgets(secondanswer,30,stdin);
   printf("You wrote: %d.b. %s", numbersf, secondanswer);
   fprintf(f,"%d.b. %s", numbersf, secondanswer);

   printf("Please, write your 3rd answer\n");
   fgets(thirdanswer,30,stdin);
   printf("You wrote: %d.c. %s", numbersf, thirdanswer);
   fprintf(f,"%d.c. %s", numbersf, thirdanswer);

   printf("Please, write your 4th answer\n");
   fgets(fourthanswer,30,stdin);
   printf("wrote: %d.d. %s", numbersf, fourthanswer);
   fprintf(f,"%d.d. %s", numbersf, fourthanswer);

   struct stat st;
   stat("quest.txt",&st);
   time_t t=st.st_mtime;
   printf("The question was added at: %s\n",ctime(&t));
   fprintf(f,"The question %d was added at: %s\n", numbersf, ctime(&t));

   fclose(f);

}

void editQuestion(){
FILE *fp,*fc;
int lineNum;
    int number=0;
    int ch;
    int num=0;
  char insertoutput[5];
    char t[30];

    fp=fopen("question.txt","r");
    fc=fopen("qu.txt","w");

    if(fp==NULL||fc==NULL)
    {
        printf("\nError, can not open the files");

    }

    printf("\n Write the number of the line that you want to edit: ");
    fgets(insertoutput,sizeof(insertoutput),stdin);
  lineNum=atoi(insertoutput);

    while((ch=fgetc(fp))!=EOF)
    {
        if(ch=='\n')
            number++;
        if(number==lineNum-1 && num==0)
        {
            printf("\nEnter input to store at line %d:",lineNum);
      fgets(t,30,stdin);
            if(number==0)
                fprintf(fc,"%s\n",t);
            else
                fprintf(fc,"\n%s",t);

            num=1;

            while( (ch=fgetc(fp))!=EOF )
            {
                if(ch=='\n')
                    break;
            }
       }
       else
          fprintf(fc,"%c",ch);
    }
    fclose(fp);
    fclose(fc);

    if(num==1)
        printf("\nThe line was changed successfully.");
    else
        printf("\n Can not find the line");

    remove("question.txt");
    rename("qu.txt", "question.txt");
    printf("\n The file after editing as follows:\n");
   FILE * openfile;
    openfile = fopen("question.txt", "r");
    ch = getc(openfile);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = getc(openfile);
    }
    fclose(openfile);
}

void deleteQestion()
{
    FILE *openfile, *openfile1;
    int delquestion, qu = 1;
    char ch;


    openfile = fopen("question.txt", "r");
    ch = getc(openfile);
   while (ch != EOF)
    {
        printf("%c", ch);
        ch = getc(openfile);
    }

    rewind(openfile);
    printf(" \n write the number of the question to delete it :\n");
    scanf("%d", &delquestion);

    openfile1 = fopen("qu.txt", "w");
    ch = getc(openfile);
    while (ch != EOF)
    {
        ch = getc(openfile);
        if (ch == '\n')
        {
            qu++;
        }

        if (qu!=(delquestion-1)*6+1 && (qu!=(delquestion-1)*6+2) && (qu!=(delquestion-1)*6+3) && (qu!=(delquestion-1)*6+4)
      && (qu!=(delquestion-1)*6+5) && (qu!=delquestion*6))
        {

            putc(ch, openfile1);
        }

    }
    fclose(openfile);
    fclose(openfile1);
    remove("question.txt");
    rename("qu.txt", "question.txt");
    printf("\n The  file editing as follows:\n");
    openfile = fopen("question.txt", "r");
    ch = getc(openfile);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = getc(openfile);
    }
    fclose(openfile);
}


int main()
{
    int input;
  char insertoutput[5];
    printf( "1. see the content of the folder\n" );
    printf( "2. insert new question and new answers\n" );
  printf( "3. edit the lines\n" );
  printf( "4. Delete questions\n" );
    printf( "5. logout\n" );
    printf( "Selection: " );
     fgets(insertoutput,sizeof(insertoutput),stdin);
  input=atoi(insertoutput);
    switch ( input ) {
        case 1:
            showFile();
            break;
        case 2:
            addQestion();
            break;

    case 3:
            editQuestion();
            break;
    case 4:
            deleteQestion();
            break;
        case 5:
            printf( "Thanks for checeking!\n" );
      exit(1);
            break;
        default:
            printf( "Please choose a number!\n" );
            break;
    }
    getchar();

}


