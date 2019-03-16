#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<wait.h>
int main(void)
{
        int pipefd[2];
		int pipefd2[2];

		pid_t pid;
		pid_t pidb;
		
		
		

		if (pipe(pipefd) == -1 || pipe(pipefd2) == -1)
		{
			perror("Hiba a pipe nyitaskor!");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("Fork hiba");
			exit(EXIT_FAILURE);
		}

		if (pid == 0)
		{ // child process
			char sz2[1024];
			close(pipefd[1]);
			close(pipefd2[0]);

			read(pipefd[0],sz2,sizeof(sz2));
			printf("Hallgato2: Megkaptam a  '%s' kerdest!\n", sz2);
			
			sleep(5);
			
			close(pipefd[0]);
			close(pipefd2[1]);
			pidb = fork();

			exit(EXIT_SUCCESS);
		}
		else
		{    // szulo process
			char sz[1024] = "Ez a feladat!";
			printf("Tanar: Kuldom a feladatot!\n");

			
			write(pipefd[1], sz, sizeof(sz));
			close(pipefd[1]);
			write(pipefd2[1], sz, sizeof(sz));
			close(pipefd2[1]);
			
			
			if (pidb == 0)
			{ // 2. child process	
				close(pipefd[1]);
				close(pipefd2[1]);
				
				char sz3[1024];
				read(pipefd2[0], sz3, sizeof(sz3));
				printf("Hallgato1: Megkaptam a '%s' kerdest!\n", sz3);
				
				sleep(5);
				
				close(pipefd[0]);
				close(pipefd2[0]);

				exit(EXIT_SUCCESS);
			}
			
			
			
		}
}