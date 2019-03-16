#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //socket
#include <sys/types.h>  //socket
#include <netinet/in.h> //sockaddress

#include <netdb.h>
#include <string.h> //bzero fuggveny


int main(int argc,char * arg[]){
 int klienssocket=socket(AF_INET,SOCK_STREAM,0); //0=???
 int portno,n;
 struct sockaddr_in serv_addr;
 struct hostent *server;
 int hossz=15000;
 char buffer[hossz];
 if (argc<3){fprintf(stderr,"usage %s hostname port\n",arg[0]);exit(0);}
 portno=atoi(arg[2]); //portszam a 2. parameter
 server=gethostbyname(arg[1]);//webszerver neve
 if (server==NULL){fprintf(stderr,"Nem elerheto a szerver\n");exit(0);}
 if (klienssocket<0){perror("Socket megnyitasi hiba\n");exit(0);}
 bzero((char*)&serv_addr,sizeof(serv_addr)); //nullazza a cimen levo tombot
 
 serv_addr.sin_family=AF_INET;
 serv_addr.sin_port=htons(portno); //konvertaljuk network port szam tipusuva - byte sorrend
 bcopy((char*)server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);
 int soc=connect(klienssocket,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
 if (soc<0){
  fprintf(stderr,"Error konnekt -kliens\n");exit(0);
 }
// printf("Irja le az uzenetet");
// fgets(buffer,hossz,stdin);
  strcpy(buffer,"GET /index.html HTTP/1.0\r\n\r\n");
  printf("Kuldom az uzenetet -kliens %s\n",buffer);
  n=write(klienssocket,buffer,strlen(buffer));
 
// printf("Visszajott az irasbol  - kliens\n\n");
 if (n<0){fprintf(stderr,"Hibas socketbe iras - kliens\n");exit(0);}
 bzero(buffer,hossz); //torles
 n=read(klienssocket,buffer,hossz);
 if (n<0){fprintf(stderr,"Hiba az olvasasnal -kliens\n");exit(0);}
 printf("A szervertol jott valasz: %s\n",buffer);
 
 return 0;
}