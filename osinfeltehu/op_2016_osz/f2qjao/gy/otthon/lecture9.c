#include <stdio.h>

int main() {
    char name[5];

    fgets(name, 5, stdin);
    
    printf("Your name is: \'%s\'\n", name);    
    return 0;
}
