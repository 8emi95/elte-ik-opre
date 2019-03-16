#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main(){
 int child;
 mkfifo("fifo_teszt", S_IRUSR|S_IWUSR );


 child = fork();

 if(child>0){
 char s[1024];
 int fd;
 fd=open("fifo_teszt",O_RDONLY);
 read(fd,s,sizeof(s));
 printf("%s",s);
close(fd);
unlink("fifo_teszt");
 }else if(child==0){
 int fd;
 fd=open("fifo_teszt",O_WRONLY);
 write(fd,"Szia Mami!\n",12);
 close(fd);
 }

 return 0;
}
