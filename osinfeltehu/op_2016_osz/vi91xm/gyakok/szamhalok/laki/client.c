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
    int client = socket(AF_INET,SOCK_STREAM,0);
	if(client == -1) {
  		cerr << "Opening stream socket failed. ERROR:" << errno << endl;
  		return -1;
	}

	struct sockaddr_in server_name;
	server_name.sin_family = AF_INET;
	server_name.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_name.sin_port = htons(PORT);
	memset(&(server_name.sin_zero),'\0',8);

	if ( connect( client, (struct sockaddr*) &server_name, sizeof server_name) < 0)
	{
		cerr << "Error - connection cannot be established..." << endl;
		close(client);
		return -2;
	}

	ReqMsg msg = {42,23,'+'};
	ReplyMsg reply;

	for (int i=0; i<3; ++i) {
		printf("Round - %d\n", i);
		send( client, &msg, sizeof(msg),0);
		printf( "   Message sent: %d %c %d\n", msg.a, msg.op, msg.b);
		recv( client, &reply, sizeof(reply), 0);
		printf( "   Message received: %d\n", reply.result);
		printf( " Go to sleep...\n");
		sleep(3);
	}

	close(client);

    
    cout << " << Client is now terminating." << endl;
    return 0;
}
