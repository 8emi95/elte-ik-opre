#ifndef READ_HELPERS_H
#define READ_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  /* isspace */
#include <errno.h>  /* errno */

char* read_new_string(FILE* file_handle)
{
    int c = '\n';
    unsigned capacity = 10;
    unsigned write_ind = 0;
    char* ret = calloc(capacity,sizeof(char));
    /*No empty strings or strings beginning with whitespace!*/
    while( (c = fgetc(file_handle)) != EOF && (isspace(c) || '\n' == c) );
    if(EOF == c)
    {
    	free(ret);
    	return NULL;	
    } 

    do
    {
        if(write_ind >= capacity)
        {
            unsigned new_capacity = capacity + capacity/2;
            char* new_ptr = realloc(ret,new_capacity + 1);
            if(new_ptr != NULL)
            {
            	ret = new_ptr;
            }
            else
            {
            	printf("FATAL: Failed to allocate memory for string!\n");
            	free(ret);
            	return NULL;
            }
            capacity = new_capacity;
        }
        ret[write_ind] = c;
        ++write_ind;
    }while((c = fgetc(file_handle)) != '\n' && c != EOF);
    ret[write_ind] = '\0';
    return ret;
}

int read_new_int()
{
    char* endptr;
    char number_buf[30];
	int ret;
    do
    {
    	errno = 0;
    	endptr = number_buf;
        (void)fgets(&number_buf[0],30,stdin);
        ret = strtol(number_buf,&endptr,10);
    }while( 0 == ret && (errno != 0 || endptr == number_buf) );
    return ret;
}

#endif //READ_HELPERS_H
