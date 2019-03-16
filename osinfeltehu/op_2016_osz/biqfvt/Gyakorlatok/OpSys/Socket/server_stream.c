

/* 
  ** server.c - egy stream socket szerver demo 
  */ 

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <sys/wait.h> 
#include <signal.h> 

#define MYPORT 3490 // A port haszn�l�i ide fognak csatlakozni. 

#define BACKLOG 10 // Ennyi kezeletlen kapcsolat v�rakozhat sorban 

void sigchld_handler(int s) { 
     while(wait(NULL) > 0);
} 

int main(void) { 
     int sockfd, new_fd; // Figyel�s-hallgat�s a sock_fd-n, 
                         // �j kapcsolat a new_fd-n.
     struct sockaddr_in my_addr;    // A saj�t c�minform�ci�m. 
     struct sockaddr_in their_addr; // aA csatlakoz� c�minform�ci�ja. 
     int sin_size; 
     struct sigaction sa;
     int yes=1;
     
     if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { 
          perror("socket"); 
          exit(1); 
     } 
     
     if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) 
         == -1) { 
          perror("setsockopt"); 
          exit(1); 
     } 
     
     my_addr.sin_family = AF_INET; // host byte order 
     my_addr.sin_port = htons(MYPORT); // short, network byte order
     my_addr.sin_addr.s_addr = INADDR_ANY; 
                                 // Automatikusan kit�lti az IP c�memmel.
     memset(&(my_addr.sin_zero), 0, 8); 
                                 // Nulla a strukt�ra t�bbi r�sz�ben 
     
     if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))     
         == -1) { 
          perror("bind"); 
          exit(1); 
     }
     
     if (listen(sockfd, BACKLOG) == -1) { 
          perror("listen"); 
          exit(1); 
     }
  sa.sa_handler = sigchld_handler; // Lev�g minden halott folyamatot. 
     sigemptyset(&sa.sa_mask); 
     sa.sa_flags = SA_RESTART; 
     
     if (sigaction(SIGCHLD, &sa, NULL) == -1) { 
          perror("sigaction"); 
          exit(1); 
     } 
     
     while(1) { // main accept() loop 
          sin_size = sizeof(struct sockaddr_in); 
          if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, 
             &sin_size)) == -1) { 
               perror("accept"); 
               continue; 
          } 
          printf("server: got connection from %s\n", 
                 inet_ntoa(their_addr.sin_addr)); 
          
          if (!fork()) { // ez a gyerekfolyamat 
               close(sockfd); // a gyereknek nincs sz�ks�ge a list�z�ra. 
               if (send(new_fd, "Hello, world!\n", 14, 0) == -1) 
                    perror("send"); 
               close(new_fd); 
               exit(0); 
          } 
          
          close(new_fd); // a sz�l�nek nincs sz�ks�ge erre.
     } 

     return 0; 
}
