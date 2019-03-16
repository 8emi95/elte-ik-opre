#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct info
{
    char question[40];
    int poAns;
    char answer[40];

};

/*int Search_in_File(char *fname, char *str) {
    FILE *fp;
    int line_num = 1;
    int find_result = 0;
    char temp[40];

    if((fp = fopen(fname, "r")) == NULL) {
        return(-1);
    }

    while(fgets(temp, 40, fp) != NULL) {
        if((strstr(temp, str)) != NULL) {
            printf("A match found on line: %d\n", line_num);
            printf("\n%s\n", temp);
            find_result++;
        }
        line_num++;
    }

    if(find_result == 0) {
        printf("\nSorry, couldn't find a match.\n");
    }

    //Close the file if still open.
    if(fp) {
        fclose(fp);
    }
    return(0);
}*/



int main()
{

    struct info rec;
    FILE *f1;
    int sel;
    char fname[10] = "newl.txt";
    int err = -1;



    do{
        do{ printf("\n**********************************************\n");
            printf("MAIN MENU - The Society of Hungarian Ball Games");
            printf("\n*************Dimitar Egumenovski**************\n");
            printf("*1. Add New Question\n");
            printf("*2. List All Question\n");
            printf("*3. Modify Question\n");
            printf("*4. Exit");
            printf("\n**********************************************\n");
            printf("Type your choice -> ");
            scanf("%d", &sel);
            if(sel<1 || sel>4){
                printf("ERROR: The choice isn't allowed\n");
            }
        }while(sel<1 || sel>4);

        getchar();
        switch(sel){

            case 1:
                f1=fopen(fname ,"a+");
                int ai = 1;
                if (!f1) 
                {
                    printf("lis.txt, no such file exits\n");
                    return -1;
                }
            	char time_v[100];
            	time_t t = time(NULL);
    			struct tm *tm = localtime(&t);
    			char s[64];
    			strftime(s, sizeof(s), "%c", tm);
    			strncpy(time_v, s, 100);
    			strcat(time_v, "\n");
    			fprintf(f1, "%s", time_v);

                printf("Insert New Question\n");
                printf("Write question:\n");
                fgets(rec.question, sizeof(rec.question), stdin);

                printf("Hom much answer you will write (2 - 4):\n");
                scanf("%d", &rec.poAns);
                /* da zgolemva po edno !
                fprintf(f1, "%d ", ai);
*/
                fprintf(f1,"Question: %s",rec.question);
                
                
                fprintf(f1, "*** %d possible answer ***\n",rec.poAns );

                
                printf("Write answer:\n");
                int x = 1;
                while(x <= rec.poAns + 1){
                fgets(rec.answer, sizeof(rec.answer), stdin);
                printf("*");
                x++;
                fprintf(f1," %s\n",rec.answer);
                }
                
                fflush(f1);
                printf("** Question added sucefuly! ***\n");
                fclose(f1);
                break;

                case 2:
                f1=fopen(fname ,"r");
                int c;
                if (f1) {
    			while ((c = getc(f1)) != EOF)
        		putchar(c);
    			fclose(f1);
				}
				break;

          		case 3:
          		printf("Enter the question you want to modify:\n");
          		char word[100];
          		scanf("%s", word);
           		char line[1024];
          		f1 = fopen(fname, "r");
          		while (fgets(line, sizeof(line), f1)!= NULL){
          			if (strstr(line, word)!= NULL)
          			{
          				printf("%s\n", line);
          			}
}

          		/*void FindWord(char *word , char *file){
   char line[1024] ;
   FILE* fp = fopen(file, "r") ;
   while (fgets(line , sizeof(line) , fp )!= NULL)
   {
      if (strstr(line , word )!= NULL)
      {
         printf("%s",line);
      }
   }
}*/

              break;
            	case 4:
                printf("Good bye...Thank you for using :)\n");
                break;
            	default:
                printf("ERROR!\n");
                break;
        }
    }while(sel!=4);

    return 0;
}
