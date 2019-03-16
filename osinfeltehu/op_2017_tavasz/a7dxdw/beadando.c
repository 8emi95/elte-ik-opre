#include <stdio.h>
#include <stdlib.h>
void selection(int &selected);
void addQuestion();

int main(int argc,char ** argv) 
{

    int selected;
    printf("1. Kérdés felvétel \n");
    printf("2. Kérdés listázás \n");
    printf("3. Kérdés módosítás \n");
    printf("4. Kérdés törlés \n");
    printf("5. Kérdések véglegesítése \n");
    scanf("%d", &selected);
    selection(selected);

 return 0;
}


void selection(int &selected)
{
     switch(selected)
    {
      case 1 :
         printf("A kérdés felvétele opciót választotta!\n" );
         addQuestion();

         break;
      case 2 :
         printf("Well done\n" );
         break;
      case 3 :
         printf("You passed\n" );
         break;
      case 4 :
         printf("Better try again\n" );
         break;
      case 5 :
         printf("Better try again\n" );
         break;
      default: 
         printf("Wrong number, please choose from 1 to 5!\n" );
         printf("1. Kérdés felvétel \n");
         printf("2. Kérdés listázás \n");
         printf("3. Kérdés módosítás \n");
         printf("4. Kérdés törlés \n");
         printf("5. Kérdések véglegesítése \n");
         scanf("%d", &selected);
         selection(selected);
    }
}

void addQuestion()
{
    printf("Kérem írja le a kérdést! :\n");
    char question[256];
    
    fgets (question, 100, stdin);
    scanf ("%[^\n]%*c", question);
    printf("Az ön kérdése: %s",question);
    printf("\n");
    printf("Hány válaszlehetőséget szeretne? :\n");

    int numOfAnswers;
    scanf("%d", &numOfAnswers);

    for(int i = 0; i < numOfAnswers; i++)
    {
        printf("Valasz \n");
    }


};