#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE* fp = fopen("hlb.txt","w");
    fprintf(fp,"Horvath L. Bence\n");
    fclose(fp);
    return 0;
}
