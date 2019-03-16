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
#include <sys/msg.h> //�j
#include <sys/ipc.h>//�j

#include "msgtype.h" //ez kell mindk�t alkalmaz�sban, ez saj�t 

//az uzenetkliens -el egyutt kell haszn�lni
//server, h�tt�rben (&) l�trehozni
int main()
{	
	struct Message message1, message2;
	int msgid = msgget(getpid(), IPC_CREAT | 0666); //2. param�ter h egy megl�v�h�z akarok kapcsol�dni vagy most akarom l�trehozni
	msgrcv(msgid, (void*)&message1, 25, getpid()/*csak azt fogadom amik egyeznek ezzel a piddel*/, 0/*v�rakozzon az �zenetre*/); //receive fv
	printf("%s", message1.text);

	message2.mtype = 1;
	strcpy(message2.text, "HELLO");
	msgsnd(msgid, (void*)&message2, 25, 0);
	
	//a v�g�n be�rni h konzolba h ipcs �s kil�ni a dolgokat ipcrm-el (ipcrm -q azonos�t�)
	
	
	//ha a szerverhez akarjuk eljuttatni a kliens pid-j�t akkor az els� �zenetben �tadhatjuk neki
	
	return 0;
}