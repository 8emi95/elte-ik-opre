
#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>

#define PORT 8001
// kliens

int main() {
    int sockfd;
    struct sockaddr_in server_cim;
    struct hostent *halo_adatok;
    char buffer[] = "Almafa alatt!";

    halo_adatok = gethostbyname("oprendszer.inf.elte.hu");

    server_cim.sin_family = AF_INET;
    server_cim.sin_port   = htons( PORT );
    server_cim.sin_addr   = *(struct in_addr *)halo_adatok->h_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0 ); // SOCK_DGRAM,vagy SOCK_STREAM
    printf("A socket id: %d\n",sockfd);
    int cid=connect(sockfd,(struct sockaddr *) &server_cim, sizeof(server_cim));
    printf("Felvettem a kapcsolatot! Azonositoja: %d\n",cid);
    write( sockfd, buffer, strlen(buffer) );

    close(sockfd);

    return 0;
}
