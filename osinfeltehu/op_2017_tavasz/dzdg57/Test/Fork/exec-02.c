#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main() {
	char *argv[3];
	argv[0] = "/bin/ls";
	argv[1] = "-la";
	argv[2] = NULL;
	
	int status;
	pid_t  child=fork();
	
	if( child == 0 ) {
		printf("Gyermekfolyamat");
		execv(argv[0], argv);
		printf("Ez mar nem jelenhet meg, mert lecsereltuk a folyamatunkat az execv-vel");
	} else {
		waitpid(child, &status, 0);
		printf("Szulo vagyok. PID=%d\n", getpid() ); 
	}
}