//-------------------- 
// Adatok.c 
//-------------------- 
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <sys/wait.h> 
int main() { 
     pid_t child = fork(); 
     int status; 
     if (child > 0) { 
          printf("Szulo vagyok, PID szamom %i,User %i,Csoport %i\n", 
                    getpid(), getuid(), getgid());
          while (1 == 1); 
     } 
     if (child == 0) { 
          printf("Gyerek vagyok, PID szamom %i,User %i, Csoport%i\n",
                    getpid(), getuid(), getgid()); 
     } 
     printf("PID szamom%i\n", getpid()); 
     return 0; 
} 
//--------------------