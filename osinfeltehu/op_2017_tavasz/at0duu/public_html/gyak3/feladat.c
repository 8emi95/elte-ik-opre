#include <stdio.h>
#include <sys/types.h>

int main(){
  pid_t gy1,gy2;
  gy1 = fork();
  if (gy1){
    gy2 = fork();
    if (gy2){
      printf("szülõ");

    } else {printf("2.gyerek")}
  }else{printf("1.gyerek")}
  
  
}