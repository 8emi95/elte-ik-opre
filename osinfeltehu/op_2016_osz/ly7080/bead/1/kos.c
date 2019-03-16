#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

#define STRING_LENGTH (32)

typedef int bool;
enum { false, true };

typedef struct{
	char Name[STRING_LENGTH];
	char Email[STRING_LENGTH];
    time_t CreationTime;
	int EventId;
	int Id;
} Guest;

typedef struct{
	char Name[STRING_LENGTH];
	int Id;
} Event;



void addRegistration();
void listRegistrations();
void editRegistration();
void removeRegistration();
void addEvent();
void listEvents();
void removeEvent();

int main(int argc, char *argv[]){
	
	printf("King of Stands\n");
	printf("===================\n");
	bool end = false;
	
	while(!end){
		printf("Kilepes: 0\n"
		"Uj vendeg: 1\n"
		"Vendegek listazasa: 2\n"
		"Vendeg adatok modositas: 3\n"
		"Vendeg torles: 4\n"
		"Uj rendezveny: 5\n"
		"Rendezvenyek listazasa: 6\n"
		"Rendezveny torlese: 7\n");
		
		printf("\nValassz egy opciot: ");
		
		int option;
		scanf("%d",&option);
		
		switch(option){
			case 0:
				end = true;
				printf("Viszlat!\n");
				break;
			case 1:
				addRegistration();
				break;
			case 2:
				listRegistrations();
				break;
			case 3:
				editRegistration();
				break;
			case 4:
				removeRegistration();
				break;
			case 5:
				addEvent();
				break;
			case 6:
				listEvents();
				break;
			case 7:
				removeEvent();
				break;
			default:
				printf("Nincs ilyen opcio!\n\n");
		}	
	}
}

