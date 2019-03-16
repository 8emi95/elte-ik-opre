#include <stdio.h>
#include <sys/types.h>

int main(){
pid_t pid;
int i;
for(i=0; i<2;i++);
printf("%i pid=%i \n", i+1, pid=fork());
}