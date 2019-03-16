

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

int main(){

  DIR *dir;
    struct dirent *ent;
    
    if((dir=opendir("~/filehomework"))!=NULL){
    
      while((ent=readdir(dir))!=NULL){
        printf("%s\n", ent->d_name);
          }
            closedir(dir);
             }else {
             
                perror ("error ");
                   return EXIT_FAILURE;
                    }
                     return 0;
                     } 