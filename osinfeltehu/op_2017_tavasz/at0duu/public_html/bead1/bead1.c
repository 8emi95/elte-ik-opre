#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

typedef struct data {
    char * id;
    char * date;
    char * kerdes;
    char * valasz1;
    char * valasz2;
    char * valasz3;
    char * valasz4;
    char * valasz5;
} data;

char * DateTime(){
    time_t timer;
    char buffer[26];
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y,%m,%d,%H,%M,%S", tm_info);
    puts(buffer);
    char * tmp = buffer;
    return  tmp;
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

struct data proc_line(char * line){
   char** arr;
   arr = (char**)malloc(sizeof(char**)*7);
   char *token = NULL;
   int n_tokens = 0;
   token = strtok (line,";");
   int i = 0;
   while (token != NULL)
   {
       arr[i]= token;
       i++;
       token = strtok (NULL, ";");
       n_tokens++;
   }
   struct data rekord = {arr[0],arr[1],arr[2],arr[3],arr[4],arr[5],arr[6],arr[7]};
   return rekord;
}

int length(char * str)
{
 int l=0;
 while (*str++!=0)l++; //*str points to the next character
 return l;
}
/*
char * osszefuz(char * egyik, char * masik){

  char *str1 ;
  char *str2 ;
  char *str3;
  int l1,l2,l3;
  str1  = (char*)malloc(10);
  str2  = (char*)malloc(10);
  strcpy(str1,egyik );
  strcpy(str2,masik );

  l1 = strlen(str1)+1;
  l2 = strlen(str2)+1;

  l3 = l1 + l2; 


  str3 = (char *)malloc(l3);
  str3 = strcat(str1,str2);
  
  //printf("%s\n",str3);
  return str3;
}
*/
int max_id(char * fname){
    FILE * f;
    int que_pc = count_line(fname);
    f=fopen(fname,"r");
    if (f==NULL){perror("File opening error\n"); exit(1);}
    char line[1600];
    int i = 0;
    int max =0;
    struct data tmp;
    while (!feof(f)){
        fgets(line,sizeof(line),f);
        tmp = proc_line(line);
        if (atoi(tmp.id)>max){
            max = atoi(tmp.id);
        }
    }
    fclose(f);
    return max;
}

void add_que(char * fname){
    FILE * pfile;
    char * line = "\n";
    char * tmp;
    int fstatus = 0;
    if( access( fname, F_OK ) != -1 ) {
       fstatus = 1;
    }
    pfile=fopen(fname, "ab");
    int answ_pc = 0;
    char tmp2[200];
    strcpy(tmp2, DateTime());
    if(fstatus == 1){
        fprintf(pfile, "%s", line);
        fprintf(pfile, "%d;", max_id(fname)+1);
    }else{
        fprintf(pfile, "%d;", 1);
        fstatus = 1;
    }
    fprintf(pfile, "%s;", DateTime());
    printf("Kérdés: ");
    char str[1025];
    //scanf("%1024s", &str);
    scanf("\n%[^\n]", &str);
    fprintf(pfile, "%s;", str);
    while(answ_pc<2 || answ_pc>4){
        printf("Válaszok száma: ");
        scanf("%d", &answ_pc);
    }
    int i;
    for(i=0;i<answ_pc;i++){
        printf("%d. válasz: ",i+1);
        //scanf("%s",&str);
        scanf("\n%[^\n]", &str);
        fprintf(pfile, "%s;", str);
    }
    fclose(pfile);
}

void copy(char *source, char *dest){
    FILE *fp1, *fp2;
   char a;
 
   fp1 = fopen(source, "r");
   if (fp1 == NULL) {
      puts("cannot open this file");
      exit(1);
   }
 
   fp2 = fopen(dest, "wb");
   if (fp2 == NULL) {
      puts("Not able to open this file");
      fclose(fp1);
      exit(1);
   }
 
    a = fgetc(fp1);
    while (a != EOF){
      fputc(a, fp2); 
      a = fgetc(fp1);
    }

/*
   do {
      a = fgetc(fp1);
      fputc(a, fp2);
   } while (a != EOF);
 */
   fcloseall();
}

void delete_emp(char *fname, int line_c){
    FILE *fp1, *fp2;
    char a;
    int line_len;
    int i = 1;
 
   fp1 = fopen(fname, "r");
   if (fp1 == NULL) {
      puts("cannot open this file");
      exit(1);
   }
 
   fp2 = fopen("tmp2.dat", "wb");
   if (fp2 == NULL) {
      puts("Not able to open this file");
      fclose(fp1);
      exit(1);
   }
   char line[1600];
   while (i<line_c){
        fgets(line,sizeof(line),fp1);
        line_len = length(line);
        if(line_len>1){
            fprintf(fp2, "%s", line);
            i++;
        }
   }
 fcloseall();
    copy("tmp2.dat",fname);
    int t = remove("tmp2.dat");
}


void del_que(char * fname, int del_num){
    FILE * f;
    FILE * f2;
    int que_pc = count_line(fname);
    f=fopen(fname,"r");
    f2 = fopen("tmp.dat","wb");
    if (f==NULL){perror("File opening error\n"); exit(1);}
    char line[1600];
    int i = 0;
    char * ures;
    struct data tmp;
    while (!feof(f)){
        fgets(line,sizeof(line),f);
        if (line != "\r\n"){
            tmp = proc_line(line);
            if (atoi(tmp.id) != del_num){
                fprintf(f2, "%d;%s;%s;%s;%s",atoi(tmp.id),tmp.date,tmp.kerdes,tmp.valasz1,tmp.valasz2);
                if (tmp.valasz4!=NULL){
                    fprintf(f2, ";%s", tmp.valasz3);
                    if (tmp.valasz5!=NULL){
                        fprintf(f2, ";%s", tmp.valasz4);
                    //}else{fprintf(f2, "\n");}
                //}else{fprintf(f2, "\n");}
                    }
            }
        }
            fprintf(f2, "\n");
        }
    }
    fclose(f);
    fclose(f2);
    copy("tmp.dat",fname);
    int a = remove("tmp.dat");
    delete_emp(fname, que_pc);
}

void list_ques(char* fname){
    FILE * f;
    int que_pc = count_line("proba001.dat");
    //records = (struct data *)malloc(sizeof(struct data)*5000);
    f=fopen(fname,"r");
    if (f==NULL){perror("File opening error\n"); exit(1);}
    char line[1600];
    int i = 0;
    struct data tmp;
    while (!feof(f)){
        fgets(line,sizeof(line),f);
        tmp = proc_line(line);   
        i++;
        printf("%d.: %s %s/%s/%s/%s",atoi(tmp.id),tmp.kerdes,tmp.valasz1,tmp.valasz2,tmp.valasz3,tmp.valasz4);
        /*if (tmp.valasz4!=NULL){
            printf("/%s", tmp.valasz3);
            if (tmp.valasz5!=NULL){
            printf("/%s", tmp.valasz4);
        }
        }*/
        printf("\n");
    }
    fclose(f);
}

void ch_que(char * fname, int num){
    del_que(fname,num);
    FILE * pfile;
    char * line = "\n";
    char * tmp;
    pfile=fopen(fname, "a");
    int answ_pc = 0;
    char tmp2[200];
    strcpy(tmp2, DateTime());
    //fprintf(pfile, "%s", line);
    fprintf(pfile, "%d;", num);
    fprintf(pfile, "%s;", DateTime());
    printf("Kérdés: ");
    char str[1025];
    //scanf("%1024s", &str);
    scanf("\n%[^\n]", &str);
    fprintf(pfile, "%s;", str);
    while(answ_pc<2 || answ_pc>4){
        printf("Válaszok száma: ");
        scanf("%d", &answ_pc);
    }
    int i;
    for(i=0;i<answ_pc;i++){
        printf("%d. válasz: ",i+1);
        //scanf("%s",&str);
        scanf("\n%[^\n]", &str);
        fprintf(pfile, "%s;", str);
    }
    fclose(pfile);
}

void ok_que(char * fname){
    FILE * f;
    FILE * f2;
    f2 = fopen("nyomtat.txt","wb");
    int ch_num = -1;
    printf("Adja meg a veglegesitett sorszamokat, ha vegzett adjon meg egy 0-t:");
    while (ch_num != 0){
        scanf("%d", &ch_num);
        f=fopen(fname,"r");
        if (f==NULL){perror("File opening error\n"); exit(1);}
        char line[1600];
        struct data tmp;
        while (!feof(f)){
            fgets(line,sizeof(line),f);
            tmp = proc_line(line);   
            if (atoi(tmp.id) == ch_num){
                fprintf(f2, "Kerdes: %s \n Valaszok: %s/%s",tmp.kerdes,tmp.valasz1,tmp.valasz2);
                if (tmp.valasz3!=NULL){
                    fprintf(f2, "/%s", tmp.valasz3);
                }
                if (tmp.valasz4!=NULL){
                    fprintf(f2, "/%s", tmp.valasz4);
                }
                fprintf(f2, "\n");
            }        
        }
        fclose(f);
    }
    printf ("Elkeszult a lista: nyomtat.txt neven!");
    fclose(f2);    
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
        case 1: add_que("proba001.dat");
            break;
        case 2: {int ch_num;printf("Modositando kerdes sorszama:");scanf("%d", &ch_num);ch_que("proba001.dat",ch_num);}
            break;
        case 3: {int del_num;printf("Torlendo kerdes sorszama:");scanf("%d", &del_num);del_que("proba001.dat",del_num);}
            break;
        case 4: list_ques("proba001.dat");
            break;
        case 5: ok_que("proba001.dat");
            break;
        case 0: printf("Kilepes\n"); 
            break;
        default: printf("Rossz valasztas\n");
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