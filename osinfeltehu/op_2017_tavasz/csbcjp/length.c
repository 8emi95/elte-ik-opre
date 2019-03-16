 #include <stdio.h>

int length(char* ptr)
{
    char* tmp=ptr;
    while(*tmp != '\0')
        tmp++;
    return tmp - ptr;
}

int main()
{
    char str[]= "cica";
    int l = length(str);
    printf("%i\n", l);
    return 0;
}