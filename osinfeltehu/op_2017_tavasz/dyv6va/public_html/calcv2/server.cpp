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
    int server = socket(AF_INET,SOCK_STREAM,0);
	if(server == -1) {
  		cerr << "Opening stream socket failed. ERROR:" << errno << endl;
  		return -1;
	}

	struct sockaddr_in server_name;
	server_name.sin_family = AF_INET;
	server_name.sin_addr.s_addr = INADDR_ANY;
	server_name.sin_port = htons(PORT);
	memset(&(server_name.sin_zero),'\0', 8);
    int _set=1;
    int iResult = setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (char *) &_set, sizeof(int));
    if (iResult == -1) {
        cerr << "The call of setsockopt failed with error:" << errno << endl;
    } else {
        cerr << "\t\tSet SO_REUSEADDR: ON" << endl;
    }

	if( bind(server, (struct sockaddr *) &server_name, sizeof server_name ) == -1 ) {
    	cerr << "Binding stream socket failed." << endl;
    	return -1;
	}

	{
        struct sockaddr_in server_info;
        socklen_t _length = sizeof(server_info);
        if( getsockname( server, (struct sockaddr *) &server_info, &_length) == -1 ) {
            cerr << "Getting socket failed." << endl;
        }
        cout << "\t------------------------------------------------------" << endl;
        cout << "\t--> IPv4 Address is " << inet_ntoa(server_info.sin_addr) << endl;
        cout << "\t--> Port number is " << ntohs(server_info.sin_port) << endl;
        cout << "\t------------------------------------------------------" << endl;
	}

	if(listen(server,4) == -1) {
		cerr << "Listening failed to start." << endl;
		return -1;
	}
    cout << "\t>> Server is ready to accept incomming calls." << endl;

    fd_set master;
    fd_set readfds;
    int maxfds = server;
    FD_ZERO(&master);
    FD_SET(server, &master);
    ReqMsg msg;
    ReplyMsg reply;
	
    while (true) {

	readfds = master;

	select(maxfds + 1, &readfds, 0, 0, 0);

	for (int i = 0; i < maxfds + 1; i++) {
		if (FD_ISSET(i, &readfds))
		{
			if (i == server) {
			        struct sockaddr_in caller;
			        socklen_t addrlen = sizeof(caller);
			        int call = accept(server,(struct sockaddr *) &caller, &addrlen);
				printf("New client connected...\n");
				FD_SET(call, &master);
				if (maxfds < call) maxfds = call;
			}
			else {
				if (recv(i, &msg, sizeof(msg), 0) <= 0) {
					printf("Client closed the connection...\n");
					close(i);
					FD_CLR(i, &master);
					continue;
				}

				printf("%d %c %d =???\n", msg.a, msg.op, msg.b);

				switch(msg.op) {
					case '+': 	reply.result = msg.a + msg.b;
					break;
					case '*': 	reply.result = msg.a * msg.b;
					break;
				}
				send(i, &reply, sizeof(reply), 0);

			}
		}
	}
    }
    close(server);

    
    cout << " << Server is now terminating." << endl;
    return 0;
}
