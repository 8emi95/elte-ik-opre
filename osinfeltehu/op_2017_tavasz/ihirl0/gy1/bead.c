//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <string.h>
#include <errno.h> //perror, errno
	/*do
	{
		read(f,&c,sizeof(c));
		if(c!='\n')
		{
		questions[0][i]=c;
		i++;
		}
	}while(c!='\n');
		i = 0;*/ //egy sor olvasása questions-be

int main(int argc,char** argv){
	
 FILE * f;
 f=fopen("out.txt","w");
 if (f==NULL){perror("File opening error\n"); exit(1);}
 char Q[100][100];
 int size = 0;
 int n=100;
 int i;
 int x;
 size_t len;
 while(n!=0)
	{
		printf("%s","1-Kiiras\n");
		 printf("%s","2-uj kerdes\n");
		 printf("%s","3-modosit\n");
		 printf("%s","4-torol\n");
		 printf("%s","0-veglesites es kilepes\n");
		 scanf("%d",&n);
		 
		 switch(n)
		 {
			 case 1:
				for(i=0; i<size; i++)
				{
					printf("%s\n",Q[i]);
				}
				break;
			 case 2:
				printf("%s","adja meg az uj kerdest(szokozok nelkul)\n");
				scanf("\n%s",Q[size]);
				len = strlen(Q[size]);
				Q[size][len] = '\n';
				size = size+1;
				printf("%s","adja meg az 1.valaszt(szokozok nelkul)\n");
				scanf("\n%s",Q[size]);
				len = strlen(Q[size]);
				Q[size][len] = '\n';
				size = size+1;
				printf("%s","adja meg az 2.valaszt(szokozok nelkul)\n");
				scanf("\n%s",Q[size]);
				len = strlen(Q[size]);
				Q[size][len] = '\n';
				size = size+1;
				printf("%s","adja meg az 3.valaszt(szokozok nelkul)\n");
				scanf("\n%s",Q[size]);
				len = strlen(Q[size]);
				Q[size][len] = '\n';
				size = size+1;
				printf("%s","adja meg az 4.valaszt(szokozok nelkul)\n");
				scanf("\n%s",Q[size]);
				len = strlen(Q[size]);
				Q[size][len] = '\n';
				size = size+1;
				break;
			case 3:
				printf("%s","adja meg hanyadik kerdest szeretne modositani: ");
				scanf("%d",&x);
				x = (x-1)*5;
				printf("%s","adja meg az uj kerdest(szokozok nelkul)\n");
				scanf("\n%s",Q[x]);
				len = strlen(Q[x]);
				Q[x][len] = '\n';
				printf("%s","adja meg az 1.valaszt(szokozok nelkul)\n");
				scanf("\n%s",Q[x+1]);
				len = strlen(Q[x+1]);
				Q[x+1][len] = '\n';
				printf("%s","adja meg az 2.valaszt(szokozok nelkul)\n");
				scanf("\n%s",Q[x+2]);
				len = strlen(Q[x+2]);
				Q[x+2][len] = '\n';
				printf("%s","adja meg az 3.valaszt(szokozok nelkul)\n");
				scanf("\n%s",Q[x+3]);
				len = strlen(Q[x+3]);
				Q[x+3][len] = '\n';
				printf("%s","adja meg az 4.valaszt(szokozok nelkul)\n");
				scanf("\n%s",Q[x+4]);
				len = strlen(Q[x+4]);
				Q[x+4][len] = '\n';
				break;
			case 4:
				printf("%s","adja meg hanyadik kerdest szeretne torolni: ");
				scanf("%d",&x);
				x = (x-1)*5;
				for(i = x; i<size; i++)
				{
					len = strlen(Q[i+5]);
					strncpy(Q[i],Q[i+5],len);
				}
				size = size - 5;
		 }
	}
	
	for(i=0; i<size; i++)
	{
		len = strlen(Q[i]);
		fwrite(Q[i],1,len,f);
	}
	 
 
 
 


	
 return 0;
}