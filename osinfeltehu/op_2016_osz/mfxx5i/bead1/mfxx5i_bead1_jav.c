#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //ctime
#include <fcntl.h> //open,creat
#include <dirent.h> //opendir, readdir
#include <sys/types.h> //opendir
#include <sys/stat.h> //stat
#include <unistd.h>   //stat

//Tengely Alex MFXX5I
//OPREG 1. beadando
//2016.11.02.
//Javitas: 2016.11.07. (guest_edit ervenytelen kapcsolo hibajelzes)

void list_guests(char* event)
{
	FILE * f;
	char filename[50], line[160], *str_split;
	int cols_cnt, lines_cnt, i;
	printf("Latogatok listaja (rendezveny: %s)\nAzon Nev                     Mail                    Idopont         \n", event);
	
	strcpy(filename, "./"); strcat(filename, event); strcat(filename, ".txt");
	f=fopen(filename,"r");
	if (f==NULL)
	{
		printf("Nincs ilyen azonositoju rendezveny!\n");
		exit(1);
	}
	
	fgets(line,sizeof(line),f);
	lines_cnt = atoi(line);
	
	for (i=0; i<lines_cnt; i=i+1)
	{
		fgets(line,sizeof(line),f);
		str_split = strtok(line, "\t\n");
		for (cols_cnt=0; cols_cnt<3; cols_cnt=cols_cnt+1)
		{
			if (cols_cnt==0) {printf("%-5s", str_split);}
			else {printf("%-24s", str_split);}
			str_split = strtok(NULL, "\t\n");
		}
		time_t t = atoi(str_split);
		printf("%s", ctime(&t));
	}
	
	printf("Latogatok szama: %i\n", lines_cnt);
	fclose(f);
}

void guest_registration(char* name, char* mail, char* event)
{
	FILE *f, *tmp;
	time_t t = time(0);
	char filename[50], line[160], newline[200], int_str[35], *str_split;
	int cols_cnt, lines_cnt, i, new_guest_number;
	
	strcpy(filename, "./"); strcat(filename, event); strcat(filename, ".txt");
	f=fopen(filename,"r");
	if (f==NULL)
	{
		printf("Nincs ilyen azonositoju rendezveny!\n");
		exit(1);
	}
	tmp=fopen("./tmp.txt","w");
	fgets(line,sizeof(line),f);
	lines_cnt = atoi(line);
	sprintf(int_str, "%d", lines_cnt+1);
	fprintf(tmp, int_str); fprintf(tmp, "\n");
	
	for (i=0; i<lines_cnt; i=i+1)
	{
		fgets(line,sizeof(line),f);
		fprintf(tmp, line);
		if (i==lines_cnt-1)
		{
			str_split = strtok(line, "\t\n");
			new_guest_number = atoi(str_split)+1;
		}
	}
	
	if (lines_cnt==0) {new_guest_number=1;}
	
	sprintf(int_str, "%d", new_guest_number);
	strcpy(newline, int_str); strcat(newline, "\t");
	strcat(newline, name); strcat(newline, "\t");
	strcat(newline, mail); strcat(newline, "\t");
	sprintf(int_str, "%d", t);
	strcat(newline, int_str); strcat(newline, "\n");
	fprintf(tmp, newline);
	
	fclose(f); fclose(tmp);
	
	f=fopen(filename,"w");
	tmp=fopen("./tmp.txt","r");
	
	for (i=0; i<lines_cnt+2; i=i+1)
	{
		fgets(line,sizeof(line),tmp);
		fprintf(f, line);
	}
	
	fclose(f); fclose(tmp);
	remove("./tmp.txt");
	
	printf("Latogato regisztralasa sikerult (%i.)\n", lines_cnt+1);
}

