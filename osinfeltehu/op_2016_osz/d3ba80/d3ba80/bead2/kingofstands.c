#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h> // for pipe()
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 



#define EVENT_FILE "event"
#define GUEST_FILE "guest"
typedef enum { false, true } bool;

key_t key;

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

struct Event
{
	int id;
	char name[200];
	int num_of_guests;
};

struct Guest
{
	int id;
	char name[100];
	char email[100];
	int event_id;
 	int event_queue;
 	time_t signup_time;
};

struct Event all_event[1000];
int last_event_index = 0;
int next_id = 0;

struct Guest all_guest[1000];
int last_guest_index = 0;
int next_guest_id = 0;


void write_guests(int from, int to)
{
	int i;
	FILE * f;
	f = fopen(GUEST_FILE, "w");
	for(i=from;i<to;++i)
	{
		fwrite(&all_guest[i], sizeof(struct Guest), 1, f);
	}
	fclose(f);	
}

void write_events(int from, int to)
{
	int i;
	FILE * f;
	f = fopen(EVENT_FILE, "w");
	for(i=from;i<to;++i)
	{
		fwrite(&all_event[i], sizeof(struct Event), 1, f);
	}
	fclose(f);	
}

//------------------------------------------
//EVENTS
//------------------------------------------


void load_events_from_file()
{
	FILE * f;
	f = fopen(EVENT_FILE, "r");
	last_event_index = 0;
	if (f) {
		while (!(feof(f))){
			if (fread(&(all_event[last_event_index]), sizeof(struct Event), 1, f)) {
				if(all_event[last_event_index].id>=next_id)
				{
					next_id = all_event[last_event_index].id + 1;
				}
				last_event_index++;
				
			}
		}
		fclose(f);
	}
}

void list_events()
{
	load_events_from_file();
	printf("Rendezvenyek listaja:\n");
	int i;
	system("clear");
	printf("Id\tNev\tVendegek szama\n");
	printf("---------------------------------\n");
	for(i=0;i<last_event_index;++i){
		printf("%d\t%s\t%d\n",all_event[i].id,all_event[i].name,all_event[i].num_of_guests );
	}
	printf("\n");
}

void new_event()
{
	load_events_from_file();
	printf("Uj rendezveny felvetele:\n");
	
	struct Event event;
	char _name[200];
	int _id;
	printf("\tAdja meg a rendezveny nevet:");
	scanf (" %[^\n]%*c", _name);

	strcpy(event.name,_name);
	event.id = next_id;
	event.num_of_guests = 0;

	FILE * f;
	f = fopen(EVENT_FILE, "a");

    fwrite(&event, sizeof(struct Event), 1, f);
	fclose(f);

	system("clear");
	printf("Uj rendezveny felvetele sikere volt!\n\n");
}
void delete_event()
{
	load_events_from_file();
	printf("Rendezveny torlese:\n");
	printf("\t Adja meg a rendezveny azonositojat torleshez: ");
	int _id;
	scanf("%d", &_id);
	//printf("Biztosan torolni kivanja a %d -idju rendezvenyt?(Y/n)\n",_id );
	//char _answer;
	//scanf(" %c",_answer);

	int i;
	int delete_position = -1;
	for(i=0;i<last_event_index;++i)
	{
		if(_id == all_event[i].id)
		{
			delete_position = i;
			break;
		}
	}
	if(-1 == delete_position)
	{
		printf("Nem talalhato %d idju rendezveny\n", _id);
		return;
	}
	for(delete_position;delete_position<last_event_index;++delete_position)
	{
		all_event[delete_position] = all_event[delete_position+1];
	}

	write_events(0,last_event_index-1);

	system("clear");
	printf("Rendezveny torlese sikeres volt!\n\n");
}



//---------------------------------------------
//GUESTS
//---------------------------------------------


void load_guests_from_file()
{
	FILE * f;
	f = fopen(GUEST_FILE, "r");
	last_guest_index = 0;
	if (f) {
		while (!(feof(f))){
			if (fread(&(all_guest[last_guest_index]), sizeof(struct Guest), 1, f)) {
				if(all_guest[last_guest_index].id>=next_guest_id)
				{
					next_guest_id = all_guest[last_guest_index].id + 1;
				}
				last_guest_index++;
				
			}
		}
		fclose(f);
	}
}

