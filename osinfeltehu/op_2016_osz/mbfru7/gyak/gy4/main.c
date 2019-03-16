#include <stdio.h>
#include <stdlib.h>

int main (int argc, char ** argv) {
	/*
	 * szignál küldése két process között : KILL parancs
	 * szignálok listája: kill -l
	 * 
	 * pl:
	 *   $sleep 100 &
	 *   $ps
	 *   $kill 15 <id>
	 *   $ps
	 *
	 * signal.c
	 * sleep(t) t másodpercet vár
	 * usleep(t) t ezredmásodpercet vár
	 * pause() vár amíg érkezik egy szignál
	 * kill(getppid(),SIGTERM); 
     * 1. parameter the pid number of process, we send the signal
     *   if -1, then eacho of the processes of the same uid get the signal
     *   we kill our bash as well! The connection will close
     * 2. parameter the name or number of signal
	*/
	
	printf("Szia :)\n");
	return 0;
}