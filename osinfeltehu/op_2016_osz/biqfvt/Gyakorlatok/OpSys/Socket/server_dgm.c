 // A kiszolg�l� folyamat: 
#include <stdio.h> 
#include <stdlib.h> 
#include <netdb.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#define PORT 1001 
int main() { 
	int sockfd; 
	struct sockaddr_in  cim; 
	char uzenet[1024]; /* Az internetc�m megad�sa (az IP c�m �s a PORT). A kiszolg�l� b�rmely IP c�mr�l fogadhat adatot (INADDR_ANY) */ 
	cim.sin_family = AF_INET; 
	cim.sin_port = htons( PORT ); 
	cim.sin_addr.s_addr = htonl( INADDR_ANY ); 
	sockfd = socket( PF_INET, SOCK_DGRAM, 0 ); 
	bind(sockfd, (struct sockaddr *) &cim, sizeof(cim) ); 
	read(sockfd, uzenet, sizeof(uzenet) ); 
	printf("Az uzenet: %s\n", uzenet ); 
	close(sockfd); 
	return 0; } 

