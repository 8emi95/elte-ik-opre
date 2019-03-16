#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h> 
#include <sys/msg.h> //új
#include <sys/ipc.h>//új

#include "msgtype.h" //ez kell mindkét alkalmazásban, ez saját 

//az uzenetkliens -el egyutt kell használni
//server, háttérben (&) létrehozni
int main()
{	
	struct Message message1, message2;
	int msgid = msgget(getpid(), IPC_CREAT | 0666); //2. paraméter h egy meglévõhöz akarok kapcsolódni vagy most akarom létrehozni
	msgrcv(msgid, (void*)&message1, 25, getpid()/*csak azt fogadom amik egyeznek ezzel a piddel*/, 0/*várakozzon az üzenetre*/); //receive fv
	printf("%s", message1.text);

	message2.mtype = 1;
	strcpy(message2.text, "HELLO");
	msgsnd(msgid, (void*)&message2, 25, 0);
	
	//a végén beírni h konzolba h ipcs és kilõni a dolgokat ipcrm-el (ipcrm -q azonosító)
	
	
	//ha a szerverhez akarjuk eljuttatni a kliens pid-jét akkor az elsõ üzenetben átadhatjuk neki
	
	return 0;
}