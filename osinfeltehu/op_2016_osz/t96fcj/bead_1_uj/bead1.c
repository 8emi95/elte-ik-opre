#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
typedef enum { false, true } bool;

struct guest
{
	char name[30];
	char email[30];
	int event_id;
	time_t registration_time;
};

struct event
{
	char name[30];
	int event_id;
};

bool is_Exist_event(int q_id)
{
	int tmp_id;
	FILE *new_events;
	new_events=fopen("event_details.txt","a+");
	while(fread(&tmp_id,sizeof(tmp_id),1,new_events)==1)
	{
		if(tmp_id==q_id)
		{
			fclose(new_events);
			return true;
		}
	}
	fclose(new_events);
	return false;		
}

bool is_Exist_guest(char * guest_email)
{
	
	int guest_number;
	FILE *new_guest;
	struct guest temp_struct;
	new_guest=fopen("guest_details.txt","r");
	
	fread(&guest_number,sizeof(guest_number),1,new_guest);
	if(guest_number==0)
	{
		fclose(new_guest);
		return false;
	}
	
	while(fread(&temp_struct,sizeof(temp_struct),1,new_guest)==1)
	{
		if(strcmp(guest_email,temp_struct.email)==0)
		{
			fclose(new_guest);
			return true;
		}
	}
	fclose(new_guest);
	return false;		
	
	
}


int Inicialize()
{
	FILE *new_guest;
	FILE *new_events;
	new_guest=fopen("guest_details.txt","a+");
	new_events=fopen("event_details.txt","a+");
	int i;
	if(fread(&i,sizeof(i),1,new_guest)!=1)
	{
		i=0;
		fwrite(&i,sizeof(i),1,new_guest);
	}
	
	
	fclose(new_guest);
	fclose(new_events);
	
	
	
	

	return 0;
	
}


void Registration()
{	

	
    while (fgetc(stdin) != '\n') // Read until a newline is found
    {}    
	
	//Ebbe olvasunk a fájlból
	struct guest temp_struct;
	// Ezekbe olvasunk be a billentyûrõl
	char tmp_name[30];
	char tmp_email[30];
	int tmpid;
	time_t tmp_time;
	//tmp_time=time(NULL);
	
	int reg_number;
	FILE *dataptr_guest;
	
	FILE *dataptr_event;
	int tmp_event;
	
	
	printf("Kerem adja meg az email cimet:");
	scanf(" %[^\n]s",tmp_email);
	
	
	
	printf("Kerem adja meg az esemeny azonositoszamat amire szeretne regisztralni: ");
	scanf(" %i", &tmpid);
	
	dataptr_event = fopen ("event_details.txt","r+");
	
	dataptr_guest = fopen("guest_details.txt","r+");
	bool exist_event=false;
	
	
	while(fread(&tmp_event,sizeof(tmp_event),1,dataptr_event)==1)
	{
		if(tmpid==tmp_event)
		{
			exist_event=true;
			break;
		}		
	}
	
	if(!(exist_event))
	{
		printf("Az esemeny nem letezik!");
		fclose(dataptr_guest);
		fclose(dataptr_event);
		return;
	}
	
	
	fread(&reg_number,sizeof(reg_number),1,dataptr_guest);
	
	if(reg_number!=0)
	{
		while(fread(&temp_struct,sizeof(temp_struct),1,dataptr_guest)==1)
		{
			
			if((strcmp(tmp_email,temp_struct.email)==0) && (tmpid==(temp_struct.event_id)) )
			{
				printf("Ezzel az email cimmel mar regisztraltak erre az esemenyre! A regisztralas nem lehetséges! \n\n");
				fclose(dataptr_guest);
				fclose(dataptr_event);
				return;
			}		
		}
	}
	
	
	fseek(dataptr_guest,0,SEEK_END);
	printf("Kerem adja meg a nevet is:");
	scanf(" %[^\n]s",tmp_name);
	printf("A megadott nev: %s",tmp_name);
	reg_number=reg_number+1;
	fseek(dataptr_guest,0,SEEK_SET);
	printf("\n A regisztracio a %i. helyen tortent meg\n",reg_number);
	fwrite(&reg_number,sizeof(reg_number),1,dataptr_guest);
	
	fseek(dataptr_guest,0,SEEK_END);
	
	fwrite(&tmp_name,sizeof(tmp_name),1,dataptr_guest);
	fwrite(&tmp_email,sizeof(tmp_email),1,dataptr_guest);
	fwrite(&tmpid,sizeof(tmpid),1,dataptr_guest);
	tmp_time=time(NULL);
	fwrite(&tmp_time,sizeof(time_t),1,dataptr_guest);
	fclose(dataptr_guest);
	fclose(dataptr_event);
	
}

void List_Events()
{
	int tmp_int;
	FILE * dataptr_event;
	dataptr_event=fopen("event_details.txt","r");
	printf("Az esemenyek listaja a kovetkezo:\n");
	while(fread(&tmp_int,sizeof(tmp_int),1,dataptr_event)==1)
	{
		printf("%d id\n",tmp_int);		
	}
	fclose(dataptr_event);	
}

