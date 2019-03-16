#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int my_strcmp(char* a, char* b)
{
	int i = 0;
        while(a[i] && a[i] == b[i]) ++i;
        return 0;
}

int main(int argc, char* argv[])
{
    char a[] = "Hello";
    char b[] = "Hello";
    char c[] = "hell0";
    printf("a == a : %s\n", my_strcmp(a,a)?"false":"true");
    printf("a == b : %s\n", my_strcmp(a,b)?"false":"true");
    printf("a == c : %s\n", my_strcmp(a,c)?"false":"true");
    
    printf("3.--------\n");
    char** p = malloc(sizeof(char*));
    char* cp;

    cp = *p;

    printf("Address of p:%p\nAddress of q:%p\n",p,&cp);

    printf("4.---------\n");
    for(char** argptr = argv; argptr != (argv+argc); ++argptr)
    {
         printf("%s\n",*argptr);
    }
    printf("5.---------\n");
    for(char** argptr = argv+argc-1;argptr != argv;--argptr)
    {
         printf("%s\n",*argptr);
    }

    printf("6.--------\n");
    int total_length = 0;
    for(char** argptr = argv; argptr != (argv+argc); ++argptr)
    {
       total_length += strlen(*argptr);
    }
    printf("Total argument lengths: %d\n",total_length);

    printf("7.--------\n");
    if(argc == 3)
    {
        char* cat_str = calloc(strlen(argv[1]) + strlen(argv[2]) + 1,sizeof(char));
        strcat(cat_str,argv[1]);
        strcat(cat_str,argv[2]);
        printf("Concat string: %s\n",cat_str);
    }
    else
        printf("Not exactly two params.\n");
    return 0;
}

