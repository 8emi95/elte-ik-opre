#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //socket
#include <sys/types.h>  //socket
#include <netinet/in.h> //sockaddress

#include <netdb.h>
#include <string.h> //bzero fuggveny

int main(int argc,char * arg[]){
 int sockfd,newsockfd,clilen;
 int portno,n;
 struct sockaddr_in serv_addr,cli_addr;
 char buffer[256];
 
 if (argc<2){fprintf(stderr,"no port %s \n",arg[0]);exit(0);}
 portno=atoi(arg[1]); //portszam az 1. parameter


 bzero((char*)&serv_addr,sizeof(serv_addr)); //nullazza a cimen levo tombot 
 serv_addr.sin_family=AF_INET;
 serv_addr.sin_addr.s_addr=INADDR_ANY; //szerver IP cime
 serv_addr.sin_port=htons(portno);
 
 sockfd=socket(AF_INET,SOCK_STREAM,0); //0=???
 if (bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
   printf("error a kapcsolatnal - szerver"); 
 }
 listen(sockfd,5);
 clilen=sizeof(cli_addr);
 newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&clilen); //accept var egy kliensre
 if (newsockfd<0){perror("Socket megnyitasi hiba -szerver");exit(0);}
 
 bzero(buffer,256);
 n=read(newsockfd,buffer,256);
 if (n<0){printf("Hiba az olvasasnal-szerver");exit(1);}else {
   printf("szerver - Kaptam egy kerest:  %s\n",buffer);
   strcpy(buffer,"<HTT[BP><BODY>Megkaptam</BODY></HTML>\n");
   printf ("szerver - Kuldom a valaszt %s\n",buffer);
   n=write(newsockfd,buffer,strlen(buffer)+1);
 } 
 
 
 return 0;
}