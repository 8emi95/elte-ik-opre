
#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<sys/socket.h>
#include<unistd.h>

#define PORT 8001
// Kiszolgáló

int main() {
    int sockfd,ujid;
    struct sockaddr_in 	cim;
    char uzenet[1024];

    cim.sin_family  = AF_INET;
    cim.sin_port 	  = htons( PORT );
    cim.sin_addr.s_addr = htonl( INADDR_ANY ); // 


    sockfd = socket( AF_INET, SOCK_DGRAM, 0 );
    bind(sockfd, (struct sockaddr *) &cim, sizeof(cim) );
    //listen(sockfd,3);
    //ujid=accept(sockfd,NULL,NULL);
    printf("Olvasunk a socket-bol!");
    fflush(NULL);
    read(sockfd, uzenet, sizeof(uzenet) );
    printf("Az uzenet: %s\n", uzenet );

    close(sockfd);

    return 0;
}
