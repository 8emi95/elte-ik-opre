#define PORT 42002
#include <stdio.h>
#include <iostream>
#include <sstream>
#define _WIN32_WINNT 0x501
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <inttypes.h>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

struct ReqMsg {
	int a;
	int b;
	char op;
};

struct ReplyMsg {
	int result;
};

int main(int argc, char **argv)
{
    int client = socket(AF_INET, SOCK_STREAM, 0);
	if(client == -1) {
  		cerr << "Opening stream socket failed. ERROR:" << errno << endl;
  		return -1;
	}

	struct sockaddr_in server_name;
	server_name.sin_family = AF_INET;
	server_name.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_name.sin_port = htons(PORT);
	memset(&(server_name.sin_zero), '\0', 8);

	if ( connect( client, (struct sockaddr*) &server_name, sizeof server_name) < 0)
	{
		cerr << "Error - connection cannot be established..." << endl;
		close(client);
		return -2;
	}

	ReqMsg msg = {123, 321, '+'};
	ReplyMsg reply;

	for (int i = 0; i < 3; i++) {
		printf("Round - %d\n", i);
		send( client, &msg, sizeof(msg), 0);
		printf("   Message sent: %d %c %d\n", msg.a, msg.op, msg.b);
		recv( client, &reply, sizeof(reply), 0);
		printf("   Message received: %d\n", reply.result);
		printf(" Go to sleep...\n");
		sleep(3);
	}

	close(client);

    
    cout << " << Client is now terminating." << endl;
    return 0;
}
