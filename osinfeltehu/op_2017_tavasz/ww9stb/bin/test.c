#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

struct foo {
	int bar;
};

int main(int argc, char* argv[]){
	int fid;
	int m = 2;
	
	fid = open("data.bin", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	
	int i;
	for(i = 0; i < m; ++i) {
		struct foo f;
		f.bar = i*200;
	
		write(fid, &f, sizeof(struct foo));
	}
	
	close(fid);
	return 0;
}