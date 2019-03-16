#include "header.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc,char *argv[]){
    bool ChildEnded = false;
    //while(true){
        printf("Nyomj [0]-t a kilépéshez.\n");
        printf("Nyomj [1]-t kérdőív szerkesztéséhez.\n");
        printf("Nyomj [2]-t kérdezőbiztos kiküldéséhez.\n");
        int cmd;
        scanf("%d", &cmd);
        if (cmd == 1){
            loadPollIfExists();
            menu();
        } else if (cmd == 2){
            loadPollIfExists();
            startPolling();
        } else {
            exit(0);
        }
   //}

   return 0;
}
