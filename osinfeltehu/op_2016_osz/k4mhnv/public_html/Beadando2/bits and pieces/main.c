#include <stdlib.h>  
#include <stdio.h>
#include <string.h>
#include "signup.c"
#include "admin.c"
#include "parser.c"

int main(){

    int response;
    const char* registeredFile = "registered.txt";
    const char* eventsFile = "events.txt";
    char *registered[256];
    char *events[256];

    getFileContent(registeredFile,registered);  

    do{
        printf("*****King of Stands*****\n");
        printf("1. Jelentkezes\n");
        printf("2. Jelentkezesek Listazasa\n");
        printf("3. Admin Felulet\n");
        printf("4. Kilepes\n");

        scanf("%d", &response);
        if(response == 1){
            signup();
            continue;
        }else if(response == 2){
            printContent(registered);
            continue;
        }
        else if(response == 3){
            adminMenu();
        }else if(response == 4){
            return 0;
        }
        else{
            printf("Nincs ilyen menupont\n");
        }
    }while (1);

    return 0;
}
