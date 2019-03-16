#ifndef STRUCT_IO_H
#define STRUCT_IO_H

#include<stdlib.h>
#include<stdio.h>  /* fprintf */
#include<fcntl.h>  /* open */
#include<unistd.h> /* write, close */
#include<stdint.h> /* uint32_t */
#include<stddef.h> /* offsetof */

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)<(b)?(b):(a))
#define sizeof_member(type,mem) (sizeof(((type*)NULL)->mem))
#define member_aligned(type,mem) (0 == (offsetof(type,mem) % sizeof_member(type,mem)) )

int assert_io(char* description,int expr)
{
    if(!expr)
    {
        fprintf(stderr,"ERROR:I/O operation failed on %s!\n",description);
        return 0;
    }
    else
    {
        return 1;
    }
}

#define DEFINE_READ_MORE_FROM_FILE(T,FNAME,READ_FUN) \
unsigned FNAME(T** p_struct_arr,const int entry_file)\
{\
    if(entry_file >= 0)\
    {\
        uint32_t num_file_structs = 0;\
        int bytes_read = read(entry_file,&num_file_structs,sizeof(num_file_structs));\
        if(!assert_io("reading number of " #T " structs",bytes_read == sizeof(num_file_structs))) {return 0;}\
        printf("Found %u " #T " in file.\n",(unsigned)num_file_structs);\
        *p_struct_arr = calloc(max(1,num_file_structs),sizeof(T));\
        int structs_read = 0;\
        for(structs_read = 0;structs_read != num_file_structs;++structs_read)\
	{\
	    if(READ_FUN(*p_struct_arr + structs_read,entry_file))\
            {\
                fprintf(stderr,"Error reading " #T " #%d\n",structs_read + 1);\
                return -1;\
            }\
	}\
	fprintf(stdout,"Read %d " #T " structs from file.\n",structs_read);\
        if(structs_read == 0){*p_struct_arr = calloc(1,sizeof(T));}\
        return structs_read;\
    }\
    else\
    {\
        *p_struct_arr = calloc(1,sizeof(T));\
        fprintf(stderr,"Warning: No record file!\n");\
        return 0;\
    }\
}\

#define DEFINE_WRITE_MORE_TO_FILE(T,FNAME,WRITE_FUN) \
unsigned FNAME(int entry_file,T* src,unsigned count)\
{\
    int bytes_written = 0;\
    fprintf(stdout,"Writing %u " #T " structs to file:\n",count);\
    uint32_t count_out = (uint32_t)count;\
    bytes_written = write(entry_file,&count_out,sizeof(count_out));\
    if(!assert_io("writing number of " #T "structs",bytes_written == sizeof(count))) {return -1;}\
    int entries_written = 0;\
    for(int entry_ind = 0;entries_written < count;++entry_ind)\
    {\
        entries_written += WRITE_FUN(&src[entry_ind],entry_file);\
    }\
    fprintf(stdout,"%d entries written successfully!\n",entries_written);\
    return entries_written;\
}\

#endif //STRUCT_IO_H
