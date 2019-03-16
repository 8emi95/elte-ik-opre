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

//lásd uzenetek.c
//kliens
int main(int argc, char** argv) //szerver pidszáma paraméterként
{
	int msgid = msgget(atoi(argv[1]), 0); //meglevohoz kapcsolodik
	struct Message message1, message2;
	
	message1.mtype = atol(argv[1]);
	strcpy(message1.text, "szia"); //message1-be beletesszük szia-t
	msgsnd(msgid/*hova írunk*/, (void*)&message1/*uzenet*/, 25/*ami a text mérete a .h-ban*/, 0/*ha nem várakozik a végére (???)*/);
	
	msgrcv(msgid, (void*)&message2, 25, atoi(argv[1])/*ilyen tipus*/, 0);
	printf("%s", message2);
	return 0;
}