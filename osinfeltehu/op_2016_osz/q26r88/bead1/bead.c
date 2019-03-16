#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

typedef int bool;
#define true 1
#define false 0

struct visitor
{
    char name[50];
    char email[50];
    int id;
    time_t time;
};

void init(struct visitor visitor_arr[], int events_arr[], int *visitorcount , int *eventcount);
void add_visitor(struct visitor visitor_arr[], int events_arr[], int *visitorcount , int *eventcount);
void list_all(struct visitor visitor_arr[], int *visitorcount);
void modify_visitor(struct visitor visitor_arr[], int events_arr[], int *visitorcount , int *eventcount);
void save_data(struct visitor visitor_arr[], int *visitorcount);
void delete_visitor(struct visitor visitor_arr[], int events_arr[], int *visitorcount , int *eventcount);
void add_event(int events_arr[], int *eventcount);
void delete_event(struct visitor visitor_arr[], int events_arr[], int *visitorcount , int *eventcount);


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
		write(1,"\n",1);
		write(1,"King of Stands\n",15);
		write(1,"Latogatoi nyilvantartas\n\n",25);
		write(1,"1. Uj latogato jelentkezes\n",27);
		write(1,"2. Jelentkezett latogatok listazasa\n",36);
		write(1,"3. Latogato adatmodositas\n",26);
		write(1,"4. Latogato torlese\n",20);
		write(1,"5. Uj rendezveny inditasa\n",26);
		write(1,"6. Rendezveny torlese\n",22);
		write(1,"0. Kilepes\n",11);
		write(1,"Valasszon a menubol: ",21);
		
		char c;
		scanf(" %c",&c);
		switch (c) 
		{
			case '1':
				add_visitor(visitor_arr,events_arr,&visitorcount,&eventcount);
				break;
			case '2':
				list_all(visitor_arr,&visitorcount);
				break;
			case '3':
				modify_visitor(visitor_arr,events_arr,&visitorcount,&eventcount);
				break;
			case '4':
				delete_visitor(visitor_arr,events_arr,&visitorcount,&eventcount);
				break;
			case '5':
				add_event(events_arr,&eventcount);
				break;
			case '6':
				delete_event(visitor_arr,events_arr,&visitorcount,&eventcount);
				break;
			case '0':
				exit(0);
				break;
			default : break;
		}
	}
	return 0;
}

void init(struct visitor visitor_arr[], int events_arr[], int *visitorcount , int *eventcount)
{
	FILE * file;
	file = fopen("data.txt","r");
	
	char name[50];
	char email[50];
	int id;
	char time[30];
	
	int temp;
	
	char line[500];
	while(fgets(line,sizeof(line),file) != NULL)
	{
		sscanf(line,"%s %s %d %[^\n]", name, email, &id, time);
		strcpy(visitor_arr[*visitorcount].name,name);
		strcpy(visitor_arr[*visitorcount].email,email);
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
	char email[50];
	int id;
	bool valid_id = false;

	write(1,"Kerem a nevet: ",15);
	scanf("%s", name);
	
	write(1,"Kerem az e-mail cimet: ",23);
	scanf("%s", email);
	
	write(1,"Kerem a rendezvenyazanostitot: ",31);
	scanf("%d", &id);
	
	int i;
	for(i = 0; i < *eventcount; i++)
	{
		if(events_arr[i] == id)
		{
			valid_id = true;
		}
	}
	
	if(valid_id)
	{
		strcpy(visitor_arr[*visitorcount].name,name);
		strcpy(visitor_arr[*visitorcount].email,email);
		visitor_arr[*visitorcount].id = id;
		visitor_arr[*visitorcount].time = time(NULL);
		*visitorcount = *visitorcount + 1;
		printf("\nOn a %d. latogato\n",*visitorcount);
	}
	else 
	{
		printf("\nNemletezo rendezveny azonosito\n");
	}
	
	FILE * file;
	file = fopen("data.txt","a+");
		
	char w_data[90] = "";
	strcat(w_data,name);
	strcat(w_data," ");
	strcat(w_data,email);
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
		printf("%s ", visitor_arr[i].email);
		printf("%d ", visitor_arr[i].id);
		printf("%s", ctime(&visitor_arr[i].time));
	}
}

