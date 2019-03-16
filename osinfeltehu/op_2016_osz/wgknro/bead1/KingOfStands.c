//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <time.h>
#include <string.h>

struct vendeg{
	
	char nev[20];
	char email[20];
	char rendezveny[20];
	char reg_dt[25];
};

struct vendeg kiir( struct vendeg a)
{
//	printf( "nev: %s", a.nev );
	printf( "nev: %s, \nemail: %s, \nrendezvény azonosító: %s, \nregisztráció időpontja: %s\n", a.nev, a.email, a.rendezveny, a.reg_dt );
}



void fomenu()
{
printf("Kérek válasszon az alábbi menüpontok közül:\n");
printf("[1]Jelentkezés rendezvényre\n");
printf("[2]Jelentkezettek listázása\n");
printf("[3]Vendég adatmódosítása\n");
printf("[4]Vendég törlése\n");
printf("[5]Új rendezvény felvétele\n");
printf("[6]Rendezvény törlése\n");
printf("[0]Kilépés\n");
	
} 

main()
{
int menu, menu_arg, g, q;
char term;
FILE * f;
char* szoveg[32];
struct tm ido;
time_t t;
menu = -1;
int result;
	
while (menu != 0)
{

fomenu();
result = scanf("%i",&menu_arg);

if( result == 1) {
    menu = menu_arg;

		switch (menu)
		{
		//jelentkezés a részvételre
		case 1:
			printf("********************************JELENTKEZÉS********************************\n");

			struct vendeg v1;

			char v_v_nev[20];
			char v_k_nev[20];
			char v_email[20];
			char v_rendezveny[10];

			
			printf("Kérjük adja meg Vezeték nevét, kereszt nevét, email címét és a rendezvény azonosítóját: ");

			result = scanf("%s %s %s %s", v_v_nev, v_k_nev, v_email, v_rendezveny);

			if (result =! 4)
			{
				printf("Nem jól adta meg az adatokat!\n");
				break;
			}else{

			strcpy(v1.nev,strcat(strcat(v_v_nev," "),v_k_nev));
			strcpy(v1.email,v_email);
			strcpy(v1.rendezveny,v_rendezveny);

			time(&t);
			asctime_r(localtime_r(&t, &ido), v1.reg_dt);

			g=open("vendeg.txt",O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
			write(g,&v1,sizeof(v1)); 
			close(g);
			
			printf("Ön a következő adatokat adta meg:\n");
			kiir(v1);
			printf("Adatai felvételre kerültek!\n");
			break;
			}
		case 2:
		printf("Ön a(z) %d menüpontot választotta!\n",menu);
		break;
		case 3:
		printf("Ön a(z) %d menüpontot választotta!\n",menu);
		break;
		case 4:
		printf("Ön a(z) %d menüpontot választotta!\n",menu);
		break;
		case 5:
		printf("Ön a(z) %d menüpontot választotta!\n",menu);
		break;
		case 6:
		printf("Ön a(z) %d menüpontot választotta!\n",menu);
		break;
		case 0:
		printf("Ön kiléptetésre kerül! Viszlát!\n");
		break;
		default:
		printf("Ön helytelen számot adott meg. Kérjük próbálja újra!\n");
		break;
		}
	
}else{
	printf("Viszlát!\n");
	break;
}

};

}