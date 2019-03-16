#ifndef REGISTRY_ENTRY
#define REGISTRY_ENTRY

/* Standard Library */
#include <time.h>   /* tm, time */
#include <string.h> /* memset, strlen */
#include "struct_io.h" /* struct reading/writing metaprogramming templates */
#include "read_helpers.h" /* read_new_string, read_new_int */

typedef struct registry_entry
{
    struct tm reg_time;
    uint32_t event_id;
    uint32_t name_len;
    uint32_t email_len;
    char initial_data_end[4];
    char* name;
    char* email;
    char to_delete;
} registry_entry;

/*  
_Static_assert (member_aligned(registry_entry,event_id),"event_id is not aligned!");
_Static_assert (member_aligned(registry_entry,name_len),"name_len is not aligned!");
_Static_assert (member_aligned(registry_entry,email_len),"email_len is not aligned!");
_Static_assert (member_aligned(registry_entry,name),"name pointer is not aligned!");
_Static_assert (member_aligned(registry_entry,email),"email pointer is not aligned!");
_Static_assert (member_aligned(registry_entry,to_delete),"deletion flag is not aligned!");
*/

/*Initial data size is every member up until the pointer to the name string*/
static const size_t INITIAL_REG_DATA_SIZE = offsetof(registry_entry,initial_data_end);

/*Securely frees a registry entry struct*/
void entry_free(registry_entry* to_free)
{
    /*Zero name to prevent security leaks*/
    memset(to_free->name,'\0',to_free->name_len);
    free(to_free->name);

    memset(to_free->email,'\0',to_free->email_len);
    free(to_free->email);

    memset(to_free,'\0',sizeof(*to_free));
}

/* Read a single registry entry from a given file handle */
int read_entry_from_file(registry_entry* entry_ptr,int file_handle)
{
    unsigned long bytes_read = 0;
    /*Read all info directly, except the two pointers to the name and email, which we handle ourselves*/
    bytes_read = read(file_handle,entry_ptr, INITIAL_REG_DATA_SIZE);
    if(!assert_io("reading initial entry data",bytes_read == INITIAL_REG_DATA_SIZE)) {return 1;}

    /*Allocate enough space for the name string + null terminator, then read the string into it*/
    entry_ptr->name = calloc(entry_ptr->name_len+1,sizeof(char));
    bytes_read = read(file_handle,entry_ptr->name,entry_ptr->name_len*sizeof(char));
    if(!assert_io("reading name string",bytes_read == (unsigned)entry_ptr->name_len)) {return 2;}

    /*Do the same with the email field*/
    entry_ptr->email = calloc(entry_ptr->email_len+1,sizeof(char));
    bytes_read = read(file_handle,entry_ptr->email,entry_ptr->email_len*sizeof(char));
    if(!assert_io("reading e-mail string",bytes_read == (unsigned)entry_ptr->email_len)) {return 3;}

    entry_ptr->to_delete = 0;
    return 0;
}

/* Write a single registry entry to a given file handle */
int write_entry_to_file(registry_entry* src,int file_handle)
{
    if(src->to_delete)
    {
	return 0;
    }
    unsigned long bytes_written = 0;

    bytes_written = write(file_handle,src,INITIAL_REG_DATA_SIZE);
    if(!assert_io("writing initial entry data",bytes_written == INITIAL_REG_DATA_SIZE)) {exit(1);}

    bytes_written = write(file_handle,src->name,src->name_len * sizeof(char));
    if(!assert_io("writing name",bytes_written == src->name_len * sizeof(char))) {exit(1);}

    bytes_written = write(file_handle,src->email,src->email_len * sizeof(char));
    if(!assert_io("writing e-mail",bytes_written == src->email_len * sizeof(char))) {exit(1);}

    return 1;
}

/* Writes entry info to a given file handle */
void write_entry_info(FILE* file_handle,registry_entry* entry)
{
        fprintf(file_handle,"Name: %s\n"
                            "E-mail address: %s\n"
                            "Event ID: %u\n"
                            "Registration date: %s"
                            "Marked for deletion: %s.\n",
                            entry->name,
                            entry->email,
		                    (int)entry->event_id,
                            asctime(&entry->reg_time),
                            entry->to_delete?"Yes":"No");
}

void list_all_entries(registry_entry* entries,unsigned entry_count)
{
    printf("--- Listing all entries ---\n");
    for(unsigned entry_ind = 0;entry_ind < entry_count;++entry_ind)
    {
        printf("---Entry #%d---\n",entry_ind + 1);
        write_entry_info(stdout,&entries[entry_ind]);
    }
    fflush(stdout);
}

void read_entry_manual(registry_entry* dest)
{
    printf("--Reading entry info--\n");

    printf("Name: ");
    dest->name = read_new_string(stdin);
    dest->name_len = strlen(dest->name);

    printf("E-mail address: ");
    dest->email = read_new_string(stdin);
    dest->email_len = strlen(dest->email);

    /* We read into an int in case conversions need to take place to uint32_t */
    printf("Event ID: ");
    dest->event_id = (uint32_t)read_new_int();

    fseek(stdin,0,SEEK_END);

    time_t the_time = time(NULL);
    dest->reg_time = *(localtime(&the_time));
    dest->to_delete = 0;
}

unsigned add_entry_manual(registry_entry** entries,unsigned *entry_count)
{

    registry_entry* new_buf = realloc(*entries,(*entry_count + 1)*sizeof(registry_entry));
    if(new_buf == NULL)
    {
	fprintf(stderr,"FATAL: Reallocation error!");
	return *entry_count;
    }

    *entries = new_buf;
    read_entry_manual(*entries + *entry_count);
    ++(*entry_count);
    printf("Registered as entry #%d\n\n",*entry_count);
    return *entry_count;
}

void modify_entry_manual(registry_entry* entries,unsigned entry_count)
{
	int to_mod_index = 0;
	printf("Modify entry #");
	to_mod_index = read_new_int();
	if( 0 < to_mod_index && to_mod_index <= entry_count)
	{
		read_entry_manual(&entries[to_mod_index - 1]);
	}
	else
	{
		printf("Invalid index!\n");
	}
}

unsigned remove_entry_manual(registry_entry* entries,unsigned entry_count)
{
    unsigned to_delete = 0;
    printf("Delete entry #");
    to_delete = read_new_int();
    if(to_delete <= entry_count)
    {
      entries[to_delete - 1].to_delete = 1;
      printf("Entry #%d marked for deletion.\n",to_delete);
      return 1;
    }
    else
    {
      printf("Invalid number!\n");
      return 0;
    }
}


/* Define entry read/write functions */
DEFINE_READ_MORE_FROM_FILE(registry_entry,read_entries_from_file,read_entry_from_file)
DEFINE_WRITE_MORE_TO_FILE(registry_entry,write_entries_to_file,write_entry_to_file)

#endif //REGISTRY_ENTRY
