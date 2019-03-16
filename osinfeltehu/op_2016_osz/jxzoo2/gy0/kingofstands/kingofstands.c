#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#define FILE_VISITORS "visitors.txt"
#define FILE_EVENTS   "events.txt"

struct checkin{
	int number;
    char name[64];
    char mail[64];
	int eventID;
	char date[12];
};
 
struct event{
	int number;
	int eventID;
	char eventname[64];
};

void read_file(struct checkin** visitors, int *vis_c);
void print_file(struct checkin* visitors, int vis_c);
void save_file(struct checkin* visitors, int vis_c);
void new_checkin(struct checkin** visitors, int *vis_c);
void modify_visitor_record(struct checkin** visitors, int *vis_c);
void delete_visitor_record(struct checkin* visitors, int *vis_c);

void read_file2(struct event** events, int *ev_c);
void print_file2(struct event* events, int ev_c);
void save_file2(struct event* events, int ev_c);

void create_event(struct event** events, int *ev_c);
void delete_event(struct event* events, int *ev_c);

int main(void){
	system("clear");	
	
	int menu;
	int need_to_save=0; 
	
	//for handling visitors
	int vis_c=0;
	int *m_vis_c=&vis_c;
	struct checkin *visitors;
	struct checkin **m_visitors=&visitors;
	
	//for handling events
	int ev_c=0;
	int *m_ev_c=&ev_c;
	struct event *events;
	struct event **m_events=&events;

	read_file(m_visitors, m_vis_c);
	read_file2(m_events, m_ev_c);
	
	do{
		printf("\nPlease select an option from the following menu:\n");
		printf("New checkin - 1\n");
		printf("Visitors report - 2\n");
		printf("Modify visitor record - 3\n");
		printf("Delete visitor record - 4\n");
		printf("Create a new event - 5\n");
		printf("Delete an event - 6\n");
		printf("Events report - 7\n");
		printf("Exit - 0\n");
		printf("Your number is: ");
		scanf("%d",&menu);
		
		if(menu == 1){
			new_checkin(m_visitors, m_vis_c);
			need_to_save=1;
			break;
		}
		if(menu == 2){
			print_file(visitors, vis_c);
			break;
		}
		if(menu == 3){
			modify_visitor_record(m_visitors, m_vis_c);
			need_to_save=1;
			break;
		}
		if(menu == 4){
			delete_visitor_record(visitors, m_vis_c);
			need_to_save=1;
			break;
		}
		if(menu == 5){
			create_event(m_events, m_ev_c);
			need_to_save=2;
			break;
		}
		if(menu == 6){
			delete_event(events, m_ev_c);
			need_to_save=2;
			break;
		}
		if(menu == 7){
			print_file2(events, ev_c);
			break;
		}
		if(menu == 0){
			break;
		}
	}while (menu!=0);
	
	if(need_to_save) save_file(visitors, vis_c);
	if(need_to_save=2) save_file2(events, ev_c);
	
	free(visitors); // Deallocates the memory
	free(events); // Deallocates the memory//free event too
};

void read_file(struct checkin** visitors, int *vis_c){

	FILE *file=fopen(FILE_VISITORS,"r");
	fscanf(file,"%d\n",vis_c);

	*visitors=(struct checkin*)malloc(*vis_c*sizeof(struct checkin));
	int i;
	for(i=0; i<*vis_c;++i){
		fscanf(file,"%d",&((*visitors)[i].number));
		fscanf(file,"%s",&((*visitors)[i].name));
		fscanf(file,"%s",&((*visitors)[i].mail));
		fscanf(file,"%d",&((*visitors)[i].eventID));
		fscanf(file,"%s\n",&((*visitors)[i].date));
	}
	fclose(file);
}

void print_file(struct checkin* visitors, int vis_c){
	system("clear");
	int i;
	for(i=0; i<vis_c;++i){
		printf("%d ",visitors[i].number);
		printf("%s ",visitors[i].name);
		printf("%s ",visitors[i].mail);
		printf("%d ",visitors[i].eventID);
		printf("%s\n",visitors[i].date);
	}
}

