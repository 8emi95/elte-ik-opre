waitpid(child, &status, 0);
	//waits the end of child process PID number = child, the returning value will be in status
    //0 means, it really waits for the end of child process - the same as wait(&status)

wait(NULL);
wait(NULL);
wait(NULL);
(3 gyerek...)