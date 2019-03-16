#include<stdio.h>
struct address
{
 char name[20];
 char place[20];
 long int pin;
};
int main()
{
 FILE *p;
 struct address x;
 int n,i;
 p=fopen("address","wb");
 printf("How many records?");
 scanf("%d",&n);
 printf("Enter %d records:\n",n);
 for(i=1;i<=n;i++)
 {
   printf("Name:");
   scanf("%s",x.name);
   printf("Place:");
   scanf("%s",x.place);
   printf("Pin code:");
   scanf("%ld",&x.pin);
   fwrite(&x,sizeof(x),1,p);
 }
 fclose(p);
 printf("1 file created");
 return 0;
}