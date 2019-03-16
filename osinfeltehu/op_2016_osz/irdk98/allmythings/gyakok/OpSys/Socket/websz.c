#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //socket
#include <sys/types.h>  //socket
#include <netinet/in.h> //sockaddress
#include <sys/stat.h>
#include <netdb.h>
#include <string.h> //bzero fuggveny
#include <fcntl.h>

int main(int argc,char * arg[]){

 FILE* f;
 int sockfd,newsockfd,clilen;
 int portno,n;
 struct sockaddr_in serv_addr,cli_addr;
 int hossz=2000;
 char buffer[hossz];
 
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
 
 bzero(buffer,hossz);
 n=read(newsockfd,buffer,hossz);
 if (n<0){printf("Hiba az olvasasnal-szerver");exit(1);}else {
   printf("szerver - Kaptam egy kerest:  %s\n",buffer);
   char* fnev;
   fnev=strtok(buffer,"/");
   fnev=strtok(NULL," ");
   printf("szerver- ezt a filet kertek tolem: %s\n",fnev);
   printf("hossz%i\n",strlen(fnev));
//   int fd=open(fnev,O_RDONLY); //olvasasra megnyitom
//   if (fd<0) {perror("fajl nyitasi hiba");exit(1);}
//   bzero(buffer,hossz);
//   printf("%i\n",hossz);
//   read(fd,buffer,strlen(buffer));
//   close(fd); // nem megy igy  

   char sor[1000];
   strcpy(buffer,"");
   f=fopen(fnev,"r");
   while (!feof(f)){
     fgets(sor,1000,f);
     strcat(buffer,sor);
   }  
   printf("\nfile tartalma%s\n",buffer);
   fclose(f);
   
   char fejlec[hossz];
   strcpy(fejlec,"HTTP/1.0 200 OK\r\nServer:proba/0.1\r\nConnection:close\r\nContent-type:text/html\r\n\r\n");
   strcat(fejlec,buffer);
   printf ("szerver - Kuldom a valaszt %s\n",fejlec);
   n=write(newsockfd,fejlec,strlen(fejlec)+1);
 } 
 
 
 return 0;
}