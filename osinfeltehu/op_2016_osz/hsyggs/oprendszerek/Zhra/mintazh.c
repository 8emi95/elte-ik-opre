#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

struct {
	int a;
	int b;
	int c;
} param;

struct {
	double res1;
	double res2;
} res;

char getChar();
void handler1(int signal);
void handler2p(int signal);
void handler2c(int signal);

int main(int argc,char ** argv) {

	char c = 0;
	
	struct sigaction sig1;
	sig1.sa_handler = handler1; //ez a handler fog lefutni
	
	struct sigaction sig2;
	sig2.sa_handler = handler2c;
	
	sigemptyset(&sig1.sa_mask);
	sig1.sa_flags = 0; 
	sigaction(SIGUSR1,&sig1,NULL);
	
	int pid;
	int pipefd[2]; // névtelen cső
	char sz[100]; 
	
	//így születik a gyerek:
	pid = fork();
	
	//actual work starts ---------------------------------------------------------------------------
	if (pid > 0) //parent 
	{
		do {
			c = getChar();
			system("clear");
			switch (c)
			{
				case 't':
					//killel küld minden signalt nem csak a sigkillt (SIGUSR1 saját jelzés)
					kill(pid,SIGUSR1);
					wait(3);
					exit(0);
					break;
				case 'm':
					param p;
					p.a = 1;
					p.b = 2;
					p.c = 3;
					
					close(pipefd[0]);
					write(pipefd[1], p, sizeof(p));
					
					pause();
					
					
					
			}
		} while (c != '0');
	} 
	else // child
	{
		while (1){
			param p;
			close(pipefd[1]);
			
			pause();
			
			res r;
			r.res1 = (-1 * p.b + sqrt(p.b * p.b - 4 * p.a * p.c)) / (2 * p.a)
			r.res2 = (-1 * p.b - sqrt(p.b * p.b - 4 * p.a * p.c)) / (2 * p.a)
			sig2.sa_handler = handler2p;
			
		}
	}
	//actual word ends -----------------------------------------------------------------------------
	return 0;
}

void handler1(int signal){
	if (signal == SIGUSR1){
		exit(0);
	}
}

void handler2p(int signal){
	read(pipefd[1], p, sizeof(p));
}

void handler2c(int signal){
	read(pipefd[1], p, sizeof(p));
}

char getChar(){
	char ch;
	ch = getchar();
	
	char c;
	while ((c = getchar()) != '\n' && c != EOF) { }
	
	return ch;
}