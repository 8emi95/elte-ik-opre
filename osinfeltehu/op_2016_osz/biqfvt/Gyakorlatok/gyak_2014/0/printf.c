#include <stdio.h>
#include <stdlib.h>
 

int main() 
{
 printf("Hello world\n"); //prints out a constant  string
 
 char str[]="Hello world"; //there is no string type instead of it use char array! 
 printf("The str variable\'s value is %s\n",str);
 //%s is a format specifier (place holder for a char array)
 //\n is an escape sequence - new line 
 
 int x=5; 
 printf("The value of x is %i \n",x); //prints out an integer variable 
 //%i is a format specifier (place holder for an integer)
 
 printf("The value of str is %s, the value of x is %i\n",str,x);
 //You can use several parameters 
 
 return 0;
}
