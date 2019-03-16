#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

struct vendeg
{
	char nev[255];
	char email[255];
    time_t regtime;
};

struct rendezveny
{
    int rend;
    int szam;
    struct vendeg ven[1000];
};

struct rendezvenyvege
{
	int rend;
	int siker;
	int megjelentek;
	struct vendeg ven[1000];
};

struct rendezveny myrend[1000];
int myrendMeret = 0;

void tagfelvet();
void tagmodositas();
void tagtorles();
void listazas();
void ujrendezveny();
void rendezvenytorlese();
void mentes();
void betoltes();
void lebonyolitas();

int main()
{
	betoltes();
	int bemenet;
    int kilep = 0;
	do{
		printf("Kerem valasszon menupontot!\n\n");
		printf("1 - Uj tag felvetele\n");
		printf("2 - Meglevo tag modositasa\n");
		printf("3 - Meglevo tag torlese\n");
		printf("4 - Rendezveny tagjainak listazasa\n");
		printf("5 - Uj rendezveny felvetele\n");
		printf("6 - Rendezveny torlese\n");
		printf("7 - Kilepes es mentes\n");
		printf("8 - Rendezvenyek lebonyolitasa\n");
		
		scanf("%d", &bemenet);
		
		switch (bemenet)
		{
        case 1:
            tagfelvet();
        break;
        case 2:
            tagmodositas();
        break;
        case 3:
            tagtorles();
        break;
        case 4:
            listazas();
        break;
        case 5:
            ujrendezveny();
        break;
        case 6:
            rendezvenytorlese();
        break;
		case 7:
			kilep = 1;
			mentes();
        break;
		case 8:
			lebonyolitas();
        break;
        default:
            printf("Ilyen menupont nem letezik!\n");
        break;
		}
    }
	while(kilep == 0);
	
	return(0);
}

void tagfelvet()
{
    int rend;
    
    printf("Melyik rendezvenyhez akarsz tagot hozzaadni?\n");
    
    if(scanf("%d", &rend) == 0)
    {
        while(fgetc(stdin) != '\n');
        printf("Nincs ilyen rendezveny.\n");
        return;
    }
    int i = 0;
    while(i < myrendMeret && myrend[i].rend != rend)
    {
        i++;
    }
    if(i == myrendMeret)
    {
        printf("Nincs ilyen rendezveny.\n");
    }
    else
    {
        char nev[255];
        char email[255];
        
        printf("Mi a neve?\n");
        scanf("%s", nev);
        printf("Mi az e-mail cime?\n");
        scanf("%s", email);
        
        strcpy(myrend[i].ven[myrend[i].szam].nev, nev);
        strcpy(myrend[i].ven[myrend[i].szam].email, email);
        time(&myrend[i].ven[myrend[i].szam].regtime);
        myrend[i].szam++;
        
        printf("Hozza lett adva.\n");
    }
        
    
    return;
}

void tagmodositas()
{
    int rend;
    
    printf("Melyik rendezvenynel akarsz tagot modositani?\n");
    if(scanf("%d", &rend) == 0)
    {
        while(fgetc(stdin) != '\n');
        printf("Nincs ilyen rendezveny.\n");
        return;
    }
    
    int i = 0;
    while(i < myrendMeret && myrend[i].rend != rend)
    {
        i++;
    }
    if(i == myrendMeret)
    {
        printf("Nincs ilyen rendezveny.\n");
    }
    else
    {
		char email[255];
		
        printf("Mi az e-mail cime?\n");
        scanf("%s", email);
        
        int j = 0;
        while(j < myrend[i].szam && strcmp(myrend[i].ven[j].email, email) != 0)
        {
            j++;
        }
        if(j == myrend[i].szam)
        {
            printf("Nincs ilyen e-mail cim.\n");
        }
        else
        {
			char nev[255];
			char email2[255];
			
            printf("Mi legyen a neve?\n");
            scanf("%s", nev);
            printf("Mi legyen az e-mail cime?\n");
            scanf("%s", email2);
            
            strcpy(myrend[i].ven[j].nev, nev);
            strcpy(myrend[i].ven[j].email, email2);
            
            printf("Sikeres modositas.\n");
        }
    }
    
    return;
}

