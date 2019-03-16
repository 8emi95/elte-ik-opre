#ifndef ORA_COMMON_H
#define ORA_COMMON_H

typedef struct {
    char host[100];
    short port;
} server_info;

typedef union {
    server_info sinfo;
    char command[100];
	char gift_name[100];
    int integer;
} buffer;

#endif //ORA_COMMON_H
