#include <stdio.h>
#include <time.h>
#include "string.h"
#include <string.h>
#include <stdlib.h>
#include "quest.h"

int main()
{
    int choice;//menu valtozoja a felhasznaloi bekereshez
    //char user_filename[100];
    choice = menu();//melyik menupontot valasztja
    char fileName[] = "tmpfile";
    int isCreated = 0;
    int end = 0;
    while(end != -1)
    {
        switch(choice)
        {
        case 1:
            if(!isCreated)
                createNewFile(fileName,&isCreated);
            else
                printf("\tThe file has been already created\n\n");
            break;
        case 2:
            if(isCreated)
                listQuest(fileName);
            else
                printf("\tYou first need to create a file\n\n");
            break;
        case 3:
            if(isCreated)
                newQuestion(fileName);
            else
                printf("\tYou first need to create a file\n\n");
            break;
        case 4:
            if(isCreated)
                modifyQuestion(fileName);
            else
                printf("\tYou first need to create a file\n\n");
            break;
        case 5:
            if(isCreated)
            {
                listQuest(fileName);
                deleteQuestion(fileName,0);
            }else
                printf("\tYou first need to create a file\n\n");
            break;
        case 6:
            if(isCreated)
                emptyFile(fileName);
            else
                printf("\tYou first need to create a file\n\n");
            break;
        case 7:
            if(isCreated)
                finalizeFile(fileName,&isCreated);
            else
                printf("\tYou first need to create a file\n\n");
            break;
        case 8:
            listFinalized();
            break;
        case 0:
            printf("\tExiting...\n");
            deleteFile(fileName);
            end = -1;
            break;
        default:
            printf("Invaild choice, please try again");
        }
        if(end != -1)
            choice = menu();//kesobbi valasztas
    }
    return 0;
}
