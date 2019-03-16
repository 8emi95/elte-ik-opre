#include <time.h>
#include <stdlib.h>
#include<stdio.h>
#include <string.h>

typedef struct data {
    char * date;
    char * kerdes;
    char * valasz1;
    char * valasz2;
    char * valasz3;
    char * valasz4;
} data;

struct data proc_line(char * line){
   char** arr;
   arr = (char**)malloc(sizeof(char**)*6);
   char *token = NULL;
   int n_tokens = 0;
   token = strtok (line,";");
   int i = 0;
   while (token != NULL)
   {
       arr[i]= token;
       i++;
       //printf("\n%s", token);
       token = strtok (NULL, ";");
       n_tokens++;
   }
   struct data rekord = {arr[0],arr[1],arr[2],arr[3],arr[4],arr[5]};
   return rekord;
}

int length(char * str)
{
 int l=0;
 while (*str++!=0)l++; //*str points to the next character
 return l;
}

void use_fopen_text(char* fname){
    FILE * f;
    int que_pc = count_line("proba001.dat");
    struct data records[6000] = {0};
    //records = (struct data *)malloc(sizeof(struct data)*5000);
    f=fopen(fname,"r");
    if (f==NULL){perror("File opening error\n"); exit(1);}
    char line[1600];
    int i = 0;
    struct data tmp;
    while (!feof(f)){
        fgets(line,sizeof(line),f);
        tmp = proc_line(line);
        strcpy(records[i].date   ,tmp.date);
        strcpy(records[i].kerdes ,tmp.kerdes);
        strcpy(records[i].valasz1,tmp.valasz1);
        strcpy(records[i].valasz2,tmp.valasz2);
        strcpy(records[i].valasz3,tmp.valasz3);
        strcpy(records[i].valasz4,tmp.valasz4);
        i++;
        printf("\n%dkezdet: %s :veg",i,tmp.kerdes);
    } 
    for(i=0;i<que_pc;i++){
    printf("\n%deleje: %s :vege",i,records[0].kerdes);}
    printf("\n");
    fclose(f);
}

int count_line(char * filename){
    FILE *fp = fopen(filename,"r");
    int lines = 1;
    int ch = 0;
    while(!feof(fp))
    {
    ch = fgetc(fp);
    if(ch == '\n')
    {
        lines++;
    }
    }
    return lines;
}

char * DateTime(){
    time_t current_time;
    char* c_time_string;
    current_time = time(NULL);
    c_time_string = ctime(&current_time);
    return (c_time_string);
} 

void menu(){
    int choice;
 
do
{
 printf("Menu\n\n");
 printf("1. Kerdes hozzaadasa\n");
 printf("2. Kerdes modositasa\n");
 printf("3. Kerdes torlese\n");
 printf("4. Kerdesek listazasa\n");
 printf("5. Kerdesek veglegesitese\n");
 printf("0. Kilepes\n");
 scanf("%d",&choice);
   
 switch (choice)
 {
     case 1: printf(DateTime());
          break;
     case 2: use_fopen_text("proba001.dat");
          break;
     case 3: 
          break;
     case 4: /*Call function here to do the required operation*/
          break;
     case 5: /*Call function here to do the required operation*/
          break;
     case 0: printf("Goodbye\n"); 
         break;
     default: printf("Wrong Choice. Enter again\n");
                    break;
 } 
  
} while (choice != 0);
}

int main()
{
    menu();
}
  //printf("\e[1;1H\e[2J");
  //printf("Hello World!");