void addRegistration(){
	int f = open("data.txt", O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
	int r = open("event.txt", O_RDWR);
	
	char name[STRING_LENGTH];
	char email[STRING_LENGTH];
	time_t currTime;
	int event;

	int c;
	int id;
	
	int d;
	int h;
	
	bool eventFound = false;
	
	Event* events = NULL;
	
	if(read(r,&d,sizeof(int)) < 0 || d==0){
		close(r);
		close(f);
		
		printf("Meg nincs rendezveny! Elobb egy rendezvenyt kell letrehozni!\n");
		return;
	}
	
	read(r,&h,sizeof(int));
	
	events = (Event*) realloc(events,d*sizeof(Event));
	read(r,events,d*sizeof(Event));

	close(r);
	
	if(read(f,&c,sizeof(int)) < 0){
		close(f);
		f = open("data.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		c = 0;
		id = 0;
		
		write(f,&c,sizeof(c));
		write(f,&id,sizeof(id));
	}

	c += 1;
	
	read(f,&id,sizeof(int));
	id += 1;
	close(f);
	f = open("data.txt", O_WRONLY, S_IRUSR | S_IWUSR);
	
	printf("\nAdj meg egy nevet: ");
	scanf("%s", name);
	
	printf("\nAdj meg egy email cimet: ");
	scanf("%s", email);
	
	
	
	while(!eventFound){
		printf("\nAdj meg egy rendezeny azonositot: ");
		scanf("%i", &event);
		
		int i = 0;
		
		while(i < d && !eventFound){
			if(events[i].Id == event){
				eventFound = true;
			}
			
			++i;
		}
		if(!eventFound){
			printf("\nNincs ilyen azonositoju rendezveny!\n");
		}
	}
	
	free(events);
	
	time(&currTime);
	
	write(f,&c,sizeof(c));
	write(f,&id,sizeof(id));
	
	close(f);
	
	f = open("data.txt", O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
	
	write(f,name,sizeof(name));
	write(f,email,sizeof(email));
	write(f,&currTime,sizeof(time_t));
	write(f,&event,sizeof(event));
	write(f,&id,sizeof(id));
	
	printf("\nÖn a a(z) %i. aktív vendeg, es osszesitesben a(z) %i.!\n\n",c,id);

	close(f);

}

void listRegistrations(){
	int f = open("data.txt",O_RDONLY);
	char name[STRING_LENGTH];
	char email[STRING_LENGTH];
	time_t t;
	int event;
	int id;
	
	char* currTime;
	struct tm* timeInfo;
	
	int num;
	int all;

	if(read(f,&num,sizeof(int)) < 0 || num==0){
		close(f);
		printf("\nMeg ures a lista!\n\n");
		return;
	}
	read(f,&all,sizeof(int));
	printf("\n");
	int i=0;
	
	printf("\nRegisztraciok listazasa:\n=========================\n(Ha az esemeny azonosito 0, akkor az adott regisztraciohoz tartozo elozo esemeny mar megszunt, modositassal uj felveheto!)\n\n");
	
	while(i<num){
		read(f,name,sizeof(name));
		read(f,email,sizeof(email));
		read(f,&t,sizeof(time_t));
		read(f,&event,sizeof(event));
		read(f,&id,sizeof(id));
		timeInfo = localtime(&t);
		
		printf("Nev: %s\n",name);
		printf("Email: %s\n",email);
		printf("Regisztracio ideje: %s",asctime(timeInfo));
		printf("Esemeny azonositoja: %i\n",event);
		printf("Vendeg sorszáma: %i\n\n", id);
		++i;
	}
	
	printf("\n");
	
	close(f);
}

void editRegistration(){
	int f = open("data.txt",O_RDONLY);
	int r = open("event.txt",O_RDONLY);
	
	int num;
	int all;
	
	int modifyId;
	
	int d;
	int id;
	
	bool eventFound = false;
	
	Event* events = NULL;
	Guest* guests = NULL;
	
	if(read(r,&d,sizeof(int)) < 0 || d==0){
		close(r);
		close(f);
		
		printf("Nincs meg rendezveny! Elobb egy rendezvenyt kell felvenni!\n");
		return;
	}
	read(r,&id,sizeof(int));
	
	events = (Event*) realloc(events,d*sizeof(Event));
	read(r,events,d*sizeof(Event));

	close(r);

	if(read(f,&num,sizeof(int)) < 0 || num==0){
		close(f);
		
		printf("Meg nincsenek vendegek!\n");
		return;
	}
	
	read(f,&all,sizeof(int));
	
	guests = (Guest*) realloc(guests,num*sizeof(Guest));
	read(f,guests,num*sizeof(Guest));
	
	printf("\nMelyik regisztraciot modositja? Adjon meg egy vendeg azonositot: ");
	scanf("%i", &modifyId);
	
	int i = 0;
	
	bool guestFound = false;

	while(i<num){
		if(guests[i].Id == modifyId){
			guestFound = true;
			printf("\nAdj meg az uj nevet: ");
			scanf("%s", guests[i].Name);
			
			printf("\nAdj meg az uj email cimet: ");
			scanf("%s", guests[i].Email);
			
			while(!eventFound){
				printf("\nAdj meg egy rendezeny azonositot: ");
				scanf("%i", &guests[i].EventId);
				
				int j = 0;
				
				while(j < d && !eventFound){
					if(events[j].Id == guests[i].EventId){
						eventFound = true;
					}
					++j;
				}
				if(!eventFound){
					printf("\nNincs ilyen azonositoju rendezveny!\n");
				}
			}
		}

		++i;
	}
	
	free(events);
	
	close(f);
	
	if(guestFound){
		f = open("data.txt",O_WRONLY);
		
		write(f,&num,sizeof(int));
		write(f,&all,sizeof(int));
		write(f,guests,num*sizeof(Guest));
		
		close(f);
	}
	else{
		printf("Nincs ilyen vendeg!\n\n");
	}
	
	free(guests);
}

void removeRegistration(){
	int f = open("data.txt",O_RDONLY);
	
	int num;
	int all;
	
	int removeId;
	
	Guest* guests = NULL;
	bool* removeData = NULL;

	if(read(f,&num,sizeof(int))<0 || num==0){
		close(f);
		
		printf("Meg nincs vendeg!\n");
		return;
	}
	read(f,&all,sizeof(int));
	
	int newNum = num;
	
	guests = (Guest*) realloc(guests,num*sizeof(Guest));
	read(f,guests,num*sizeof(Guest));
	
	removeData = (bool*) realloc(removeData,num*sizeof(bool));
	
	printf("\nMelyik regisztraciot torli? Adjon egy vendeg sorszámot: ");
	scanf("%i", &removeId);
	
	int i = 0;

	while(i<num){
		if(guests[i].Id == removeId){
			removeData[i] = true;
			newNum -= 1;
		}
		else{
			removeData[i] = false;
		}
		++i;
	}
	close(f);
	f = open("data.txt",O_WRONLY | O_TRUNC);
	
	i = 0;
	
	write(f,&newNum,sizeof(int));
	write(f,&all,sizeof(int));
	
	while(i<num){
		if(!removeData[i]){
			write(f,&guests[i],sizeof(Guest));
		}
		++i;
	}
	free(guests);
	free(removeData);
	close(f);
}

void addEvent(){
	int f = open("event.txt", O_RDWR);
	
	char name[STRING_LENGTH];

	int c;
	int id;
	
	
	if(read(f,&c,sizeof(int)) < 0 ){
		close(f);
		f = open("event.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		c = 0;
		id = 0;
		
		write(f,&c,sizeof(c));
		write(f,&id,sizeof(id));
		close(f);
	}
	else{
		read(f,&id,sizeof(int));
		
		close(f);
		f = open("event.txt", O_WRONLY);
	}

	c += 1;
	id += 1;	

	printf("\nAdj meg egy rendezveny nevet: ");
	scanf("%s", name);
	
	f = open("event.txt", O_WRONLY);
	
	write(f,&c,sizeof(c));
	write(f,&id,sizeof(id));
	
	close(f);
	
	f = open("event.txt", O_WRONLY | O_APPEND);
	
	write(f,name,sizeof(name));
	write(f,&id,sizeof(id));
	
	printf("\nFelvetted a(z) %i. aktív rendezvenyt, ami osszesitesben a(z) %i.!\n\n",c,id);

	close(f);
}

void listEvents(){
	int f = open("event.txt",O_RDONLY);
	char name[STRING_LENGTH];
	int id;
	
	int num;
	int all;

	if(read(f,&num,sizeof(int)) < 0){
		close(f);
		printf("\nMeg ures a lista!\n\n");
		return;
	}
	read(f,&all,sizeof(int));

	int i=0;

	printf("\nRendezvenyek listazasa:\n========================\n");
	
	while(i<num){
		read(f,name,sizeof(name));
		read(f,&id,sizeof(id));
		
		printf("Nev: %s\n",name);
		printf("Rendezveny azonositoja: %i\n\n", id);
		++i;
	}
	
	printf("\n");
	
	close(f);
}

void removeEvent(){
	int f = open("event.txt",O_RDONLY);
	
	int num;
	int all;
	
	int removeId;
	
	Event* events = NULL;
	bool* removeData = NULL;
	
	Guest* guests = NULL;

	if(read(f,&num,sizeof(int)) < 0 || num==0){
		close(f);
		
		printf("Meg nincs rendezveny! Elobb vegyen fel egy rendezvenyt!\n");
		return;
	}
	read(f,&all,sizeof(int));
	
	int newNum = num;
	
	events = (Event*) realloc(events,num*sizeof(Event));
	read(f,events,num*sizeof(Event));
	
	removeData = (bool*) realloc(removeData,num*sizeof(bool));
	
	printf("\nMelyik regisztraciot torli? Adjon egy vendeg sorszámot: ");
	scanf("%i", &removeId);
	
	int i = 0;

	while(i<num){
		if(events[i].Id == removeId){
			removeData[i] = true;
			newNum -= 1;
		}
		else{
			removeData[i] = false;
		}
		++i;
	}
	close(f);
	f = open("event.txt",O_WRONLY | O_TRUNC);
	
	i = 0;
	
	write(f,&newNum,sizeof(int));
	write(f,&all,sizeof(int));
	
	while(i<num){
		if(!removeData[i]){
			write(f,&events[i],sizeof(Event));
		}
		++i;
	}
	
	close(f);
	
	int r = open("data.txt",O_RDONLY);
	
	if(read(r,&num,sizeof(num))<0 || num==0){
		close(r);
		
		return;
	}
	read(r,&all,sizeof(all));
	
	guests = (Guest*) realloc(guests,num*sizeof(Guest));
	read(r,guests,num*sizeof(Guest));
	
	i = 0;
	while(i<num){
		if(guests[i].EventId == removeId){
			guests[i].EventId = 0;
		}
		
		++i;
	}
	
	close(r);
	
	r = open("data.txt",O_WRONLY | O_TRUNC);
	
	write(r,&num,sizeof(num));
	write(r,&all,sizeof(all));
	write(r,guests,num*sizeof(Guest));
	
	free(events);
	free(guests);
	free(removeData);
	
	close(r);
}