//Mikus Márk - Oprendszerek.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>   
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h> // for errno, the number of last error


#define _XOPEN_SOURCE 500
#define default_size 100
#define ext_def_size 200
#define SUCCESS 0;
#define ERROR -1;



int jarat_count = 0;

int jelentkezo_count = 0;

int celallomas_count = 0;

struct jarat {
	int id;
	int celallomas_id;
	time_t indulas;
};

struct jelentkezo {
	int id;
	char nev [ext_def_size];
	time_t jelentkezesi_ido;
	int jarat_id;
	char telefonszam [ext_def_size];
	
};

struct celallomas {
	int id;
	char nev [ext_def_size];
};

struct jarat jaratok[default_size];

struct jelentkezo jelentkezok [default_size];

struct celallomas celallomasok [default_size];

struct celallomas celallomas_beolvas_userinput() {
	struct celallomas temp;
	temp.id = celallomasok[celallomas_count-1].id+1;
	printf("Adja meg a celallomas nevet");
	scanf("%s",&temp.nev);
	return temp;
}

struct jarat jarat_beolvas_userinput()
{
	struct jarat temp;
	temp.id = jaratok[jarat_count-1].id+1;
	printf("Adja meg a jarat celallomasanak azonositojat");
	scanf("%i",&temp.celallomas_id);
	printf("Adja meg a jarat indulasanak datumat");
	char timstr [default_size];
	scanf("%s",&timstr);
	struct tm t;
	strptime(&timstr, "%Y-%m-%d,%H:%M", &t);
	time_t t2 = mktime(&t);
	temp.indulas = t2;
	return temp;
}

struct jelentkezo jelentkezo_beolvas_userinput() {
	struct jelentkezo jel;
	jel.id = jelentkezok[jelentkezo_count-1].id + 1;
	printf("nev:");
	char nev1 [ext_def_size];
	char nev2 [default_size];
	scanf("%s %s",&nev1, &nev2);
	strcat(nev1," ");
	strcat(nev1,nev2);
	strcpy(jel.nev,nev1);
	printf("jarat azonositoja:");
	scanf(" %i",&jel.jarat_id);
	printf("telefonszam:");
	scanf(" %s",&jel.telefonszam);
	return jel;
}

void celallomas_lista() {
	printf("\nAllomasok adatai\n");
	int i;
	for (i = 0; i < celallomas_count; ++i)
	{
		printf("Aallomas adatai:\n");
		
		printf("id: %i\nnev: ", celallomasok[i].id);
		puts(celallomasok[i].nev);
		printf("\n--------------\n\n");
	}
}

void celallomas_uj(struct celallomas cel) {
	celallomasok[celallomas_count].id = cel.id;
	strcpy(celallomasok[celallomas_count].nev,cel.nev);
	celallomas_count++;
}

int celallomas_torol(int id) {
	int i;
	for (i = 0; i < celallomas_count; ++i)
	{
		if(celallomasok[i].id == id) {
			celallomasok[i].id = -1;
			return SUCCESS;
		}
	}
	return ERROR;
}

int celallomas_index_by_id(int id) {
	int i;
	for (i = 0; i < celallomas_count; ++i)
	{
		if(celallomasok[i].id == id) {
			return i;
		}
	}
	return ERROR;
}

void jarat_info(int id) {	
	int i;
	for (i = 0; i<jarat_count; ++i)
	{
		if(jaratok[i].id == id) {
			break;
		}
	}
	char buff[20];
	time_t now = jaratok[i].indulas;
	strftime(buff, 20, "%Y-%m-%d,%H:%M", localtime(&now));
	printf("A jarat adatai:\njaratazonosito: %i\ncelallomas: %s\nindulas ideje: %s\n",
			jaratok[i].id,
			celallomasok[celallomas_index_by_id(jaratok[i].celallomas_id)].nev, 
			buff);
	struct jelentkezo tempjel [default_size];
	int count = 0;
	int j;
	for (j = 0; j < jelentkezo_count;j++)
	{
		if(jelentkezok[j].jarat_id == id)
		{
			tempjel[count] = jelentkezok[j];
			count++;
		}
	}
	printf("\n\nA jarat utasai:\n");
	for (j = 0; j < count; ++j)
	{
		printf("\n_____\n\n");
		char cel [ext_def_size];
		char buff[20];
		time_t now = jelentkezok[j].jelentkezesi_ido;
		strftime(buff, 20, "%Y-%m-%d%H:%M:%S", localtime(&now));
		printf("azonosito:%i\nnev:%s\njelentkezes ideje:%s\njarat azonositoja:%i\ncelallomas:%s\ntelefonszam:%s\n",
			jelentkezok[j].id,
			jelentkezok[j].nev, 
			buff,
			jelentkezok[j].jarat_id,
			celallomasok[celallomas_index_by_id(jaratok[jarat_index_by_id(jelentkezok[j].jarat_id)].celallomas_id)].nev,
			jelentkezok[j].telefonszam);
		
	}
	printf("\n\n\n");
	
}

