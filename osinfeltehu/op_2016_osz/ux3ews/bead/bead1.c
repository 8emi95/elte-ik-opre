#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFFERSIZE 512

void menu();
void dataChange();
void deleteGuest();
void deleteEvent();
void newEvent();
void newGuest();
void listing();

int event;
int guest;

int main(int argc, char* argv[])
{
	event = 0;
	guest = 0;

	FILE* counts;
	counts = fopen("counts.txt","r");
	rewind(counts);
	fscanf(counts, "%d %d", &event, &guest);
	fclose(counts);


	printf("kurva: %d ",event);
	printf("kurva: %d ",guest);


	menu();

	remove("counts.txt");
	counts = fopen("counts.txt","w");
	fprintf(counts, "%d %d", event, guest);
	fclose(counts);



        return 0;
}

void menu()
{
	char m[BUFFERSIZE]; sscanf("-1","%s",m);
	while(strcmp(m,"0")!=0){
		system("clear");
		printf("\t1 - Uj esemeny\n");
		printf("\t2 - Jelentkezes\n");
		printf("\t3 - Adat modositas\n");
		printf("\t4 - Jelentkezettek listazasa\n");
		printf("\t5 - Esemeny torlese\n\n");
		printf("\t6 - Jelentkezett torlese\n\n");
		printf("\t0 - Kilepes (vagy nyomja meg a CTRL+C - t)\n");
		printf("\n\nValasszon a menupontok kozul -> ");
		scanf("%s",m);

		system("clear");
		if(strcmp(m,"1") == 0){
			newEvent();
		}
		else if(strcmp(m,"2") == 0){
			newGuest();
		}
		else if(strcmp(m,"3") == 0){
			dataChange();
		}
		else if(strcmp(m,"4") == 0){
			listing();
		}
		else if(strcmp(m,"5") == 0){
			deleteEvent();
		}
		else if(strcmp(m,"6") == 0){
			deleteGuest();
		}
		printf("\nNyomjon meg egy gombot!\n");
		system("read");
	}

}

void dataChange(){
	printf("Jelentkezett adatainak modositasa\n\n");

	char name[64],mail[64],eventid[16],month[16],day[16],year[16],time[64];

	printf("Adja meg a modositani kivant jelentkezett nevet:\n -> ");
	scanf("%s",name);

	char modosit[64]; int mod=0;
	printf("\nMit szeretne modositani?\n");
	printf("  1 - jelentkezett neve\n");
	printf("  2 - jelentkezett mail cime\n");
	printf("  3 - esemeny azonositoja\n");
	scanf("%s",modosit);

	if(strcmp(modosit,"1")==0){
		printf("\n\nAdja meg az uj nevet: ");
		scanf("%s",modosit);
		mod=1;
	}
	else if(strcmp(modosit,"2")==0){
		printf("\n\nAdja meg az uj mail cimet: ");
		scanf("%s",modosit);
		mod=2;
	}
	else if(strcmp(modosit,"3")==0){
		printf("\n\nAdja meg az uj esemeny azonositot: ");
		scanf("%s",modosit);
		mod=3;
	}

	FILE* guest, *newfile;
	guest = fopen("guests.txt","r");
	newfile = fopen("new.txt","w");
	char buf[BUFFERSIZE];

	while(fgets(buf,sizeof(buf),guest)){
		char _name[64],_mail[64],_eventid[16],_month[16],_day[16],_year[16],_time[64];
		sscanf(buf,"%s %s %s %s %s %s %s",&_name,&_mail,&_eventid,&_month,&_day,&_year,&_time);
		if(strcmp(name,_name)==0){
			if(mod==1) 		fprintf(newfile,"%s\t %s\t %s\t %s %s %s %s\n",modosit,_mail,_eventid,_month,_day,_year,_time);
			else if(mod==2) fprintf(newfile,"%s\t %s\t %s\t %s %s %s %s\n",_name,modosit,_eventid,_month,_day,_year,_time);
			else if(mod==3) fprintf(newfile,"%s\t %s\t %s\t %s %s %s %s\n",_name,_mail,modosit,_month,_day,_year,_time);
			printf("~ Modositva\n");
		}
		else
			fprintf(newfile,"%s",buf);
	}

	fclose(newfile);
	fclose(guest);


	guest=fopen("guests.txt","w");
	newfile=fopen("new.txt","r");
	while(fgets(buf,sizeof(buf),newfile))
		fprintf(guest,"%s",buf);
	fclose(newfile);
	fclose(guest);
	remove("new.txt");
}