void save_file(struct checkin* visitors, int vis_c){
	FILE *file=fopen(FILE_VISITORS, "w");
	
	fprintf(file,"%d\n",vis_c);
	int i;
	for(i=0; i<vis_c;++i){
		fprintf(file,"%d ",i+1);
		fprintf(file,"%s ",visitors[i].name);
		fprintf(file,"%s ",visitors[i].mail);
		fprintf(file,"%d ",visitors[i].eventID);
		fprintf(file,"%s\n",visitors[i].date);
	}
	fclose(file);
}

void read_file2(struct event** events, int *ev_c){

	FILE *file=fopen(FILE_EVENTS,"r");
	fscanf(file,"%d\n",ev_c);

	*events=(struct event*)malloc(*ev_c*sizeof(struct event));
	int i;
	for(i=0; i<*ev_c;++i){
		fscanf(file,"%d",&((*events)[i].number));
		fscanf(file,"%d",&((*events)[i].eventID));
		fscanf(file,"%s\n",&((*events)[i].eventname));
	}
	fclose(file);
}

void print_file2(struct event* events, int ev_c){
	system("clear");
	int i;
	for(i=0; i<ev_c;++i){
		printf("%d ",events[i].number);
		printf("%d ",events[i].eventID);
		printf("%s\n",events[i].eventname);
	}
}

void save_file2(struct event* events, int ev_c){
	FILE *file=fopen(FILE_EVENTS, "w");
	
	fprintf(file,"%d\n",ev_c);
	int i;
	for(i=0; i<ev_c;++i){
		fprintf(file,"%d ",i+1);
		fprintf(file,"%d ",events[i].eventID);
		fprintf(file,"%s\n",events[i].eventname);
	}
	fclose(file);
}

void new_checkin(struct checkin** visitors, int *vis_c){
	system("clear");

	*visitors=(struct checkin*)realloc(*visitors,++(*vis_c)*sizeof(struct checkin));
	
	printf("Name: ");
	scanf("%s",&((*visitors)[(*vis_c)-1].name));
	system("clear");
	
	printf("E-mail: ");
	scanf("%s",&((*visitors)[(*vis_c)-1].mail));
	system("clear");
	
	printf("Event ID: ");
	scanf("%d",&((*visitors)[(*vis_c)-1].eventID));
	system("clear");
	
	time_t now;
	time(&now);
	struct tm* time_info;
	time_info=localtime(&now);
	
	strftime((*visitors)[(*vis_c)-1].date, 12, "%Y.%m.%d.", time_info);
	
	printf("Welcome! You are the number#%d visitor.\n", *vis_c);
}

void modify_visitor_record(struct checkin** visitors, int *vis_c){
	system("clear");
	
	int edit = 0;
	printf("Please submit your number!\n");
	scanf("%d",&edit);

	printf("Name: ");
	scanf("%s",&((*visitors)[edit-1].name));
	
	printf("E-mail: ");
	scanf("%s",&((*visitors)[edit-1].mail));
	
	printf("Event ID: ");
	scanf("%d",&((*visitors)[edit-1].eventID));
	
	time_t now;
	time(&now);
	struct tm* time_info;
	time_info=localtime(&now);
	
	strftime((*visitors)[edit-1].date, 12, "%Y.%m.%d.", time_info);
}

void delete_visitor_record(struct checkin* visitors, int *vis_c){
	system("clear");
	
	int edit = 0;
	printf("Which visitor# are you going to delete?\n");
	scanf("%d",&edit);
	int i;
	for(i=edit-1; i<*vis_c-1;++i){
		visitors[i] = visitors[i+1];
	}
	*vis_c=*vis_c-1;
}

void create_event(struct event** events, int *ev_c){
	system("clear");

	*events=(struct event*)realloc(*events,++(*ev_c)*sizeof(struct event));
	
	printf("Event ID: ");
	scanf("%d",&((*events)[(*ev_c)-1].eventID));
	system("clear");
	
	printf("Event name: ");
	scanf("%s",&((*events)[(*ev_c)-1].eventname));
	system("clear");
	
	printf("#%d events are logged in the database.\n", *ev_c);
};

void delete_event(struct event* events, int *ev_c){
	system("clear");

	int edit = 0;
	printf("Which event are you going to delete?\n");
	scanf("%d",&edit);
	int i;
	for(i=edit-1; i<*ev_c-1;++i){
		events[i] = events[i+1];
	}
	*ev_c=*ev_c-1;
	
	printf("#%d events are logged in the database.\n", *ev_c);
};