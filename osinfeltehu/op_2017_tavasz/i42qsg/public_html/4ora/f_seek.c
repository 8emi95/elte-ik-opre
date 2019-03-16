#include<stdio.h>
#include<stdlib.h>

struct charNum{
	char c;
	int n;
};

int main(int argc, char ** argv){
	const int n=10; 
	struct myStruct stv[n], st={'a',1};

	int i;
	for(i =0; i<10;i++)
	{
	strv[i]=st;
	st.c++;
	st.n++;		
	}	
	FILE *f;
	f=fopen("input.txt","wb");
	for(i=0;i<n;i++){
		fseek(f,-2*sizeof(, SEEK_END);
		fread(myStruct[i],sizeof(myStruct[0]),2,f);
	}
}


