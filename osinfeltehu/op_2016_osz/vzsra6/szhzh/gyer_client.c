#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "common.h"

void get_tserver_info(char *ts_host, short *ts_port, char *us_host, short us_port, char *gob_or_ch, char *gift)
{
	int sock;
	struct sockaddr_in name;
	struct hostent *hp, *gethostbyname();
    struct sockaddr adr;
    size_t length;
	buffer buf;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1) {
		perror("opening datagram socket");
		exit(1);
	}
	hp = gethostbyname(us_host);
	if (hp == (struct hostent *)0) {
		fprintf(stderr, "%s: unknown host\n", us_host);
		exit(2);
	}
	memcpy((void *)&name.sin_addr, (void *)hp->h_addr, hp->h_length);
	name.sin_family = AF_INET;
	name.sin_port = htons(us_port);
    memset(&buf, '\0', sizeof(buf));
	strcpy(buf.command, gob_or_ch);
	if (sendto(sock, &buf, sizeof(buf), 0, (struct sockaddr *)&name, sizeof(name)) == -1)
    {
        perror("sending datagram message");
        exit(1);
    }
	if (recvfrom(sock, &buf, sizeof(buf), 0, &adr, &length) == -1)
    {
        perror("receiving datagram socket");
        exit(1);
    }
    server_info sinfo;
    memcpy(&sinfo, &buf.sinfo, sizeof(server_info));
	sinfo.port = ntohs(sinfo.port);
	strcpy(ts_host, sinfo.host);
	*ts_port = sinfo.port;
	close(sock);
}

void send_to_tserver(char *ts_host, short port, int value)
{
    int sock, rval;
    size_t length;
    struct sockaddr_in server;
    struct hostent *hp, *gethostbyname();
    buffer buf;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("opening stream socket");
        exit(1);
    }
    server.sin_family = AF_INET;
    hp = gethostbyname(ts_host);
    if (hp == (struct hostent *)0)
    {
        fprintf(stderr, "%s: unknown host \n", ts_host);
        exit(2);
    }
    memcpy((void *)&server.sin_addr, (void *)hp->h_addr, hp->h_length);
    server.sin_port = htons(port);
    if (connect(sock, (struct sockaddr *)&server, sizeof server) == -1)
    {
        perror("connecting stream socket");
        exit(3);
    }
    printf("sending --> %d\n", value);
    buf.integer = htonl(value);
    if (write(sock, &buf, sizeof(buf)) == -1)
    {
        perror("writing on stream socket");
        exit(1);
    }
    memset(&buf, '\0', sizeof(buf));
    if ((rval = read(sock, &buf, sizeof(buf))) == -1)
    {
        perror("reading stream socket");
        exit(1);
    }
    int mod_val;
    mod_val = ntohl(buf.integer);
    printf("received --> %d\n", mod_val);
    shutdown(sock, 1);
    close(sock);
}

int main(int argc, char **argv)
{
	char hostname[100];
	short port;
	char* answer;
	struct sockaddr_in name;
	size_t length = sizeof(name);
	get_tserver_info(hostname, &port, argv[1], atoi(argv[2]), argv[3]);
    if (strcmp(hostname, "unknown") == 0)
    {
        printf("unknown command (use either INC or DEC)\n");
        exit(1);
    }
    printf("tserver address for %s is {host: %s, port: %d}\n", argv[3], hostname, port);
    send_to_tserver(hostname, port, atol(argv[4]));
    return 0;
}
