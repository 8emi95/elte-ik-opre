 #include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>

#define BUFFERSIZE 	64
#define GUESTFILE	"GUEST.bin"
#define EVENTFILE	"EVENT.bin"
#define EMPTYFLAG	"<empty>"

typedef struct guest guest;
typedef struct event event;

struct guest
{
	char name[BUFFERSIZE];
	char email[BUFFERSIZE];
	char curr_time[BUFFERSIZE];
	int event_id;
};

struct event
{
	int id;
	char name[BUFFERSIZE];
};

static guest guest_array[BUFFERSIZE];
static event event_array[BUFFERSIZE];

void write_bin_file(void*, size_t, char*, char*);
void read_bin_file(void*, size_t, char*);
int check_if_file_exists(char*);
void fill_guest_array();
void fill_event_array();
void list_guests();
void modify_guest(bool);
void delete_guest(int);
void new_event();
void list_events();
void delete_event();
void draw_menu();
void do_action(char);
void save_data();
void helper_create();
void organize_event();
void sighandler_org(int);
void sighandler_part(int);
int helper_find_empty_guest();
int helper_find_empty_event();
char* get_time();

void fill_guest_array()
{
	int i;

	for(i = 0; i < BUFFERSIZE; i++)
	{
		strcpy(guest_array[i].name, EMPTYFLAG);
		strcpy(guest_array[i].email, EMPTYFLAG);
		strcpy(guest_array[i].curr_time, get_time());
		guest_array[i].event_id = -1;
	}

	write_bin_file(&guest_array, sizeof(guest_array), GUESTFILE, "w+");
}

void fill_event_array()
{
	int i;

	for(i = 0; i < BUFFERSIZE; i++)
	{
		event_array[i].id = -1;
		strcpy(event_array[i].name, EMPTYFLAG);
	}

	write_bin_file(&event_array, sizeof(event_array), EVENTFILE, "w+");
}

void write_bin_file(void* to_write, size_t size, char* which, char* mode)
{
	FILE* f = fopen(which, mode);
	fwrite(to_write, size, 1, f);
	fclose(f);
}

void read_bin_file(void* to_read, size_t size, char* which)
{
	FILE* f = fopen(which, "r");
	fread(to_read, size, 1, f);
	fclose(f);
}

int check_if_file_exists(char* which)
{
	int f = open(which, O_RDONLY);
	int success = 1;

	if(f < 0) { success = 0; }

	close(f);

	return success;
}

void list_guests()
{
	int i;

	for(i = 0; i < BUFFERSIZE && strcmp(guest_array[i].name, EMPTYFLAG); i++)
	{
		printf("%d.\t", i + 1);
		printf("%s\t", guest_array[i].name);
		printf("%s\t", guest_array[i].email);
		printf("%d\t", guest_array[i].event_id);
		printf("%s\t", guest_array[i].curr_time);
		printf("\n");
	}
}

void modify_guest(bool new_guest)
{
	int row = -1;
	int event_num;
	char name[BUFFERSIZE];
	char email[BUFFERSIZE];

	if(!new_guest)
	{
		list_guests();
		printf("Vendeg sorszama: ");
		scanf("%d", &row);
		row--;
	}
	else
	{
		list_events();
		row = helper_find_empty_guest();
	}
	
	printf("Nev: ");
	scanf("%s%*c", name);
	printf("Email: ");
	scanf("%s%*c", email);
	printf("Esemeny szama: ");
	scanf("%d", &event_num);

	strcpy(guest_array[row].name, name);
	strcpy(guest_array[row].email, email);
	strcpy(guest_array[row].curr_time, get_time());
	guest_array[row].event_id = event_num;

	if(new_guest)
	{
		int i;
		int counter;

		for(i = 0;i < BUFFERSIZE; i++)
		{
			if(guest_array[i].event_id == event_num)
				counter++;
		}

		printf("%d. vendeg az esemenyen.\n", counter + 1);
	}
}

void delete_guest(int which)
{
	int i;
	int j;
	int row;
	guest g_temp[BUFFERSIZE];

	list_guests();

	if(which == -1)
	{
		printf("Vendeg sorszama: ");
		scanf("%d", &row);
		row--;
	}
	else
	{
		row = which;
	}

	for(i = 0; i < BUFFERSIZE && i < row; i++) { g_temp[i] = guest_array[i]; }
	for(i++; i < BUFFERSIZE; i++) { g_temp[i - 1] = guest_array[i]; }
	for(j = 0; j < BUFFERSIZE; j++) { guest_array[j] = g_temp[j]; }
}

void new_event()
{
	int event_num;
	char name[BUFFERSIZE];
	
	printf("Esemeny szama: ");
	scanf("%d", &event_num);
	printf("Esemeny neve: ");
	scanf("%s%*c", name);

	event_array[helper_find_empty_event()].id = event_num;
	strcpy(event_array[helper_find_empty_event()].name, name);

	list_events();
}

void list_events()
{
	int i;

	for(i = 0; i < BUFFERSIZE && strcmp(event_array[i].name, EMPTYFLAG); i++)
	{
		printf("%d.\t", i + 1);
		printf("%d\t", event_array[i].id);
		printf("%s\t", event_array[i].name);
		printf("\n");
	}
}

