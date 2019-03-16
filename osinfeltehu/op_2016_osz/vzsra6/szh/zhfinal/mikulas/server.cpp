/*
F7RMYO
szh ZH
sledge server
*/

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
	// init
	int sledgeMax = 15;	
	int sledgeWeight = 0;
	
	int sledgeFull = 0; //0 = accepting 1 = full
	
    int server = socket(AF_INET,SOCK_STREAM,0);
	if(server == -1) {
  		cerr << "Opening stream socket failed. ERROR:" << errno << endl;
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

	if(listen(server,4)==-1) {
		cerr << "Listening failed to start." << endl;
		return -1;
	}
    cout << "\t>> Server is ready to accept incomming calls." << endl;

    fd_set master;
    fd_set readfds;
    int maxfds = server;
    FD_ZERO(&master);
    FD_SET(server, &master);
	
	//old
    ReqMsg msg;
    ReplyMsg reply;
	
	//new
	GiftReq msg_in;
	ResultMsg msg_out;
	
    while (true) {

	readfds = master;

	select(maxfds+1, &readfds, 0, 0, 0);

	for (int i=0;i<maxfds+1;++i) {
		if (FD_ISSET(i, &readfds))
		{
			if (i==server) {
			        struct sockaddr_in caller;
			        socklen_t addrlen = sizeof(caller);
			        int call = accept(server,(struct sockaddr *) &caller, &addrlen);
				printf("New client connected...\n");
				FD_SET(call, &master);
				if (maxfds<call) maxfds = call;
			}
			else {
				//if (recv(i, &msg, sizeof(msg),0) <= 0) {
				if (recv(i, &msg_in, sizeof(msg_in),0) <= 0) {	
					printf("Client closed the connection...\n");
					close(i);
					FD_CLR(i, &master);
					continue;
				}

				
				if ( strcmp(msg_in.giftName,"elindultam")!=0 ) { 	//santa is not here
					if (sledgeFull == 0)  						//sledge can take more
					{
						sledgeWeight += msg_in.weight;  //gift is accepted
						
						if (sledgeWeight>=sledgeMax) 
						{
							sledgeFull = 1; //its full now;
						}	
						
						strcpy(msg_out.msg, "OK");
					}
					else //sledge is full
					{
						strcpy(msg_out.msg, "TELE");
					}	
					
					send(i, &msg_out, sizeof(msg_out), 0);
				}
				else //santa is here 
				{
					sledgeWeight = 0;
					sledgeFull = 0;
					
					//no reply for santa
				}	
				//printf("%d %c %d =???\n", msg.a, msg.op, msg.b);
				/*
				switch(msg.op) {
					case '+': 	reply.result = msg.a + msg.b;
					break;
					case '*': 	reply.result = msg.a * msg.b;
					break;
				}*/
				
				//response
				//send(i, &reply, sizeof(reply), 0);

			}
		}
	}
    }
    close(server);

    
    cout << " << Server is now terminating." << endl;
    return 0;
}
