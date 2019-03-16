#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <netdb.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#define DATA "Az üzenet megjött! \n"
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

/*
 * This program creates a datagram socket, binds a name to it, then
 * reads from the socket.
 */

main()
{
 
  int sock_fd1, sock_fd2;
  size_t length1, length2;
  struct sockaddr_in name1, name2;
  int buf1, buf2;
  // set up select parameters
  
  
  /* Create socket from which to read. */
  
  sock_fd1 = socket( AF_INET, SOCK_DGRAM, 0 );
  if( sock_fd1 == -1 ) {
	// osszeado port
	perror( "opening datagram socket 1" );
	exit(1);
		    }
  sock_fd2 = socket( AF_INET, SOCK_DGRAM, 0 );
  if( sock_fd2 == -1 ) {
	// kulonbseg port
	perror( "opening datagram socket 2" );
	exit(1);
		    }

  name1.sin_family = AF_INET;
  name1.sin_addr.s_addr = INADDR_ANY;
  name1.sin_port = 0; // elso szabad portot kapja meg

  name2.sin_family = AF_INET;
  name2.sin_addr.s_addr = INADDR_ANY;
  name2.sin_port = 0;

  
  if( bind( sock_fd1, (struct sockaddr *) &name1, sizeof name1 ) == -1 ) {
	perror( "binding datagram socket" );
	exit(2);}

  if( bind( sock_fd2, (struct sockaddr *) &name2, sizeof name2 ) == -1 ) {
	perror( "binding datagram socket" );
	exit(2);}

  length1 = sizeof(name1);
  length2 = sizeof(name2);

  if( getsockname( sock_fd1, (struct sockaddr *) &name1, &length1) == -1 ) {
	perror( "getting socket name1" );
	exit(3);
								      }
  printf( "Socket port1 #%d\n", ntohs( name1.sin_port ) );

  if( getsockname( sock_fd2, (struct sockaddr *) &name2, &length2) == -1 ) {
	perror( "getting socket name2" );
	exit(3);
								      }
  printf( "Socket port2 #%d\n", ntohs( name2.sin_port ) );


	int j = 0;
	while(j == 0)
	{
		fd_set socks;
		FD_ZERO(&socks);
		FD_SET(sock_fd1, &socks);
		FD_SET(sock_fd2, &socks);


		int nsocks = max(sock_fd1, sock_fd2) + 1;
		
	    if (select(nsocks, &socks, (fd_set *)0, (fd_set *)0, 0) >= 0) 
	    {
			printf("%s\n", "#");
		
			if (FD_ISSET(sock_fd1, &socks)) {

				recvfrom(sock_fd1, &buf1, sizeof buf1, 0, (struct sockaddr *) &name1, &length1);
				printf( "-->%s\n", "Megjött az üzenet1." );
				buf1 = buf1 + 1;
				if( sendto( sock_fd1, &buf1, sizeof buf1, 0, (struct sockaddr *) &name1, sizeof name1 ) == -1 ) 
				{
					++j;
					perror( "sending datagram message" );
				}
			}
		   
			if (FD_ISSET(sock_fd2, &socks)) {
				recvfrom(sock_fd2, &buf2, sizeof(buf2), 0, (struct sockaddr *) &name2, &length2);
				printf( "-->%s\n", "Megjött az üzenet2." );
				buf2 = buf2 - 1;
				if( sendto( sock_fd2, &buf2, sizeof buf2, 0, (struct sockaddr *) &name2, sizeof name2 ) == -1 ) 
				{
					++j;
					perror( "sending datagram message" );
				}
			}
		}
	}

	exit(0);

}
