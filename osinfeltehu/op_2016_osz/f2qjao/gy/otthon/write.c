#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Player {
    char name[10];
    int age;
    int sport;
};

struct File {
    int size;
    struct Player players[10];
};

int main() {
    FILE *fp;

    struct File myFile;

    strcpy(myFile.players[0].name, "Krisz");
    myFile.players[0].age = 21;
    myFile.players[0].sport = 1;

    strcpy(myFile.players[1].name, "Akos");
    myFile.players[1].age = 16;
    myFile.players[1].sport = 1;
    
    strcpy(myFile.players[2].name, "Laci");
    myFile.players[2].age = 22;
    myFile.players[2].sport = 0;
    
    strcpy(myFile.players[3].name, "Krisz2");
    myFile.players[3].age = 212;
    myFile.players[3].sport = 12;
    
    strcpy(myFile.players[4].name, "Krisz3");
    myFile.players[4].age = 213;
    myFile.players[4].sport = 13;

    myFile.size = 5;

    fp = fopen("be.txt", "w+");

    fwrite(&myFile, sizeof(struct File), 1, fp);


    fclose(fp);
    return 0;
}