void guest_edit(char* event, char* number, char* which_to_edit, char* new_value)
{
	if (strcmp(which_to_edit, "-n") && strcmp(which_to_edit, "-m"))
	{
		printf("Ervenytelen kapcsolo (látogató melyik adata legyen módosítva)!\n");
		exit(1);
	}
	FILE *f, *tmp;
	char filename[50], line[160], line_copy[160], *str_split;
	int lines_cnt, i, found;
	
	strcpy(filename, "./"); strcat(filename, event); strcat(filename, ".txt");
	f=fopen(filename,"r");
	if (f==NULL)
	{
		printf("Nincs ilyen azonositoju rendezveny!\n");
		exit(1);
	}
	tmp=fopen("./tmp.txt","w");
	fgets(line,sizeof(line),f);
	lines_cnt = atoi(line);
	fprintf(tmp, line);
	
	found = 0;
	
	for (i=0; i<lines_cnt; i=i+1)
	{
		fgets(line,sizeof(line),f);
		strcpy(line_copy, line);;
		str_split = strtok(line, "\t\n");
		if (!strcmp(str_split, number))
		{
			found = 1;
			fprintf(tmp, str_split);
			fprintf(tmp, "\t");
			str_split = strtok(NULL, "\t\n");
			if (!strcmp(which_to_edit,"-n")) {fprintf(tmp, new_value);}
			else {fprintf(tmp, str_split);}
			fprintf(tmp, "\t");
			
			str_split = strtok(NULL, "\t\n");
			if (!strcmp(which_to_edit,"-m")) {fprintf(tmp, new_value);}
			else {fprintf(tmp, str_split);}
			fprintf(tmp, "\t");
			
			str_split = strtok(NULL, "\t\n");
			fprintf(tmp, str_split); fprintf(tmp, "\n");
		}
		else
		{
			fprintf(tmp, line_copy);
		}
	}
	
	if (!found) {printf("Nincs ilyen azonositoju vendeg!\n");}
	fclose(f); fclose(tmp);
	
	f=fopen(filename,"w");
	tmp=fopen("./tmp.txt","r");
	
	for (i=0; i<lines_cnt+1; i=i+1)
	{
		fgets(line,sizeof(line),tmp);
		fprintf(f, line);
	}
	
	fclose(f); fclose(tmp);
	remove("./tmp.txt");
	
	if (found) {printf("Latogato adatainak modositasa sikerult\n");}
}

void guest_delete(char* event, char* number)
{
	FILE *f, *tmp;
	char filename[50], line[160], line_copy[160], int_str[35], *str_split;
	int lines_cnt, i, found;
	
	strcpy(filename, "./"); strcat(filename, event); strcat(filename, ".txt");
	f=fopen(filename,"r");
	if (f==NULL)
	{
		printf("Nincs ilyen azonositoju rendezveny!\n");
		exit(1);
	}
	tmp=fopen("./tmp.txt","w");
	fgets(line,sizeof(line),f);
	lines_cnt = atoi(line);
	if(lines_cnt<1)
	{
		printf("Nincs ilyen azonositoju vendeg!\n");
		exit(1);
	}
	
	found = 0;
	
	for (i=0; i<lines_cnt; i=i+1)
	{
		fgets(line,sizeof(line),f);
		strcpy(line_copy, line);;
		str_split = strtok(line, "\t\n");
		if (!strcmp(str_split, number)) {found = 1; break;}
	}
	
	fseek(f,0,SEEK_SET);
	fgets(line,sizeof(line),f);
	
	if (found)
	{
		sprintf(int_str, "%d", lines_cnt-1);
		fprintf(tmp, int_str); fprintf(tmp, "\n");
	}
	else
	{
		fclose(f);
		fclose(tmp);
		printf("Nincs ilyen azonositoju vendeg!\n");
		exit(1);
	}
	
	for (i=0; i<lines_cnt; i=i+1)
	{
		fgets(line,sizeof(line),f);
		strcpy(line_copy, line);
		str_split = strtok(line, "\t\n");
		if (strcmp(str_split, number)) {fprintf(tmp, line_copy);}
	}
	fclose(f); fclose(tmp);
	
	f=fopen(filename,"w");
	tmp=fopen("./tmp.txt","r");
	
	for (i=0; i<lines_cnt; i=i+1)
	{
		fgets(line,sizeof(line),tmp);
		fprintf(f, line);
	}
	
	fclose(f); fclose(tmp);
	remove("./tmp.txt");
	
	if (found) {printf("Latogato adatainak torlese sikerult\n");}
}

void list_events()
{
	char filename[25], *str_split;
	DIR* d;
	struct dirent * dp; //pointer to dirent struct
	struct stat st;
	d=opendir("."); //parameter the directory name (with path) we want to open
	if (d<0){perror("Some problem in opening");exit(1);}
	printf("Rendezvenyek kilistazasa\n");
	while (dp=readdir(d))
	{
		stat(dp->d_name,&st);  //fstat(file descriptor, stat structure)
		if (S_ISDIR(st.st_mode))
		{
			continue;
		}
		//printf("%s\n",dp->d_name);
		strcpy(filename, dp->d_name);
		str_split = strtok(filename, ".\n");
		str_split = strtok(NULL, ".\n");
		if(!strcmp(str_split, "txt")) {printf("%s\n",filename);}
	}
	closedir(d); //close directory after opendir
	return;
}