void new_guest()
{
	load_events_from_file();
	load_guests_from_file();


	printf("Uj vendeg felvetele:\n");
	
	struct Guest guest;
	char _name[200];
	char _email[200];
	int _event_id;
	
	printf("\tAdja meg a vendeg nevet:");
	scanf (" %[^\n]%*c", _name);

	printf("\tAdja meg a vendeg email cimet:");
	scanf (" %[^\n]%*c", _email);

	printf("\tAdja meg a rendevzeny azonositojat:");
	scanf (" %d", &_event_id);

	int i;


	bool found = false;
	for(i=0;i<last_event_index;++i)
	{
		if(all_event[i].id ==_event_id)
		{
			found = true;
			all_event[i].num_of_guests++;
			guest.event_queue = all_event[i].num_of_guests;
			break;
		}
	}
	if(!found)
	{
		system("clear");
		printf("Vendeg felvetele sikertelen!\nNem talalhato ilyen azonositoju rendezveny!\n\n");
		return;
	}

	guest.id = next_guest_id;
	strcpy(guest.name,_name);
	strcpy(guest.email,_email);
	guest.event_id = _event_id;
	guest.signup_time = time(NULL);

	FILE * f;
	f = fopen(GUEST_FILE, "a");

    fwrite(&guest, sizeof(struct Guest), 1, f);
	fclose(f);

	write_events(0,last_event_index);

	system("clear");
	printf("Uj vendeg felvetele sikeres volt!%s\n");
	printf("%d.-nek jelentkezett a %d azonositoju rendevzenyre. \n\n",guest.event_queue,_event_id );
}

void list_guests()
{
	load_events_from_file();
	load_guests_from_file();
	printf("Vendegek listaja:\n");
	int i;
	system("clear");
	printf("Id\tNev\tEmail\tRendezveny azon\tSorban\tJelentkezesi ido\n");
	printf("------------------------------------------------------------------\n");
	for(i=0;i<last_guest_index;++i){
		printf("%d\t%s\t%s\t%d\t%d\t%s\n",all_guest[i].id,all_guest[i].name,all_guest[i].email,all_guest[i].event_id,all_guest[i].event_queue,ctime(&all_guest[i].signup_time));
	}
	printf("\n");
}

void delete_guest()
{
	load_events_from_file();
	load_guests_from_file();
	printf("Vendeg torlese:\n");
	printf("\t Adja meg a vendeg azonositojat torleshez: ");
	int _id;
	scanf("%d", &_id);
	//printf("Biztosan torolni kivanja a %d -idju rendezvenyt?(Y/n)\n",_id );
	//char _answer;
	//scanf(" %c",_answer);

	int i;
	int delete_position = -1;
	for(i=0;i<last_guest_index;++i)
	{
		if(_id == all_guest[i].id)
		{
			delete_position = i;
			int j;
			for(j=0;j<last_event_index;++j)
			{
				if(all_event[j].id == all_guest[i].event_id)
				{
					all_event[j].num_of_guests--;
					break;
				}
			}
			break;
		}
	}
	if(-1 == delete_position)
	{
		printf("Nem talalhato %d idju vendeg\n", _id);
		return;
	}
	for(delete_position;delete_position<last_guest_index;++delete_position)
	{
		all_guest[delete_position] = all_guest[delete_position+1];
	}

	write_guests(0,last_guest_index-1);
	write_events(0,last_event_index);

	system("clear");
	printf("Vendeg torlese sikeres volt!\n\n");
}

void mod_guest()
{
	load_events_from_file();
	load_guests_from_file();

	printf("Vendeg modositasa:\n");
	printf("\t Adja meg a vendeg azonositojat modositashoz: ");
	int _id;
	scanf("%d", &_id);

	int i;

	struct Guest guest;
	for(i=0;i<last_guest_index;++i)
	{
		if(all_guest[i].id == _id)
		{
			printf("%d\t%s\t%s\t%d\t%d\t%s\n",all_guest[i].id,all_guest[i].name,all_guest[i].email,all_guest[i].event_id,all_guest[i].event_queue,ctime(&all_guest[i].signup_time));
			
			printf("\tAdja meg a vendeg uj nevet:");
			scanf (" %[^\n]%*c", all_guest[i].name);

			printf("\tAdja meg a vendeg uj email cimet:");
			scanf (" %[^\n]%*c", all_guest[i].email);
			break;
		}
	}
	write_guests(0,last_guest_index);
	system("clear");
	printf("A vendeg modositasa sikeres volt!%s\n\n");



}

