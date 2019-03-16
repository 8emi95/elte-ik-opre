#include <stdio.h>
#include <stdlib.h>

struct mystruct {
	char ch;
	int in;
};

int main() {

	const int n = 10;
	struct mystruct stv[n], st={'a',1};

	int i;
	
	for (i = 0; i < n; ++i) {
		stv[i]=st;
		st.ch++;
		st.in++;
	}

	FILE * f = fopen("bin.dat", "wb");
	
	int j = n-1;

	for (i = n; i > 0; ++i) {
		if (fseek(f,i,SEEK_SET) != 0) {
			printf("error");
			exit(1);
		}
		fwrite(stv[j], sizeof(stv[j]), sizeof(stv)/sizeof(stv[0]), f);
		--j;

	}
	return 0;
}

