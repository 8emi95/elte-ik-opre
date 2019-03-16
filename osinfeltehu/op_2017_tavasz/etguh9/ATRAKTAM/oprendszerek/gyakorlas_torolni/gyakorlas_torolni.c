#include <stdio.h>
#include <stdlib.h>

struct Struki
{
    int a;
    int b;
    float y;
};

int main(int argc,char ** argv) 
{
    struct Struki s;
    s.a = 4;
    s.a = 2;
    s.b = 10;
    s.y = 32.34;
    printf("s.a = %i\ns.b = %i\ns.y = %f\n", s.a, s.b, s.y);
    if(s.a == 2)
	printf("igen\n");
    else
	printf("nem\n");
    return 0;
}