static int organiser_arrived = 0;
static void handleSignal(int signumber)
{
	if (signumber == SIGTERM)
	{
		organiser_arrived = 1;
	}
}

void start_event()
{
	load_events_from_file();
	load_guests_from_file();

	int id;
	printf("\nMelyik esemeny indul?(id): ");
	scanf("%d",&id);

	struct Guest event_guests[1000];
	int event_guest_count = 0;
	int i;
	for(i = 0; i < last_guest_index; ++i)
	{			
		if (all_guest[i].event_id == id)
		{
			event_guests[event_guest_count] = all_guest[i];
			event_guest_count++;
		}
	}

	int uzenetsor = msgget(key, 0600 | IPC_CREAT);
	if (uzenetsor <0) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}

	int pipefd[2]; // unnamed pipe file descriptor array
	pid_t pid;
	if (pipe(pipefd) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
	    exit(EXIT_FAILURE);
	}

	signal(SIGTERM, handleSignal);

	pid = fork();

	if (pid==-1)
	{
   		perror("Fork hiba");
   		exit(EXIT_FAILURE);
    }

	if (pid == 0)
    {
   	 	//child process
   	 	
   	 	sleep(2);
   	 	kill(getppid(),SIGTERM);

   	 	close(pipefd[1]);

   	 	sleep(1);
   	 	printf("Esemeny rendezo megerkezett, vendegek fogadasa:\n");

   	 	srand(time(NULL));

   	 	for(i=0; i<event_guest_count; ++i)
   	 	{
   	 		char guest_from_parent[100];
   	 		read(pipefd[0],&guest_from_parent,sizeof(guest_from_parent));
   	 		if (rand() % 10 < 1)
   	 		{
				printf("%s nem erkezett meg.\n", guest_from_parent);
				struct uzenet u = { 2 , "" };
				sprintf(u.mtext,"%s",guest_from_parent);
				msgsnd(uzenetsor,&u,strlen(u.mtext),0);
   	 		} else {
   	 			printf("%s megerkezett.\n", guest_from_parent);
   	 		}
   	 	}
   	 	sleep(1);
   	 	struct uzenet siker = { 1 , ""};
   	 	sprintf(siker.mtext,"%d",rand() % 10);
   	 	msgsnd(uzenetsor, &siker, strlen(siker.mtext),0);
   	 	printf("Esemeny veget ert.\n");
   	 	close(pipefd[0]);
   	 	exit(EXIT_SUCCESS);

    } else {
   	 	//parent process
    	pause();
    	if(organiser_arrived);
    	{
    		printf("\nRendezo megerkezett, vendeg lista kuldese..\n");
    	}
    	close(pipefd[0]);
    	int j;
    	for (j=0; j<event_guest_count;++j) 
    	{
    		char guest_to_send[100];
    		strcpy(guest_to_send,event_guests[j].name);
    		write(pipefd[1],&guest_to_send,sizeof(guest_to_send));
    	}
    	close(pipefd[1]);

    	struct uzenet u;
    	msgrcv(uzenetsor, &u, 1024, 1, 0);
    	printf("\nEsemeny ertekeles: %s\n", u.mtext);

    	int status = msgctl( uzenetsor, IPC_RMID, NULL );
    	waitpid(pid,&status,0);
    	printf("Rendezo vissza tert.\n");
    }

}
//---------------------------------------------
//Main + Menu
//---------------------------------------------
void menu() {
	int menu;
	size_t size = 0;
	do {
		printf("1. Rendezvenyek listazasa\n");
		printf("2. Uj rendezveny felvetele\n");
		printf("3. Rendezveny torlese\n");
		printf("4. Jelentkezok listazasa\n");
		printf("5. Uj vendeg felvetele\n");
		printf("6. Vendeg modositasa\n");
		printf("7. Vendeg torlese\n");
		printf("8. Rendezveny inditasa\n");
		printf("9. Kilepes\n");
		printf("Kerem valasszon: ");
		scanf("%d", &menu);
		switch (menu) {
			case 1: list_events(); break;
			case 2: new_event(); break;
			case 3: delete_event();break;
			case 4: list_guests();break;
			case 5: new_guest();break;
			case 6: mod_guest();break;
			case 7: delete_guest();break;
			case 8: start_event();break;
			default:break;
		}
	} while (menu!=9);
}

void main(int argc, char** argv) {
	load_events_from_file();
	load_guests_from_file();

	key = ftok(argv[0],1);

	menu();
}
