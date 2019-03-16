#ifndef EVENT_ENTRY_H
#define EVENT_ENTRY_H

/* Standard Library */
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Custom Includes */
#include "struct_io.h"
#include "read_helpers.h"

typedef struct event_entry
{
    uint32_t id;
    uint32_t name_len;
    char* name;
    char to_delete;
} event_entry;

static const size_t INITIAL_EVENT_DATA_SIZE = offsetof(event_entry,name);

void event_free(event_entry* to_free)
{
    memset(to_free->name, '\0', to_free->name_len);
    free(to_free->name);

    memset(to_free,'\0',sizeof(*to_free));
}

int read_event_from_file(event_entry* dest,int file_handle)
{
    unsigned long bytes_read = 0;
    /* Read initial event data (event ID and name length) */
    bytes_read = read(file_handle,dest,INITIAL_EVENT_DATA_SIZE);
    if(!assert_io("reading initial event data",bytes_read == INITIAL_EVENT_DATA_SIZE)) {return 1;}

    /* Allocate enough space for the name and read the name string into it */
    dest->name = calloc(dest->name_len+1,sizeof(char));
    bytes_read = read(file_handle,dest->name,dest->name_len);
    if(!assert_io("reading name string",bytes_read == dest->name_len)) {return 1;}

    dest->to_delete = 0;
    return 0;
}

int write_event_to_file(event_entry* src,int file_handle)
{
    if(src->to_delete)
    {
	return 0;
    }
    unsigned long bytes_written = 0;

    bytes_written = write(file_handle,src,INITIAL_EVENT_DATA_SIZE);
    if(!assert_io("writing initial event data",bytes_written == INITIAL_EVENT_DATA_SIZE)) {exit(1);}

    bytes_written = write(file_handle,src->name,src->name_len * sizeof(char));
    if(!assert_io("writing name",bytes_written == src->name_len * sizeof(char))) {exit(1);}

    return 1;
}

void write_event_info(FILE* file_handle,event_entry* event)
{
    fprintf(file_handle,"Name: %s\n"
	                "ID: %d\n"
	                "Marked for deletion: %s.\n",
	                event->name,
	                (int)event->id,
	                event->to_delete?"Yes":"No");
}

void list_all_events(event_entry* events,unsigned event_count)
{
    printf("--- Listing all events ---\n");
    for(unsigned event_ind = 0; event_ind < event_count; ++event_ind)
    {
	printf("---Event #%u---\n",event_ind + 1);
        write_event_info(stdout,&events[event_ind]);
    }
    fflush(stdout);
}

void read_event_manual(event_entry* dest)
{
    printf("--Reading event info--\n");
    printf("Event Name: ");
    dest->name = read_new_string(stdin);
    dest->name_len = strlen(dest->name);
    
    printf("Event ID: ");
    dest->id = (uint32_t)read_new_int();

    dest->to_delete = 0;
    fseek(stdin,0,SEEK_END);
}

unsigned add_event_manual(event_entry** events,unsigned *event_count)
{
    event_entry* new_buf = realloc(*events,(*event_count + 1)*sizeof(event_entry));
    if(new_buf == NULL)
    {
	fprintf(stderr,"FATAL: Reallocation error!");
	return *event_count;
    }

    *events = new_buf;
    read_event_manual(*events + *event_count);
    ++(*event_count);
    printf("Registered as event #%d\n\n",*event_count);
    return *event_count;
}

void modify_event_manual(event_entry* events,unsigned event_count)
{
    unsigned to_mod_index = 0;
    printf("Modify event #");
    to_mod_index = read_new_int();
    if( 0 < to_mod_index && to_mod_index <= event_count)
    {
	read_event_manual(&events[to_mod_index - 1]);
    }
    else
    {
	printf("Invalid index!\n");
    }
}

unsigned remove_event_manual(event_entry* events,unsigned event_count)
{
    unsigned to_delete = 0;
    printf("Delete event #");
    to_delete = read_new_int();
    if(to_delete <= event_count)
    {  
      events[to_delete - 1].to_delete = 1;
      printf("Event #%d marked for deletion.\n",to_delete);
      return 1;
    }
    else
    {
      printf("Invalid number!\n");
      return 0;
    }
}

/* Define event read/write functions */
DEFINE_READ_MORE_FROM_FILE(event_entry,read_events_from_file,read_event_from_file)
DEFINE_WRITE_MORE_TO_FILE(event_entry,write_events_to_file,write_event_to_file)

#endif //EVENT_ENTRY_H
    
