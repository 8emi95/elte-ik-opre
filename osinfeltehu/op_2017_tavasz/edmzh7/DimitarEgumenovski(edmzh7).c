#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct info
{
    char name[40];
    int poAns;
    char sur[40];
};

int main()
{
    struct info rec;
    FILE *f1;
    int sel;
    char cmp[40];
    char fname[10] = "file_qu.txt";
    int err = -1;

    do{
        do{ printf("\n**********************************************\n");
            printf("MAIN MENU - The Society of Hungarian Ball Games");
            printf("\n**********************************************\n");
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
                    printf("file_qu.txt, no such file exits\n");
                    return -1;
                }

                char moja[100];
            	time_t t = time(NULL);
    			struct tm *tm = localtime(&t);
    			char s[64];
    			strftime(s, sizeof(s), "%c", tm);
    			strncpy(moja, s, 100);
    			strcat(moja, "\n");
    			fprintf(f1, "%s", moja);

                printf("Insert New Question\n");
                printf("Write question:\n");
                fgets(rec.name, sizeof(rec.name), stdin);

                printf("Hom much answer you will write (2 - 4):\n");
                scanf("%d", &rec.poAns);
                /* da zgolemva po edno !
                fprintf(f1, "%d ", ai);
*/
                fprintf(f1,"Question: %s",rec.name);
                
                
                fprintf(f1, "*** %d possible answer ***\n",rec.poAns );

                
                printf("Write answer:\n");
                int x = 1;
                while(x <= rec.poAns + 1){
                fgets(rec.sur, sizeof(rec.sur), stdin);
                printf("*");
                x++;
                fprintf(f1,"Answer: %s\n",rec.sur);
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
          		printf("Modify account\n");
          		char find[250];
          		printf("Enter whick question you want to modify:\n");
          		scanf("%s", find);
          		printf("You like to change this question:\n");
          		printf("%s\n", find);

              	break;

            	case 4:
                printf("Good bye...Thank you for using\n");
                break;
            	default:
                printf("ERROR!\n");
                break;
        }
    }	while(sel!=4);

    return 0;
}