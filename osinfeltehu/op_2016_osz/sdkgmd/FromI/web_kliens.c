#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //socket
#include <sys/types.h>  //socket
#include <netinet/in.h> //sockaddress

#include <netdb.h>
#include <string.h> //bzero fuggveny helye


int main(int argc,char * arg[])
{
 
 int port;
 struct sockaddr_in serv_addr;
 struct hostent *server;
 int hossz=2000;
 char buffer[hossz];
 if (argc<4)
	{perror("Hasznalat: web_kliens hostnev fajl port\n");exit(0);}
 port=atoi(arg[3]); //portszam a 3. parameter
 server=gethostbyname(arg[1]);//webszerver neve
  //if (server==NULL) {perror("Nem elerheto a szerver\n");exit(0);}
 int klienssocket=socket(AF_INET,SOCK_STREAM,0);
 if (klienssocket<0)
	{perror("Socket megnyitasi hiba\n");exit(0);}
 //bzero((char*)&serv_addr,sizeof(serv_addr)); //nullazza a cimen levo tombot
 // cim beallaitas
 serv_addr.sin_family=AF_INET;
 serv_addr.sin_port=htons(port); //konvertaljuk network port szam tipusuva
 serv_addr.sin_addr=*(struct in_addr*)server->h_addr;
  //
 int soc=connect(klienssocket,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
 if (soc<0)
 {
  perror("Error kliens konnekt!\n");exit(0);
 }
 // uzenet kuldes
  sprintf(buffer,"GET /%s HTTP1.1",arg[2]);
  printf("Kuldom az uzenetet: %s\n",buffer);
  write(klienssocket,buffer,strlen(buffer));
 // valasz
  bzero(buffer,hossz); //torles
  read(klienssocket,buffer,hossz);

  printf("A szervertol jott valasz: %s\n",buffer);
 
 return 0;
}
