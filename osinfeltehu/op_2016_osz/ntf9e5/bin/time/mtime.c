//print out the last modification of file


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h> //stat
#include <unistd.h>   //stat
#include <time.h> //ctime

int main(int argc,char** argv){
 /*struct stat st;
 stat(argv[0],&st);  //fstat(file descriptor, stat structure)
 time_t t=st.st_mtime;
 printf("The last modification was: %s\n",ctime(&t)); //convert time_t to string
 */
 time_t t_now = time(0);
 time_t ahora;
 printf("Time now is: %d\n", (int)t_now);

 FILE *fp;
   fp = fopen("abc.txt", "w");

 fprintf(fp, "%d\n", (int)t_now);

 close(fp);

 FILE *fr;
 fr = fopen("abc.txt", "r");
 //fscanf(fp, "%s", ahora);
 fscanf(fr, "%d\n", ahora);
 printf("%d\n", ctime(&ahora));
 close(fr);
 return 0;
}
