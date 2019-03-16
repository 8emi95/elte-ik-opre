 //-------------------- 
// MyShell.c 
//-------------------- 
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>        // Például a fork. 
#include <string.h>        // Például az strtok. 
#include <sys/wait.h>      // Például a wait, és a waitpid. 
#include <sys/types.h>     // Például a pid_t típus. 
 
char cmd[80]; 
char * arg[10]; 
char line[80]="   "; 
int argc; 
 
void my_split() { // Feldarabolja a beolvasott sort a space-ek mentén. 
     int i = 0; 
     argc = 0; 
     arg[argc] = strtok(line," "); 
     while (arg[argc] != NULL) { 
          printf("arg %s\n", arg[argc]); 
          argc++; 
          arg[argc] = strtok(NULL," "); 
     } 
     arg[++argc] = NULL; 
     strcpy(cmd, arg[0]); 
} 
 
int main() { 
     pid_t child; 
     int status; 
     printf("Befejezodik, ha egyetlen . kerul a prompt soraba, majd egy ENTER-t utunk"); 
     printf("\n>> "); 
     fgets(line, 80, stdin); 
     while (strcmp(line,".\n") != 0) { 
          my_split(); 
          child = fork();   // A gyermekfolyamat létrehozása. 
          if (child == 0) { // A gyermek. 
               printf("Vegrehajtas indul\n"); 
               execv(cmd, arg);  // A gyermekfolyamat „lecserélése” a
                                 // megadott paraméterben megadott parancsra. 
          } else{ // A szülő. 
               waitpid(child, &status, 0); // A gyermekfolyamat 
                                           // befejeződését várja. 
               printf("A %s parancs vegrahajtas vege\n", cmd); 
               printf("\n>> "); 
               strcpy(line, "   "); 
               fgets(line, 80, stdin); 
          } 
      } 
     printf("Vege a programnak\n"); 
     return 0; 
} 
//--------------------