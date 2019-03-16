#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED
#include <inttypes.h>

struct reqMSG{
    int32_t a;
    int32_t b;
    char op;
};

struct replyMSG
{
    int32_t result;
};

#endif // TYPES_H_INCLUDED