void tagtorles()
{
    int rend;
	
	printf("Melyik rendezvenynel akarsz tagot torolni?\n");
	if(scanf("%d", &rend) == 0)
    {
        while(fgetc(stdin) != '\n');
        printf("Nincs ilyen rendezveny.\n");
        return;
    }
	int i = 0;
    while(i < myrendMeret && myrend[i].rend != rend)
    {
        i++;
    }
    if(i == myrendMeret)
    {
        printf("Nincs ilyen rendezveny.\n");
    }
    else
    {
		char email[255];
	
        printf("Mi az e-mail cime?\n");
        scanf("%s", email);
        
        int j = 0;
        while(j < myrend[i].szam && strcmp(myrend[i].ven[j].email, email) != 0)
        {
            j++;
        }
        if(j == myrend[i].szam)
        {
            printf("Nincs ilyen e-mail cim.\n");
        }
        else
        {
			int k;
			for(k = j; k < myrend[i].szam - 1; k++)
			{
				stpcpy(myrend[i].ven[k].nev, myrend[i].ven[k+1].nev);
				stpcpy(myrend[i].ven[k].email, myrend[i].ven[k+1].email);
				myrend[i].ven[k].regtime = myrend[i].ven[k+1].regtime;
			}
			myrend[i].szam--;
            printf("Sikeres torles.\n");
        }
    }
	
    return;
}

void listazas()
{
	int rend;
	
	printf("Melyik rendezveny tagjainak listazasat szeretned?\n");
	if(scanf("%d", &rend) == 0)
    {
        while(fgetc(stdin) != '\n');
        printf("Nincs ilyen rendezveny.\n");
        return;
    }
	
	int i = 0;
    while(i < myrendMeret && myrend[i].rend != rend)
    {
        i++;
    }
    if(i == myrendMeret)
    {
        printf("Nincs ilyen rendezveny.\n");
    }
    else
    {
		int j;
        for(j = 0; j < myrend[i].szam; j++)
		{
			printf("%s   %s\n", myrend[i].ven[j].nev, myrend[i].ven[j].email);
		}
    }
	
    return;
}

void ujrendezveny()
{
    myrend[myrendMeret].szam = 0;
    if(myrendMeret > 0)
    {
        myrend[myrendMeret].rend = myrend[myrendMeret-1].rend + 1;
        printf("Letrejott a %d szamu rendezveny.\n", myrend[myrendMeret].rend);   
    }
    else
    {
        myrend[myrendMeret].rend = 1;
        printf("Letrejott a 1 szamu rendezveny.\n");
    }
    myrendMeret++;
    return;
}

void rendezvenytorlese()
{
	int rend;
	printf("Melyik rendezvenyt szeretned torolni?\n");
	if(scanf("%d", &rend) == 0)
    {
        while(fgetc(stdin) != '\n');
        printf("Nincs ilyen rendezveny.\n");
        return;
    }
	int i = 0;
    while(i < myrendMeret && myrend[i].rend != rend)
    {
        i++;
    }
    if(i == myrendMeret)
    {
        printf("Nincs ilyen rendezveny.\n");
    }
    else
    {
        // i törlendő rendezveny
		int j;
		for(j = i; j < myrendMeret-1; j++)
		{
			myrend[j].rend = myrend[j+1].rend;
			myrend[j].szam = myrend[j+1].szam;
			int k;
			for(k = 0; k < myrend[j].szam; k++)
			{
				strcpy(myrend[j].ven[k].nev, myrend[j+1].ven[k].nev);
				strcpy(myrend[j].ven[k].email, myrend[j+1].ven[k].email);
				myrend[j].ven[k].regtime = myrend[j+1].ven[k].regtime;
			}
		}
		myrendMeret--;
		printf("Sikeres torles.\n");
    }
    return;    
}

void betoltes()
{
	FILE *f;	
	f = fopen("rendezveny.txt", "rb");
	if(f==NULL)
	{
		perror("Error");
		return;
	}
	else{
		while(!feof(f))
		{
			fread(&myrend[myrendMeret], sizeof(myrend[myrendMeret]), 1, f);
			if(myrend[myrendMeret].rend != 0) myrendMeret++;
		}
	}
	fclose(f);
	return;
}

void mentes()
{
	FILE *f;	
	f = fopen("rendezveny.txt", "wb");
	
	int i;
	for(i = 0; i < myrendMeret; i++)
	{
		fwrite(&myrend[i], sizeof(myrend[i]), 1, f);
	}
	
	fclose(f);
	return;
}

void handler(int signumber){
	if(signumber == SIGUSR1)
	{
		;
	}
}

