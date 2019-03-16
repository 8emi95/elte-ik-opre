#include <stdio.h>
struct student
{
   char name[50];
   int mark;
};
int main(){
    struct student stud1[5], stud2[5];   
    FILE *fptr;
    int i;
    /*
    fptr = fopen("file3.txt","wb");
    for(i = 0; i < 5; ++i)
    {
        fflush(stdin);
        printf("Enter name: ");
        //fgets(stud1[i].name);
        scanf("%s", stud1[i].name);

        printf("Enter height: "); 
        scanf("%d", &stud1[i].height); 
    }

    fwrite(stud1, sizeof(stud1), 1, fptr);
    fclose(fptr);
    */
    fptr = fopen("student1.txt", "rb");
    fread(stud2, sizeof(stud2), 1, fptr);
    for(i = 0; i < 5; ++i)
    {
        printf("neve: %s\njegye: %i", stud2[i].name, stud2[i].mark);
    }
    fclose(fptr);
}