#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <netdb.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#define DATA "Udvozlet udvozlet udvozlet ..."

/* This program sends a datagram to e receiver whose name is
 * given by the command line arguments.
 */

main( argc, argv )
  int argc;
  char *argv[];
{
  int sock;
  struct sockaddr_in name;
  struct hostent *hp, *gethostbyname();

/* create socket on which to send. */

  sock = socket( AF_INET, SOCK_DGRAM, 0 );
  if( sock == -1 ) {
	perror( "opening datagram socket" );
	exit(1);
		    }

  /* Construct name, with no wildcards, of the socket to "send"
   * to.
   */

  hp = gethostbyname( argv[1] );
  if( hp == (struct hostent *) 0 ) {
	fprintf( stderr, "%s: unknown host\n", argv[1] );
	exit(2);
				    }

  memcpy( (void *) &name.sin_addr, (void *) hp->h_addr,
	  hp->h_length );
  name.sin_family = AF_INET;
  name.sin_port = htons( atoi ( argv[2] ));

 /* Send message. */
 
  if( sendto( sock, DATA, sizeof DATA, 0, 
                (struct sockaddr *) &name, sizeof name )
    == -1 ) 
	perror( "sending datagram message" );

  close( sock );

  exit(0);

}
