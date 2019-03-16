#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

//rendezvény adatai
struct event
{
	char id  [100];			//rendezvény azonosítója
	char name[100];			//rendezvény neve
};

//vendég adatai
struct guest
{
	char name    [100];		//vendég neve
	char email   [100];		//vendég email címe
	char event_id[100];		//rendezvény azonosító
	char date    [100];		//jelentkezés dátuma
};

//a menürendszer szövege
void menu()
{
	printf("\n---------------------------------------------------\n\nKérem, válasszon az alábbi menüpontok közül:\n\n");
	printf("1. Új jelentkezés\n");
	printf("2. Jelentkezés módosítása\n");
	printf("3. Jelentkezés törlése\n");
	printf("4. Új rendezvény rögzítése\n");
	printf("5. Rendezvény résztvevőinek listázása\n");
	printf("6. Rendezvény törlése\n");
	printf("7. Kilépés");
	printf("\n\nKérem, adja meg a menüpont sorszámát: ");
}

//adattároló fájlok ellenőrzése (létrejönnek, ha még nem léteznek)
void check_files(const char * event_fname, const char * guest_fname)
{
	int event, guest;
	event = open(event_fname,O_CREAT,S_IRUSR|S_IWUSR);
	guest = open(guest_fname,O_CREAT,S_IRUSR|S_IWUSR);
	close(event);
	close(guest);
}

//menüpont számának bekérése
int scan_menupoint()
{
	int i;
	int success;
	int length;
	char c;
	
	do{
		success = scanf("%i",&i);
		length = 0;
		
		//input kitakarítása
		while((c = getchar()) != '\n' && c != EOF)
		{
			if(c != ' ' && c != '\t')
			{
				length++;
			}
		}
		
		if(success != 1 || length != 0 || i < 1 || i > 7)
		{
			printf("\nHIBA! 1 és 7 közötti egész számot adjon meg: ");
		}
		
	}while(success != 1 || length != 0 || i < 1 || i > 7);
	
	return i;
}

//rendezvény meglétének ellenőrzése (azonosító alapján)
int check_event(struct event e, const char * fname)
{
	struct event chk;
	char s[250];
	FILE * f;
	char * token;
	int check = 0;
	
	f = fopen(fname,"r");
	
	while(fgets(s,sizeof(char)*(1 + 100 * 2),f))
	{
		read(f,&s,sizeof(s));
		token = strtok(s, "|");
		strcpy(chk.id,token);
		token = strtok(NULL, "|");
		strcpy(chk.name,token);
		
		if(strcmp(e.id,chk.id) == 0)
		{
			check++;
		}
	};
	
	fclose(f);
	
	return check; //0-t ad vissza, ha nincs egyezés
}

//vendég meglétének ellenőrzése (email cím és rendezvény azonosító alapján)
int check_guest(struct guest g, const char * fname)
{
	struct guest chk;
	char s[500];
	FILE * f;
	char * token;
	int check = 0;
	
	f = fopen(fname,"r");
	
	while(fgets(s,sizeof(char)*(3 + 100 * 4),f))
	{
		read(f,&s,sizeof(s));
		token = strtok(s, "|");
		strcpy(chk.name,token);
		token = strtok(NULL, "|");
		strcpy(chk.email,token);
		token = strtok(NULL, "|");
		strcpy(chk.event_id,token);
		token = strtok(NULL, "|");
		strcpy(chk.date,token);
		
		if(strcmp(g.email,chk.email) == 0 && strcmp(g.event_id,chk.event_id) == 0)
		{
			check++;
		}
	};
	
	fclose(f);
	
	return check; //0-t ad vissza, ha nincs egyezés
}

//rendezvény adatainak bekérése
struct event scan_event()
{
	struct event e;
	int success;
	int length;
	char c;
	
	//azonosító bekérése
	printf("\nKérem, adja meg a rendezvény azonosítóját: ");
	do{
		success = scanf("%s",e.id);
		length = 0;
		
		//input kitakarítása
		while((c = getchar()) != '\n' && c != EOF)
		{
			if(c != ' ' && c != '\t')
			{
				length++;
			}
		}
		
		if(success != 1 || length != 0)
		{
			printf("\nHIBA! Egybefüggő azonosítót adjon meg: ");
		}
		
	}while(success != 1 || length != 0);
	
