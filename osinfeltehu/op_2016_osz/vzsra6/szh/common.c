#include "common.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int sendSafe( int sock, const void * buffer, size_t length, int flags)
{
    return 0;
}

int recvSafe( int sock, void * buffer, size_t length, int flags)
{
    return 0;
}


int sendUnboundedString( int sock, char * msg)
{	
    return 0;
}

int recvUnboundedString( int sock, char * msg, int length)
{
    return 0;
}