void delete_event()
{
	list_events();

	int i;
	int j;
	int k;
	int row;
	event e_temp[BUFFERSIZE];

	printf("Esemeny sorszama: ");
	scanf("%d", &row);
	row--;

	for(k = 0; k < BUFFERSIZE; k++)
	{
		if(guest_array[k].event_id == event_array[row].id)
		{
			delete_guest(k);
			k = 0;
		}
	}

	for(i = 0; i < BUFFERSIZE && i < row; i++) { e_temp[i] = event_array[i]; }
	for(i++; i < BUFFERSIZE; i++) { e_temp[i - 1] = event_array[i]; }
	for(j = 0; j < BUFFERSIZE; j++) { event_array[j] = e_temp[j]; }
}

int helper_find_empty_guest()
{
	int i;

	for(i = 0; i < BUFFERSIZE && strcmp(guest_array[i].name, EMPTYFLAG); i++);

	return i;
}

int helper_find_empty_event()
{
	int i;

	for(i = 0; i < BUFFERSIZE && strcmp(event_array[i].name, EMPTYFLAG); i++);

	return i;
}

void save_data()
{
	write_bin_file(&guest_array, sizeof(guest_array), GUESTFILE, "w+");
	write_bin_file(&event_array, sizeof(event_array), EVENTFILE, "w+");
}

void helper_create()
{
	if(!check_if_file_exists(GUESTFILE)) { fill_guest_array(); }
	else { read_bin_file(&guest_array, sizeof(guest_array), GUESTFILE); }

	if(!check_if_file_exists(EVENTFILE)) { fill_event_array(); }
	else { read_bin_file(&event_array, sizeof(event_array), EVENTFILE); }
}

char* get_time()
{
	time_t rawtime;
	struct tm* timeinfo;
	char* current_time;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	current_time = asctime(timeinfo);

	return current_time;
}

void organize_event()
{
    system("clear");
    int i;
    int fid = mkfifo("fifo.ftc", S_IRUSR | S_IWUSR);
    int fd;
    
    pid_t pid = fork();
    
    if(pid == -1)
    {
        perror("Fork failed.");
        
        exit(EXIT_FAILURE);
    }
    
    if(fid == -1)
    {
        perror("Fifo.ftc failed.");
        
        exit(EXIT_FAILURE);
    }
    
    //signal(SIGUSR1, sighandler_org);
    
    for(i = 0; i < BUFFERSIZE && strcmp(event_array[i].name, EMPTYFLAG); i++)
    {
        printf("Organizing event #%d.\n", i + 1);
        
        if(pid == 0) //child process
        {
            sleep(10);
            printf("[Part] Partner is on the way.\n");
            printf("[Part] Partner arrived.\n");
            printf("[Part] Waiting for guests.\n");
            
            char buffer[1024] = "empty";
            fd = open("fifo.ftc", O_RDONLY);
            read(fd, buffer, sizeof(buffer));
            close(fd);
            printf("%s\n", buffer);
        }
        else //parent process
        {
            int j;
            
            printf("[Org] Organizer is working.\n");

            for(j = 0; j < BUFFERSIZE && strcmp(guest_array[j].name, EMPTYFLAG); j++)
            {
                if(guest_array[j].event_id == event_array[i].id)
                {
                    fd = open("fifo.ftc", O_WRONLY);
                    write(fd, guest_array[j].name, strlen(guest_array[j].name));
                    close(fd);
                }
            }

            //printf("[Org] Guest list has been sent to partner.\n");
        }
        
        printf("----------------------------------------\n");
    }
}

void sighandler_org(int signum)
{
    sleep(1);
    printf("[Org] Organizer notified (%d).\n", signum);
    exit(0);
}

void sighandler_part(int signum)
{
    
}

void do_action(char which)
{
	switch(which)
	{
		case '1': { modify_guest(true); 	break; }
		case '2': { list_guests(); 			break; }
		case '3': { modify_guest(false); 	break; }
		case '4': { delete_guest(-1); 		break; }
		case '5': { new_event(); 			break; }
		case '6': { delete_event(); 		break; }
        case '7': { organize_event(); 		break; }
		default:  { printf("Bye!\n"); 		break; }
	}
    
	printf("Press RETURN to continue...\n");
	getchar();
}

void draw_menu()
{
	helper_create();

	char c;

	do
	{
		char empty[BUFFERSIZE];

		system("clear");
		printf("----------------------------------------\n");
		printf("MENU\n");
		printf("----------------------------------------\n");
		printf("1 - Vendeg jelentkeztetese\n");
		printf("2 - Jelentkezettek listaja\n");
		printf("3 - Vendeg adatainak modositasa\n");
		printf("4 - Vendeg torlese\n");
		printf("----------------------------------------\n");
		printf("5 - Rendezveny inditasa\n");
		printf("6 - Rendezveny adatainak torlese\n");
		printf("----------------------------------------\n");
        printf("7 - Rendezvenyek inditasa\n");
        printf("----------------------------------------\n");
		printf("0 - Kilepes\n");
		printf("----------------------------------------\n");
		scanf("%c", &c);
		scanf("%c", empty);

		do_action(c);
	} while(c != '0');

	save_data();
}

int main()
{
	draw_menu();

	return 0;
}

