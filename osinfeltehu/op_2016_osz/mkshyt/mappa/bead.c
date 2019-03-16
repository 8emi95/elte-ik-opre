#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <string.h>
#include <errno.h> // for errno, the number of last error



#define BUFFERSIZE 512
struct GuestData
{
	char name[BUFFERSIZE];
	char mail[BUFFERSIZE];
	int eventid;
};

struct EventData
{
	int eid;
	char title[BUFFERSIZE];
	char eventPlace[BUFFERSIZE];
};

void menu();
void dataChange();
void deleteGuest();
void deleteEvent();
void newEvent();
void newGuest();
void listing();
int completEvent();

int event;
int guest;

void handler(int signumber){
	printf("Jelzek a szulonek\n");
}


void handler2(int signumber){
	printf("Jelzek a gyereknek\n");
}

int main(int argc, char* argv[])
{
	event = 0;
	guest = 0;

	FILE* counts;
	if( fopen("counts.txt","r") )
	{
		counts = fopen("counts.txt","r");
		rewind(counts);
		fscanf(counts, "%d %d", &event, &guest);
		fclose(counts);
	}
	else
	{
		fopen("counts.txt","w");
		fprintf(counts, "%d %d", event, guest);
		fclose(counts);
	}
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
		printf("\t7 - Esemeny lebonyolitasa\n\n");
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
		else if(strcmp(m,"7") == 0){
			completEvent();
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
	char _eid[16], _ename[64], _eventPlace[64];
	
	while(fgets(buf,sizeof(buf),event)){
		sscanf(buf,"%s %s",&_eid,&_ename, &_eventPlace);
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
	char eventPlace[BUFFERSIZE];
	
	printf("Add meg a rendezveny nevet:");
	scanf("%s",title);
	printf("Add meg a rendezveny helyszinet:");
	scanf("%s",eventPlace);
	printf("A rendezveny azonositoja:");
	printf("%d",eid);
	
	fprintf(events,"\n%d %s %s",eid,title,eventPlace);
	
	fclose(events);
	
	printf("\nNyomjon meg egy gombot!\n");
	system("read");

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
int completEvent()
{
	printf("%d esemeny var a lebonyolitasra",event);
	struct EventData events[64];
	struct GuestData guests[64];
	FILE* guestinput, *eventinput;
	guestinput = fopen("guests.txt","r");
	eventinput = fopen("events.txt","r");	
	char buf[BUFFERSIZE];
	char buf2[BUFFERSIZE];

	char time1[64], time2[64], time3[64], time4[64];
	int q=0;
    	while(q<event) {
   	     fscanf(eventinput, "%d %s %s", &(events[q].eid), &(events[q].title), &(events[q].eventPlace));
		q = q+1;
    	}
	if(event>0)
	{
		printf("\n Lebonyolitando esemeny: %d\t%s\t%s\t\n", events[0].eid, events[0].title, events[0].eventPlace);
	}

	int p=0;
		printf("guest %d \n",guest);
    	while(p<guest) {
   	     fscanf(guestinput, "%s %s %d %s %s %s %s\n", &(guests[p].name), &(guests[p].mail), &(guests[p].eventid), &time1,&time2, &time3,&time4);
		 printf("\n Lebonyolitando vendeg: %d\t%s\t%s\t\n",	guests[p].eventid, guests[p].name, guests[p].mail);
		p = p+1;
    	}


	fclose(guestinput);
	fclose(eventinput);
	
	int j;
	for(j = 0; j < 1; ++j)
	{
		int firstPipe[2];
		int secondPipe[2];
		pid_t pid;
		srand(time(NULL));
		int gyerekpid;
		
		if (pipe(firstPipe) == -1 || pipe(secondPipe) == -1) 
		{
			perror("Az egyik pipe nyitasakor hiba lepett fel!");
			exit(EXIT_FAILURE);
		}
		
		signal(SIGUSR1,handler);
		signal(SIGUSR2,handler2);
		pid = fork();
		if (pid == -1) 
		{
			perror("Fork hiba");
			exit(EXIT_FAILURE);
		}
		if(pid>0)
		{
			gyerekpid = pid;
			printf("Gyerekpid: %d",gyerekpid);
		}
		if(pid == 0)
		{
			char eventPlace[100];
			int guestCount;
			struct GuestData aktGuests[100];
			struct GuestData noGuest[100];
			int noGuestCount = 0;
			
			close(firstPipe[1]);
			close(secondPipe[0]);
			read(firstPipe[0], eventPlace, sizeof(eventPlace));

			kill(getppid(), SIGUSR1);
			printf("Partner: Megérkeztem! Helyszin: %s\n", eventPlace);
			
			pause();
			//sleep(2);
			read(firstPipe[0], &guestCount, sizeof(int));
			printf("Partner: Vart vendegek szama: %d\n", &guestCount);
			//pause();
			read(firstPipe[0], aktGuests, sizeof(aktGuests));
			close(firstPipe[0]);
			
			printf("Partner: Megkaptam a vendeglistat:\n");
	
			int i;
			for(i = 0; i < guestCount; ++i)
			{
				printf("Partner: %d. %s\n", i + 1, aktGuests[i]);
			}
			
			int successRate = rand()%100 + 1;
			
			for(i = 0; i < guestCount; ++i)
			{
				if(rand()%10 == 0)
				{
					noGuest[noGuestCount] = aktGuests[i];
					noGuestCount++;
				}
					
			}
			
			write(secondPipe[1], &successRate, sizeof(int));
			write(secondPipe[1], &noGuestCount, sizeof(int));
			write(secondPipe[1], noGuest, sizeof(noGuest));
			close(secondPipe[1]);
		}
		else
		{
			struct GuestData noGuest[100];
			struct GuestData actGuest[100];
			int guestCount = 0;
			int noGuestCount = 0;
			int successRate;
			
			
			close(firstPipe[0]); 
			close(secondPipe[1]); 
			write(firstPipe[1], events[j].eventPlace, sizeof(events[j].eventPlace));
			pause();
			printf("Szervezo: A partner tajekoztatott, hogy megerkezett. Helyszin %s\n", events[j].eventPlace);
			int k;
			for(k = 0; k<guest; k++)
			{
				printf("Szulo fog kuldeni nev %s\n",guests[k].name);
				if( guests[k].eventid == events[j].eid)
				{
					actGuest[guestCount] = guests[k];
					printf("Szulo fog kuldeni nev %s\n",actGuest[guestCount].name);
					guestCount++;
				}
			}
			//kill(gyerekpid, SIGUSR2);
			printf("Szulo: Vart vendegek szama: %d\n",guestCount);			
			write(firstPipe[1], &guestCount, sizeof(int));
			kill(gyerekpid, SIGUSR2);
			printf("Tovabb1\n");
			write(firstPipe[1], actGuest, sizeof(actGuest));
			//kill(gyerekpid, SIGUSR2);
			printf("Tovabb2\n");
			close(firstPipe[1]);
			printf("Tovabb3\n");
			kill(gyerekpid, SIGUSR2);
			
			//eredmeny kiolvasasa
			read(secondPipe[0], &successRate, sizeof(int));
			read(secondPipe[0], &noGuestCount, sizeof(int));
			read(secondPipe[0], noGuest, sizeof(noGuest));
			close(secondPipe[0]);
			
			printf("Szervezo: Megkaptam az eredmenyeket: \n");
			printf("Szervezo: Sikeresseg: %d\n", successRate);
			printf("Szervezo: Akik nem jottek el \n");
			if( noGuestCount > 0)
			{
				int i;	
				for(i = 0; i < noGuestCount; ++i)
				{
					printf("Szervezo: %s\n", noGuest[i].name );
				}
			}
			wait();
		}
	}
	return 0;
	
}




