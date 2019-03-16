#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> //open,create
#include <sys/types.h> //open
#include <sys/stat.h>
#include <time.h> //ctime
#include <errno.h> //perror, errno
#include <unistd.h>

void showContent() 
{	int f; 
	f=open("test.txt",O_RDONLY); 
	if (f<0){ perror("Error at opening the file\n");exit(1);}
    
	printf( "Welcome to the Hungarian Ball Games! Here you can find all the available questions and answers:\n" ); 
    char c;
	while (read(f,&c,sizeof(c))){     
    printf("%c",c); 
	}
	close(f);	
}

void addNewQA()
{	int f; 
	f=open("test.txt", O_RDWR|O_APPEND); 
	if (f<0){ perror("Error at opening the file\n");exit(1);}
	char *q;
	q=malloc(30);
    char *a1;
	a1=malloc(30);
	char *a2;
	a2=malloc(30);
	char *a3;
	a3=malloc(30);
	char *a4;
	a4=malloc(30);
	char *a5;
	a5=malloc(30);
	int o=0;		
	
	char c;
	int g;
	while (read(f,&c,sizeof(c))){    
    //printf("%c",c); 
	if (c == '\n')
        {
            o++;
        }
             
    } // So I have the number of all lines. I need to divide them by 6 (1 question+4 answers+1 line for the time), so next line will be under order=o/6+1
	int order=o/6+1;	
	//printf("%s %d %d",b, order, o);
	//We will go to the end of the file, read the order of last question and append new question there under the incremented order
	
   FILE *p;
   p = fopen("test.txt", "a");  
   
   printf("Please, write down new question\n");
   fgets(q,30,stdin);   
   printf("You have entered: %d. %s", order, q); 
   fprintf(p,"%d. %s", order, q);       
   
   printf("Please, write down first answer\n");
   fgets(a1,30,stdin);   
   printf("You have entered: %d.1. %s", order, a1);
   fprintf(p,"%d.1. %s", order, a1);
   
   printf("Please, write down second answer\n");
   fgets(a2,30,stdin); 	
   printf("You have entered: %d.2. %s", order, a2);
   fprintf(p,"%d.2. %s", order, a2);
   
   printf("Please, write down third answer\n");
   fgets(a3,30,stdin); 
   printf("You have entered: %d.3. %s", order, a3);
   fprintf(p,"%d.3. %s", order, a3);
   
   printf("Please, write down fourth answer\n");
   fgets(a4,30,stdin); 
   printf("You have entered: %d.4. %s", order, a4);
   fprintf(p,"%d.4. %s", order, a4);
        
   //for the automatic time
   struct stat st;
   stat("test.txt",&st);  //fstat(file descriptor, stat structure)
   time_t t=st.st_mtime;
   printf("The question was added at: %s\n",ctime(&t)); //convert time_t to string 
   fprintf(p,"The question %d was added at: %s\n", order, ctime(&t));

   fclose(p);   
   
}

void deleteQwithA()
{
    FILE *fileptr1, *fileptr2;   
    char ch;
    int delete_question, temp = 1;  
 
    //open file in read mode
    fileptr1 = fopen("test.txt", "r");
    ch = getc(fileptr1);
   while (ch != EOF)
    {
        printf("%c", ch);
        ch = getc(fileptr1);
    }
    //rewind
    rewind(fileptr1);
    printf(" \nEnter number of the question to be deleted:\n");
    scanf("%d", &delete_question);
    //open new file in write mode
    fileptr2 = fopen("temp.txt", "w");
    ch = getc(fileptr1);
    while (ch != EOF)
    {
        ch = getc(fileptr1);
        if (ch == '\n')
        {
            temp++;
        }
        //except the line to be deleted
        if (temp!=(delete_question-1)*6+1 /*&& temp !=(delete_question+1)*6*/ && (temp!=(delete_question-1)*6+2) && (temp!=(delete_question-1)*6+3) && (temp!=(delete_question-1)*6+4) 
			&& (temp!=(delete_question-1)*6+5) && (temp!=delete_question*6))
        {
            //copy all lines in file temp.txt
            putc(ch, fileptr2);
        }
		
    }
    fclose(fileptr1);
    fclose(fileptr2);
    remove("test.txt");
    //rename the file temp.txt to original name
    rename("temp.txt", "test.txt");
    printf("\n The contents of file after being modified are as follows:\n");
    fileptr1 = fopen("test.txt", "r");
    ch = getc(fileptr1);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = getc(fileptr1);
    }
    fclose(fileptr1);
}

void modifyLineVersion2(){
FILE *fp,*fc;
	char inputasstring[5];
    int lineNum;  //stores line number which should be edited.
    int count=0;  //count number lines in source file.
    int ch;   //temporary place to store character of source file(one at a time).
    int flag=0;  //0=false and 1=true
    char t[30];   //temporary place to store input which you want to be replaced with error in text file.


    fp=fopen("test.txt","r");
    fc=fopen("temp.txt","w");

    if(fp==NULL||fc==NULL)
    {
        printf("\nError...cannot open/create files");
        
    }

    printf("\nEnter Line Number Which You Want to edit: ");
    fgets(inputasstring,sizeof(inputasstring),stdin);  
	lineNum=atoi(inputasstring);

    while((ch=fgetc(fp))!=EOF)
    {
        if(ch=='\n')  
            count++;
        if(count==lineNum-1 && flag==0)  
        {
            printf("\nEnter input to store at line %d:",lineNum);           
			fgets(t,30,stdin); 
            if(count==0)  
                fprintf(fc,"%s\n",t);   
            else 
                fprintf(fc,"\n%s",t);  

            flag=1;  

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

    if(flag==1)
        printf("\nThe line was changed successfully.");
    else
        printf("\nLine  is not found");	
	
    remove("test.txt");
    //rename the file temp.txt to original name
    rename("temp.txt", "test.txt");
    printf("\n The contents of file after being modified are as follows:\n");
	 FILE * fileptr1;
    fileptr1 = fopen("test.txt", "r");
    ch = getc(fileptr1);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = getc(fileptr1);
    }
    fclose(fileptr1);
}

int main()
{
    int input;
	char inputasstring[5];
	//int a=1;
    printf( "1. Show the content of file\n" );
    printf( "2. Add new question and answer\n" );   
	printf( "4. Modify selected line\n" );	
	printf( "5. Delete selected question\n" );
    printf( "7. Exit\n" );
    printf( "Selection: " );
     fgets(inputasstring,sizeof(inputasstring),stdin);  
	input=atoi(inputasstring);
	//while (a==1){
    switch ( input ) {
        case 1:            
            showContent();
            break;
        case 2:          
            addNewQA();
            break;
        
		case 4:         
            modifyLineVersion2();
            break;
		case 5:         
            deleteQwithA();
            break;
        case 7:        
            printf( "Thanks for visiting our Hungarian Ball game survey!\n" );
			exit(1);
            break;
        default:            
            printf( "Please enter a number in given range!\n" );
            break;
	//}
    }
    getchar();

}