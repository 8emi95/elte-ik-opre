#include <stdio.h>
void write_lines (FILE *file,char prasanje,char odgovori,int mozni) {

    fprintf (file, "%s\n", prasanje);
    fprintf (file, "%d\n", mozni);
    fprintf (file, "%s\n", odgovori);
}

int main () {
    FILE *file = fopen ("zach.txt", "a+");
    char prasanje[100];
    int mozni;
    char odgovori[100];
    fgets(prasanje, 100, stdin);
    scanf("%d", &mozni);
    fgets(odgovori, 100, stdin);

    write_lines (file);
    fclose (file);
    printf ("File has been written. Please review. \n");
    return 0;
}