#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen,strcmp,strcat,strcpy and more ....

//there is no string type - instead of it You have to use char array
int length(char * str)
{
 int l=0;
 while (*str++!=0)l++; //*str points to the next character
 return l;
}
  void attributumKiiro( int db , char** str3 ){
    int i;
    printf("\n");
    printf("attrString:");
    for( i=1;i<db;i++ ){
        printf(" %s",str3[i]);
    }
    printf("\n\n");
    return ;
  }

 char*string3;
  char * osszefuz(char * string1,char * string2){
    char string3[strlen(string1) + strlen(string2)];
    int i;
    for( i=0;i<strlen(string1);i++ ){
        string3[i] = string1[i];
    }
    int j= 0;

    for(i=strlen(string1);i<strlen(string1) + strlen(string2);i++ ){
        string3[i] = string2[j];
        j++;
    }
    return string3;
  }
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int main(int argc,char ** argv)
{
              //char str1[80]; //it would be an 80 characters long empty "string", - it is empty
  char str1[]="Hello world"; //it's length is the length of the text
  printf("The content of variable str1: \'%s\'\n",str1);
  //1. parameter formatstring, next parameter(s) variables
  //%s = string, %i = integer \n = new line e.g.

  printf("The length of variable str1 \'%s\'\n  %i (with length()), %i (with strlen())\n",str1,length(str1),strlen(str1));
  //The end of a string is a 0 character
  //There is the function strlen!! in string.h

  char* str2; //it is a pointer variable (we have to allocate memory)
  str2=str1; //the pointer shows to the same memory place
  printf("The content of variable str2 \'%s\' is the same as str1 \'%s\' \n(pointing the same memory place)\n",str2,str1);
  str2=(char*)malloc(80*sizeof(char)); //allocate new memory
  //calloc, realloc, free - functions for allocating memory and freeing up the memory
  str2[0]='O'; //to write value character by character
  str2[1]='S';
  str2[2]=0; //at the end of a string there is a 0 character!
  printf("We've given value character by character -  str2: \'%s\'\n",str2); //new content

  strcpy(str2,"New content by using strcpy");
  printf("The new content of variable str2: \'%s\'\n",str2); //new content
  //instead of giving value character by character use function in string.h
  //important other functions: strcmp, strcat, strlen;

  free(str2);

  attributumKiiro(argc , argv);

  char str4[]="eleje ";
  char str5[]="vege";
  char *str6=osszefuz( str4, str5);
  printf("str6: \'%s\'\n",str6);
  char *str7=osszefuz( str6, str6);
 // printf("str6: \'%s\'\n",str6);
  printf("str7: \'%s\'\n",str7);
  char* s = concat("derp", "herp");
    printf("%s\n",s);
  return 0;
}
