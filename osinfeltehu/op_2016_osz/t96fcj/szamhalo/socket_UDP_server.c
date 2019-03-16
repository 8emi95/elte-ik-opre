#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <netdb.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

/*
 * This program creates a datagram socket, binds a name to it, then
 * reads from the socket.
 */

main()
{
  int sock;
  socklen_t length;
  struct sockaddr_in name;
  char buf[1024];

  /* Create socket from which to read. */

  sock = socket( AF_INET, SOCK_DGRAM, 0 );
  if( sock == -1 ) {
	perror( "opening datagram socket" );
	exit(1);
		    }

  /* create name with wildcards */

  name.sin_family = AF_INET;
  name.sin_addr.s_addr = INADDR_ANY;
  name.sin_port = 0;

  if( bind( sock, (struct sockaddr *) &name, sizeof name ) == -1 ) {
	perror( "binding datagram socket" );
	exit(2);
								    }

 /* Find assigned port value and print it out. */

  length = sizeof(name);
  
  if( getsockname( sock, (struct sockaddr *) &name, &length) == -1 ) {
	perror( "getting socket name" );
	exit(3);
								      }
  printf( "Socket port #%d\n", htons (name.sin_port) );
  
  

  /* Read from the socket. */

  if( read( sock, buf, 1024 ) == -1 )
	perror( "receiving datagram socket" );
  printf( "-->%s\n", buf );
  
  close( sock );

  exit(0);

}
