#include <stdio.h>

int max(int n, char ** arg){
	int i;
	int maxi=atoi(arg[0]);
	for(i=0;i<n;++i){
		if(maxi<atoi(arg[i]))
		{
			maxi=atoi(arg[i]);
		}
	}
	
	return maxi;
}

int main(int argc,char ** argv){
	int c=0;
	
	int i;
	for(i=0;i<argc;++i)
	{
		c=c+atoi(argv[i]);
	}
	
	printf("az eredmeny %i\n",c);
	
	printf("a maximum %i\n",max(argc,argv));
	
	return 0;
}