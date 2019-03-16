#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct question
{
 char queNum[100]; 
 char questionE[100]; 
 char posAns[100]; 
 char answer[100];
}queS;
//    FUNCTION TO INSERT RECORDS TO THE FILE
void insert()
{
	int a;
	int num;
	FILE *fp;
	fp = fopen("banka3.txt", "a");
	
	
	char moja[100];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    strftime(s, sizeof(s), "%c", tm);
    strncpy(moja, s, 100);
    strcat(moja, "\n");
    fprintf(fp, "\n%s", moja);
	
	printf("**************Insert new question**************\n");
	printf("Enter the que num: ");
	fgets(queS.queNum, 100, stdin);
	printf("Enter the que: ");
	fgets(queS.questionE, 100, stdin);
	
	fprintf(fp,"Question num : %s", queS.queNum);
	fprintf(fp,"Question : %s", queS.questionE);
	
	do
	{
	printf("Enter how much answer you gona write to this que (1-4): ");
	fgets(queS.posAns, 100, stdin);
	num = atoi(queS.posAns);
	} while (num > 4 || num == 0);
	
	fprintf(fp,"Posible answer : %d\n", num);
	
	while(a != num)
  	{
  	printf("\n Enter answer number %d : ", a+1);
  	fgets(queS.answer, 100, stdin);
  	fprintf(fp,"Answer %d: %s", a+1, queS.answer);
  	a++;
  	}
  	
	fclose(fp);
}
//    FUNCTION TO DISPLAY RECORDS 
void disp()
{
	FILE *f1;
	f1 = fopen("banka3.txt" ,"r");
    int c;
	if (f1) {
    while ((c = getc(f1)) != EOF)
    putchar(c);
    fclose(f1);
	}
}
//    FUNCTION TO MODIFY A QUESTION
void modify()
{
	
}
int main()
{
        insert();
	/*
	int l;
 	do
 	{
 		printf("\n**********************************************\n");
        printf("  SURVEY - The Society of Hungarian Ball Games");
        printf("\n**********************************************\n");
        printf("1. Add New Question\n");
        printf("2. Display Question\n");
        printf("3. Modify Question\n");
        printf("4. Exit");
        printf("\n**********************************************\n");
        printf("\nEnter your choice: ");
  		scanf("%d", &l);
  		printf("\n");
  		
  		switch(l)
  		{
  			case 1:
			  insert();
			break;
			case 2:
				disp();
			break;
			
			default:
   				printf("\nYour choice is wrong\nPlease try again...\n");
   			break;
				
				  	
		}
  		
	 } while (l != 4)*/
	return 0;
}
