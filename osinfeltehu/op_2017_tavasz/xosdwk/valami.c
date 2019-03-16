#include <stdio.h>

void inc1(int *i) { *i = (*i)+1; }
void inc2(int *i) { inc1(i); inc1(i); };

int main(void) {
    int i = 0;
    for (i ; i < 4 ; ++i) {
        printf("%d\n",i);
    }

    int k = 1;
    printf("%d\n",k);
    inc1(&k);
    printf("%d\n",k);
    inc2(&k);
    printf("%d\n",k);
    return 0;
}