void New_Event()
{
	FILE * dataptr_event;
	int tmpid,tmp_event;
	
	
	printf("Kerem adja meg a letrehozando esemeny kodjat:");
	scanf(" %i",&tmpid);
	
	
	
	dataptr_event = fopen("event_details.txt","r+");
	
	
	
	while(fread(&tmp_event,sizeof(tmp_event),1,dataptr_event)==1)
	{
		if(tmpid==tmp_event)
		{
			printf("Az esemeny mar letezik az adatbazisban! A letrehozas meghiusult.");
			fclose(dataptr_event);
			return;
		}		
	}
	
	printf("%i", tmpid);
	fwrite(&tmpid,sizeof(tmpid),1,dataptr_event);
	printf("%i", tmpid);
	fclose(dataptr_event);
	printf("felvettuk az esemeny azonositojat! Koszonom!\n");
	
	
	
	
	
}

void List_Database()
{
	while (fgetc(stdin) != '\n') // Read until a newline is found
    {}
	FILE* dataptr_guest;
	int client_number;
	
	struct guest temp_guest;
	printf("Az adabazisban regisztraltak adatai: \n");
	dataptr_guest=fopen("guest_details.txt","r");
	fread(&client_number,sizeof(client_number),1,dataptr_guest);
	if(client_number==0)
	{
		printf("Meg nincsenek kliensek regisztralva! \n");
		return;
	}
	
	printf("%-20s%-30s%-20s%-40s\n","Vendeg neve","Vendeg email cime","Regisztralt esemeny","utolso modositas");
	while(fread(&temp_guest,sizeof(temp_guest),1,dataptr_guest)==1)
	{
		printf("%-20s%-30s%-20i%-40s\n",temp_guest.name,temp_guest.email,temp_guest.event_id,ctime(&temp_guest.registration_time));		
	}
	fclose(dataptr_guest);
	
	
	
	
	
	
}


void Delet_Registration()
{
	while (fgetc(stdin) != '\n') // Read until a newline is found
    {}
	
	FILE* dataptr_guest;
	FILE* dataptr_tmp;
	int client_number;
	char tmp_email[30];
	int tmp_id;
	struct guest temp_guest;
	
	
	dataptr_guest=fopen("guest_details.txt","r+");
	
	
	
	
	
	printf("Kerem adja meg a torlendo email cimet:");
	scanf(" %[^\n]s", tmp_email);
	
	printf("Kerem adja meg a torlendo esemeny jelentkezesnek az id szamat:");
	scanf(" %i", &tmp_id);
	
	fread(&client_number,sizeof(client_number),1,dataptr_guest);
	
	
	if(client_number==0)
	{
		printf("Nincs adat az adatbazisban! \n");
		fclose(dataptr_guest);
		return;
	}
	
	// temp file megnyitása
	dataptr_tmp=fopen("temp.txt","w+");
	fwrite(&client_number,sizeof(client_number),1,dataptr_tmp);
	
	
	
	
	//másolás kihagyással
	bool delete_flag=false;
	
	while(fread(&temp_guest,sizeof(temp_guest),1,dataptr_guest)==1)
	{
		if(   (  strcmp(tmp_email,temp_guest.email)==0) && (tmp_id==temp_guest.event_id) )
			
		 {
			 client_number=client_number-1;
			 delete_flag=true;
			 continue;
			 
		 }
		 else
		 {
			 fwrite(&temp_guest,sizeof(temp_guest),1,dataptr_tmp);
		 }		
	}
	
	int i;
	fclose(dataptr_guest);
	i=remove("guest_details.txt");
	
	if(i==0)
	{
		rename("temp.txt","guest_details.txt");
		//printf("Torles sikerult!!");
		
	}
	else
	{
		printf("Nem sikerult a torles");
		remove("temp.txt");
		
	}
	if(delete_flag)
		printf("Torlest megtortent\n");
	else
		printf("Nem tortent torles\n");
	
	fseek(dataptr_tmp,0,SEEK_SET);
	fwrite(&client_number,sizeof(client_number),1,dataptr_tmp);
	fclose(dataptr_tmp);
	
	
	
	
}


