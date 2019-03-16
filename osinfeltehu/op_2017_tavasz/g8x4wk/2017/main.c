#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
//#include <sys/wait.h>
#include <signal.h>

struct Guest
{
	int id;
	char name[70];
	char email[70];
	int event_id;
	char time[100];
};

struct Event
{
	int id;
	char name[70];
	char location[100];
};

int nmbr_of_events();
int nmbr_of_guests();
void change_filename(char* oldname, char* newname);

void new_guest();
void guest_list();
void guest_update();
void guest_delete();

void new_event();
void event_delete();
void event_start_up();

void menu()
{
	int choice = -1;
    do {
        printf("Menu:\n");
        printf("Jelentkezok:\n");
        printf("	1. Listazas\n");
        printf("	2. Jelentkezes\n");
        printf("	3. Modositas\n");
        printf("	4. Torles\n");
        printf("Rendezveny:\n");
        printf("	5. Uj rendezveny\n");
        printf("	6. Torles\n");
		printf("Egyeb:\n");
		printf("	7. Rendezveny lebonyolítása\n");
        printf("	0. Kilepes\n");

        scanf("%d", &choice);
        switch(choice)
        {
            case 0 : exit(0);
            case 1 : guest_list();		break;
            case 2 : new_guest();   	break;
            case 3 : guest_update();	break;
            case 4 : guest_delete();	break;
            case 5 : new_event();		break;
            case 6 : event_delete();	break;
			case 7 : event_start_up();	break;
            default: break;
        }
    }while (choice>0 && choice<7);
}

int main()
{
	menu();
	return 0;
}

void new_guest()
{
	if (nmbr_of_events() == 0)
    {
        printf("Nincsen rendezveny az adatbazisban!\n");
        return;
    }

	struct Guest guest;
	 printf("Rendezveny szam: ");
    while (scanf("%d",&guest.event_id) != 1 || guest.event_id>nmbr_of_events() || guest.event_id<0 )
    {
        while (getchar() != '\n');
        printf("Hibas vagy nem letezo rendezveny szam: %d !\n",guest.event_id);
        printf("Rendezveny szam: ");
    }
    printf("Nev: ");
    scanf("%s", guest.name);
    while (fgetc(stdin) != '\n');
    printf("E-Mail: ");
    scanf("%s", guest.email);
    while (fgetc(stdin) != '\n');

    time_t tim;
    struct tm * timeinfo;
    time(&tim);
    timeinfo = localtime(&tim);
    strftime(guest.time, 80, "%c", timeinfo);

	guest.id=nmbr_of_guests()+1;

	FILE * file = fopen("guest.dat", "ab");
    if (file == NULL)
    {
        perror("Nem lehet megnyitni a fajlt!\n");
        return;
    }
    if (file != NULL)
    {
        fwrite(&guest,sizeof(guest),1, file);

        fclose(file);
    }


    printf("Az uj jelentkezo sorszama: %d\n", guest.id);
}

int nmbr_of_guests()
{
	int db = 0;
    FILE * f;
    f = fopen("guest.dat", "rb");
    if (f == NULL)
    {
        return 0;
    }
    struct Guest guest;
    while (fread( &guest, sizeof(guest), 1, f))
    {
        db++;
    }
    fclose(f);
    return db;
}

void guest_list()
{
    if (nmbr_of_guests() == 0)
    {
        printf("Nincsen jelentkezo az adatbazisban!\n");
        return;
    }

    printf("Jelentkezettek listazasa:");
	printf("\n| UserID | RendezvenyID |         Nev         |          Email          | Jelentkezes ideje |");

    printf("\n");
    FILE * f;
    f = fopen("guest.dat","rb");
    if (f == NULL)
    {
        perror("Nem lehet megnyitni a fajlt!");
        exit(1);
    }
    struct Guest guest;
    while (fread(&guest, sizeof(guest),1, f))
    {
        printf("| %-6d | %-12d | %-19s | %-23s | %s |\n", guest.id, guest.event_id, guest.name, guest.email,guest.time);
    }
    fclose(f);
	printf("\n");
}