void modify_visitor(struct visitor visitor_arr[], int events_arr[], int *visitorcount , int *eventcount)
{
	char name[50];
	char email[50];
	int id;
	bool valid_id = false;

	write(1,"Modositando latogato neve: ",27);
	scanf("%s", name);
	
	int i = 0;
	while(i < *visitorcount && strcmp(visitor_arr[i].name,name) != 0) i++;
	if (i < *visitorcount)
	{
		write(1,"Modositando adat(1:email, 2:rendezveny, 0:kilepes/mentes): ",59);
		char c;
		scanf(" %c",&c);
		while(c != '0')
		{
			switch (c) 
			{
				case '1':
					write(1,"Kerem az e-mail cimet: ",23);
					scanf("%s", email);
					strcpy(visitor_arr[i].email,email);
					break;
				case '2':
					write(1,"Kerem a rendezvenyazanostitot: ",31);
					scanf("%d", &id);
					int j;
					for(j = 0; j < *eventcount; j++)
					{
						if(events_arr[j] == id)
						{
							valid_id = true;
						}
					}
					if (valid_id) visitor_arr[i].id = id;
					else printf("\nNemletezo rendezveny azonosito\n");
					break;
				default : break;
			}
			write(1,"Modositando adat(1:email, 2:rendezveny, 0:kilepes/mentes): ",59);
			read(0,&c,2);
		}
	}
	else
	{
		printf("\nNemletezo latogato\n");
	}
	save_data(visitor_arr,visitorcount);
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
		strcat(w_data,visitor_arr[i].email);
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

void delete_visitor(struct visitor visitor_arr[], int events_arr[], int *visitorcount , int *eventcount)
{
	char name[50];

	write(1,"Torlendo latogato neve: ",24);
	scanf("%s", name);
	
	int i = 0;
	while(i < *visitorcount && strcmp(visitor_arr[i].name,name) != 0) i++;
	if (i < *visitorcount)
	{
		for(;i < *visitorcount - 1; i++)
		{
			strcpy(visitor_arr[i].name,visitor_arr[i+1].name);
			strcpy(visitor_arr[i].email,visitor_arr[i+1].email);
			visitor_arr[i].id = visitor_arr[i+1].id;
			visitor_arr[i].time = visitor_arr[i+1].time;
		}
		*visitorcount = *visitorcount - 1;
	}
	else
	{
		printf("\nNemletezo latogato\n");
	}
	save_data(visitor_arr,visitorcount);
}

void add_event(int events_arr[], int *eventcount)
{
	int id;
	bool exists = false;
	
	write(1,"Kerem a rendezvenyazanostitot: ",31);
	scanf("%d", &id);
	
	int i;
	for(i = 0; i < *eventcount; i++)
	{
		if(events_arr[i] == id)
		{
			exists = true;
		}
	}
	
	if (!exists)
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

	write(1,"Kerem a rendezvenyazanostitot: ",31);
	scanf("%d", &id);
	
	int i = 0;
	while(i < *eventcount && events_arr[i] != id) i++;
	if (i < *eventcount)
	{
		for(;i < *eventcount - 1; i++) events_arr[i] = events_arr[i+1];
		*eventcount = *eventcount - 1;
	}
	else printf("\nNemletezo azonosito\n");
	
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
				strcpy(visitor_arr[i].email,visitor_arr[i+1].email);
				visitor_arr[i].id = visitor_arr[i+1].id;
				visitor_arr[i].time = visitor_arr[i+1].time;
			}
			*visitorcount = *visitorcount - 1;
		}
		else i++;
	}
	save_data(visitor_arr,visitorcount);
	
}