	//név bekérése
	printf("\nKérem, adja meg a rendezvény nevét: ");
	do{
		success = scanf("%[^\n]s",e.name);
		
		//input kitakarítása
		while((c = getchar()) != '\n' && c != EOF)
		{
			
		}
		
	}while(success != 1);
	
	return e;
}

//vendég adatainak bekérése
struct guest scan_guest()
{
	struct guest g;
	int success;
	int length;
	char c;
	time_t current_time;
	struct tm * time_info;
	
	//név bekérése
	printf("\nKérem, adja meg a vendég nevét: ");
	do{
		success = scanf("%[^\n]s",g.name);
		
		//input kitakarítása
		while((c = getchar()) != '\n' && c != EOF)
		{
			
		}
		
	}while(success != 1);
	
	//email cím bekérése
	printf("\nKérem, adja meg a vendég email címét: ");
	do{
		success = scanf("%s",g.email);
		length = 0;
		
		//input kitakarítása
		while((c = getchar()) != '\n' && c != EOF)
		{
			if(c != ' ' && c != '\t')
			{
				length++;
			}
		}
		
		if(success != 1 || length != 0)
		{
			printf("\nHIBA! Valódi email címet adjon meg: ");
		}
		
	}while(success != 1 || length != 0);
	
	//rendezvény azonosító bekérése
	printf("\nKérem, adja meg a rendezvény azonosítóját: ");
	do{
		success = scanf("%s",g.event_id);
		length = 0;
		
		//input kitakarítása
		while((c = getchar()) != '\n' && c != EOF)
		{
			if(c != ' ' && c != '\t')
			{
				length++;
			}
		}
		
		if(success != 1 || length != 0)
		{
			printf("\nHIBA! Egybefüggő azonosítót adjon meg: ");
		}
		
	}while(success != 1 || length != 0);
		
	//jelentkezés dátuma
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(g.date, 20, "%Y.%m.%d %H:%M:%S", time_info);
	
	return g;
}

//esemény adatainak fájlba írása
void write_event(struct event e, const char * fname)
{
	char * s = (char *) malloc(sizeof(char) * (1 + strlen(e.id) + strlen(e.name)));
	strcpy(s,e.id);
    strcat(s,"|");
	strcat(s,e.name);
	
	FILE * f;
	f = fopen(fname,"a");
	fputs(s,f);
	fputs("\n",f);
	
	fclose(f);
}

//vendég adatainak fájlba írása
void write_guest(struct guest g, const char * fname)
{
	char * s = (char *) malloc(sizeof(char) * (3 + strlen(g.name) + strlen(g.email) + strlen(g.event_id) + strlen(g.date)));
	strcpy(s,g.name);
    strcat(s,"|");
	strcat(s,g.email);
	strcat(s,"|");
	strcat(s,g.event_id);
	strcat(s,"|");
	strcat(s,g.date);
	
	FILE * f;
	f = fopen(fname,"a");
	fputs(s,f);
	fputs("\n",f);
	
	fclose(f);
}

 //rendezvény vendégeinek listázása
void list_guest(struct event e, const char * fname)
{
	struct guest chk;
	char s[500];
	FILE * f;
	char * token;
	int check = 0;
	
	f = fopen(fname,"r");
	
	while(fgets(s,sizeof(char)*(3 + 100 * 4),f))
	{
		read(f,&s,sizeof(s));
		token = strtok(s, "|");
		strcpy(chk.name,token);
		token = strtok(NULL, "|");
		strcpy(chk.email,token);
		token = strtok(NULL, "|");
		strcpy(chk.event_id,token);
		token = strtok(NULL, "|");
		strcpy(chk.date,token);
		
		if(strcmp(e.id,chk.event_id) == 0)
		{
			check++;
			printf("\n%i. jelentkező:",check);
			printf("\nNév: %s\nEmail cím: %s\nJelentkezés időpontja: %s",chk.name,chk.email,chk.date);
		}
	};
	
	if(check == 0)
	{
		printf("\nA rendezvényre még nem történt jelentkezés.");
	}
	
	fclose(f);
}