void guest_update()
{
	if (nmbr_of_guests() == 0)
    {
        printf("Nincsen jelentkezo az adatbazisban!\n");
        return;
    }

    int updatedID = 0;

    printf("Jelentkezo modositasa:\n");
    printf("Sorszam: \n");

    while (scanf("%d",&updatedID) != 1 ||  updatedID>nmbr_of_guests() || updatedID<0)
    {
        while (getchar() != '\n');
        printf("Jelentkezo modositasa:\n");
        printf("Hibas ID: %d ! Kerem usse be ujra a sorszamot!\n",updatedID);
        printf("Sorszam: \n");
    }

    struct Guest updated;
    printf("Uj nev: ");
    scanf( " %[^\n]" , updated.name );
	printf("Jelentkezo modositasa:\n");
	printf("Sorszam: %d\n",updatedID);
	printf("Uj nev: %s\n",updated.name);
	printf("E-Mail: ");
	scanf( " %[^\n]" , updated.email );

    printf("Rendezveny szam: ");
    while (scanf("%d",&updated.event_id) != 1 || updated.event_id>nmbr_of_events() || updated.event_id<0 )
    {
        while (getchar() != '\n');
        printf("Hibas vagy nem letezo rendezveny szam: %d !\n",updated.event_id);
        printf("Rendezveny szam: ");
    }

    FILE * f;
    f = fopen("guest.dat", "rb");
    if (f == NULL)
    {
        perror("Nem lehet megnyitni a fajlt!");
        exit(1);
    }

    FILE * temp;
    temp = fopen("temp.dat", "wb");
    if (temp == NULL)
    {
        perror("Nem lehet megnyitni a fajlt!\n");
        exit(1);
    }
    struct Guest guest;
    int i = 0;
    while (fread(&guest, sizeof(guest), 1, f))
    {
        i++;
        if (i == updatedID)
        {
            updated.id = updatedID;
            strcpy(updated.time, guest.time);
            guest = updated;
        }
        fwrite(&guest, sizeof(guest), 1, temp);
    }
    fclose(f);
    fclose(temp);

	char oldname[] = "temp.dat";
	char newname[] = "guest.dat";

	change_filename(oldname, newname);
}

void change_filename(char* oldname, char* newname)
{
	int ret;

   char tempname1[] = "x.dat";

   remove(newname);
   ret = rename(oldname, newname);


   if(ret == 0 )
   {
      printf("Szerencsesen modosult a fajl! \n");
   }
   else
   {
      printf("Hiba tortent a modositas kozben! \n");
   }
}

void guest_delete()
{
    if (nmbr_of_guests() == 0)
    {
        printf("Nincsen jelentkezo az adatbazisban! \n");
        return;
    }

    int deleteID = 0;

    printf("Jelentkezo torlese:\n");
    printf("Sorszam: ");

    while (scanf("%d",&deleteID) != 1 || deleteID>nmbr_of_guests() || deleteID<0 )
    {
        while (getchar() != '\n');
        guest_list(false,true);
        printf("Hibas sorszam: %d !\n",deleteID);
        printf("sorszam: ");
    }

    FILE * f;
    f = fopen("guest.dat", "rb");
    if (f == NULL)
    {
        perror("Nem lehet megnyitni a fájlt!");
        exit(1);
    }

    FILE * temp;
    temp = fopen("temp.dat", "wb");
    if (temp == NULL)
    {
        perror("Nem lehet megnyitni a fájlt!");
        exit(1);
    }
    struct Guest guest;
    while (fread(&guest, sizeof(guest), 1, f))
    {
        if (guest.id != deleteID) fwrite(&guest, sizeof(guest), 1, temp);
    }
    fclose(f);
    fclose(temp);

	char oldname[] = "temp.dat";
	char newname[] = "guest.dat";

	change_filename(oldname, newname);
}

void new_event()
{
	struct Event event;
    printf("A rendezveny neve: ");
    scanf("%s", event.name);
	printf("A rendezveny helyszine: ");
    scanf("%s", event.location);

	event.id=nmbr_of_events()+1;

    FILE * file = fopen("event.dat", "ab");
    if (file == NULL)
    {
        perror("Nem lehet megnyitni a fajlt!\n");
        exit(1);
    }
    if (file != NULL)
    {
        fwrite( & event, sizeof(event), 1, file);
        fclose(file);
    }
    printf("Az uj rendezveny szama: %d\n", event.id);
}

int nmbr_of_events()
{
	int db = 0;
    FILE * f;
    f = fopen("event.dat", "rb");
    if (f == NULL)
    {
        return 0;
    }
    struct Event event;
    while (fread( &event, sizeof(event), 1, f))
    {
        db++;
    }
    fclose(f);
    return db;
}

