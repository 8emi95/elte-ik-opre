#include <stdio.h>

int length(char *str){
    int i;

    for(i = 0; *(str+i)==0; i = i+1){
        if(*(str+i)==0){
            return i;
        }
    }
}

int main(){
    printf("%i", length("teszt"));
}