void jarat_lista() {
	printf("\nJaratok\n\n");
	int i;
	for (i = 0; i < jarat_count; i++)
	{
		jarat_info(jaratok[i].id);
	}
}

void jarat_uj(struct jarat jar) {
	
	jaratok[jarat_count].id = jar.id;
	jaratok[jarat_count].celallomas_id = jar.celallomas_id;
	jaratok[jarat_count].indulas = jar.indulas;
	jarat_count++;
}

int jarat_torol(int id) {
	int i = jarat_index_by_id(id);
	if(i < 0)
	{
		perror("\nNincs ilyen azonositoju jarat\n");
		return ERROR;
	}
	jaratok[i].id = -1;
	return SUCCESS;
}

int jarat_index_by_id(int id) {
	int i;
	for (i = 0; i < jarat_count; ++i)
	{
		if(jaratok[i].id == id)
		{
			return i;
		}
	}
	return ERROR;
}

void jelentkezo_lista() {
	printf("\nJelentkezokn\n");
	int i;
	for (i = 0; i < jelentkezo_count; ++i)
	{
		char cel [ext_def_size];
		char buff[20];
		time_t now = jelentkezok[i].jelentkezesi_ido;
		strftime(buff, 20, "%Y-%m-%d%H:%M:%S", localtime(&now));
		printf("azonosito:%i\nnev:%s\njelentkezes ideje:%s\njarat azonositoja:%i\ncelallomas:%s\ntelefonszam:%s\n",
			jelentkezok[i].id,
			jelentkezok[i].nev, 
			buff,
			jelentkezok[i].jarat_id,
			celallomasok[celallomas_index_by_id(jaratok[jarat_index_by_id(jelentkezok[i].jarat_id)].celallomas_id)].nev,
			jelentkezok[i].telefonszam);
		printf("\n\n\n");
	}
}

void jelentkezo_rogzit(struct jelentkezo jel) {
	time_t timer;
	jelentkezok[jelentkezo_count].id = jel.id;
	strcpy(jelentkezok[jelentkezo_count].nev, jel.nev);
	strcpy(jelentkezok[jelentkezo_count].telefonszam,jel.telefonszam);
	jelentkezok[jelentkezo_count].jarat_id = jel.jarat_id;
	jelentkezok[jelentkezo_count].jelentkezesi_ido = time(NULL);
	
	jelentkezo_count++;
}

int jelentkezo_modosit(int id, struct jelentkezo jel,int set_time) {
	time_t timer;
	int i = jelentkezo_index_by_id(id);
	if(i < 0) {
		perror("nincs ilyen azonositoju jelentkezo");
		return ERROR;
	}
	jelentkezok[i].id = jel.id;
	strcpy(jelentkezok[i].nev, jel.nev);
	jelentkezok[i].jarat_id = jel.jarat_id;
	if(set_time == 1) {
		jelentkezok[i].jelentkezesi_ido = jel.jelentkezesi_ido;//time(&timer);
	} else {
		jelentkezok[i].jelentkezesi_ido = time(&timer);
	}
	strcpy(jelentkezok[i].telefonszam,jel.telefonszam);
	return SUCCESS;
}

int jelentkezo_torol(int id) {
	int i = jelentkezo_index_by_id(id);
	if(i < 0) {
		perror("nincs ilyen azonositoju jelentkezo");
		return ERROR;
	}
	for (i = 0; i < jelentkezo_count; ++i)
	{
		if(jelentkezok[i].id == id)
		{
			jelentkezok[i].id = -1;
			break;
		}
	}
	return SUCCESS;
}

int jelentkezo_index_by_id(int id) {
	int i;
	for (i = 0; i < jelentkezo_count; ++i)
	{
		if(jelentkezok[i].id == id)
		{
			return i;
		}
	}
	return ERROR;
}

void jelentkezo_modosit_userinput() {
	printf("Kerem adja meg a modositani kivant jelentkezo azonositojat\n");
	int id;
	scanf("%i",id);
	printf("Kerem adja meg az uj adatokat\n");
	struct jelentkezo jel;
	printf("nev:");
	scanf("%s",jel.nev);
	printf("jarat azonositoja:");
	scanf("%i",jel.jarat_id);
	printf("telefonszam:");
	scanf("s",jel.telefonszam);
	jelentkezo_modosit(id,jel,0);
}

