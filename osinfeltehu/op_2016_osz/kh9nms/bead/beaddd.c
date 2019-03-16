#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <signal.h>


void handler(int signumber)
{
  signal(SIGTERM,SIG_DFL);
}

struct visitor
{
    char name[50];
    char mail[50];
    int id;
    time_t time;
};

void init(struct visitor visitor_arr[], int events_arr[], int *visitorcount , int *eventcount)
{
	FILE * file;
	file = fopen("data.txt","r");
	
	char name[50];
	char mail[50];
	int id;
	char time[30];
	
	int temp;
	
	char line[500];
	while(fgets(line,sizeof(line),file) != NULL)
	{
		sscanf(line,"%s %s %d %[^\n]", name, mail, &id, time);
		strcpy(visitor_arr[*visitorcount].name,name);
		strcpy(visitor_arr[*visitorcount].mail,mail);
		visitor_arr[*visitorcount].id = id;
		
		struct tm tm;
		strptime(time,"%a %b %d %T %Y",&tm);
		visitor_arr[*visitorcount].time = mktime(&tm);
		
		*visitorcount = *visitorcount + 1;
	}
	
	fclose(file);
	
	file = fopen("events.txt","r");
	
	int event;
	
	while(fgets(line,sizeof(line),file) != NULL)
	{
		sscanf(line,"%d", &event);
		events_arr[*eventcount] = event;
		*eventcount = *eventcount + 1;
	}
	
	fclose(file);

}

void add_visitor(struct visitor visitor_arr[], int events_arr[], int *visitorcount , int *eventcount)
{
	char name[50];
	char mail[50];
	int id;
	int valid_id = 0;

	write(1,"Name: ",6);
	scanf("%s", name);
	
	write(1,"E-mail address: ",16);
	scanf("%s", mail);
	
	write(1,"Event ID: ",10);
	scanf("%d", &id);
	
	int i;
	for(i = 0; i < *eventcount; i++)
	{
		if(events_arr[i] == id)
		{
			valid_id = 1;
		}
	}
	
	if(valid_id==1)
	{
		strcpy(visitor_arr[*visitorcount].name,name);
		strcpy(visitor_arr[*visitorcount].mail,mail);
		visitor_arr[*visitorcount].id = id;
		visitor_arr[*visitorcount].time = time(NULL);
		*visitorcount = *visitorcount + 1;
		printf("\nYou are visitor #%d\n",*visitorcount);
	}
	else 
	{
		printf("\nInvalid ID\n");
	}
	
	FILE * file;
	file = fopen("data.txt","a+");
		
	char w_data[90] = "";
	strcat(w_data,name);
	strcat(w_data," ");
	strcat(w_data,mail);
	strcat(w_data," ");
	char temp[50];
	sprintf(temp,"%d",id);
	strcat(w_data,temp);
	strcat(w_data," ");
	
	time_t mytime;
	time(&mytime);
	
	strcat(w_data,ctime(&mytime));
	
	fputs (w_data,file);
	fclose(file);
	
}

void list_all(struct visitor visitor_arr[], int *visitorcount)
{
	printf("\n");
	int i;
	for(i = 0; i < *visitorcount; i++)
	{
		printf("%s ", visitor_arr[i].name);
		printf("%s ", visitor_arr[i].mail);
		printf("%d ", visitor_arr[i].id);
		printf("%s", ctime(&visitor_arr[i].time));
	}
}
void save_data(struct visitor visitor_arr[], int *visitorcount)
{
	FILE * file;
	file = fopen("data.txt","w");
	
	int i;
	for (i = 0; i < *visitorcount; i++)
	{
		char w_data[90] = "";
		strcat(w_data,visitor_arr[i].name);
		strcat(w_data," ");
		strcat(w_data,visitor_arr[i].mail);
		strcat(w_data," ");
		char temp[50];
		sprintf(temp,"%d",visitor_arr[i].id);
		strcat(w_data,temp);
		strcat(w_data," ");
		strcat(w_data,ctime(&visitor_arr[i].time));
		
		fputs (w_data,file);
	}
	fclose(file);
}
void modify_visitor(struct visitor visitor_arr[], int events_arr[], int *visitorcount , int *eventcount)
{
	char name[50];
	char mail[50];
	int id;
	int valid_id = 0;

	write(1,"Name of the guest: ",19);
	scanf("%s", name);
	
	int i = 0;
	while(i < *visitorcount && strcmp(visitor_arr[i].name,name) != 0) i++;
	if (i < *visitorcount)
	{
		write(1,"What to modify? (1:email, 2:event, 0:nothing, exit): ",53);
		char c;
		scanf(" %c",&c);
		while(c != '0')
		{
			switch (c) 
			{
				case '1':
					write(1,"E-mail address: ",16);
					scanf("%s", mail);
					strcpy(visitor_arr[i].mail,mail);
					break;
				case '2':
					write(1,"Event ID: ",10);
					scanf("%d", &id);
					int j;
					for(j = 0; j < *eventcount; j++)
					{
						if(events_arr[j] == id)
						{
							valid_id = 1;
						}
					}
					if (valid_id==1) visitor_arr[i].id = id;
					else printf("\nInvalid ID\n");
					break;
				default : break;
			}
			write(1,"What to modify? (1:email, 2:event, 0:nothing, exit): ",53);
			
			read(0,&c,2);
		}
	}
	else
	{
		printf("\nInvalid visitor\n");
	}
	save_data(visitor_arr,visitorcount);
}



