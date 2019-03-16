#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	
	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	char buff[1024];
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("ERROR opening socket");
		exit(1);
	}

	server.sin_family = AF_INET;
	
	hp = gethostbyname(argv[1]);

	if (hp == 0) {
		perror("ERROR gethostbyname");
		exit(1);
	}

	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	
	server.sin_port = htons(5000);
	
	if (connect(sock, (struct sockaddr*) &server, sizeof(server)) < 0) {
		perror("ERROR connecting");
		close(sock);
		exit(1);
	}

	if (send(sock, argv[2], sizeof(argv[2]), 0) < 0) {

		perror("ERROR sending msg");
		close(sock);
		exit(1);
	}
	
	printf("Sent %s\n", argv[2]);
	close(sock);

	return 0;
}
