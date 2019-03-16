//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <unistd.h> //read
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

struct data{
  char name[80];
  int year;
};
struct data arrayout[]={{"Tomi",1989},{"Zsolti",1990},{"Dina",1994}};
struct data arrayin[3];
//an array of data struct with 3 elements

void opening(int* fd,char* fname,int forwhat,int permission){
 //in C there is no & type in parameter list
 (*fd)=open(fname,forwhat,permission); 
 if (fd<0){ 
   perror("Error at opening\n");
   exit(EXIT_FAILURE);
 }
}
void writedata(int fd,int n,struct data * array){
  int i;
  for (i=0;i<n;i++){
    if (write(fd,&array[i],sizeof(struct data))!=sizeof(struct data)){
      perror("Error in writing\n");
      exit(EXIT_FAILURE);
    };
    printf("write out %s, %i\n",array[i].name,array[i].year);
  }; 
}
 
void readdata(int fd,struct data * array){
  int i=0;
  while( read(fd,&array[i],sizeof(struct data))){
   printf("read in %s, %i\n",array[i].name,array[i].year);
   i++;
  }; //the return value of read is the number of bytes read in successful
} 

int lengthoffile(int fd){
 return (int)lseek(fd,0,SEEK_END)/sizeof(struct data);
 //lseek gives back the file position in bytes
 //the 2. parameter means the relative moveing of the file position due to the 3. parameter
 //SEEK_END - means the end of file
 //SEEK_SET - the beginning of the file
 //SEEK_CUR - the current position 
}
void searchdata(int fd,int which,struct data * d){
  //change the location of the file pointer 
  //WARNING You can out positioning from the file
  lseek(fd,which*sizeof(struct data),SEEK_SET);
  read(fd,d,sizeof(struct data));
  //SEEK_SET - position from the beginning of the file
} 


int main(){
 int f,g;
 struct data d;
 
 opening(&f,"data.db",O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
 writedata(f,3,arrayout);
 printf("The length of file is %i\n",lengthoffile(f));
 close(f);
 printf("\n------------------------------\n"); 
 opening(&g,"data.db",O_RDONLY,0);
 readdata(g,arrayin);
 printf("\n------------------------------\n");
 searchdata(g,1,&d);
 printf("Searching the 2. element... it is %s, %i\n",d.name,d.year);
 close(g);
 return 0;
}
