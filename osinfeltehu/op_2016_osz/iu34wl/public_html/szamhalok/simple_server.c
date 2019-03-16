//Általában ezek a fejelemek kellenek:
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

#define TRUE 1

/*
 * This program creates a socket and then begins an infinite loop.
 * Each time through the loop it accepts a connection an prints
 * data from it. Whwn the connection breaks, or the client closes
 * the connection, the program accepts a new connection.
 */

void printServerInfo(int sock);

int main()
{
  int sock;			// socket leiro
  struct sockaddr_in server;	// socket cimzesi struktura, a serverre vonatkozik
  struct sockaddr_in client;
  int conn;			// A kapcsolat felepulese utan ezen a socket leiron keresztul kommunikalunk a klienssel
  char buf[1024];		// buffer
  int rval;			// seged valtozo
  socklen_t length;		// ez is
  int variable;
  int encodedVar;

  /* Letrehozzuk a socketet: AF_INET - IPv4 halozat lesz a kommunikacios tartomany, SOCK_STREAM - TCP protokollt szeretnenk hasznalni */
  sock = socket( PF_INET, SOCK_STREAM, 0 ); // 0 helyett lehetne IPPROTO_TCP is, az a default STREAMing protokoll
  if( sock == -1 ) 
  {
	perror( "opening stream socket" );
	exit(1);
  }
 
  /* A server "socketcimzes" beallitasa */

  server.sin_family = AF_INET;		// IPv4 cimzes
  server.sin_addr.s_addr = INADDR_ANY;	// A futtato gep IP cimet hasznaljuk
  server.sin_port = 0;			// 0 port = valassz egy hasznalaton kivuli portot, az oprendsz adja... 

  /* Socket tarsitasa a "server" strukturaban megadott porthoz...*/
  if( bind( sock, (struct sockaddr *) &server, sizeof server ) == -1 ) 
  {
	perror( "binding stream socket" );
	exit(2);
  }

  /* Irassuk ki a sockethez rendelt IP címet, és a portot. A bind után egy szabad portot rendelt a sockethez az OS. 
   * Most már lekérdezhetjük melyiken figyel majd a szerverünk!
   */

  printServerInfo( sock );

  /* Jelezzuk, hogy a socket kapcsolatot akar fogadni ... 5 legyen a queue merete */
  listen( sock, 5 );
  printf("Listening...\n");


  /* Lénygeében itt ér véget server elõkészítése. Ezt követõen kell a kliensek fogadásával foglalkozni! */
  do {
	length = sizeof client;
	
	/* Kapcsolatra varakozas es annak elfogadasa: a visszakapott (socket) kapcsolat leiron(conn) keresztul tudunk kommunikalni a klienssel */
	/* A client strukturaba pedig bekerulnek a kliens adatai */
	conn = accept( sock, (struct sockaddr *) &client, &length );
	
	if( conn == -1 ) 
	{
		/* Hiba eseten*/
		perror( "accept" );
	}
	else
	{
		/* A kliens adatainak kiirasa */
		printf( "\nA client has arrived.\n");
		printf( "\tform IP:port : %s:%d\n", inet_ntoa( client.sin_addr ), ntohs( client.sin_port ) );

		/* buf = {0,0,0,0...} */
		memset( (void *)buf, 0, sizeof buf );
		
		/* Varjuk, hogy a kliens kuldjon valamilyen uzenetet: ez a buf-ba fog kerulni, az olvasott byteok szama pedig rval-ba  */
		/* recv helyett használható a read fv. is! */
		if(( rval = recv( conn, buf, 1024, 0 )) == -1 )
		{
			/* Hiba */
			perror( "reading stream socket" );
		}
		else 
		{
			/* Irassuk ki a std kimenetre a kapott uzenetet */
			printf( "Message from the above client:\"%d\"\n", buf );
			/* send helyett használható a write fv. is! */
  			if( send( conn, buf, rval, 0 ) == -1 )
			    perror( "writing on stream socket" );
			
                }
	

		// Kapcsolat lezarasa
	        close( conn ); 
	}
      } while( TRUE );

  /*
   * Since thid program has an infinite loop, the socket "sock" is
   * never explicitly closed. However, all sockets will be closed
   * automatically when a process is killed or terminates normally.
   */
  
  exit(0);

}

// Kiiratja a szerver adatait: ip címet, ill. azt, hogy melyik porton figyel a szerver!
void printServerInfo(int sock)
{
	struct sockaddr_in server_info;
	socklen_t length = sizeof server_info;
  
	/* Lekerdezzuk a socket adatait, ezutan a server_info struktura mar a valos port-ot es ip cimet fogja tartalmazni... */
	if( getsockname( sock, (struct sockaddr *) &server_info, &length ) == -1 ) 
  	{
		perror( "getting socket name" );
		exit( 3 );
  	}

  	/* Irassuk ki az IP címet és a portot */
	printf( "Server IP: %s\n", inet_ntoa( server_info.sin_addr ) );
  	printf( "Server port: %d\n", ntohs( server_info.sin_port ) );


}




