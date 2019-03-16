NEVESÍTETLEN CSŐVEZETÉK



-> LÉTREHOZÁS:

	int pipefd[2];
		// unnamed pipe file descriptor array
		
	if (pipe(pipefd) == -1)
		printf("HIBA\n");



-> ÍRÁS:

	close(pipefd[0]);   //usually we close unused read end
	write(pipefd[1], "Hajra Fradi!", 13);
	close(pipefd[1]);   //closing write descriptor 



-> OLVASÁS:

Adott:
- char sz[100]  -  ide olvasunk

	close(pipefd[1]);   //usually we close the unused write end
	read(pipefd[0], sz, sizeof(sz));   //reading max 100 characters
	close(pipefd[0]);   //finally we close the used read end