void deleteGuest(){
	printf("Jelentkezett torlese\n\n");
	
	char name[64], mail[64],eventid[16], date[64], time[64];

	printf("Adja meg melyik vendeget szeretne torolni:\n -> ");
	scanf("%s",name);

	FILE* guest, *newfile;
	guest = fopen("guests.txt","r");
	newfile = fopen("new.txt","w");
	char buf[BUFFERSIZE];

	char _name[64], _mail[64], _eventid[16], _date[64], _time[64];
	
	while(fgets(buf,sizeof(buf),guest)){
		sscanf(buf,"%s %s %s %s %s",&_name,&_mail,&_eventid,&_date,&_time);
		if(strcmp(name,_name)==0)
			printf("~ Torolve\n");
		else
			fprintf(newfile,"%s",buf);
	}
	fclose(newfile);
	fclose(guest);


	guest=fopen("guests.txt","w");
	newfile=fopen("new.txt","r");
	while(fgets(buf,sizeof(buf),newfile))
		fprintf(guest,"%s",buf);
	fclose(newfile);
	fclose(guest);
	remove("new.txt");
}

void deleteEvent(){
	printf("Esemeny torlese\n\n");
		
	char eid[16], ename[64];
	char name[64], mail[64],eventid[16], date[64], time[64];

	printf("Adja meg a torolni kivant esemeny id:\n -> ");
	scanf("%s",eid);

	FILE* guest, *newfile,*event, *newfile2;
	guest = fopen("guests.txt","r");
	event = fopen("events.txt","r");
	newfile = fopen("new.txt","w");
	newfile2 = fopen("new2.txt","w");
	
	char buf[BUFFERSIZE];
	char buf2[BUFFERSIZE];

	char _name[64], _mail[64], _eventid[16], _date[64], _time[64];
	char _eid[16], _ename[64];
	
	while(fgets(buf,sizeof(buf),event)){
		sscanf(buf,"%s %s",&_eid,&_ename);
		if(strcmp(eid,_eid)==0)
			printf("~ Torolve\n");
		else
			fprintf(newfile,"%s",buf);
	}
	fclose(newfile);
	fclose(event);
	
	while(fgets(buf2,sizeof(buf),guest)){
		sscanf(buf2,"%s %s %s %s %s",&_name,&_mail,&_eventid,&_date,&_time);
	if(strcmp(eid,_eventid)==0)
			printf("~ Torolve\n");
		else
			fprintf(newfile2,"%s",buf2);
	}
	fclose(newfile2);
	fclose(guest);


	event=fopen("events.txt","w");
	newfile=fopen("new.txt","r");
	while(fgets(buf,sizeof(buf),newfile))
		fprintf(event,"%s",buf);
	fclose(newfile);
	fclose(event);
	remove("new.txt");
	
	guest=fopen("guests.txt","w");
	newfile2=fopen("new2.txt","r");
	while(fgets(buf2,sizeof(buf2),newfile2))
		fprintf(guest,"%s",buf2);
	fclose(newfile2);
	fclose(guest);
	remove("new2.txt");
	

}

void newEvent(){
	printf("Esemeny letrehozasa\n\n");
	
	FILE *events;
	events = fopen("events.txt","a+");
	int eid = event + 1;
	event = event + 1;
	
	char title[BUFFERSIZE];
	
	printf("Add meg a rendezveny nevet:");
	scanf("%s",title);
	printf("A rendezveny azonositoja:");
	printf("%d",eid);
	
	fprintf(events,"\n%d %s",eid,title);
	
	fclose(events);

}

void newGuest(){
	printf("Jelentkezes\n\n");
	
	FILE *guests;
	guests = fopen("guests.txt","a+");
	int gid = guest + 1;
	guest = guest + 1;
	
	char name[BUFFERSIZE];
	char mail[BUFFERSIZE];
	int eventid;

	printf("Adja meg a nevet:\n -> ");
	scanf("%s",name);
	printf("Adja meg az e-mail cimet:\n -> ");
	scanf("%s",mail);
	printf("Adja meg a rendezveny azonositojat:\n -> ");
	scanf("%d",&eventid);

	fprintf(guests,"%s\t%s\t%d %s %s\n",name,mail,eventid,__DATE__,__TIME__);
	
	fclose(guests);

	printf("On a %d . jelentkezett",gid);

}

void listing(){
	printf("Jelentkezettek listazasa\n\n");
	
	char eid[BUFFERSIZE];
	printf("Mit szeretnel kilistazni?\n");
	printf("  1 - minden\n");
	printf("  2 - adott esemeny id szerint\n");
	printf(" -> ");
	scanf("%s",eid);

	FILE* guest = fopen("guests.txt","r");
	char buf[BUFFERSIZE];

	if(strcmp(eid,"1")==0) {
		while(fgets(buf,sizeof(buf),guest))
			printf("%s",buf);
	}
	else if(strcmp(eid,"2")==0) {
		printf("\nMelyik id szerint?\n");
		printf(" -> ");
		scanf("%s",eid);
		printf("\n");

		char name[64], mail[64],eventid[16], date[64], time[64];
		while(fgets(buf,sizeof(buf),guest)){
			sscanf(buf,"%s %s %s %s %s",&name,&mail,&eventid,&date,&time);
			if(strcmp(eventid,eid)==0)
				printf("%s",buf);
		}
	}
	fclose(guest);
}

