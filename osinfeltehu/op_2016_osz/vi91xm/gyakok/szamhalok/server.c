#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 19821

struct userrecord
{
  char name[10];
  char msg[60];
};

int main(void)
{
	fd_set master;           
	fd_set temp_fds;
	struct sockaddr_in local_addr; 
	struct sockaddr_in remote_addr;
	int fdmax;           
	int listener; // fájl deszkriptor index
	int accept_fd;        
                       
	struct userrecord client_buf;
	int nbytes; 
	int yes=1;  
	int addrlen;
	int i, j; 

	FD_ZERO(&master);
	FD_ZERO(&temp_fds);
  
	if((listener = socket(AF_INET, SOCK_STREAM,0)) == -1)  //
	{
		perror("socket");
		exit(1);
	}
  	
	printf("%d socket\n",listener);
	if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt");
		exit(1);
	}
  
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = INADDR_ANY;
	local_addr.sin_port = htons(PORT);   
	memset(&(local_addr.sin_zero),'\0',8);
	
	if(bind(listener, (struct sockaddr *)&local_addr,sizeof(local_addr)) == -1)
	{
		perror("bind");
		exit(1);
	}
  
	if(listen(listener, 10) == -1)
	{
		perror("listen");
		exit(1);
	}
  
	FD_SET(listener, &master); // beteszi a figyelendő dolgok közé
	fdmax = listener;
	
	for(;;)
	{
	  temp_fds = master;
		// selectben megmondom, mi az a maximális tartomány, amiben a fájldeszkriptor vizsgál
		// ezekre fogja elvégezni a kérdést: van-e ott várakozó adat?
		// readfds, writefds, exceptfds
		if(select(fdmax+1, &temp_fds, NULL, NULL, NULL) == -1)
		{
			perror("select");
			exit(1);
		}
		for(i=0; i<= fdmax; i++) 
		{
		  if(FD_ISSET(i, &temp_fds))
			{
			   if(listener==i){
				    addrlen = sizeof(remote_addr); 
				    
            if((accept_fd = accept(listener, (struct sockaddr *)&remote_addr, &addrlen))==-1)
				    {  
					     perror("accept");
				    }
				    else
				    { 
					    FD_SET(accept_fd, &master); // ezt is berakja
					    if(accept_fd>fdmax)
					    {
						    fdmax= accept_fd;
					   }
					  
					   printf("selectserver: new connection from %s on""socket %d\n",
						    inet_ntoa(remote_addr.sin_addr), accept_fd);
				    }
			   }
         else 
			   {  
				memset(client_buf.msg,'\0',sizeof(client_buf.msg));
				memset(client_buf.name,'\0',sizeof(client_buf.name));
			      if((nbytes = recv(i, &client_buf, sizeof(client_buf),0)) <= 0)
				    {  
					     if(nbytes == 0)
					     {
						      printf("selectserver: socket %d hung up\n", i);
					     }
					     else
					     {
						      perror("recv");
					     }
               
							close(i);
							FD_CLR(i, &master); // kiszedi a figyelendő pool-ból
				    }
				    else
				    { 
				       printf("%s says %s",client_buf.name,client_buf.msg);
				       for( j=0; j <= fdmax ; j++ )
					     {
						      if(FD_ISSET(j, &master))
						      {
							       if(j!= listener && j!=i)
							       {
								        if(send(j,&client_buf,sizeof(client_buf),0) == -1) perror("send");
							       }
						      }
					     }
				    }
			   }
		  }
	  }
   }
	 return 0;
}