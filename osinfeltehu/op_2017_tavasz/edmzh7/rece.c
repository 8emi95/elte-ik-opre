#include <stdio.h>
int main() {

char rece[500];
FILE *fptr;
fptr = fopen("C:\Users\Dimitar\Desktop\stu-BUD\fax sem6\OS\tekst.txt", "w");

if(fptr == NULL)
{
	printf("Error!");
	exit(1);
}

printf("Enter sen: ");
scanf("%d" ,&rece);

fprintf(fptr, "%d" ,rece);
fclose(fptr);

return 0;
}