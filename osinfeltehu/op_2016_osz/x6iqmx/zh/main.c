#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h> // pause
#include <stdbool.h> // bool
#include <unistd.h>  // fork

struct jelentkezo
{
    int j_id;
    unsigned long int r_id;
    char nev[200];
    char email[200];
    char ido[100];
};

struct rendezveny
{
    char nev[200];
	char helyszin[200];
    unsigned long int r_id;
};

void j_registrate();
void j_list(bool pause, bool clear);
void j_edit();
void j_delete();
void j_swapfile(char* from_file, char* to_file);
int j_is_id_exists(int userID);
int j_next_id();
int j_getDB();

void r_registrate();
void r_delete();
void r_list(bool pause, bool clear);
void r_swapfile(char* from_file, char* to_file);
void r_run();
int r_is_id_exists(unsigned long int RendezvenyID);
int r_getDB();

int getDB();
int email_isvalid(const char *address);
void menu();
void console_clear();
void console_pause();
bool isdotdash ( const char c );

int main()
{
    menu();
    return 0;
}
void s_handler(int signal)
{
	printf("### [SIGNAL] Signal erkezett: %d.\n",signal);
}
void j_registrate()
{
    console_clear();

    struct jelentkezo j;

    if (r_getDB() == 0)
    {
        printf("!!! Nincsen rendezveny az adatbazisban\n");
        printf("!!! Nincsen rendezveny amibe jelentkezni lehetne\n");
        console_pause();
        return;
    }

    printf("Jelentkezes:");
    printf("\n--> Nev: ");
    scanf( " %[^\n]" , j.nev );
    bool first = true;
    do {
        console_clear();
        printf("Jelentkezes:\n");
        printf("--> Nev: %s\n",j.nev);
        if(!first && !email_isvalid(j.email))
            printf("!!! Hibas email cim: %s\n",j.email);
        printf("--> E-Mail: ");
        scanf( " %[^\n]" , j.email );
        first = false;
    } while (!email_isvalid(j.email));

    console_clear();
    printf("Jelentkezes:\n");
    printf("--> Nev: %s\n",j.nev);
    printf("--> E-mail: %s\n",j.email);
    printf("--> Rendezveny szam: ");
    while (scanf("%lu",&j.r_id) != 1 || !r_is_id_exists(j.r_id))
    {
        while (getchar() != '\n');
        r_list(false,true);
        printf("Jelentkezes:\n");
        printf("--> Nev: %s\n",j.nev);
        printf("--> E-mail: %s\n",j.email);
        printf("!!! Hibas vagy nem letezo rendezveny szam: %lu\n",j.r_id);
        printf("--> Rendezveny szam: ");
    }
    // ido
    time_t ido;
    struct tm * timeinfo;
    time(&ido);
    timeinfo = localtime(&ido);
    strftime(j.ido, 80, "%c", timeinfo);

    j.j_id = j_next_id();
    FILE * file = fopen("j_data.dat", "ab");
    if (file == NULL)
    {
        perror("!!! Nem lehet megnyitni a fajlt\n");
        return;
    }
    if (file != NULL)
    {
        fwrite(&j,sizeof(j),1, file);

        fclose(file);
    }
    console_clear();
    printf("--> Az uj jelentkezo sorszama: %d\n", j.j_id);
    j_list(true,false);

}
void j_list(bool pause, bool clear)
{
    if (clear) console_clear();

    if (j_getDB() == 0)
    {
        printf("!!! Nincsen jelentkezo az adatbazisban\n");
        console_pause();
        return;
    }

    printf("Jelentkezettek listazasa:");
    printf("\n+--------+--------------+---------------------+-------------------------+-------------------+");
    printf("\n| UserID | RendezvenyID |         Nev         |          Email          | Jelentkezes ideje |");
    printf("\n+--------+--------------+---------------------+-------------------------+-------------------+");

    printf("\n");
    FILE * f;
    f = fopen("j_data.dat","rb");
    if (f == NULL)
    {
        perror("!!! Nem lehet megnyitni a fajlt!");
        exit(1);
    }
    struct jelentkezo j;
    while (fread(&j, sizeof(j),1, f))
    {
        printf("| %-6d | %-12lu | %-19s | %-23s | %s |\n", j.j_id, j.r_id, j.nev, j.email,j.ido);
    }
    fclose(f);
    printf("+--------+--------------+---------------------+-------------------------+-------------------+\n");
    if (pause) console_pause();
}
void j_edit()
{
    if (j_getDB() == 0)
    {
        printf("!!! Nincsen jelentkezo az adatbazisban\n");
        console_pause();
        return;
    }

    j_list(false,true);

    int editID = 0;

    printf("Jelentkezo modositasa:");
    printf("\n--> UserID: ");

    while (scanf("%d",&editID) != 1 || !j_is_id_exists(editID))
    {
        while (getchar() != '\n');
        j_list(false,true);
        printf("Jelentkezo modositasa:\n");
        printf("!!! Hibas UserID: %d\n",editID);
        printf("--> UserID: ");
    }

    struct jelentkezo j_edited;
    printf("--> Uj nev: ");
    scanf( " %[^\n]" , j_edited.nev );
    bool first = true;
    do {
        j_list(false,true);
        printf("Jelentkezo modositasa:\n");
        printf("--> UserID: %d\n",editID);
        printf("--> Uj nev: %s\n",j_edited.nev);
        if(!first && !email_isvalid(j_edited.email)) printf("!!! Hibas email cim: %s\n", j_edited.email);
        printf("--> E-Mail: ");
        scanf( " %[^\n]" , j_edited.email );
        first = false;
    } while (!email_isvalid(j_edited.email));

    printf("--> Rendezveny szam: ");
    while (scanf("%lu",&j_edited.r_id) != 1 || !r_is_id_exists(j_edited.r_id))
    {
        while (getchar() != '\n');
        j_list(false,true);
        r_list(false,false);
        printf("Jelentkezo modositasa:\n");
        printf("--> Uj nev: %s\n",j_edited.nev);
        printf("--> E-Mail: %s\n",j_edited.email);
        printf("!!! Hibas vagy nem letezo rendezveny szam: %lu\n",j_edited.r_id);
        printf("--> Rendezveny szam: ");
    }

    FILE * f;
    f = fopen("j_data.dat", "rb");
    if (f == NULL)
    {
        perror("!!! Nem lehet megnyitni a fajlt!");
        exit(1);
    }

    FILE * temp;
    temp = fopen("temp.dat", "wb");
    if (temp == NULL) // még nincs fájl
    {
        perror("!!! Nem lehet megnyitni a fajlt!");
        exit(1);
    }
    struct jelentkezo j;
    int i = 0;
    while (fread(&j, sizeof(j), 1, f))
    {
        i++;
        if (i == editID)
        {
            j_edited.j_id = editID;
            strcpy(j_edited.ido, j.ido);
            j = j_edited;
        }
        fwrite(&j, sizeof(j), 1, temp);
    }
    fclose(f);
    fclose(temp);
    j_swapfile("temp.dat","j_data.dat");

    j_list(true,true);
}
void j_swapfile(char* from_file, char* to_file)
{
    FILE * to = fopen(to_file, "wb");
    if (to == NULL)
    {
        perror("!!! Nem lehet megnyitni a fájlt!");
        exit(1);
    }
    FILE * from = fopen(from_file, "rb");
    if (from == NULL)
    {
        perror("!!! Nem lehet megnyitni a fájlt!");
        exit(1);
    }
    struct jelentkezo j;
    while (fread(&j, sizeof(j), 1, from)) {
        fwrite(&j, sizeof(j), 1, to);
    }
    fclose(to);
    fclose(from);
    remove(from_file);
}
void j_delete()
{
    console_clear();
    if (j_getDB() == 0)
    {
        printf("!!! Nincsen jelentkezo az adatbazisban\n");
        console_pause();
        return;
    }

    j_list(false,true);

    int deleteID = 0;

    printf("Jelentkezo torlese:");
    printf("\n--> UserID: ");

    while (scanf("%d",&deleteID) != 1 || !j_is_id_exists(deleteID))
    {
        while (getchar() != '\n');
        j_list(false,true);
        printf("Jelentkezo torlese:\n");
        printf("!!! Hibas UserID: %d\n",deleteID);
        printf("--> UserID: ");
    }

    FILE * f;
    f = fopen("j_data.dat", "rb");
    if (f == NULL)
    {
        perror("!!! Nem lehet megnyitni a fájlt!");
        exit(1);
    }

    FILE * temp;
    temp = fopen("temp.dat", "wb");
    if (temp == NULL)
    {
        perror("!!! Nem lehet megnyitni a fájlt!");
        exit(1);
    }
    struct jelentkezo j;
    while (fread(&j, sizeof(j), 1, f))
    {
        if (j.j_id != deleteID) fwrite(&j, sizeof(j), 1, temp);
    }
    fclose(f);
    fclose(temp);
    j_swapfile("temp.dat","j_data.dat");

    console_clear();
    printf("--> Torolt jelentkezo: %d\n", deleteID);
    j_list(true,false);
}
int j_is_id_exists(int userID)
{
    FILE *f;
    f = fopen("j_data.dat", "rb");
    if (f == NULL)
    {
        perror("Nem lehet megnyitni a fájlt!");
        exit(1);
    }
    struct jelentkezo j;
    while (fread(&j, sizeof(j), 1, f))
    {
        if (j.j_id == userID) return true;
    }
    fclose(f);
    return false;
}
int j_next_id()
{
    int db = j_getDB();
    if (db == 0) return 1;

    int v[db];
    FILE * f;
    f = fopen("j_data.dat", "rb");
    if (f == NULL)
    {
        return 1;
    }
    struct jelentkezo j;
    int k = 0;
    while (fread(&j, sizeof(j), 1, f))
    {
        v[k] = j.j_id;
        k++;
    }
    int max = v[0];
    int i = 1;
    while(i< (int)( sizeof(v) / sizeof(v[0])))
    {
        if (v[i]>max) max = v[i];
        i++;
    }
    fclose(f);
    return max+1;
}
int j_getDB()
{
    int db = 0;
    FILE * f;
    f = fopen("j_data.dat", "rb");
    if (f == NULL)
    {
        return 0;
    }
    struct jelentkezo j;
    while (fread( &j, sizeof(j), 1, f))
    {
        db++;
    }
    fclose(f);
    return db;
}
void r_registrate()
{
    console_clear();

    printf("Uj rendezveny:");

    struct rendezveny r;
    printf("\n--> Rendezveny szam: ");
    while (scanf("%lu",&r.r_id) != 1 || r_is_id_exists(r.r_id) ) // TODO CHECK IF rendezvény iz DA:S_ EXISSTTTS
    {
        while (getchar() != '\n');
        console_clear();
        printf("Uj rendezveny:\n");
        printf("!!! Hibas rendezveny szam: %lu\n",r.r_id);
        printf("--> Rendezveny szam: ");
    }

    printf("--> Nev: ");
    scanf( " %[^\n]" , r.nev );
	printf("--> Helyszin: ");
	scanf( " %[^\n]" , r.helyszin );
	
    FILE * file = fopen("r_data.dat", "ab");
    if (file == NULL)
    {
        perror("Nem lehet megnyitni a fajlt\n");
        exit(1);
    }
    if (file != NULL)
    {
        fwrite( & r, sizeof(r), 1, file);
        fclose(file);
    }
    console_clear();
    printf("--> Az uj rendezveny szama: %d\n", r.r_id);
    r_list(true,false);
}
void r_list(bool pause, bool clear)
{
    if (clear) console_clear();

    if (r_getDB() == 0)
    {
        printf("!!! Nincsen rendezveny az adatbazisban\n");
        console_pause();
        return;
    }

    printf("Rendezvenyek listazasa:");
    printf("\n+--------------+-----------------------------------------------+------------------------+");
    printf("\n| RendezvenyID |                   Helyszin                    |           Nev          |");
    printf("\n+--------------+-----------------------------------------------+------------------------+");

    printf("\n");
    FILE * f;
    f = fopen("r_data.dat","rb");
    if (f == NULL)
    {
        perror("Nem lehet megnyitni a fajlt!");
        return; // ha nincs fájl csinálunk majd ezé ez opff
    }
    struct rendezveny r;
    while (fread(&r, sizeof(r),1, f))
    {
        printf("| %-12lu | %-45s | %-22s | \n", r.r_id, r.helyszin, r.nev);
    }
    fclose(f);
    printf("+--------------+-----------------------------------------------+------------------------+\n");
    if (pause) console_pause();
}
int r_is_id_exists(unsigned long int RendezvenyID)
{
    FILE *f;
    f = fopen("r_data.dat", "rb");
    if (f == NULL)
    {
        return false;
    }
    struct rendezveny r;
    while (fread(&r, sizeof(r), 1, f))
    {
        if (r.r_id == RendezvenyID) return true;
    }
    fclose(f);
    return false;
}
void r_delete()
{
    console_clear();
    if (r_getDB() == 0)
    {
        printf("!!! Nincsen rendezveny az adatbazisban\n");
        console_pause();
        return;
    }

    r_list(false,true);

    unsigned long int deleteID = 0;

    printf("Rendezveny torlese:");
    printf("\n--> RendezvenyID: ");

    while (scanf("%lu",&deleteID) != 1 || !r_is_id_exists(deleteID))
    {
        while (getchar() != '\n');
        r_list(false,true);
        printf("Jelentkezo torlese:\n");
        printf("!!! Hibas RendezvenyID: %lu\n",deleteID);
        printf("--> RendezvenyID: ");
    }

    FILE * f;
    f = fopen("r_data.dat", "rb");
    if (f == NULL)
    {
        perror("!!! Nem lehet megnyitni a fájlt!");
        exit(1);
    }

    FILE * temp;
    temp = fopen("temp.dat", "wb");
    if (temp == NULL)
    {
        perror("!!! Nem lehet megnyitni a fájlt!");
        exit(1);
    }
    struct rendezveny r;
    while (fread(&r, sizeof(r), 1, f))
    {
        if (r.r_id != deleteID) fwrite(&r, sizeof(r), 1, temp);
    }
    fclose(f);
    fclose(temp);
    r_swapfile("temp.dat","r_data.dat");

    console_clear();
    printf("--> Torolt rendezveny: %lu\n", deleteID);
    r_list(true,false);
}
void r_swapfile(char* from_file, char* to_file)
{
    FILE * to = fopen(to_file, "wb");
    if (to == NULL)
    {
        perror("!!! Nem lehet megnyitni a fájlt!");
        exit(1);
    }
    FILE * from = fopen(from_file, "rb");
    if (from == NULL)
    {
        perror("!!! Nem lehet megnyitni a fájlt!");
        exit(1);
    }
    struct rendezveny r;
    while (fread(&r, sizeof(r), 1, from)) {
        fwrite(&r, sizeof(r), 1, to);
    }
    fclose(to);
    fclose(from);
    remove(from_file);
}
int r_getDB()
{
    int db = 0;
    FILE * f;
    f = fopen("r_data.dat", "rb");
    if (f == NULL)
    {
        return 0;
    }
    struct rendezveny r;
    while (fread( &r, sizeof(r), 1, f))
    {
        db++;
    }
    fclose(f);
    return db;
}
void r_run()
{
	console_clear();
	if (r_getDB() == 0)
    {
        printf("!!! Nincsen rendezveny az adatbazisban\n");
        console_pause();
        return;
    }
	if (j_getDB() == 0)
    {
        printf("!!! Nincsen jelentkezo az adatbazisban\n");
        console_pause();
        return;
    }
	
	r_list(false,true);
	printf("Rendezveny inditasa:");
	
	struct rendezveny r;
	printf("\n--> Rendezveny szam: ");
    while (scanf("%lu",&r.r_id) != 1 || !r_is_id_exists(r.r_id) ) // TODO CHECK IF rendezvény iz DA:S_ EXISSTTTS
    {
        while (getchar() != '\n');
		r_list(false,true);
        printf("Rendezveny inditasa:\n");
        printf("!!! Hibas rendezveny szam: %lu\n",r.r_id);
        printf("--> Rendezveny szam: ");
    }

	// rendezveny adatai
	FILE * f_r;
	struct rendezveny tempr;
    f_r = fopen("r_data.dat","rb");
    if (f_r == NULL)
    {
        perror("Nem lehet megnyitni a fajlt!");
        return; // ha nincs fájl csinálunk majd ezé ez opff
    }
    while (fread(&tempr, sizeof(tempr),1, f_r))
    {
		if (tempr.r_id == r.r_id)
		{
			r = tempr;
		}
    }
    fclose(f_r);
	
	// jelentkezok adatai
	struct jelentkezo tempj;
	FILE * f_j;
    f_j = fopen("j_data.dat","rb");
	int count = 0; // megszamoljuk hany db van adott rendezvenyen
	while (fread( &tempj, sizeof(tempj), 1, f_j))
    {
		if(tempj.r_id == r.r_id)
		{
			count++;
		}
    }
	fclose(f_j);
	
	if (count == 0) // nincs senki az adott rendezvenyben
	{
		printf("!!! Nincsen jelentkezo a kivalasztott rendezvenyben\n");
        console_pause();
        return;
	}
	
	fopen("j_data.dat","rb");
	struct jelentkezo j[count]; 
	int index = 0;
	while (fread( &tempj, sizeof(tempj), 1, f_j))
    {
		if(tempj.r_id == r.r_id)
		{
			j[index] = tempj;
			index++;
		}
    }
	fclose(f_j);
	/*
	for(index;index<count;index++)
	{
		printf("| %-6d | %-12lu | %-19s | %-23s | %s |\n", j[index].j_id, j[index].r_id, j[index].nev, j[index].email, j[index].ido);
	}*/
	
    int pipefd[2];
	
	if (pipe(pipefd) == -1)
    {
      printf("!!! Nem lehet létrehozni a csövet\n");
      console_pause();
      return;
    }
	
	pid_t pid = fork();
	if (pid > 0) // szulo
    {
		// elkuldom a rendezvenyt
		printf("### [PARENT] Elkuldom a rendezveny adatait!\n");
		printf("### [PARENT] ID: %lu Helyszin: %s Nev: %s\n", r.r_id, r.helyszin, r.nev);
		write(pipefd[1], &r, sizeof(r));
		
		// megvarjuk h megerkezzen
		signal(SIGUSR1, s_handler);
		pause();
		signal(SIGUSR1, SIG_DFL);
		printf("### [PARENT] SIGNAL erkezett: {CHILD} megerkezett a helyszinre.\n");
		sleep(1);
		
		// latogatok kuldes
		printf("### [PARENT] Elkuldom a jelentkezok szamat!\n");
		write(pipefd[1], &count, sizeof(count));
		
		index = 0;
		printf("### [PARENT] Elkuldom a jelentkezok adatait!\n");
		for(index;index<count;index++)
		{
			write(pipefd[1], &j[index], sizeof(j[0]));
		}
		
		int status;
		waitpid(pid, &status,0);
		
		// sikeresseg
		float report_success;
		printf("### [PARENT] Fogadom a statisztika adatait.\n");
		read(pipefd[0], &report_success, sizeof(report_success));
		sleep(1);
		printf("### [PARENT] A jelentkezettek %f szazaleka jelent meg a rendezvenyen\n", report_success);
		
		// meg nem jelentek
		int nem_j_db;
		printf("### [PARENT] Fogadom a meg nem jelentek szamat.\n");
		read(pipefd[0], &nem_j_db, sizeof(nem_j_db));
		sleep(1);
		printf("### [PARENT] Meg nem jelentek szama: %d.\n",nem_j_db);
		
		printf("### [PARENT] Fogadom a meg nem jelentek adatait.\n");
		struct jelentkezo got_nem_j[nem_j_db];
		index = 0;
		for(index; index<nem_j_db; index++)
		{
			read(pipefd[0], &got_nem_j[index], sizeof(tempj));
		}
		sleep(1);
		printf("### [PARENT] A meg nem jelentek adatait:\n");
		index = 0;
		for (index; index<nem_j_db;index++)
		{
			printf("### [PARENT] %d %lu %s %s %s\n", got_nem_j[index].j_id, got_nem_j[index].r_id, got_nem_j[index].nev, got_nem_j[index].email, got_nem_j[index].ido);
		}
	}
	else
	{
		int parent = getppid();
		
		struct rendezveny got_rendezveny;		
		printf("### [CHILD] Fogadom a rendezveny adatait.\n");
		read(pipefd[0], &got_rendezveny, sizeof(got_rendezveny)); // megapom a rendezvenyt
		sleep(1);
		printf("### [CHILD] %s helszinre utazok.\n",got_rendezveny.helyszin);
		sleep(1);
		printf("### [CHILD] Jelentem a kozpontnak, hogy megerkeztem es keszen allok.\n");
		kill(parent , SIGUSR1);
		sleep(1);
		int j_db;
		printf("### [CHILD] Fogadom a jelentkezok szamat.\n");
		sleep(1);
		read(pipefd[0], &j_db, sizeof(j_db));
		printf("### [CHILD] Jelentkezok szama: %d\n", j_db);
		
		struct jelentkezo got_j[j_db]; 
		printf("### [CHILD] Fogadom a jelentkezok adatait.\n");
		sleep(1);
		int index = 0;
		for (index; index<j_db;index++)
		{
			read(pipefd[0], &got_j[index], sizeof(got_j[index]));
		}
		
		printf("### [CHILD] Fogadott a jelentkezok adatai.\n");
		index = 0;
		for (index; index<j_db;index++)
		{
			printf("### [CHILD] %d %lu %s %s %s\n", got_j[index].j_id, got_j[index].r_id, got_j[index].nev, got_j[index].email, got_j[index].ido);
		}
		
		printf("### [CHILD] Lebonyolitom az esemenyt.\n");
		
		int randomnumber[j_db];
		int notappeared = 0;
		int appeared = 0;
		
		srand ( time(NULL) );
		index = 0;
		for(index;index<j_db;index++)
		{
			randomnumber[index] = rand() % 10;
			//printf("RandomNumber = %d =?= 5\n", randomNumber);
			if (randomnumber[index] != 5)
			{
				notappeared++;
			}
			else
			{
				appeared++;
			}
		}
		
		// nem jottek el adatai
		struct jelentkezo nem_j[notappeared];
		index = 0;
		int nem_j_index = 0;
		for (index; index<j_db;index++)
		{
			if(randomnumber[index] != 5) // eljott
			{
				nem_j[nem_j_index] = got_j[index];
				nem_j_index++;
				//printf("### [CHILD] %s nem ellatogatott a rendezvenyre\n", got_j[index].nev);	
				//printf("### [NEM_CHILD] %s nem ellatogatott a rendezvenyre\n", nem_j[index].nev);	
			}
			else
			{
				//printf("### [CHILD] %s Nem jelent meg!\n", got_j[index].nev); // send to parent
			}
		}
		
		// sikeresseg kiszamitasa es kuldese
		float report_success = ((double)(appeared)/j_db)*100;
		printf("### [CHILD] Elkuldom a statisztika adatait!\n");
		write(pipefd[1], &report_success, sizeof(report_success));
		//printf("### [CHILD] A jelentkezettek %f  jelent meg a rendezvenyen\n", ((double)(appeared)/j_db)*100);
		sleep(1);
		
		// meg nem jelentek szama
		printf("### [CHILD] Elkuldom a meg nem jelentek szamat!\n");
		write(pipefd[1], &notappeared, sizeof(notappeared));
		
		// meg nem jelentek adatai
		printf("### [CHILD] Elkuldom a meg nem jelentek adatait!\n");
		index = 0;
		for (index; index<notappeared;index++)
		{
			//printf("### [CHILD] %d %lu %s %s %s\n", nem_j[index].j_id, nem_j[index].r_id, nem_j[index].nev, nem_j[index].email, nem_j[index].ido);
			write(pipefd[1], &nem_j[index], sizeof(nem_j[0]));
		}
		
		exit(0);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	console_pause();
}
void menu()
{
    int option = 0;
    do {
        console_clear();
        printf("Menu:");
        printf("\nJelentkezok:");
        printf("\n--> 1. Jelentkezes");
        printf("\n--> 2. Listazas");
        printf("\n--> 3. Modositas");
        printf("\n--> 4. Torles");
        printf("\nRendezveny:");
        printf("\n--> 5. Uj ");
        printf("\n--> 6. Listazas");
        printf("\n--> 7. Torles");
		printf("\n--> 8. Inditas");
        printf("\n--> 0. Kilepes");
        printf("\n>  ");

        scanf("%d", &option);
        switch(option)
        {
            case 0 : exit(0);
            case 1 : j_registrate();    break;
            case 2 : j_list(true,true);      break;
            case 3 : j_edit();          break;
            case 4 : j_delete();        break;
            case 5 : r_registrate();    break;
            case 6 : r_list(true,true);          break;
            case 7 : r_delete();        break;
			case 8 : r_run();	break;
            default: break;
        }
    }while (true);

}
void console_clear()
{
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}
void console_pause()
{
	printf("Irj be barmilyen karakter(sorozatot) a tovabbhaladashoz: ... \n");
	char tmp[20];
    scanf("%s",tmp);
}
// return TRUE if and only if c is a period or a dash.
bool isdotdash (const char c) // https://www.experts-exchange.com/questions/27831991/C-email-validation.html#answer38318460
{
  switch ( c )
  {
    case '.':
    case '-':
      return true;
  }
  return false;
} // isdotdash
int email_isvalid(const char *address) // https://www.experts-exchange.com/questions/27831991/C-email-validation.html#answer38318460
{
  int        count = 0;
  const char *c, *domain;
  static char *rfc822_specials = "()<>@,;:\\\"[]";

  c = address;

  // First character must be a letter.
  if ( ! isalpha ( *c ))
    return 0;

  for ( c = address + 1;  *c && *c != '@';  c++ )
  {
    // dot/dash check.  Consequtive illegal.  (--, .., .- and -. illegal)
    if ( isdotdash ( *c ))
    {
      if ( isdotdash ( *( c - 1 )))
        return 0;
    }
    // if not a dot/dash then only alphanumeric characters are legal.
    else if ( !isalnum( *c ))
      return 0;
  } // for

  // Make sure the current character is an @
  if ( *c != '@' )
    return 0;

  // Now make sure the character before the @ is a alphanumeric.
  if ( ! isalnum ( *(c - 1 )))
    return 0;


  /* next we validate the domain portion (name@domain) */
  if (!*(domain = ++c)) return 0;
  do
  {
    if (*c == '.')
    {
      if (c == domain || *(c - 1) == '.') return 0;
      count++;
    }
    if (*c <= ' ' || *c >= 127) return 0;
    if (strchr(rfc822_specials, *c)) return 0;
  } while (*++c);

  return (count >= 1);
} // spc_email_isvalid()
