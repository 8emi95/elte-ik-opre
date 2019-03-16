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

void SendSanta (char message[2048])
{
    int client = socket(AF_INET,SOCK_STREAM,0);
    if (client == -1) {
     	 printf ("Opening stream socket failed. ERROR: %d\n", errno);
    }

    struct sockaddr_in server_name;
    server_name.sin_family = AF_INET;
    server_name.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_name.sin_port = htons(PORT + 1);
    memset(&(server_name.sin_zero),'\0',8);

    if ( connect( client, (struct sockaddr*) &server_name, sizeof server_name) < 0) {
   	 printf ("Error - connection cannot be established...\n");
   	 close(client);
    }

    send( client, &message, sizeof(&message),0);
    printf( "Request sent to santa: %s\n", message);
    close(client);
	printf ("Telapo on\n!");
}


int main(int argc, char **argv)
{
	int server = socket(AF_INET,SOCK_STREAM,0);
    if(server == -1) {
     	 printf ("Opening stream socket failed. ERROR: %d\n", errno);
     	 return -1;
    }

    struct sockaddr_in server_name;
    server_name.sin_family = AF_INET;
    server_name.sin_addr.s_addr=INADDR_ANY;
    server_name.sin_port = htons(PORT);
    memset(&(server_name.sin_zero),'\0',8);
	int _set=1;
	int iResult = setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (char *) &_set, sizeof(int));
	if (iResult == -1) {
    	printf ("The call of setsockopt failed with error: %d\n",errno);
	} else {
    	printf ("\t\tSet SO_REUSEADDR: ON\n");
	}

    if( bind(server, (struct sockaddr *) &server_name, sizeof server_name ) == -1 ) {
   	 printf ("Binding stream socket failed.\n");
   	 return -1;
    }

    {
    	struct sockaddr_in server_info;
    	socklen_t _length = sizeof(server_info);
    	if( getsockname( server, (struct sockaddr *) &server_info, &_length) == -1 ) {
   		 printf ("Getting socket failed.\n");
    	}
    }

    if(listen(server,4)==-1) {
   	 printf ("Listening failed to start.\n");
   	 return -1;
    }
    printf ("Server is ready to accept incomming calls.\n");

	fd_set master;
	fd_set readfds;
	int maxfds = server;
	FD_ZERO(&master);
	FD_SET(server, &master);
    
    char allRequests [2048];
	int sledgeWeight  = 0;
    GiftReq msg_in;    
	while (1==1) {

   	 readfds = master;
   	 select (maxfds+1, &readfds, 0, 0, 0);
	int i;
   	 for (i=0;i<maxfds+1;++i) {
   		 if (FD_ISSET(i, &readfds)) {
   			 if (i==server) {
   				 struct sockaddr_in caller;
   				 socklen_t addrlen = sizeof(caller);
   				 int call = accept(server,(struct sockaddr *) &caller, &addrlen);
   				 printf ("New client connected...\n");
   				 FD_SET (call, &master);
   				 if (maxfds < call)
   					 maxfds = call;
   			 } else {
   				 if (recv (i, &msg_in, sizeof(msg_in),0) <= 0) {    
   					 printf ("Client closed the connection...\n");
   					 close (i);
   					 FD_CLR (i, &master);
   					 continue;
   				 }
   				 strncat (allRequests, msg_in.giftName, 128);
   				 strncat (allRequests, ", ", 2);
   				 sledgeWeight += msg_in.weight;
   				 if (sledgeWeight >= 15) {
   					 SendSanta (allRequests);
   					 memset(allRequests, 0, 2048);
   					 sledgeWeight = 0;
   				 }
   			 }
   		 }
   	 }
	}
	close(server);
    printf ("Server is now terminating.\n");
	return 0;
}
