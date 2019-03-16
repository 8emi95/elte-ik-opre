/*
 * This program displays the names of all files in the current directory.
 */

#include <dirent.h> 
#include <stdio.h> 
#include <string.h>

int main(void)
{
  DIR           *d;
  struct dirent *dir;
  while(1){
  d = opendir("1");
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      printf("%s\n", dir->d_name);
    }

    closedir(d);
  }
  
  int ret;
  
  char s[10];
  char path[50];
  printf("what do delete: ");
  scanf("%s", s);
  
  strcpy(path,"1/");
  strcat(path,s);
  
   
   ret = remove(path);

   if(ret == 0) 
   {
      printf("File deleted successfully\n");
   }
   else 
   {
      printf("Error: unable to delete the file\n");
   }
  }
  return(0);
}