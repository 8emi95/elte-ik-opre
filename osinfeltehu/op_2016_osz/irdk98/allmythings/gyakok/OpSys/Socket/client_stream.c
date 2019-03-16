/* ** client.c - egy stream socket kliens demo */ 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#define PORT 3490 // Az a port, ahova a kliens csatlakozni fog. 
#define MAXDATASIZE 100 // Az egyszerre kaphat� b�jtok maxim�lis �rt�ke. 

int main(int argc, char * argv[]) { 
	int sockfd, numbytes; 
	char buf[MAXDATASIZE]; 
	struct hostent * he; 
	struct sockaddr_in their_addr; // A csatlakoz� c�minform�ci�ja. 
	if (argc != 2) { fprintf(stderr,"usage: client hostname\n"); exit(1); } 
	if ((he=gethostbyname(argv[1])) == NULL) 
	 { 
	  // Megkapja a hosztinform�ci�t. 
	  perror("gethostbyname"); exit(1); 
	  } 
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { perror("socket"); exit(1); } 	
	their_addr.sin_family = PF_INET; // host byte order 
	their_addr.sin_port = htons(PORT); // short, network byte order 
	their_addr.sin_addr = *((struct in_addr *)he->h_addr); 
	memset(&(their_addr.sin_zero),0, 8); // Kinull�zza a strukt�ra t�bbi r�sz�t. 
	if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) { perror("connect"); exit(1); } 
	if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) 
	      { perror("recv"); exit(1); } 
	buf[numbytes] = 0; 
	printf("Received: %s\n",buf); 
	close(sockfd); 
	return 0; 
}