#include <stdio.h>
typedef struct
{
    int number;
    char text[10];
    double real_number;
} example;
int main()
{
    example data = {1, "Hello!", 3.14159265}, read_data;
    printf("%d %s %lf\n", data.number, data.text, data.real_number);
    FILE* fout = fopen("blah.txt", "w");
    fwrite(&data, sizeof(example), 1, fout);
    fclose(fout);
    FILE* fin = fopen("blah.txt", "r");
    fread(&read_data, sizeof(example), 1, fout);
    printf("%d %s %lf\n", read_data.number, read_data.text, read_data. 
real_number);
    fclose(fin);
    return 0;
}