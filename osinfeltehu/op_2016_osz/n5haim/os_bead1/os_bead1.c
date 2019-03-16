/* Standard Library */
#include <stdio.h>  /* printf, scanf */

/* Custom Includes */
#include "registry_entry.h" /* all things related to registry entries */
#include "event_entry.h" /* all things related to event entries */

#define for_each(begin,end,fun)\
    for(__typeof__(begin) iterator = begin;iterator != end;++iterator)\
    {\
        fun(iterator);\
    }\
    
char default_data_file[] = "data.bin";

int main(int argc, char* argv[])
{
    char* data_file = (argc > 1)?argv[1]:default_data_file;
	
    printf("Data file is %s\n",data_file);

    registry_entry* entries = NULL;
    unsigned entry_count = 0;
    unsigned entry_delete_count;
    
    event_entry* events = NULL;
    unsigned event_count = 0;
    unsigned event_delete_count;
    
    int data_file_handle = open(data_file,O_RDONLY);
    if(data_file_handle > 0)
    {
	entry_count = read_entries_from_file(&entries,data_file_handle);
	entry_delete_count = 0;

	event_count = read_events_from_file(&events,data_file_handle);
	event_delete_count = 0;
	close(data_file_handle);
    }
    else
    {
	printf("Warning: Couldn't open data file!\n");
    }

    int input;
    do
    {
       printf("----Entry manager----\n"
              "%d entries, %d marked for deletion\n"
	      "%d events, %d marked for deletion\n"
              "----------------\n"
              "1 - List all entries\n"
              "2 - List all events\n"
              "----------------\n"
              "3 - Add new entry\n"
              "4 - Modify entry\n"
              "5 - Remove entry\n"
              "----------------\n"
              "6 - Add Event\n"
              "7 - Modify Event\n"
              "8 - Remove Event\n"
              "----------------\n"
              "9 - Write changes to file\n"
              "0 - Exit program\n"
              "Command: ",
	      entry_count,entry_delete_count,
	      event_count,event_delete_count);
       scan:
       input = getchar();
       switch(input)
       { 
           case '0':
               input = EOF;
               break;
           case '1':
               list_all_entries(entries,entry_count);
               break;
           case '2':
	       list_all_events(events,event_count);
               break;
           case '3':
               add_entry_manual(&entries,&entry_count);
               break;
           case '4':
               modify_entry_manual(entries,entry_count);
               break;
           case '5':
               entry_delete_count += remove_entry_manual(entries,entry_count);
               break;
           case '6':
	       add_event_manual(&events,&event_count);
	       break;
           case '7':
	       modify_event_manual(events,event_count);
	       break;
           case '8':
	       event_delete_count += remove_event_manual(events,event_count);
	       break;
           case '9':
	       data_file_handle = open(data_file,O_WRONLY | O_CREAT, 0644);
	       if(data_file_handle > 0)
	       {
		   write_entries_to_file(data_file_handle,entries,
					 entry_count - entry_delete_count);
		   write_events_to_file(data_file_handle,events,
					event_count - event_delete_count);
		   for_each(&entries[0],&entries[entry_count],entry_free);
		   free(entries);
		   close(data_file_handle);
		   data_file_handle = open(data_file,O_RDONLY);
		   if(data_file_handle > 0)
		   {
		       entry_count = read_entries_from_file(&entries,data_file_handle);
		       event_count = read_events_from_file(&events,data_file_handle);
		       close(data_file_handle);
		       entry_delete_count = 0;
		       event_delete_count = 0;
		   }
		   else
		   {
		       printf("Error rereading the data file!\n");
		   }
	       }
	       else
	       {
		   printf("Error opening data file for writing!\n");
	       }
               break;
           case '\n':
               goto scan;
               break;
           default:
               printf("Invalid input: %hhd\n",input);
               break;
       }
    }while( input != EOF);

    for_each(&entries[0],&entries[entry_count],entry_free);
    free(entries);
    for_each(&events[0],&events[event_count],event_free);
    free(events);
}
