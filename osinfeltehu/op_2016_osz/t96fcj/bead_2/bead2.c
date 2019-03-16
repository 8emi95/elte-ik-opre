#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <unistd.h>



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

/*
void get_event_guests(int event_num,)
{
	
	
	
}
*/

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

void handler(int signumber){
  printf("Uj uzenet! \n \n",signumber);
}

void start_event()
{
	while (fgetc(stdin) != '\n') // Read until a newline is found
    {}  
	int event_num;
	printf("Kerem adja meg az esemeny azonosito szamat:  \n");
	scanf("%i",&event_num);
	
	if (!(is_Exist_event(event_num)))
	{
		printf("Nincs ilyen esemeny!!");
		return;
	}
	int f=mkfifo("namedpipe",0600); ;
	// Csõvezetékhez...
	
	if (f<0){perror("error");exit(1);}
 
	struct timeval tv;
	tv.tv_sec=5; //the select will wait for operation at last for 10 seconds
	tv.tv_usec=0; //
	
	f=open("namedpipe",O_RDWR);
	fd_set watchedpipes_read, copy_watchedpipes_read;
	FD_ZERO(&watchedpipes_read); //group of file descriptors
	FD_SET(f,&watchedpipes_read); //watch for  input
	copy_watchedpipes_read=watchedpipes_read;
	//
	
	//Signalozáshoz
	signal(SIGUSR1,handler);
	pid_t parent=getpid();
	//
	
	pid_t  child=fork();
	if (child<0){perror("The fork calling was not succesful\n"); printf("Problema a gyerek indulasaval!! \n"); return;} 
	if (child>0) //the parent process, it can see the returning value of fork - the child variable!
 {
	 struct guest temp_guest;
	 char place[30]="kukutyin";
 printf("fonok: Megirom a fiamnak, hogy az esemeny %s -ban lesz \n ",place);
	sleep(1);
	 write(f,&place,sizeof(place));
	 pause();
	 printf("fonok: Megjott az uzenet a fiamtol! Azt mondja odaert az esemeny helyszinere! \n");
	 printf("fonok: Elkuldom a neki az esemenyhez tartozo vendegek nevet!\n");
	 sleep(2);
	 
	 FILE *dataptr_guest;
	 dataptr_guest=fopen("guest_details.txt","r+");
	 fseek(dataptr_guest,sizeof(int),SEEK_CUR);
	 int number_of_client=0;
	 while(fread(&temp_guest,sizeof(temp_guest),1,dataptr_guest)==1)
	{
		if(temp_guest.event_id==event_num)
		{
			printf("fonok: Kuldom ezt a nevet: %s \n", temp_guest.name);
			write(f,&temp_guest.name,sizeof(temp_guest.name));
			number_of_client++;
			
		}		
	}
	printf("fonok :Na mostmar csak megkell varnom amig a fiam visszaszol! \n");
	pause();
	printf("fonok: Szoltak, hogy vege az esemenynek!\n");
	select(f+1,&watchedpipes_read,NULL,NULL,NULL);
	int success;
	if (FD_ISSET(f,&watchedpipes_read))
	{
		read(f,&success,sizeof(int));
	}
	printf("fonok: A fiam azt mondta a rendezveny sikeressege %d szazalek lett\n ",success);
	printf("fonok: Akik nem jottek el: \n");
	char missing_one[30];
	int number_of_missing_client=0;
	for(;;)
	 {
			tv.tv_sec = 5;
			tv.tv_usec = 0;
			watchedpipes_read=copy_watchedpipes_read;
			int result=select(f+1,&watchedpipes_read,NULL,NULL,&tv);
			if (FD_ISSET(f,&watchedpipes_read)) 
			  { 
				read(f,&missing_one,30);
				printf("fonok: Kaptam egy nevet: %s\n",missing_one);
				number_of_missing_client++;
				
				
			                           
			  } 
			else
			{
				printf("Na hat ennyien ...\n");
				printf("Tehat %i vendegbol / %i itt volt! \n",number_of_client,number_of_client-number_of_missing_client);
				break;
			}
	}
	
	
	fflush(stdout);
	fclose(dataptr_guest);
	
	int status;
    waitpid(child,&status,0); 
	unlink("namedpipe");
    //waits the end of child process PID number=child, the returning value will be in status
    //0 means, it really waits for the end of child process - the same as wait(&status)
    
 
 }
 else //child process
 {
	 char place[30];
	select(f+1,&watchedpipes_read,NULL,NULL,NULL);
	if (FD_ISSET(f,&watchedpipes_read))
	{
		read(f,&place,sizeof(place));
	} 
 printf("gyerek: Szoval az esemeny %s -ban lesz \n",place);
	
	 struct timeval tv;
	 char guest_names[10][30];
	 int g_numb=0;
	 printf("gyerek: Elindultam az esemenyre!! \n Eltart egy darabig... (3 sec)\n");
	 sleep(3);
	 printf("gyerek: Megerkeztem a helyszinre!!! Ertesitem errol a fonokot! \n");
	 kill(parent,SIGUSR1);
	 for(;;)
	 {
			tv.tv_sec = 10;
			tv.tv_usec = 0;
			watchedpipes_read=copy_watchedpipes_read;
			int result=select(f+1,&watchedpipes_read,NULL,NULL,&tv);
			if (FD_ISSET(f,&watchedpipes_read)) 
			  { 
				read(f,&guest_names[g_numb],30);
				printf("gyerek: Kaptam egy nevet: %s\n",guest_names[g_numb]);
				g_numb++;
				
			                           
			  } 
			else
			{
				break;
			}
	}
	
	printf("gyerek: Ugy latszik %i emberre szamithatok.. \n",g_numb);
	
	printf("gyerek: Elindult az esemeny! \n");
	printf("Ennyi ido mulva veget er: 3 sec\n");
	sleep(1);
	printf("Ennyi ido mulva veget er: 2 sec\n");
	sleep(1);
	printf("Ennyi ido mulva veget er: 1 sec\n");
	sleep(1);
	
	
	printf("gyerek: Beszamolok az eredmenyrol a fonoknek! \n");
	kill(parent,SIGUSR1);
	srand(time(NULL));
	int r=rand()%100;
	write(f,&r,sizeof(r));	
	int i;
	for(i=0;i<g_numb;i++)
	{
			srand(time(NULL)+i);
			
			r = rand()%10;
			if(r==0)
				write(f,&guest_names[i],30);		
	}
	
	
	
	kill(getpid(),SIGKILL);
	
 }
		 
		 
		 
	 
	 
	 
 
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
	printf("8: Esemeny elinditasa\n");
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
		case 8:start_event();break;
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




