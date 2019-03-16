#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct buffer
{
	short buf;
};

int prepareSocket(int);
void handleData(int, struct sockaddr_in, struct buffer, int);

int main(int argc, char* argv[])
{
	fd_set master;                 
	fd_set temp_fds;               
	
	struct sockaddr_in recv_addr;
	int fdmax, snum;
	int increment, decrement;   
                       
	struct buffer buff;
	int i;
	int incr_port, decr_port;
	
	struct timeval timeout;

	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	
	incr_port = atoi(argv[1]);
	decr_port = atoi(argv[2]);
	
	FD_ZERO(&master);
	FD_ZERO(&temp_fds);
  	
	increment = prepareSocket(incr_port);
	decrement = prepareSocket(decr_port);
	
	printf("Increment socket: %d Port: %d\n", increment, incr_port);
	printf("Decrement socket: %d Port: %d\n", decrement, decr_port);
  
	FD_SET(increment, &master);
	FD_SET(decrement, &master);
	fdmax = decrement;
	
	for(;;)
	{
		temp_fds = master;
		if((snum = select(fdmax + 1, &temp_fds, NULL, NULL, &timeout)) == -1)
		{
			perror("select");
			exit(1);
		}
		else if (snum == 0)
		{
			printf("...\n");
			timeout.tv_sec = 5;
			timeout.tv_usec = 0;
		}
		else
			for(i = 0; i <= fdmax; i++) 
			{
				if(FD_ISSET(i, &temp_fds))
				{
					if(i == increment)
						handleData(increment, recv_addr, buff, 1);
					else if (i == decrement)
						handleData(decrement, recv_addr, buff, -1);
				}
			}
	}
	return 0;
}

void handleData(int sock, struct sockaddr_in addr, struct buffer buff, int operation)
{
	int length;
	length = sizeof(addr);
	recvfrom(sock, &buff, sizeof(buff), 0, (struct sockaddr*) &addr, &length);
	printf("Data received from: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	printf("--> %d\n", ntohs(buff.buf));
	buff.buf = htons(ntohs(buff.buf) + operation);
	sendto(sock, &buff, sizeof(buff), 0, (struct sockaddr*) &addr, sizeof(addr));
	printf("--> %d\n", ntohs(buff.buf));
}

int prepareSocket(int port)
{
	int yes=1; 
	int sock;			
	struct sockaddr_in addr; 
	
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{
		perror("socket");
		exit(1);
	}
	
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt");
		exit(1);
	}
	
	addr.sin_family = AF_INET;  
	addr.sin_addr.s_addr = INADDR_ANY; 
	addr.sin_port = htons(port);       
	memset(&(addr.sin_zero),'\0',8);
	
	if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror("bind");
		exit(1);
	}
	
	return sock;
}