//rendezvényre vendégeinek száma
int count_guest(struct event e, const char * fname)
{
	struct guest chk;
	char s[500];
	FILE * f;
	char * token;
	int cnt = 0;
	
	f = fopen(fname,"r");
	
	while(fgets(s,sizeof(char)*(3 + 100 * 4),f))
	{
		read(f,&s,sizeof(s));
		token = strtok(s, "|");
		strcpy(chk.name,token);
		token = strtok(NULL, "|");
		strcpy(chk.email,token);
		token = strtok(NULL, "|");
		strcpy(chk.event_id,token);
		token = strtok(NULL, "|");
		strcpy(chk.date,token);
		
		if(strcmp(e.id,chk.event_id) == 0)
		{
			cnt++;
		}
	};
	
	fclose(f);
	
	return cnt;
}

//rendezvény törlése
void delete_event(struct event e, const char * fname)
{
	struct event tmp;
	char s[250];
	char p[250];
	FILE * f;
	FILE * h;
	char * token;
	int t;
	const char tmp_file[100] = "tmp_event.txt";
	
	//ideiglenes fájl létrehozása
	t = open(tmp_file,O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	close(t);
	
	f = fopen(fname,"a+");
	h = fopen(tmp_file,"a+");	
	
	while(fgets(s,sizeof(char)*(1 + 100 * 2),f))
	{
		read(f,&s,sizeof(s));
		strcpy(p,s);
		token = strtok(s, "|");
		strcpy(tmp.id,token);
		token = strtok(NULL, "|");
		strcpy(tmp.name,token);
		
		//a törlendő sort nem másolja át
		if(strcmp(e.id,tmp.id) != 0)
		{
			fputs(p,h);
		}
	};
	
	fclose(f);
	fclose(h);
	
	remove(fname);
	rename(tmp_file,fname);

}

//vendég törlése
void delete_guest(struct guest g, const char * fname)
{
	struct guest tmp;
	char s[500];
	char p[500];
	FILE * f;
	FILE * h;
	char * token;
	int t;
	const char tmp_file[100] = "tmp_guest.txt";
	
	//ideiglenes fájl létrehozása
	t = open(tmp_file,O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	close(t);
	
	f = fopen(fname,"a+");
	h = fopen(tmp_file,"a+");	
	
	while(fgets(s,sizeof(char)*(3 + 100 * 4),f))
	{
		read(f,&s,sizeof(s));
		strcpy(p,s);
		token = strtok(s, "|");
		strcpy(tmp.name,token);
		token = strtok(NULL, "|");
		strcpy(tmp.email,token);
		token = strtok(NULL, "|");
		strcpy(tmp.event_id,token);
		token = strtok(NULL, "|");
		strcpy(tmp.date,token);
		
		//a törlendő sort nem másolja át
		if(!(strcmp(g.email,tmp.email) == 0 && strcmp(g.event_id,tmp.event_id) == 0))
		{
			fputs(p,h);
		}
	};
	
	fclose(f);
	fclose(h);
	
	remove(fname);
	rename(tmp_file,fname);
}

//törölt rendezvény vendégeinek törlése
void delete_eventguest(struct event e, const char * fname)
{
	struct guest tmp;
	char s[500];
	char p[500];
	FILE * f;
	FILE * h;
	char * token;
	int t;
	const char tmp_file[100] = "tmp_guest.txt";
	
	//ideiglenes fájl létrehozása
	t = open(tmp_file,O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	close(t);
	
	f = fopen(fname,"a+");
	h = fopen(tmp_file,"a+");	
	
	while(fgets(s,sizeof(char)*(3 + 100 * 4),f))
	{
		read(f,&s,sizeof(s));
		strcpy(p,s);
		token = strtok(s, "|");
		strcpy(tmp.name,token);
		token = strtok(NULL, "|");
		strcpy(tmp.email,token);
		token = strtok(NULL, "|");
		strcpy(tmp.event_id,token);
		token = strtok(NULL, "|");
		strcpy(tmp.date,token);
		
		//a törlendő sort nem másolja át
		if(!(strcmp(e.id,tmp.event_id) ==0))
		{
			fputs(p,h);
		}
	};
	
	fclose(f);
	fclose(h);
	
	remove(fname);
	rename(tmp_file,fname);
}





int main(int argc,char ** argv)
{
	const char * event_fname = "event.txt";
	const char * guest_fname = "guest.txt";
	int menupoint;
	int check;
	int cnt;
	struct event e;
	struct guest g;
	struct guest m;
	
	system("clear");
	check_files(event_fname,guest_fname);
	printf("\n---------------------------------------------------\nLátogatói nyilvántartás - KING OF STANDS");
	
	do {
		menu();
		menupoint = scan_menupoint();
		
		switch (menupoint) {
			
			case 1:
				//Új jelentkezés
				g = scan_guest();
				strcpy(e.id,g.event_id);
				strcpy(e.name,"X");
				check = check_event(e,event_fname);
				if(check == 0)
				{
					printf("\nHIBA! A megadott rendezvény azonosító nem létezik. Csak meglévő eseményre lehet jelentkezni.");
				}else
				{
					check = check_guest(g,guest_fname);
					if(check != 0)
					{
						printf("\nHIBA! A megadott email címmel már történt jelentkezés a megadott rendezvényre.");
					}else{
						write_guest(g,guest_fname);
						cnt = count_guest(e,guest_fname);
						printf("\nAz új jelentkezés a rendezvényre a %i. helyen megtörtént.",cnt);
					}
				}
				break;
				
			case 2:
				//Jelentkezés módosítása
				printf("\nKérem, adja meg a módosítandó jelentkezés adatait!\n");
				g = scan_guest();
				check = check_guest(g,guest_fname);
				if(check == 0)
				{
					printf("\nHIBA! A megadott email címmel még nem történt jelentkezés a megadott rendezvényre.");
				}else
				{
					printf("\nKérem, adja meg a jelentkezés módosított adatait!\n");
					m = scan_guest();
					strcpy(e.id,m.event_id);
					strcpy(e.name,"X");
					check = check_event(e,event_fname);
					if(check == 0)
					{
						printf("\nHIBA! A megadott rendezvény azonosító nem létezik.");
					}else
					{
						delete_guest(g,guest_fname);
						write_guest(m,guest_fname);
						printf("\nA jelentkezés módosítása megtörtént.");
					}
				}
				break;
				
			case 3:
				//Jelentkezés törlése
				g = scan_guest();
				check = check_guest(g,guest_fname);
				if(check == 0)
				{
					printf("\nHIBA! A megadott email címmel még nem történt jelentkezés a megadott rendezvényre.");
				}else
				{
					delete_guest(g,guest_fname);
					printf("\nA jelentkezés törlése megtörtént.");
				}
				
				break;
				
			case 4:
				//Új rendezvény felvitele
				e = scan_event();
				check = check_event(e,event_fname);
				if(check != 0)
				{
					printf("\nHIBA! A megadott rendezvény azonosító már létezik.");
				}else
				{
					write_event(e,event_fname);
					printf("\nAz új rendezvény rögzítése megtörtént.");
				}
				
				break;
				
			case 5:
				//Rendezvény jelentkezőinek listázása
				e = scan_event();
				check = check_event(e,event_fname);
				if(check == 0)
				{
					printf("\nHIBA! A megadott rendezvény azonosító nem létezik.");
				}else
				{
					printf("\nA rendezvényre az alábbi jelentkezések történtek:\n");
					list_guest(e,guest_fname);
				}
				break;
				
			case 6:
				//Rendezvény törlése
				e = scan_event();
				check = check_event(e,event_fname);
				if(check == 0)
				{
					printf("\nHIBA! A megadott rendezvény azonosító nem létezik.");
				}else
				{
					delete_event(e,event_fname);
					delete_eventguest(e,guest_fname);
					printf("\nA rendezvény és résztvevőinek törlése megtörtént.");
				}
				break;
				
			case 7:
				printf("\nA program kilép.\n\n");
				break;
				
			default:
				break;
		}
		
	}while(menupoint != 7);
	
	return 0;
}
