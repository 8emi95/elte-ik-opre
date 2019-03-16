#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

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