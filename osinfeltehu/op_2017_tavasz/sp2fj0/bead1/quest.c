#include <stdio.h>
#include <time.h>
#include "string.h"
#include <string.h>
#include <stdlib.h>
#include "quest.h"
#include <unistd.h>

int menu(void)
{
    int option;
    printf("\n****MLSZ quest maker****\n\n");
    printf("Menu:\n");
    printf("1.\tCreate a new file\n");
    printf("2.\tList the quest\n");
    printf("3.\tAdd a new question/answer\n");
    printf("4.\tModify a question/answer\n");
    printf("5.\tDelete a question/answer\n");
    printf("6.\tEmpty the file\n");
    printf("7.\tFinalize\n");
    printf("8. \tList a finalized file\n");
    printf("0.\tQuit\n\n");

    //scanf(" %i", &option);
    while( (scanf(" %d", &option) != 1) //nem szam
            || (option < 0)                //tul kicsi a szam
            || (option > 8))              //tul nagy a szam
    {
        fflush(stdin);                    //buffer uritese
        printf("That selection invalid. Please try again.\n");
    }


    return option;
}

void createNewFile(char* fileName, int *isCreated)
{
    FILE *fp = fopen(fileName, "a+");
    fclose(fp);
    *isCreated = 1;
    printf("\t Temp file has been successfuly created!\n\n");
}

void listForMlsz(char a[18][1000])
{
	char line[1000];
	FILE *fp = fopen("backup.txt","r");
	int i = 0;
	while(fgets(line,sizeof(line),fp))
	{
		strcpy(a[i], line);
		i++;
	}
	fclose(fp);
}

void listQuest(char* fileName)
{
    printf("\tListing...\n\n");
    char line[1000];
    FILE *fp = fopen(fileName,"r");
    int i=0;
    while (fgets(line, sizeof(line), fp))
    {
        ++i;
        if(i % 6 == 1)
        {
            int number = (i/6)+1;
            printf("%i. question: ", number );
        }
        if(i % 6 == 2) printf("\t answer 1:\t" );
        if(i % 6 == 3) printf("\t answer 2:\t" );
        if(i % 6 == 4 && strcmp(line, "NULL\n") != 0) printf("\t answer 3:\t" );
        if(i % 6 == 5 && strcmp(line, "NULL\n") != 0) printf("\t answer 4:\t" );
        if(i % 6 == 0) printf( "\t\t time: ");
        if(strcmp(line, "NULL\n") != 0)
            printf("%s", line);
    }
    fclose(fp);

}

int newQuestion(char* fileName)
{
    char option;
    char question[1000];
    char answer1[1000];
    char answer2[1000];
    char answer3[1000] = "NULL\n";
    char answer4[1000]= "NULL\n";


    flush();
    printf("Question:\t");
    fgets(question, 1000, stdin);
    printf("1. Answer:\t");
    fgets(answer1, 1000, stdin);
    printf("2. Answer:\t");
    fgets(answer2, 1000, stdin);
    printf("Add 3rd answer? (y/n):\t");
    scanf("%c", &option);
    flush();
    if(option == 'y')
    {
        printf("3. Answer:\t");
        fgets(answer3, 1000, stdin);;
        printf("Add 4rd answer? (y/n):\t");
        scanf("%c", &option);
        if(option == 'y')
        {
            printf("4. Answer:\t");
            flush();
            fgets(answer4, 1000, stdin);
        }
    }

    int regnumber = countLines(fileName);
    writeToFile(fileName, question, answer1, answer2, answer3, answer4);
    return ((regnumber+1)/6);

    return 1;

}

void flush()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int countLines(char* fileName)
{
    FILE *fp = fopen(fileName, "r");
    char c;
    int lines=1;
    while((c=fgetc(fp)))
    {
        if(c == EOF) break;
        if( c == '\n' )
        {
            lines++;
        }
    }
    fclose(fp);
    return lines;
}

