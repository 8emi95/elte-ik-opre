#include <stdio.h>

struct abc[n] {
    char betu;
    int szam;
};

int main() {
    const int n = 10;
    struct abc tomb;
    tomb = new abc[10];
    tomb[0].betu = 'a';
    tomb[1].betu = 'b';
    tomb[2].betu = 'c';
    tomb[3].betu = 'd';
    tomb[4].betu = 'e';
    tomb[5].betu = 'f';
    tomb[6].betu = 'g';
    tomb[7].betu = 'h';
    tomb[8].betu = 'i';
    tomb[9].betu = 'j';
    for(int i=0;i<n;i++) {
        tomb[i].szam = i+1;
    }

    FILE *file;
    file = fopen("file.txt","w+");

    for(int i = n-1;i>=0;i-2) {
        fputs("%c %d\n %c %d\n",tomb[i].betu, tomb[i].szam, tomb[i-1].betu, tomb[i-1].szam,file);
    }

    fclose(file);

    return(0);
}