void delete_visitor(struct visitor visitor_arr[], int events_arr[], int *visitorcount , int *eventcount)
{
	char name[50];

	write(1,"Name to delete: ",16);
	scanf("%s", name);
	
	int i = 0;
	while(i < *visitorcount && strcmp(visitor_arr[i].name,name) != 0) i++;
	if (i < *visitorcount)
	{
		for(;i < *visitorcount - 1; i++)
		{
			strcpy(visitor_arr[i].name,visitor_arr[i+1].name);
			strcpy(visitor_arr[i].mail,visitor_arr[i+1].mail);
			visitor_arr[i].id = visitor_arr[i+1].id;
			visitor_arr[i].time = visitor_arr[i+1].time;
		}
		*visitorcount = *visitorcount - 1;
	}
	else
	{
		printf("\nInvalid visitor\n");
	}
	save_data(visitor_arr,visitorcount);
}

void add_event(int events_arr[], int *eventcount)
{
	int id;
	int exists = 0;
	
	write(1,"Event ID: ",10);
	scanf("%d", &id);
	
	int i;
	for(i = 0; i < *eventcount; i++)
	{
		if(events_arr[i] == id)
		{
			exists = 1;
		}
	}
	
	if (exists==0)
	{
		events_arr[*eventcount] = id;
		*eventcount = *eventcount + 1;
	
		FILE * file;
		file = fopen("events.txt","a+");
			
		char temp[50];
		sprintf(temp,"%d",id);
		fputs(temp,file);
		fputs("\n",file);
		fclose(file);
	}
}

void delete_event(struct visitor visitor_arr[], int events_arr[], int *visitorcount , int *eventcount)
{
	int id;

	write(1,"Event ID: ",10);
	scanf("%d", &id);
	
	int i = 0;
	while(i < *eventcount && events_arr[i] != id) i++;
	if (i < *eventcount)
	{
		for(;i < *eventcount - 1; i++) events_arr[i] = events_arr[i+1];
		*eventcount = *eventcount - 1;
	}
	else printf("\nInvalid ID\n");
	
	FILE * file;
	file = fopen("events.txt","w");
	
	for (i = 0; i < *eventcount; i++)
	{
		char temp[50];
		sprintf(temp,"%d",events_arr[i]);
		fputs(temp,file);
		fputs("\n",file);
	}
	
	fclose(file);
	
	i = 0;
	int j;
	while(i < *visitorcount)
	{
		if (visitor_arr[i].id == id)
		{
			for(j = 0; j < *visitorcount - 1; j++)
			{
				strcpy(visitor_arr[i].name,visitor_arr[i+1].name);
				strcpy(visitor_arr[i].mail,visitor_arr[i+1].mail);
				visitor_arr[i].id = visitor_arr[i+1].id;
				visitor_arr[i].time = visitor_arr[i+1].time;
			}
			*visitorcount = *visitorcount - 1;
		}
		else i++;
	}
	save_data(visitor_arr,visitorcount);
	
}

