#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

struct hatos{
 int a;
 int b;
};

void w_fopen_bin(struct hatos * a){
 FILE * f;
 f=fopen("6.txt","wb");
 if (f==NULL){
  perror("File creating error\n");
  exit(1);
 }
 fwrite(a,sizeof(a[0]),sizeof(a)/sizeof(a[0]),f);
 close(f);
}


void r_fopen_bin(struct hatos * a){
 FILE * f;
 f=fopen("6.txt","r");
 if (f==NULL){
  perror("File reading error\n");
  exit(1);
 } 
 struct hatos b[1];
 fread(&b[0],sizeof(b[0]),1,f);
printf("%i %i %i %i \n",a->a,b->a,a->b,b->b);
 if(a->a==b->a && a->b==b->b){
  printf("OK \n");
 }else{
  printf("WRONG \n");
 }

 close(f);
}

int main(int argc,char** argv){
 if (argc!=3) {
  printf("2 parameters are needed!\n");
  exit(1);
 }
 struct hatos a[1];
 a->a=atoi(argv[1]);
 a->b=atoi(argv[2]);
 w_fopen_bin(a);
 r_fopen_bin(a);


 return 0;
}
