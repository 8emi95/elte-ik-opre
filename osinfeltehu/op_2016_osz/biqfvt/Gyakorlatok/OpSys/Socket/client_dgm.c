 

// Az �gyf�lfolyamat: 
	#include <stdio.h> 
	#include <stdlib.h> 
	#include <netdb.h> 
	#include <sys/socket.h> 
	#include <string.h> 
	#include <unistd.h> 
//	#define PORT 1001
#define PORT 0 
int main() { 
	int sockfd; 
	struct sockaddr_in  server_cim; 
	struct hostent * halo_adatok; 
	char buffer[] = "Hello!"; /* A kiszolg�l�nak csak a DNS nev�t ismerj�k, teh�t megszerezz�k az IP c�m�t. */ 	
	halo_adatok = gethostbyname("oprendszer.inf.elte.hu"); 
	/* A kiszolg�l� internetc�m�nek a megad�sa (IP c�m �s PORT). */ 
	server_cim.sin_family = AF_INET; 
	server_cim.sin_port = htons( PORT ); 
	//server_cim.sin_addr = *(struct in_addr *)halo_adatok->h_addr; 
	server_cim.sin_addr.s_addr=inet_addr("127.0.0.1");
	sockfd = socket(PF_INET, SOCK_DGRAM, 0 ); 
	connect(sockfd,(struct sockaddr *) &server_cim, sizeof(server_cim)); 
	write( sockfd, buffer, strlen(buffer) + 1 ); 
	close(sockfd); 
	return 0; 
}