#include <stdio.h>

struct mystruct{
	char ch;
	int in;
};

int main(){
	const int n=10;
	struct mystruct stv[n], st={'a',1};
	int i;
	FILE *f;
	fopen("bin.dat", "w+");
	for(i=0;i<n;++i){
		stv[i]=st;
		st.ch++;
		st.in++;
	}
	fwrite(stv, sizeof(stv[0]),sizeof(stv)/sizeof(stv[0]),f);
	fclose(f);
	fopen("bin.dat", "rb");
	fseek(f, 0, SEEK_END);
	for(i=0;i<n;++i){
		stv[n].in=fseek(f, 1, SEEK_CUR);
		stv[n].ch=fseek(f, 1, SEEK_CUR);
	}
	
	for(i=0;i<n;++i){
		printf("%d | %c", stv[i].ch, stv[i].ch);
	}
	
}