void writeToFile(char* fileName, char* question, char* answer1, char* answer2, char* answer3, char* answer4)
{
    FILE *fp = fopen(fileName, "a");
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf ( "Current local time and date: %s", asctime (timeinfo) );

    fprintf(fp, "%s%s%s%s%s%s", question, answer1, answer2, answer3, answer4, asctime (timeinfo));
    fclose(fp);
}

void modifyQuestion(char* fileName)
{
    if(countLines(fileName) > 5)
    {
    listQuest(fileName);
    int option;
    char cOption;
    char question[1000];
    char answer1[1000];
    char answer2[1000];
    char answer3[1000] = "NULL\n";
    char answer4[1000]= "NULL\n";

    printf("Question to modify: ");
    while( (scanf(" %d", &option) != 1) //nem szam
            || (option < 1)                //tul kicsi a szam
            || (option > countLines(fileName)/6))              //tul nagy a szam
    {
        fflush(stdin);                    //buffer uritese
        printf("That selection invalid. Please try again.\n");
    }
    flush();
    printf("Question:\t");
    fgets(question, 1000, stdin);
    printf("1. Answer:\t");
    fgets(answer1, 1000, stdin);
    printf("2. Answer:\t");
    fgets(answer2, 1000, stdin);
    printf("Add 3rd answer? (y/n):\t");
    scanf("%c", &cOption);
    flush();
    if(cOption == 'y')
    {
        printf("3. Answer:\t");
        fgets(answer3, 1000, stdin);;
        printf("Add 4rd answer? (y/n):\t");
        scanf("%c", &cOption);
        if(cOption == 'y')
        {
            printf("4. Answer:\t");
            flush();
            fgets(answer4, 1000, stdin);
        }
    }
    char tmp[] = "temp";
    FILE *fp1 = fopen(fileName, "r");
    FILE *fp2 = fopen(tmp, "w");

    char line[1000];
    int i = 0;
    while (fgets(line, sizeof(line), fp1))
    {
        if( (i/6)+1 == option)
        {
            fprintf(fp2, "%s", question);
            fprintf(fp2, "%s", answer1);
            fprintf(fp2, "%s", answer2);
            fprintf(fp2, "%s", answer3);
            fprintf(fp2, "%s", answer4);
            i+=6;

            time_t rawtime;
            struct tm * timeinfo;
            time ( &rawtime );
            timeinfo = localtime ( &rawtime );
            fprintf(fp2, "%s", asctime (timeinfo));

            int j = 0;
            while(j<5)
            {
                fgets(line, sizeof(line), fp1);
                j++;
            }

        }
        else
        {
            fprintf(fp2, "%s", line);
            i++;
        }
    }
    fclose(fp1);
    fclose(fp2);
    remove(fileName);
    rename("temp",fileName);
    }else{
        printf("\tFile is empty\n");
    }
}

void deleteQuestion(char* fileName, int todelete)
{
    if(countLines(fileName) > 5)
    {
    //listQuest(fileName);
    int option;
    if(todelete == 0)
    {
        printf("Question to delete: ");
        while( (scanf(" %d", &option) != 1) //nem szam
                || (option < 1)                //tul kicsi a szam
                || (option > countLines(fileName)/6))              //tul nagy a szam
        {
            fflush(stdin);                    //buffer uritese
            printf("That selection invalid. Please try again.\n");
        }
    }
    else
    {
        option = todelete;
    }


    char tmp[] = "temp";
    FILE *fp1 = fopen(fileName, "r");
    FILE *fp2 = fopen(tmp, "w");

    char line[1000];
    int i = 0;
    while (fgets(line, sizeof(line), fp1))
    {
        if( (i/6)+1 == option)
        {
            i+=6;
            int j = 0;
            while(j<5)
            {
                fgets(line, sizeof(line), fp1);
                j++;
            }

        }
        else
        {
            fprintf(fp2, "%s", line);
            i++;
        }
    }
    fclose(fp1);
    fclose(fp2);
    remove(fileName);
    rename("temp",fileName);
    }else{
        printf("\tFile is empty\n");
    }
}