void event_registration(char *event)
{
	FILE *f;
	char filename[50];
	strcpy(filename, "./"); strcat(filename, event); strcat(filename, ".txt");
	f=fopen(filename,"w");
	if (f==NULL)
	{
		printf("Hiba a rendezveny letrehozasakor!\n");
		exit(1);
	}
	fprintf(f, "0\n");
	fclose(f);
	printf("Rendezveny letrehozasa sikeres\n");
}

void event_delete(char *event)
{
	FILE *f;
	char filename[50];
	strcpy(filename, "./"); strcat(filename, event); strcat(filename, ".txt");
	f=fopen(filename,"r");
	if (f==NULL)
	{
		printf("Nincs ilyen azonositoju rendezveny!\n");
		exit(1);
	}
	fclose(f);
	remove(filename);
	printf("Rendezveny adatainak torlese sikeres\n");
}

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
	if (argc>1)
	{
		if (!strcmp("-l", argv[1]) || !strcmp("--list-guests", argv[1]))
		{
			if(argc<3)
			{
				printf("Keves argumentum!\n");
				return 1;
			}
			else
			{
				list_guests(argv[2]);
			}
		}
		else if (!strcmp("-r", argv[1]) || !strcmp("--guest-registration", argv[1]))
		{
			if(argc<5)
			{
				printf("Keves argumentum!\n");
				return 1;
			}
			else
			{
				guest_registration(argv[2], argv[3], argv[4]);
			}
		}
		else if (!strcmp("-e", argv[1]) || !strcmp("--guest-edit", argv[1]))
		{
			if(argc<6)
			{
				printf("Keves argumentum!\n");
				return 1;
			}
			else
			{
				guest_edit(argv[2], argv[3], argv[4], argv[5]);
			}
		}
		else if (!strcmp("-d", argv[1]) || !strcmp("--guest-delete", argv[1]))
		{
			if(argc<4)
			{
				printf("Keves argumentum!\n");
				return 1;
			}
			else
			{
				guest_delete(argv[2], argv[3]);
			}
		}
		else if (!strcmp("-L", argv[1]) || !strcmp("--list-events", argv[1]))
		{
			list_events();
		}
		else if (!strcmp("-R", argv[1]) || !strcmp("--event-registration", argv[1]))
		{
			if(argc<3)
			{
				printf("Keves argumentum!\n");
				return 1;
			}
			else
			{
				event_registration(argv[2]);
			}
		}
		else if (!strcmp("-D", argv[1]) || !strcmp("--event-delete", argv[1]))
		{
			if(argc<3)
			{
				printf("Keves argumentum!\n");
				return 1;
			}
			else
			{
				event_delete(argv[2]);
			}
		}
		else
		{
			printf("Ervenytelen kapcsolo!\n");
		}
	}
	else if (argc=1)
	{
		printf("Latogatoi nyilvantartas\nHasznalat:\n");
		printf("\t%s\n\t\tUtmutato megjelenitese\n\n", argv[0]);
		printf("\t%s {-l|--list-guests} {<rendezveny azonositoja>}\n\t\tAdott rendezvenyre jelentkezett latogatok kilistazasa adataikkal: sorszam, nev, mail cim, jelentkezesi ido\n\n", argv[0]);
		printf("\t%s {-r|--guest-registration} {<nev>} {<mail cim>} {<rendezveny azonositoja>}\n\t\tLatogato regisztralasa\n\n", argv[0]);
		printf("\t%s {-e|--guest-edit} {<rendezveny azonositoja>} {<vendeg sorszama>} {-n <nev>|-m <mail cim>}\n\t\tLatogato adatainak modositasa\n\n", argv[0]);
		printf("\t%s {-d|--guest-delete} {<rendezveny azonositoja>} {<vendeg sorszama>}\n\t\tLatogato adatainak torlese\n\n", argv[0]);
		printf("\t%s {-L|--list-events}\n\t\tRendezvenyek kilistazasa\n\n", argv[0]);
		printf("\t%s {-R|--event-registration} {<rendezveny azonositoja>}\n\t\tRendezveny regisztralasa\n\n", argv[0]);
		printf("\t%s {-D|--event-delete} {<rendezveny azonositoja>}\n\t\tRendezveny adatainak torlese\n", argv[0]);
	}
	return 0;
}
