#include <stdio.h>
int main() {
    int num = 208495040;
    int res = 0;
    for (int i = 0; i < 111111111; i++) {
		res =(res+num) % 1000000000;
    }
    printf("06 %09d", res);
    return 0;
}