//Második Beadandó//
void run_event(struct visitor visitor_arr[], int events_arr[], int *visitorcount , int *eventcount)
{

	signal(SIGTERM,handler);
	
	for(;;)
	{
		printf("\nChoose an event ID: \n");
		int i;
		for(i = 0; i < *eventcount; i++)
		{
			printf("%d.%d\n", i+1, events_arr[i]);
		}
		
		int id;
		printf("0.Exit\n");
		printf("\nEvent ID: ");
		scanf("%d", &id);
		if (id == 0) break;
		
		int attendant_count = 0;
		struct visitor attendants[*visitorcount];
		
		int pipefd[2];
		int pid;
		
		pipe(pipefd);
		
		pid = fork();
		if (pid == 0) //child / partner
		{
			int recieved_id;
			close(pipefd[1]);
			read(pipefd[0], &recieved_id, sizeof(int));
			printf("Partner: I've received the event ID: %d\n", recieved_id);
			
			printf("Partner: Travelling to the venue...\n");
			sleep(3);
			
			printf("Partner: Waiting for participants at the venue\n");
			kill(getppid(),SIGTERM);
			
			read(pipefd[0], &attendant_count, sizeof(int));
			read(pipefd[0], &attendants, sizeof(attendants));
			
			close(pipefd[0]);
			
			printf("\n");
			for (i = 0; i < attendant_count; i++)
			{
				printf("%s ", attendants[i].name);
				printf("%s ", attendants[i].mail);
				printf("%d ", attendants[i].id);
				printf("%s", ctime(&attendants[i].time));
			}
			
			printf("Partner: Running the event...\n");
			sleep(3);
			
			printf("Partner: Success of the event: %d/10\n",rand() % 10);
			
			for (i = 0; i < attendant_count; i++)
			{
				if (rand()%10==1) printf("%s hasn't appeared", attendants[i].name);
			}
		}
		else //parent / center
		{
			close(pipefd[0]);
			printf("\nCenter: Sending ID %d\n",events_arr[id-1]);
			write(pipefd[1], &events_arr[id-1], sizeof(int));
			
			pause();
			
			printf("Center: Sending the participants\n");
			
			i = 0;
			while(i < *visitorcount)
			{
				if (visitor_arr[i].id == events_arr[id-1])
				{
					attendants[attendant_count] = visitor_arr[i];
					attendant_count++;
				}
				i++;
			}
			write(pipefd[1], &attendant_count, sizeof(int));
			write(pipefd[1], &attendants, sizeof(attendants));
			
			close(pipefd[1]);
			
			pause();
			
		}
		
		id = events_arr[id-1];
		
		i = 0;
		while(i < *eventcount && events_arr[i] != id) i++;
		if (i < *eventcount)
		{
			for(;i < *eventcount - 1; i++) events_arr[i] = events_arr[i+1];
			*eventcount = *eventcount - 1;
		}
		else printf("\nInvalid id\n");
		
		FILE * file;
		file = fopen("events.txt","w");
		
		for (i = 0; i < *eventcount; i++)
		{
			char temp[50];
			sprintf(temp,"%d",events_arr[i]);
			fputs(temp,file);
			fputs("\n",file);
		}
		
		fclose(file);
		
		i = 0;
		int j;
		while(i < *visitorcount)
		{
			if (visitor_arr[i].id == id)
			{
				for(j = 0; j < *visitorcount - 1; j++)
				{
					strcpy(visitor_arr[i].name,visitor_arr[i+1].name);
					strcpy(visitor_arr[i].mail,visitor_arr[i+1].mail);
					visitor_arr[i].id = visitor_arr[i+1].id;
					visitor_arr[i].time = visitor_arr[i+1].time;
				}
				*visitorcount = *visitorcount - 1;
			}
			else i++;
		}
		save_data(visitor_arr,visitorcount);
		
	}
	
}
int main() 
{
	struct visitor visitor_arr[50];
    int events_arr[50];
	
	int visitorcount = 0;
	int eventcount = 0;
	
	init(visitor_arr,events_arr,&visitorcount,&eventcount);
		
	int i;
	
	for(;;)
	{

		printf("Welcome!\n[G] New Guest\n[L] Guestlist\n[M] Guest Modification\n[D] Guest Deletion\n[E] New Event\n[A] EventList\n[B] Event Deletion\n[R] Run an Event\n[X] Exit\n");
		
		char c;
		scanf(" %c",&c);
		switch (c) 
		{
			case 'G':
				add_visitor(visitor_arr,events_arr,&visitorcount,&eventcount);
				break;
			case 'L':
				list_all(visitor_arr,&visitorcount);
				break;
			case 'M':
				modify_visitor(visitor_arr,events_arr,&visitorcount,&eventcount);
				break;
			case 'D':
				delete_visitor(visitor_arr,events_arr,&visitorcount,&eventcount);
				break;
			case 'E':
				add_event(events_arr,&eventcount);
				break;
			case 'B':
				delete_event(visitor_arr,events_arr,&visitorcount,&eventcount);
				break;
			case 'R':
				run_event(visitor_arr,events_arr,&visitorcount,&eventcount);
				break;
			case 'X':
				exit(0);
				break;
			default : break;
		}
	}
	return 0;
}


