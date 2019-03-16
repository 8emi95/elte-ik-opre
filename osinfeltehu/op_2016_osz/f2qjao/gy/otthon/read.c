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

    fp = fopen("be.txt", "r+");

    fread(&myFile, sizeof(struct File), 1, fp);
    
    int i = 0;
    for(i = 0; i < myFile.size; i++) {
        printf("Name: %s\n", myFile.players[i].name);
        printf("Age: %d\n", myFile.players[i].age);
        printf("Sport: %d\n", myFile.players[i].sport);
    }

    fclose(fp);
    return 0;
}
