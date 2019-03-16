#include <stdio.h>

int main(int argc, char* argv[])
{
    int arg_ind = 0;

    printf("Arguments:\n");
    for(;argv[arg_ind] != NULL;++arg_ind)
    {
        printf("%s\n",argv[arg_ind]);
    }
    ++arg_ind;

    printf("After argv:\n");
    for(;argv[arg_ind] != NULL;++arg_ind)
    {
        printf("%s\n",argv[arg_ind]);
    }
}
