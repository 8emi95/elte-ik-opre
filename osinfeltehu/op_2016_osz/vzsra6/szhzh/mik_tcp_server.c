#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>

#include "common.h"

#define PORT1 19800
#define PORT2 19801

int create_listener(short port, fd_set *master)
{
    struct sockaddr_in local_addr;
    int listener;
    int yes = 1;
    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    printf("socket (FD #%d) created\n", listener);
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit(1);
    }
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(port);
    memset(&(local_addr.sin_zero), '\0', 8);
    if (bind(listener, (struct sockaddr *)&local_addr, sizeof(local_addr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    if (listen(listener, 10) == -1)
    {
        perror("listen");
        exit(1);
    }
    FD_SET(listener, master);
    return listener;
}

int accept_listener(int listener, fd_set *master, int *fdmax)
{
    struct sockaddr_in remote_addr;
    int accept_fd;
    int addrlen = sizeof(remote_addr);
    if ((accept_fd = accept(listener, (struct sockaddr *)&remote_addr, &addrlen)) == -1)
        perror("accept");
    else
    {
        FD_SET(accept_fd, master);
        if (accept_fd > *fdmax)
            *fdmax = accept_fd;
        printf("selectserver: new connection from %s (socket FD #%d)\n", inet_ntoa(remote_addr.sin_addr), accept_fd);
    }
    return accept_fd;
}

void *print_thread(void *arg)
{
    while (true)
    {
        sleep(5);
        printf(".\n");
    }
    return 0;
}

int main(void)
{
    buffer buf;
    fd_set master;
    fd_set read_fds;
    fd_set write_fds;
    int fdmax;        
    int goblin_listener;
    int child_listener;
    bool incr;        
    int nbytes;
    int value;
    int sendval;
    int i, j;
    pthread_t tid;
    pthread_create(&tid, NULL, &print_thread, NULL);
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);

    goblin_listener = create_listener(PORT1, &master);
    child_listener = create_listener(PORT2, &master);

    fdmax = goblin_listener < child_listener ? child_listener : goblin_listener;

    for (;;)
    {
        read_fds = master;
        write_fds = master;
        if (select(fdmax + 1, &read_fds, &write_fds, NULL, NULL) == -1)
        {
            perror("select");
            exit(1);
        }
        for (i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds) && (goblin_listener == i || child_listener == i))
            {
                accept_listener(i, &master, &fdmax);
                incr = (goblin_listener == i);        
            }
            else if (FD_ISSET(i, &read_fds) && FD_ISSET(i, &write_fds))
            {
                if ((nbytes = recv(i, &buf, sizeof(buf), 0)) <= 0)
                {
                    if (nbytes == 0)
                        printf("selectserver: socket (FD #%d) hung up\n", i);
                    else
                        perror("recv");
                    close(i);
                    FD_CLR(i, &master);
                }
                else
                {
                    value = ntohl(buf.integer);
                    sendval = value + (incr ? 1 : -1);
                    printf("received %d, sending %d\n", value, sendval);
                    buf.integer = htonl(sendval);
                    if (send(i, &buf, sizeof(buf), 0) == -1)
                        perror("send");
                }
            }
        }
    }

    return 0;
}