void Delete_Event()
{
	while (fgetc(stdin) != '\n') // Read until a newline is found
    {}
	
	FILE* dataptr_guest;
	FILE* dataptr_tmp;
	
	int client_number;
	int tmp_event;
	
	struct guest temp_guest;
	
	
	dataptr_guest=fopen("guest_details.txt","r+");	
	
	printf("Kerem adja meg a torlendo esemeny azonosito szamat:");
	scanf(" %i", &tmp_event);
	
	
	
	fread(&client_number,sizeof(client_number),1,dataptr_guest);
	
	
	if(client_number==0)
	{
		printf("Nincs adat az adatbazisban! \n");
		fclose(dataptr_guest);
		return;
	}
	
	// temp file megnyitása
	dataptr_tmp=fopen("temp.txt","w+");
	fwrite(&client_number,sizeof(client_number),1,dataptr_tmp);
		
	
	//másolás kihagyással
	bool delete_flag=false;
	
	while(fread(&temp_guest,sizeof(temp_guest),1,dataptr_guest)==1)
	{
		if  (  tmp_event==temp_guest.event_id )
			
		 {
			 client_number=client_number-1;
			 delete_flag=true;
			 continue;
			 
		 }
		 else
		 {
			 fwrite(&temp_guest,sizeof(temp_guest),1,dataptr_tmp);
		 }		
	}
	
	int i;
	fclose(dataptr_guest);
	i=remove("guest_details.txt");
	
	if(i==0)
	{
		rename("temp.txt","guest_details.txt");
		//printf("Torles sikerult!!");
		
	}
	else
	{
		printf("Nem sikerult a torles");
		remove("temp.txt");
		
	}
	
	
	fseek(dataptr_tmp,0,SEEK_SET);
	fwrite(&client_number,sizeof(client_number),1,dataptr_tmp);
	fclose(dataptr_tmp);
	
	
	//Innentõl az event fileból törlünk
	FILE * dataptr_event;
	//FILE * dataptr_tmp;
	int read_event;
	
	dataptr_event = fopen("event_details.txt","r+");
	dataptr_tmp = fopen("event_temp.txt","w+");
		
	while(fread(&read_event,sizeof(read_event),1,dataptr_event)==1)
	{
		if(read_event==tmp_event)			
		{			
			delete_flag=true;
			continue;
		}
		else
		{
			fwrite(&read_event,sizeof(read_event),1,dataptr_tmp);
		}
	}
	
	if(delete_flag)
		printf("Torlest megtortent\n");
	else
		printf("Nem tortent torles\n");
	
	
	//
	fclose(dataptr_event);
	i=remove("event_details.txt");
	
	if(i==0)
	{
		rename("event_temp.txt","event_details.txt");
		//printf("Torles sikerult!!");
		
	}
	else
	{
		printf("Nem sikerult a torles");
		remove("temp.txt");
		
	}
	fclose(dataptr_tmp);	
	
	
}


void Modify_data()
{

while (fgetc(stdin) != '\n') // Read until a newline is found
    {}    
	
	//Ebbe olvasunk a fájlból
	struct guest temp_struct;
	int reg_number;
	// Ezekbe olvasunk be a billentyûrõl
	char tmp_name[30];
	char tmp_email[30];
	int tmpid;
	// ido valt.
	time_t tmp_time;
	
	tmp_time=time(NULL);
	
	
	FILE *dataptr_guest;
	
	
	dataptr_guest=fopen("guest_details.txt","r+");
	printf("Kerem adja meg az email cimet amivel regisztralt: ");
	scanf(" %[^\n]s",tmp_email);
	
	printf ("Kerem adja meg az uj, fiokhoz tartozo nevet:");
	scanf(" %[^\n]s",tmp_name);
	fread(&reg_number,sizeof(reg_number),1,dataptr_guest);
	
	bool modified= false;
	
	while(fread(&temp_struct,sizeof(temp_struct),1,dataptr_guest)==1)
	{
		if(strcmp(tmp_email,temp_struct.email)==0)
		{
			modified = true;
			fseek(dataptr_guest,-sizeof(temp_struct),SEEK_CUR);
			fwrite(&tmp_name,sizeof(tmp_name),1,dataptr_guest);
			fseek(dataptr_guest,sizeof(tmp_name)+sizeof(tmpid),SEEK_CUR);
			tmp_time=time(NULL);
			fwrite(&tmp_time,sizeof(tmp_time),1,dataptr_guest);
		}		
	}
	if(modified)
		printf("Megtortent a modositas! \n");
	else
		printf("Nem volt modositas, nincs ilyen nev\n");
	
	fclose(dataptr_guest);
}





void Main_Menu()
{
	int Chosen_Menu;
	do{
	printf("A kovetkezo muveletek kozul valaszthat:\n\n");
	printf("1: Esemenyek listaja\n");
	printf("2: Regisztralas meglevo esemenyre\n");
	printf("3: Lejelentkezes egy esemenyrol\n");
	printf("4: Uj esemeny felvetele\n");
	printf("5: Az adatbazis listazasa\n");
	printf("6: Esemeny torlese\n");
	printf("7: Regisztraciohoz tartozo nev modositasa\n");
	printf("0: Kilepes\n\n");
	
	printf("Kerem adja meg a valasztott menu sorszamat:");
	
	scanf(" %i",&Chosen_Menu);
	switch(Chosen_Menu)
	{
		case 1:List_Events(); break;
		case 2:Registration(); break;
		case 3:Delet_Registration(); break;
		case 4:New_Event(); break;
		case 5:List_Database(); break;
		case 6:Delete_Event(); break;
		case 7:Modify_data();break;
		case 0:printf("Az alkalmazas leall...\n");				
	}
	
	
	
	}while(Chosen_Menu!=0);		
}





int main()
{
	Inicialize();
	Main_Menu();
	return (0);
 
}




