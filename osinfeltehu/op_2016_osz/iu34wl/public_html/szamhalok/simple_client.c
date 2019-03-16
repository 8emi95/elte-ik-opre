#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common.h"

/* Az atkuldendo uzenet: */
#define DATA " Udvozlet udvozlet udvozlet... "

/*
 * This program creates s socket and initiates a connection with the
 * socket given in the command line. Some data are sent over the
 * connection and then socket is closed, ending the connection.
 * The form of the command line is: streamwrite hostname portnumber
 */

int main( argc, argv)
  int argc;
  char *argv[];
{
  int sock;			// socket leiro
  struct sockaddr_in server;	// socket cimzesi struktura, a server eleresere vonatkozo infok lesznek benne
  struct hostent *hp;		// hostent a DNS-tol kapott informaciokat fogjuk majd ebben "tarolni", igy a domainnevhez tartozo IP cim is : hp->h_addr
  
  char buff[1024];		// csak egy buffer
  int rval;			// visszatero ertekek segedvaltozoja
  int variable;
  int encodedVar;

  /* Ellen�rizz�k az argumentumokat */
  if (argc<3)
  {
	printf("Usage:\n%s <server's hostname> <port>", argv[0]);
	exit(-1);
  } 


  /* Letrehozzuk a socketet: AF_INET - IPv4 halozat lesz a kommunikacios tartomany, SOCK_STREAM - TCP protokollt szeretnenk hasznalni */
  
  sock = socket( AF_INET, SOCK_STREAM, 0 );
  if( sock == -1 ) 
  {
	perror( "opening stream socket" );
	exit(1);
  }

  /* A socketet hozzarendeljuk a parancssorban megadott nevhez */

  server.sin_family = AF_INET; 		// IPv4 cimzes

 /*
  * megpr�b�ljuk meghat�rozni, feloldani a server IP c�m�t.
  * �ltal�ban a domain nevet haszn�ljuk a szerver azonos�t�s�ra: pl. www.elte.hu
  * Ezt a DNS nev� "adatb�zis" seg�ts�g�vel tudjuk IP c�mm� konvert�lni.
  * A DNS el�r�s�hez a gethostbyname fv.-t haszn�ljuk, mely egy strukt�r�t ad vissza, 
  * ami tartalmazza a n�vhez tartoz� inform�ci�kat.  
  */
  hp = gethostbyname( argv[1] ); 	// A nevhez tartozo IP cim feloldasa a DNS segitsegevel
  if ( hp == (struct hostent *) 0 ) 
  {
	fprintf( stderr, "%s: unknown host \n", argv[1] );
	exit(2);
  }

  /* server.sin_addr = hp->h_addr */
  memcpy( (void *) &server.sin_addr, (void *) hp->h_addr, hp->h_length );
  
  /* A server melyik portjahoz akarunk kapcsolodni... masodik parancssori argumentum*/
  server.sin_port = htons( atoi ( argv[2] ) );

  /* Kapcsolodas a serverhez... A "server" struktura tartalmazza a szukseges infokat  */
  /* Kesobb lesz: 3-way handshake : kapcsolat felepitese */    
  if( connect( sock, (struct sockaddr *) &server, sizeof server ) == -1 ) 
  {
	perror( "connecting stream socket" );
	exit(3);
  }

  /* Sikeres kapcsolat eseten. Ezen a ponton a kliens�nk �s a server k�z�tt l�trej�tt a biztons�gos b�jtcsatorna. 
   * Megkezd�dhet az �zenetv�lt�s... */

  /* Kuldjuk at a DATA stringet... ehhez a sock leirot hasznaljuk, erre kell irni a send paranccsal */
  if( send( sock, 5, sizeof int, 0 ) == -1 )
  {
	/* Hiba eseten -1 a visszatero ertek */
	perror( "writing on stream socket" );
  }

  /* Ha sikeresen atkuldtuk, akkor olvassunk a socketrol, azaz varjuk, hogy a kliens valaszoljon... */
  if(( rval = recv( sock, buff, sizeof buff, 0 )) == -1 )
  {
	/* Hiba */
        perror( "reading stream socket" );
  }
  else 
  { 
	/* A server uzenetet irassuk ki!!! */
	buff[rval] = '\0';
        printf( "Message from the server: %s\n", buff );
  }


  /* Socket leiro lezarasa */
  close( sock );
  
  exit(0);

}
