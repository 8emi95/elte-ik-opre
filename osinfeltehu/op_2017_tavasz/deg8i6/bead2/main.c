#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "kerdessor.h"
#include "kerdesbiztos.h"

void printMenu(){
    printf("Please choose one of the following options:\n\
			C - Kerdessor letrehozasa\n\
                        E - Kerdessor modositasa\n\
                        L - Kerdessor listazasa\n\
                        F - Kerdessor veglegesitese\n\
                        K - Biztos kikuldese\n\
			0 - Exit\n");
}

char getChoose(){
    char a[100];
    scanf("%s",a);
    return a[0];
}

int main(){
    bool exit = false;
    do{
        printMenu();
	switch(getChoose()){
            case 'C': create_kerdessor(); break;
            case 'E': edit_kerdessor(); break;
            case 'L': listaz_kerdessor(); break;
            case 'F': veglegesit_kerdessor(); break;
            case 'K': biztosKuld(); break;
            case '0': exit = true; break;
            default: exit = true;
			
	}
    }while(!exit);
    
    return 0;
}
