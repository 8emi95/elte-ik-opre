#include <stdio.h>

struct mystruct {
	char ch;
	int in;
};

int main(int argc,char ** argv){
	const int n=10;
	struct mystruct stv[n], st={'a',1};
	
	int i;
	for(i=0; i<n; ++i) {
		stv[i]=st;
		st.ch++;
		st.in++;
	}
	
	FILE *f;
	f = fopen("bin.dat","wb");
	fwrite(stv, sizeof(stv[0]), sizeof(stv)/sizeof(stv[0]), f);
	fclose(f);
	
	f=fopen("bin.dat", "rb");
	//innent�l elt�r�s!
	fseek(f, -2*sizeof(stv[0]), SEEK_END); // elemm�ret k�tszeres�vel 2-vel vissza a f�jl v�g�t�l
	for( i=0; i<(n/2); i++) {
		fread(&stv[i*2], sizeof(stv[0]), 2, f); // hova, elemm�ret, mennyis�g, f fileb�l
		fseek(f, -4*sizeof(stv[0]), SEEK_CUR); // k�zben olvas�s k�zben kett�t el�rel�pt�nk...ez�rt 4-et seekel�nk vissza
	}
	fclose(f);
	
	for(i=0; i<n; ++i) {
		printf("%c, %i\n", stv[i].ch, stv[i]);
	}
}
