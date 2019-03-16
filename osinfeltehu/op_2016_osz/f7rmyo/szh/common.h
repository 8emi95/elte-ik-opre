#ifndef __COMMON_H__
#define __COMMON_H__

#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int sendSafe( int sock, const void * buffer, size_t length, int flags);

int recvSafe( int sock, void * buffer, size_t length, int flags);

int sendUnboundedString( int sock, char * msg);

int recvUnboundedString( int sock, char * msg, int length);

#endif