void event_delete()
{
    if (nmbr_of_events() == 0)
    {
        printf("Nincsen rendezveny!\n");
        return;
    }

    int delete_eventID = 0;

    printf("Rendezveny torlese:\n");
    printf("Rendezveny sorszama: ");

    while (scanf("%d",&delete_eventID) != 1 || delete_eventID>nmbr_of_events() || delete_eventID<0)
    {
        while (getchar() != '\n');
        printf("Jelentkezo torlese:\n");
        printf("Hibas Rendezveny sorszam: %d\n",delete_eventID);
        printf("Rendezveny sorszama: ");
    }

    FILE * f;
    f = fopen("event.dat", "rb");
    if (f == NULL)
    {
        perror("Nem lehet megnyitni a fájlt!");
        exit(1);
    }

    FILE * temp;
    temp = fopen("temp.dat", "wb");
    if (temp == NULL)
    {
        perror("Nem lehet megnyitni a fájlt!");
        exit(1);
    }
    struct Event event;
    while (fread(&event, sizeof(event), 1, f))
    {
        if (event.id != delete_eventID) fwrite(&event, sizeof(event), 1, temp);
    }
    fclose(f);
    fclose(temp);

	char oldname[] = "temp.dat";
	char newname[] = "event.dat";
    change_filename("temp.dat","event.dat");
}

void handler()
{
	printf("Az adatok megérkeztek!");
}

