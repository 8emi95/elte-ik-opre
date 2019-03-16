#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<signal.h>
#include<string.h>
#include<syslog.h>

char    file_neve[1024];
void    *buffer;
size_t  file_meret;

void fajlt_betolt() {
	struct stat st;
	int fd,i;
	i=stat(file_neve,&st);
	if(i==0)
	{
	file_meret = st.st_size;
	buffer = malloc(file_meret);
	fd = open(file_neve,O_RDONLY);
	read(fd,buffer, file_meret);
	close(fd);
	}
	else
	{
	buffer=malloc(20);
	file_meret=10;
	strcpy(buffer,"Hiba van!");
	}
}

void keres(int socket_id)
{
	// a kliens keres beolvasasa, mentese
	char tmp[1024];
	bzero(tmp,1024);
	int db=read(socket_id,tmp,sizeof(tmp));
	int fd=open("keres.log",O_CREAT|O_WRONLY|O_APPEND,S_IRUSR|S_IWUSR);
	tmp[db]=10;
	write(fd,tmp,sizeof(tmp));
	close(fd);
	bzero(file_neve+1,1023);
	sscanf(tmp,"GET %s",&file_neve[1]);
	printf("Keres jott: %s",file_neve);
}

void valasz(int socket_id)
{
	char* s="HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n";
	write(socket_id,s,strlen(s));
	write(socket_id,buffer,file_meret);
}

void sighandler(int type){
	switch(type) {
	   case SIGCHLD:
	      while(waitpid( WAIT_ANY, NULL, WNOHANG )>0); 
	      break;
	   case SIGTERM:
	      openlog("proba webserver",LOG_PERROR,LOG_USER);
	      syslog(LOG_INFO,"SIGTERM signalt kaptam\n");
	      closelog();
	      exit(0);
	      break;
	}
}


int main(int argc, char** argv){

   int socket_id,uj_socket_id;
   struct sockaddr_in sock_cim;

   if(argc<2){
      puts("Parameternek adja meg a PORT-ot.");
      return 1;
   }    

   //
   bzero(file_neve,1024);
   file_neve[0]='.';

   signal(SIGCHLD,sighandler);
   signal(SIGTERM,sighandler);   

   sock_cim.sin_family	= AF_INET;
   sock_cim.sin_port	= htons(atoi(argv[1]));
   sock_cim.sin_addr.s_addr = htonl(INADDR_ANY);

   socket_id = socket(AF_INET,SOCK_STREAM,0);
   bind(socket_id,(struct sockaddr *) &sock_cim,sizeof(sock_cim));
   listen(socket_id,5);

   while(1){
	uj_socket_id = accept(socket_id,NULL,NULL);
	if(fork()==0){
	    keres(uj_socket_id);
	    fajlt_betolt();
	    valasz(uj_socket_id);
	    shutdown(uj_socket_id,SHUT_RDWR);
	    free(buffer);
	    return 0;
	}
   }
   return 0;	
}