void emptyFile(char* fileName)
{
    char tmp[] = "temp";
    FILE *fp1 = fopen(fileName, "r");
    FILE *fp2 = fopen(tmp, "w");
    fclose(fp1);
    fclose(fp2);
    remove(fileName);
    rename("temp",fileName);

}

void finalizeForMlsz(char* fileName)
{
    if(countLines(fileName) < 18)
    {
	printf("Too few lines in the file");
	exit(-1);
    }
    srand ( time(NULL) );
    char finalized[1000] = "backup.txt";
    if( access(finalized, F_OK) != -1)
    {
      emptyFile(finalized);
    }
    int cnt = 0;
    int minimum_number = 1;
    int opt[3] = {-1,-2,-3};
    int maximum_number = countLines(fileName) / 6;
    opt[0] = rand() % (maximum_number + 1 - minimum_number) + minimum_number;
    opt[1] = rand() % (maximum_number + 1 - minimum_number) + minimum_number;
    opt[2] = rand() % (maximum_number + 1 - minimum_number) + minimum_number;
    while(opt[0] == opt[1])
	opt[1] = rand() % (maximum_number + 1 - minimum_number) + minimum_number;
    while(opt[0] == opt[2] || opt[1] == opt[2])
        opt[2] = rand() % (maximum_number + 1 - minimum_number) + minimum_number;

	while(cnt < 3)
	{
	FILE *fp1 = fopen(fileName, "r");
        FILE *fp2 = fopen(finalized, "a");
        char line[1000];
        int i = 0;
        while (fgets(line, sizeof(line), fp1) && (i/6)+1 != opt[cnt])
            i++;
        fprintf(fp2, "%s", line);
        int j = 0;
        while (fgets(line, sizeof(line), fp1) && j<5)
        {
            fprintf(fp2, "%s", line);
            j++;
        }
        fclose(fp1);
        fclose(fp2);
     //   deleteQuestion(fileName,(i/6)+1);
   	cnt++;     
    }
   // remove(fileName);

}


void finalizeFile(char* fileName, int *isCreated)
{
    char finalized[1000];
    printf("Finalized file name: ");
    scanf("%s", finalized);
    char again = 'y';
    while(again == 'y' && countLines(fileName) > 5)
    {
        int option;
        *isCreated = 0;
        listQuest(fileName);
        
        printf("Question to add: ");
        while( (scanf(" %d", &option) != 1) //nem szam
              || (option < 1)                //tul kicsi a szam
              || (option > countLines(fileName)/6))              //tul nagy a szam
        {
            fflush(stdin);                    //buffer uritese
            printf("That selection invalid. Please try again.\n");
        }
        flush();
        
        FILE *fp1 = fopen(fileName, "r");
        FILE *fp2 = fopen(finalized, "a");
        
        char line[1000];
        int i = 0;
        while (fgets(line, sizeof(line), fp1) && (i/6)+1 != option)
            i++;
        
        fprintf(fp2, "%s", line);
        int j = 0;
        while (fgets(line, sizeof(line), fp1) && j<5)
        {
            fprintf(fp2, "%s", line);
            j++;
        }
        fclose(fp1);
        fclose(fp2);
        deleteQuestion(fileName,(i/6)+1);
        printf("Add more question? (y/n): " );
        scanf("%c", &again);
        
    }
    if(countLines(fileName) < 6)
        printf("\tFile is empty\n");
    remove(fileName);
    printf("\tTemp file has been deleted\n\n");
}

void listFinalized()
{
    char fileName[1000];
    printf("File name: ");
    scanf("%s",fileName);
    while( access( fileName, F_OK ) == -1 )
    {
        printf("That selection invalid. Please try again.\n");
        scanf("%s",fileName);
    }
    listQuest(fileName);
}

void deleteFile(char* fileName)
{
    remove(fileName);
}