void event_start_up()
{
	if(nmbr_of_events==0)
	{
		printf("Nincs rendezveny!");
		return;
	}

	struct Event event;
	printf("A rendezni kívánt esemány száma: ");
	while (scanf("%d",&event.id) != 1 || event.id>nmbr_of_events() || event.id<0 )
    {
        while (getchar() != '\n');
        printf("Hibas vagy nem letezo rendezveny szam: %d !\n",event.id);
        printf("Rendezveny szam: ");
    }

	FILE * f_r;
	struct Event Evnt;
    f_r = fopen("event.dat","rb");
    if (f_r == NULL)
    {
        perror("Nem lehet megnyitni a fajlt!");
        return;
    }
    while (fread(&Evnt, sizeof(Evnt),1, f_r))
    {
		if (Evnt.id == event.id)
		{
			event = Evnt;
		}
    }
    fclose(f_r);

	struct Guest gst;
	FILE * f_j;
    f_j = fopen("guest.dat","rb");
	int count = 0;
	while (fread( &gst, sizeof(gst), 1, f_j))
    {
		if(gst.event_id == event.id)
		{
			count++;
		}
    }
	fclose(f_j);

	if (count == 0)
	{
		printf("Nincsen jelentkezo a kivalasztott rendezvenyen!\n");
        return;
	}

	fopen("guest.dat","rb");
	struct Guest _guest[count];
	int i = 0;
	while (fread( &gst, sizeof(gst), 1, f_j))
    {
		if(gst.event_id == event.id)
		{
			_guest[i] = gst;
			i++;
		}
    }
	fclose(f_j);

	signal(SIGTERM,handler);

	int pipefd[2];
	if(pipe(pipefd)==-1)
	{
		perror("Pipe hiba keletkezett!");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();
	if(pid==-1)
	{
		perror("Fork hiba keletkezett!");
		exit(EXIT_FAILURE);
	}
	if (pid > 0)
    {
		printf("[K.O.S.] Rendezveny adatainak elkuldese.\n");
		printf("[K.O.S.] ID: %lu Helyszin: %s Nev: %s\n", event.id, event.location, event.name);
		write(pipefd[1], &event, sizeof(event));

		signal(SIGUSR1, handler);
		pause();
		signal(SIGUSR1, SIG_DFL);
		printf("[K.O.S.]  partnerunk megerkezett a helyszinre.\n");
		sleep(1);

		printf("[K.O.S.] A jelenkezok szamanak elkuldese\n");
		write(pipefd[1], &count, sizeof(count));

		i = 0;
		printf("[K.O.S.] Elkuldom a jelentkezok adatait!\n");
		for(i;i<count;i++)
		{
			write(pipefd[1], &_guest[i], sizeof(_guest[0]));
		}

		int status;
		waitpid(pid, &status,0);

		float _success;
		printf("[K.O.S.] Statisztikai adatok fogadasa.\n");
		read(pipefd[0], &_success, sizeof(_success));
		sleep(1);
		printf("[K.O.S.] A jelentkezettek %f szazaleka jelent meg a rendezvenyen\n", _success);

		int nmbr_of_not_appeared;
		printf("[K.O.S.] Fogadom a meg nem jelentek szamat.\n");
		read(pipefd[0], &nmbr_of_not_appeared, sizeof(nmbr_of_not_appeared));
		sleep(1);
		printf("[K.O.S.] Meg nem jelentek szama: %d.\n",nmbr_of_not_appeared);

		printf("[K.O.S.] Fogadom a meg nem jelentek adatait.\n");
		struct Guest _not_appeared[nmbr_of_not_appeared];
		i = 0;
		for(i; i<nmbr_of_not_appeared; i++)
		{
			read(pipefd[0], &_not_appeared[i], sizeof(gst));
		}
		sleep(1);
		printf("[K.O.S.] A meg nem jelentek adatait:\n");
		i = 0;
		for (i; i<nmbr_of_not_appeared;i++)
		{
			printf("[K.O.S.] %d %lu %s %s %s\n", _not_appeared[i].id, _not_appeared[i].event_id,
			_not_appeared[i].name, _not_appeared[i].email, _not_appeared[i].time);
		}
	}
	else
	{
		int parent = getppid();

		struct Event _event;
		printf("[Partner] Rendezveny adatainak fogadasa.\n");
		read(pipefd[0], &_event, sizeof(_event));
		sleep(1);
		printf("[Partner] %s helszinre utazok.\n",_event.location);
		sleep(1);
		printf("[Partner] Visszajelzes a K.O.S.-nek.\n");
		kill(parent , SIGUSR1);
		sleep(1);
		int nmbr_of_gst;
		printf("[Partner] Jelentkezok szamanak fogadasa.\n");
		sleep(1);
		read(pipefd[0], &nmbr_of_gst, sizeof(nmbr_of_gst));
		printf("[Partner] Jelentkezok szama: %d\n", nmbr_of_gst);

		struct Guest _guest[nmbr_of_gst];
		printf("[Partner] Jelentkezok adatainak fagadasa.\n");
		sleep(1);
		int i = 0;
		for (i; i<nmbr_of_gst;i++)
		{
			read(pipefd[0], &_guest[i], sizeof(_guest[i]));
		}

		printf("[Partner] A jelentkezok adatai.\n");
		i = 0;
		for (i; i<nmbr_of_gst;i++)
		{
			printf("[Partner] %d %lu %s %s %s\n", _guest[i].id, _guest[i].event_id, _guest[i].name,
			_guest[i].email, _guest[i].time);
		}

		printf("[Partner] Esemeny megkezdese.\n");

		int randomnumber[nmbr_of_gst];
		int _not_appeared = 0;
		int _appeared = 0;

		srand ( time(NULL) );
		i = 0;
		for(i;i<nmbr_of_gst;i++)
		{
			randomnumber[i] = rand() % 100;
			if (randomnumber[i] < 10)
			{
				_not_appeared++;
			}
			else
			{
				_appeared++;
			}
		}

		printf("[Partner] Megjelent vendegek: \n");

		struct Guest not_appeared[_not_appeared];
		i = 0;
		int not_appeared_i = 0;
		for (i; i<nmbr_of_gst;i++)
		{
			if(randomnumber[i] < 10)
			{
				not_appeared[not_appeared_i] = _guest[i];
				not_appeared_i++;
			}
			else
			{
				printf("[Partner] %s \n", _guest[i].name);
			}
		}

		float _success = ((double)(_appeared)/nmbr_of_gst)*100;
		printf("[Partner] Elkuldom a statisztika adatait!\n");
		write(pipefd[1], &_success, sizeof(_success));
		sleep(1);

		printf("[Partner] Elkuldom a meg nem jelentek szamat!\n");
		write(pipefd[1], &_not_appeared, sizeof(_not_appeared));

		printf("[Partner] Elkuldom a meg nem jelentek adatait!\n");
		i = 0;
		for (i; i<_not_appeared;i++)
		{
			write(pipefd[1], &not_appeared[i], sizeof(not_appeared[0]));
		}

		exit(0);
	}
	close(pipefd[0]);
	close(pipefd[1]);
}