void lebonyolitas()
{
	int a = 0;
	int act = 0;
	for(act = 0; act < myrendMeret; act++)
	{
		int toparent[2];
		int tochild[2];
		int val = 0, len, i;
		pid_t pid;

		// create all the descriptor pairs we need
		if (pipe(toparent) < 0)
		{
			perror("Failed to allocate pipes\n");
			exit(EXIT_FAILURE);
		}
		if (pipe(tochild) < 0)
		{
			perror("Failed to allocate pipes\n");
			exit(EXIT_FAILURE);
		}

		signal(SIGUSR1, handler);
		// fork() returns 0 for child process, child-pid for parent process.
		if ((pid = fork()) < 0)
		{
			perror("Failed to fork process\n");
			return;
		}

		// if the pid is zero, this is the child process
		if (pid == 0)
		{
			sleep(2);
			// Child. Start by closing descriptors we
			//  don't need in this process
			close(toparent[0]);
			close(tochild[1]);

			// used for output
			pid = getpid();

			// wait for parent to send us a value
			len = read(tochild[0], &val, sizeof(val));
			if (len < 0)
			{
				perror("Gyerek: Nem sikerult olvasni a csatornabol.\n");
				exit(EXIT_FAILURE);
			}
			else if (len == 0)
			{
				// not an error, but certainly unexpected
				fprintf(stderr, "Gyerek: EOF-ot olvasott a csatornabol.\n");
			}
			else
			{
				// report what we received
				printf("Gyerek(%d): Megerkeztem %d-s szamu helyszinre. Varom a vendegeket!\n", pid, val);
				kill(getppid(), SIGUSR1);
				struct rendezveny rendd;
				len = read(tochild[0], &rendd, sizeof(rendd));
				int k;
				for(k = 0; k < rendd.szam; k++)
				{
					printf("Gyerek: %s\n", rendd.ven[k].nev);
				}
				struct rendezvenyvege vegso;
				vegso.rend = rendd.rend;
				vegso.siker = rand() % 10;
				vegso.megjelentek = 0;
				
				for(k = 0; k < rendd.szam; k++)
				{
					if(rand() % 10 == 1)
					{
						vegso.ven[vegso.megjelentek] = rendd.ven[k];
						vegso.megjelentek++;
					}
				}
				if (write(toparent[1], &vegso, sizeof(vegso)) < 0)
				{
					perror("Child: Failed to write response value\n");
					exit(EXIT_FAILURE);
				}
			}

			// finished. close remaining descriptors.
			close(tochild[0]);
			close(toparent[1]);

			//return;
		}
		else
		{
			// Parent. close unneeded descriptors
		close(tochild[0]);
		close(toparent[1]);

		// used for output
		pid = getpid();

		// send a value to the child
		printf("Szulo(%d): Elkezdodott a %d.esemeny\n", pid, myrend[act].rend);
		if (write(tochild[1], &myrend[act].rend, sizeof(myrend[act].rend)) != sizeof(myrend[act].rend))
		{
			perror("aSzulo: Nem sikerult elkuldeni a gyereknek az adatot.\n");
			exit(EXIT_FAILURE);
		}

		// report what we received
		pause();
		printf("Szulo(%d): A %d rendezveny resztvevoit kuldom\n", pid, myrend[act].rend);
		
		if (write(tochild[1], &myrend[act], sizeof(myrend[act])) != sizeof(myrend[act]))
		{
			perror("bSzulo: Nem sikerult elkuldeni a gyereknek az adatot.\n");
			exit(EXIT_FAILURE);
		}
		
		struct rendezvenyvege rendvege;
		len = read(toparent[0], &rendvege, sizeof(rendvege));
		if (len < 0)
		{
			perror("Szulo: nem sikerult kiolvasni az adatot a csatornabol.\n");
			exit(EXIT_FAILURE);
		}
		else if (len == 0)
		{
			// not an error, but certainly unexpected
			fprintf(stderr, "Szulo(%d): EOF-ot olvasott a csatornabol.\n", pid);
		}
		else
		{
			printf("A rendezveny sikeressege: %d\nAkik nem erkeztek meg:\n", rendvege.siker);
			int k;
			for(k = 0; k < rendvege.megjelentek; k++)
			{
				printf("%s\n", rendvege.ven[k].nev);
			}
			
		}
		
		// close down remaining descriptors
		close(tochild[1]);
		close(toparent[0]);

		// wait for child termination
		wait();
		}
//////////////////////////////////////////////////////////////////
		
	}
}