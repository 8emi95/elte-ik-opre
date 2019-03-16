#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){



	/* variables */
	int sock;
	struct sockaddr_in server;
	int mysock;
	char buff[1024];
	int rval;


	/* create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("ERROR opening socket");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5000);
	
	/* call bind */
	if (bind(sock,(struct sockaddr *)&server, sizeof(server))) {
		perror("ERROR binding");
		exit(1);
	}
	/* listen */
	listen(sock, 5);
		

	/* accept */
	do {
		mysock = accept(sock, (struct sockaddr*) 0, 0);
		if (mysock == -1)
		{
			perror("ERROR accept failed");
		}
		else 
		{
			memset(buff, 0, sizeof(buff));
			if ((rval = recv(mysock, buff, sizeof(buff), 0)) < 0) {
				perror("ERROR reading stream message");
			} else if (rval == 0)
				printf("Ending connection\n");
			else {
				printf("Stream message successfully read");
				printf("Message is: %s\n", buff);
			}		
			close(mysock);
		}
	} while (1);
	return 0;
}

