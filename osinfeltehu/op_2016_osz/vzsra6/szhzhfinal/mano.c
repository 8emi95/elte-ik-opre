#define PORT 22333
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <sstream>

#include "common.h"

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 3) {
   	 printf ("A manonak kell ket parameter: ajandek, es suly\n", errno);
     	 return -1;
    }
    
	int client = socket(AF_INET,SOCK_STREAM,0);
    if (client == -1) {
   	 printf ("Opening stream socket failed. ERROR: %d \n", errno);
     	 return -2;
    }

    struct sockaddr_in server_name;
    server_name.sin_family = AF_INET;
    server_name.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_name.sin_port = htons(PORT);
    memset(&(server_name.sin_zero),'\0',8);

    if ( connect( client, (struct sockaddr*) &server_name, sizeof server_name) < 0) {
   	 printf ("Error - connection cannot be established\n",errno);
   	 close(client);
   	 return -3;
    }

    GiftReq msg = {'\0'};
    strcpy (msg.giftName, argv[1]);
    msg.weight = atoi (argv[2]);
    send( client, &msg, sizeof(msg),0);
    printf( "Gift request sent: %s %d\n", msg.giftName, msg.weight);
    close(client);
	printf ("Mano off.|\n");
	return 0;
}