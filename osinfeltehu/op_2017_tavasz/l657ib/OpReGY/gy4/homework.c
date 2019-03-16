#include <stdio.h>

struct abc {
    char betu;
    int szam;
};

int main() {
    const int n = 10;
    struct abc tomb[n], st = {'a',1};
	
	int i;
	for(i=0;i<n;++i) {
		tomb[i] = st;
		st.betu++;
		st.szam++;
	}
	
    FILE *file;
    file = fopen("bin.dat","wb");
	fwrite(tomb,sizeof(tomb[0]), sizeof(tomb)/sizeof(tomb[0]),file);
	fclose(file);
	
	file = fopen("bin.dat","rb");
	/*for(i = n-2;i>=0;i-=2) {
		fread(&tomb[i],sizeof(tomb[0]),2,file);
	}
	fclose(file);*/
	fseek(file,-2*sizeof(tomb[0]),SEEK_END);
	for(i=0;i<(n/2);i++) {
		fread(&tomb[i*2],sizeof(tomb[0]),2,file);
		fseek(file,-4*sizeof(tomb[0]),SEEK_CUR);
	}
	fclose(file);
	
	for(i=0;i<n;++i) {
		printf("%c, %i\n",tomb[i].betu,tomb[i].szam);
	}

    return(0);
}
