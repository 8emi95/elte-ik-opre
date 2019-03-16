#include <stdio.h>
#include <string.h>
#include <locale.hu>

int main()
{
    char tomb[32];
    int n, N;
    setlocale(LC_ALL, "");
    
    strcpy(tomb, "Helló világ");
    N=strlen(tomb);
    
    for(n=0; n<N; n++) {
        tomb[n]=toupper(tomb[n]);
        printf("%s\n", tomb );
    }
    

    return 0;
}