#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <signal.h>

#include "common.h"

int sock;

void sigint_handler()
{
	printf("Closing socket...\n");
	close(sock);
	exit(0);
}

int main(int argc, char **argv)
{
	size_t length;
	struct sockaddr_in name;
	buffer buf;
	memset(&buf, '\0', sizeof(buf));
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1) {
		perror("opening datagram socket");
		exit(1);
	}
	name.sin_family = AF_INET;
	name.sin_addr.s_addr = INADDR_ANY;
	name.sin_port = 0;
	if (bind(sock, (struct sockaddr *)&name, sizeof(name)) == -1) {
		perror("binding datagram socket");
		exit(2);
	}
	length = sizeof(name);
	if (getsockname(sock, (struct sockaddr *)&name, &length) == -1) {
		perror("getting socket name");
		exit(3);
	}
	printf("Socket port #%d\n", ntohs(name.sin_port));
	signal(SIGINT, sigint_handler);

	while (true)
	{
		if (recvfrom(sock, &buf, sizeof(buf), 0, (struct sockaddr *)&name, &length) == -1)
		{
			perror("receiving datagram socket");
			exit(1);
		}
		char command[100];
		strncpy(command, buf.command, sizeof(command));
		printf("received --> %s\n", command);
		server_info sinfo;
		if (strcasecmp(command, "INC") == 0)
		{
			strcpy(sinfo.host, "localhost");
			sinfo.port = 19800;
		}
		else if (strcasecmp(command, "DEC") == 0)
		{
			strcpy(sinfo.host, "localhost");
			sinfo.port = 19801;
		}
		else
		{
			strcpy(sinfo.host, "unknown");
			sinfo.port = 0;
		}
		printf("sending --> { host: %s, port: %d}\n", sinfo.host, sinfo.port);
		sinfo.port = htons(sinfo.port);
		memset(&buf, '\0', sizeof(buf));
		memcpy(&buf.sinfo, &sinfo, sizeof(server_info));
		if (sendto(sock, &buf, sizeof(buf), 0, (struct sockaddr *)&name, sizeof(name)) == -1)
		{
			perror("sending datagram message");
			exit(1);
		}
	}
	return 0;
}
