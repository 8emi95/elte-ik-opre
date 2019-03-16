#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <netdb.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

struct buffer
{
	short buf;
};
 
main(argc, argv)
	int argc;
	char *argv[];
{
	int sock;
	int num;
	int length;
	struct sockaddr_in name;
	struct hostent *hp, *gethostbyname();
	struct buffer buff;

	num = atoi(argv[3]);
	buff.buf = htons(num);
	
	sock = socket( AF_INET, SOCK_DGRAM, 0 );
	if( sock == -1 )
	{
		perror( "opening datagram socket" );
		exit(1);
	}

	hp = gethostbyname(argv[1]);
	if( hp == (struct hostent*) 0)
	{
		fprintf( stderr, "%s: unknown host\n", argv[1] );
		exit(2);
	}

	memcpy( (void *) &name.sin_addr, (void *) hp->h_addr, hp->h_length );
	name.sin_family = AF_INET;
	name.sin_port = htons(atoi(argv[2]));
	
	printf("--> %d\n", num);
	if(sendto(sock, &buff, sizeof(buff), 0,(struct sockaddr *) &name, sizeof name) == -1) 
		perror( "sending datagram message" );

	length = sizeof(name);
	if(recvfrom(sock, &buff, sizeof(buff), 0,(struct sockaddr *) &name, &length) == -1) 
		perror( "receiving datagram message" );
	printf("--> %d\n", ntohs(buff.buf));
	
	close( sock );

	exit(0);
}