void ment_mindent() {
	int status = remove("jelentkezok");
	if(status < 0) {perror("error while saving jelentkezok"); exit(1);}
	int f_jel = open("jelentkezok",O_WRONLY|O_CREAT,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (f_jel<0){ perror("Error at saving jelentkezok\n");exit(1);}
	int i;
	for (i = 0; i < jelentkezo_count; i++)
	{
		if(jelentkezok[i].id >= 0) 
		{
			write(f_jel,&jelentkezok[i],sizeof(jelentkezok[i]));
		}
	}
	close(f_jel);
	
	status = remove("jaratok");
	if(status < 0) {perror("error while saving jaratok"); exit(1);}
	 // jaratok beolvasasa
	int f_jar = open("jaratok",O_WRONLY|O_CREAT,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (f_jar<0){ perror("Error at saving jaratok\n");exit(1);}
	for (i = 0; i < jarat_count; i++)
	{
		if(jaratok[i].id >= 0) 
		{
			write(f_jar,&jaratok[i],sizeof(jaratok[i]));
		}
	}
	close(f_jar);
	 
	 status = remove("celallomasok");
	 if(status < 0) {perror("error while saving celallomasok"); exit(1);}
	//celallomasok kiirasa
	int f_cel = open("celallomasok",O_WRONLY|O_CREAT,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (f_cel<0){ perror("Error at saving celallomasok\n");exit(1);}
	for (i = 0; i < celallomas_count; i++)
	{
		if(celallomasok[i].id >= 0) 
		{
			write(f_cel,&celallomasok[i],sizeof(celallomasok[i]));
		}
	}
	close(f_cel);
}

void beolvas() {
	// jelentkezok beolvasas
	int f_jel = open("jelentkezok",O_RDONLY );
	if (f_jel<0){ perror("Error at opening jelentkezok\n");exit(1);}
	struct jelentkezo jel;
	
	jelentkezo_count = 0;
	while (read(f_jel,&jel,sizeof(jel))) {
		//printf("olvasott id:%i,olvasott nev: %s\n\n",jel.id,jel.nev);
		jelentkezo_rogzit(jel);
	}
	close(f_jel);
	 // jaratok beolvasasa
	int f_jar = open("jaratok",O_RDONLY);
	if (f_jar<0){ perror("Error at opening jaratok\n");exit(1);}
	struct jarat jar;
	jarat_count = 0;
	while (read(f_jar,&jar,sizeof(jar)))
	{
		jarat_uj(jar);
	}
	close(f_jar);
	//celallomasok beolvasasa
	
	int f_cel = open("celallomasok",O_RDONLY);
	if (f_cel<0){ perror("Error at opening celallomasok\n");exit(1);}
	struct celallomas cel;
	celallomas_count = 0;
	while (read(f_cel,&cel,sizeof(cel)))
	{
		celallomas_uj(cel);
	}
	close(f_cel);
 } 
	
void tesztadatok_rogzit() {
	time_t timer;
	struct celallomas cel;
	cel.id = 0;
	char cel_nev1 [ext_def_size]= "Krakkó";
	//strncpy(cel.nev, cel_nev1, ext_def_size);
	strcpy(cel.nev,"Krakkó");
	celallomas_uj(cel);
	
	struct jarat jar;
	jar.id = 0;
	jar.celallomas_id = 0;
	const char *timstr = "2013-03-21 20:25";
	struct tm t;
	strptime(timstr, "%Y-%m-%d,%H:%M", &t);
	//printf("az ev:%i\n a honap:%i", t.tm_year,t.tm_mon);
	time_t t2 = mktime(&t);
	//printf("%i",t2);
	jar.indulas = t2;
	jarat_uj(jar);
	
	struct jelentkezo ujjel;
	ujjel.id = jelentkezo_count+1;
	//char nev [ext_def_size]= "NOLL PÉTER";
	//strncpy(ujjel.nev, nev, ext_def_size);
	//ujjel.nev = nev;
	strcpy(ujjel.nev,"Noll Péter");
	strcpy(ujjel.telefonszam,"06309437969");
	ujjel.jarat_id = 0;
	//puts(ujjel.nev);
	ujjel.jelentkezesi_ido = time(NULL);
	
	
	jelentkezo_rogzit(ujjel);
	
	
	ment_mindent();
	
}

void submenu_Jaratok(int mainmenu) {
	
	int submenuchoice = 1;

	printf("Jaratok\n");
	printf("1: Listazas\n");
	printf("2: Uj felvetele\n");
	
	printf("3: Torles\n");
	printf("4: vissza\n");
	
	scanf(" %i",&submenuchoice);
	switch(submenuchoice)
	{
		struct jarat temp;
		case 1:
		{
			jarat_lista();
		}break;
		case 2:
		{
			temp = jarat_beolvas_userinput();
			jarat_uj(temp);
			
		}break;
		case 3:
		{
			int id;
			printf("Kerem adja meg a torlendo jarat azonositojat:");
			scanf(" %i",&id);
			jarat_torol(id);
		}break;
		case 4:
		{
			//go = 0;
		}break;
		default:
		{
			printf("Ervenytelen menupont");
		}break;
		
	}
}

void submenu_celallomasok() {
	printf("Celallomasok\n");
	printf("1 - Listazas\n");
	printf("2 - Uj felvetele\n");
	printf("3 - Torles\n");
	printf("4 - Vissza\n");
	int choice;
	scanf("%i", &choice);
	switch(choice)
	{
		struct celallomas temp;
		case 1:
		{
			celallomas_lista();
		}break;
		case 2:
		{
			temp = celallomas_beolvas_userinput();
			celallomas_uj(temp);
			
		}break;
		case 3:
		{
			int id;
			printf("Kerem adja meg a torlendo celallomas azonositojat:");
			scanf(" %i",&id);
			celallomas_torol(id);
		}break;
		case 4:
		{
			
		}break;
		default:
		{
			printf("Ervenytelen menupont");
		}break;
	}
	
}

void submenu_jelentkezok() {
	printf("Celallomasok\n");
	printf("1 - Listazas\n");
	printf("2 - Uj felvetele\n");
	printf("3 - Modositas\n");
	printf("4 - Torles\n");
	printf("5 - Vissza\n");
	int submenuchoice;
	scanf("%i",&submenuchoice);
	switch(submenuchoice)
	{
		struct jelentkezo temp;
		case 1:
		{
			jelentkezo_lista();
		}break;
		case 2:
		{
			temp = jelentkezo_beolvas_userinput();
			jelentkezo_rogzit(temp);
		}break;
		case 3:
		{
			int id;
			printf("Kerem adja meg a modositando jelentkezo azonositojat:");
			scanf(" %i",&id);
			temp = jelentkezo_beolvas_userinput();
			jelentkezo_modosit(id,temp,0);
		}break;
		case 4:
		{
			int id;
			printf("Kerem adja meg a torlendo jelentkezo azonositojat:");
			scanf(" %i",&id);
			jelentkezo_torol(id);
		}break;
		case 5:
		{
			//go = 0;
		}break;
		default:
		{
			printf("Ervenytelen menupont");
			//go = 0;
		}break;
		
	}
	
}

void f() {
	int pid,fd;
    printf("Fifo start!\n");
    int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); // creating named pipe file
    // S_IWGRP, S_IROTH (other jog), file permission mode
    // the file name: fifo.ftc
    // the real fifo.ftc permission is: mode & ~umask 
    if (fid==-1)
    {
	printf("Error number: %i",errno);
	exit(EXIT_FAILURE);
    }
    printf("Mkfifo vege, fork indul!\n");
    pid = fork();
    
    if(pid>0)   //parent 
	{
	char s[1024]="Semmi";		
	printf("Csonyitas eredmenye szuloben: %d!\n",fid);
	fd=open("fifo.ftc",O_RDONLY);
	read(fd,s,sizeof(s));
	printf("Ezt olvastam a csobol: %s \n",s);
	close(fd);
	// remove fifo.ftc
	unlink("fifo.ftc");
        }
	else // child
	{
	printf("Gyerek vagyok, irok a csobe!\n");
	printf("Csonyitas eredmenye: %d!\n",fid);
        fd=open("fifo.ftc",O_WRONLY);
        write(fd,"Hajra Fradi!\n",12);
        close(fd);
	printf("Gyerek vagyok, beirtam, vegeztem!\n");
    }	
}

void menu() {
	int run = 1;
	
	while (run == 1)
	{
		printf("1 - Jaratok\n");
		printf("2 - Celallomasok\n");
		printf("3 - Jelentkezok\n");
		printf("4 - Mentes\n");
		printf("5 - Nevcso teszt\n");
		printf("6 - Kilepes\n");
		int choice;
		scanf(" %i",&choice);
		switch(choice)
		{
			case 1:
			{
				submenu_Jaratok(1);
			} break;
			case 2:
			{
				submenu_celallomasok();
			} break;
			case 3:
			{
				submenu_jelentkezok();
			} break;
			case 4:
			{
				ment_mindent();
				beolvas();
			} break;
			case 5:
			{
				f();
			} break;
			case 6:
			{
				return;
			}break;
			default:
			{
				printf("hiba");
				return;
			} break;
		}
	}
	
}

int main() {
    beolvas();
	
	menu();
	
    return 0;
}