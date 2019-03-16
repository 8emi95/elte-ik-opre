#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int length(char* str)
{
	int l = 0;
	while (*str++ != 0) l++;
	return l;
}
int printer(char* str1, char* str2) {
  if(strcmp(str1, str2) == 0) {
    printf("%s\n", str1);
  }
  else {
    printf("%s\n", str2);
  }
	  return 0;
}
int compAll(int argc,char ** strl) {
  int i = 0;
  while(i<argc-1 && strcmp(strl[i], strl[i+1]) == 0)
  {
    i++;
  }
  return i==argc;
}
int main(int argc, char ** argv) {
	// Adding char * together is not working
	//char str1[] = "Hello world!";
//	char str2[] = "!!!";
//	printf(str1);
//	if(str2[0] == '!') printf("\n");
//	str2[0] = 'A';
//	str2[1] = 0;
//	printf("%s\n", str2);
  char* str1 = "First example!";
  printf("%d\n", length(str1));
  char* str21 = "Megegyezik";
  char* str22 = "Megegyezik";
  char* str23 = "Nem egyezik meg";
  printer(str21, str22);
  printer(str21, str23);
  char* str31 = (char*)malloc(80*sizeof(char));
  char* str32 = str31;
  (str31 == str32) ?  printf("Valid\n") : printf("Not valid\n");
  free(str31);
  int i;
  int sum;
  for (i=0; i<argc; i++) {
    printf("%s\n", argv[i]);
    sum = sum + length(argv[i]);
  }
  printf("-------------\n");
  for (i=argc-1; i>= 0; i--) {
    printf("%s\n", argv[i]);
  }
  printf("---------------\n");
  printf("%d\n", sum);
  if (argc >= 2 && compAll(argc, argv) == 0) {
    printf("7. feladat\n");
  }
  // Hacking solution for last example
  if (argc == 3) {
    printf("%s%s\n", argv[1], argv[2]);
  }
  // nicer solution for last example
  if (argc==3) {
    int l1 = length(argv[1]);
    int l2 = length(argv[2]);
    char* newstr = (char*)malloc((l1+l2)*sizeof(char));
    strcpy(newstr, argv[1]);
    int index;
    for(index=0; index<l2; index++) {
      newstr[l1+index] = argv[2][index];
    }
    printf("%s\n", newstr);
    free(newstr);
  }
  return 0;
}
