#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int MAX_NUMBER=100;
struct Latogato
{
    int rendID;
    int sorszam;
    time_t localtime;
    char mail[20];
    char nev[20];
};


int main()
{
    char valasz[1]; //melyik menüpont fusson
    char rendID[3];
    struct Latogato latogatok[MAX_NUMBER]; //látogatók
    int kilep=1;	//kilepeshez
    int sorszam=1;	//hányaddiként jelentkezett
    int jelentkezettek=0; //eddig hányan jelentkeztek fel

    int i;
    for(i=0; i<MAX_NUMBER; ++i)
    {
        latogatok[i].sorszam=-1;
        latogatok[i].rendID=-1;
    }

    printf("Udvozlom a King of Stands rendezvenyszervezo cegnel!\n");

    do
    {
        printf("Valasszon az opciok kozul:\n 0-Minta adatok betoltese filebol\n 1-uj vendeg\n 2-jelentkezettek listazasa\n 3-vendeg adatainak modositasa\n 4-vendeg adatinak torlese\n 5-rendezvenyek adatainak torlese\n");
        scanf("%s", valasz);
        int input=atoi(valasz);

        if(input==1)//uj vendeg
        {

            struct Latogato latogato;
            printf("Kerem adja meg nevet: ");
            scanf("%s", latogato.nev);
            printf("Kerem adja meg e-mail cimet: ");
            scanf("%s", latogato.mail);
            printf("Kerem adja meg a rendezveny azonosito szamat: ");
            scanf("%s", rendID);
            latogato.rendID=atoi(rendID);
            latogato.sorszam=sorszam;
            latogato.localtime=time(0);
            for(i=0; i<sorszam; ++i)
            {
                if(latogatok[i].sorszam==-1)
                {
                    latogatok[i]=latogato;
                    break;
                }
            }
            printf("Koszonjuk a jelentkezest! On a(z) %i. vendeg aki jelentkezett.!\n", latogato.sorszam);
            jelentkezettek=sorszam;
            sorszam++;

            printf("Szeretnel kilepni? 0-nem, 1-igen\n");
            scanf("%s", valasz);
            if(atoi(valasz)==1)
            {
                kilep=0;
            }
            else
            {
                kilep=1;
            }
        }
        if(input==2)
        {
            int jelenlegi=0;
            int i;
            for(i=0; i<jelentkezettek; ++i)
            {
                if(latogatok[i].sorszam!=-1)
                {
                    jelenlegi++;
                }
            }

            if(jelenlegi==0)
            {
                printf("Eddig meg senki sem jelentkezett fel!\n");
            }
            else
            {

                for(i=0; i<jelentkezettek; ++i)
                {
                    if(latogatok[i].sorszam!=-1)
                    {
                        printf("\nLatogato neve: %s \n",latogatok[i].nev);
                        printf("Latogato email cime: %s \n",latogatok[i].mail);
                        printf("Rendezveny id: %i \n",latogatok[i].rendID);
                        printf("Latogato sorszama: %i \n",latogatok[i].sorszam);
                        printf("Jelentkezes ideje: %s \n",ctime(&latogatok[i].localtime));
                    }
                }
            }
            printf("\nSzeretnel kilepni? 0-nem, 1-igen\n");
            scanf("%s", valasz);
            if(atoi(valasz)==1)
            {
                kilep=0;
            }
            else
            {
                kilep=1;
            }

        }
        if(input==3)
        {
            char sorszam[3];
            int index=-1;
            printf("Melyik adatot szeretne modositani?\n 1-nev\n 2-email cim\n 3-rendezveny ID\n");
            scanf("%s",valasz);

            if(atoi(valasz)==1)
            {
                do
                {
                    printf("Kerem a vendeg sorszamat!\n Sorszam:");
                    scanf("%s", sorszam);
                    if(atoi(sorszam)>jelentkezettek)
                    {
                        printf("Hiba a sorszammal!\n");
                    }
                }
                while(atoi(sorszam)>jelentkezettek);

                int i;
                for(i=0; i<jelentkezettek; ++i)
                {
                    if(latogatok[i].sorszam==atoi(sorszam))
                    {
                        index=i;
                    }
                }
                if(index!=-1)
                {
                    printf("Mi legyen az uj nev?\n");
                    scanf("%s", latogatok[index].nev);
                    printf("A(z) %i. sorszamu jelentkezo nevenek modositasa sikeresen megtortent :)\n", latogatok[index].sorszam);
                    index=-1;
                }
                else
                {
                    printf("Ez a felhasznalo mar torolve lett!");
                }

                printf("\nSzeretnel kilepni? 0-nem, 1-igen\n");
                scanf("%s", valasz);
                if(atoi(valasz)==1)
                {
                    kilep=0;
                }
                else
                {
                    kilep=1;
                }
            }

            if(atoi(valasz)==2)
            {
                do
                {
                    printf("Kerem a vendeg sorszamat!\n Sorszam:");
                    scanf("%s", sorszam);
                    if(atoi(sorszam)>jelentkezettek)
                    {
                        printf("Hiba a sorszammal!\n");
                    }
                }
                while(atoi(sorszam)>jelentkezettek);


                int i;
                for(i=0; i<jelentkezettek; ++i)
                {
                    if(latogatok[i].sorszam==atoi(sorszam))
                    {
                        index=i;
                    }
                }

                if(index!=-1)
                {
                    printf("Mi legyen az uj email cim?\n");
                    scanf("%s", latogatok[index].mail);
                    printf("A(z) %i. sorszamu jelentkezo email cimenek modositasa sikeresen megtortent :)\n", latogatok[index].sorszam);
                    index=-1;
                }
                else
                {
                    printf("Ez a felhasznalo mar torolve lett!");
                }

                printf("\nSzeretnel kilepni? 0-nem, 1-igen\n");
                scanf("%s", valasz);
                if(atoi(valasz)==1)
                {
                    kilep=0;
                }
                else
                {
                    kilep=1;
                }
            }

            if(atoi(valasz)==3)
            {
                do
                {
                    printf("Kerem a vendeg sorszamat!\n Sorszam:");
                    scanf("%s", sorszam);
                    if(atoi(sorszam)>jelentkezettek)
                    {
                        printf("Hiba a sorszammal!\n");
                    }
                }
                while(atoi(sorszam)>jelentkezettek);


                int i;
                for(i=0; i<jelentkezettek; ++i)
                {
                    if(latogatok[i].sorszam==atoi(sorszam))
                    {
                        index=i;
                    }
                }

                if(index!=-1)
                {
                    printf("Mi legyen az uj rendezveny ID?\n");
                    scanf("%s", rendID);
                    latogatok[index].rendID=atoi(rendID);
                    printf("A(z) %i. sorszamu jelentkezo rendezveny ID modositasa sikeresen megtortent :)\n", latogatok[index].sorszam);
                    index=-1;
                }
                else
                {
                    printf("Ez a felhasznalo mar torolve lett!");
                }

                printf("\nSzeretnel kilepni? 0-nem, 1-igen\n");
                scanf("%s", valasz);
                if(atoi(valasz)==1)
                {
                    kilep=0;
                }
                else
                {
                    kilep=1;
                }
            }
        }

        if(input==4)
        {
            char sorszam[3];
            int index=-1;
            do
            {
                printf("Melyik sorszamu vendeg adatait akarja torolni?\n Sorszam: ");
                scanf("%s", sorszam);
                if(atoi(sorszam)>jelentkezettek)
                {
                    printf("Hiba a sorszammal!\n");
                }
            }
            while(atoi(sorszam)>jelentkezettek);

            int i;
            for(i=0; i<jelentkezettek; ++i)
            {
                if(latogatok[i].sorszam==atoi(sorszam))
                {
                    index=i;
                }
            }

            if(index!=-1)
            {
                latogatok[index].localtime=time(0);
                printf("A(z) %i. sorszamu jelentkezo torlese sikeresen megtortent!\nEkkor: %s\n", latogatok[index].sorszam, ctime(&latogatok[index].localtime));
                latogatok[index].sorszam=-1;
            }
            else
            {
                printf("Ez a felhasznalo mar torolve lett!");
            }

            printf("\nSzeretnel kilepni? 0-nem, 1-igen\n");
            scanf("%s", valasz);
            if(atoi(valasz)==1)
            {
                kilep=0;
            }
            else
            {
                kilep=1;
            }

        }
        if(input==5)
        {
            int i;
            for(i=0; i<jelentkezettek; ++i)
            {
                latogatok[i].sorszam=-1;
                latogatok[i].rendID=-1;
            }
            sorszam=1;
            jelentkezettek=0;
            printf("A rendezvenyek adatai sikeresen torolve lettek!\n");
        }
	if(input==0)
	{
	FILE* file = fopen("Rendezvenyek.txt", "r");
    	char line[256];
	char *token;
	if(file==NULL)
            {
                printf("Hiba a file megnyitasakor!");
                exit(1);
            }

	while (fgets(line, sizeof(line), file)){
		token=strtok(line, " ");
		strcpy(latogatok[jelentkezettek].nev, token);
		latogatok[jelentkezettek].sorszam=sorszam;
		latogatok[jelentkezettek].localtime=time(0);
		token=strtok(NULL, " ");
		strcpy(latogatok[jelentkezettek].mail, token);
		token=strtok(NULL, " ");
		latogatok[jelentkezettek].rendID=atoi(token);

		jelentkezettek=sorszam;
		sorszam++;
	}

	printf("A mintaadatok sikeresen betoltve! :)\n");

    	fclose(file);
	
	}
    }while(kilep==1);

    //kiíratás fileba ID-k szerint----------------------------------------------------
    int volte=-1;
    int latogatokbyid[jelentkezettek];
    int hanyadik=0;
    int feltolt;
    for(feltolt=0; feltolt<jelentkezettek; ++feltolt)
    {
        latogatokbyid[feltolt]=-1;
    }

    for(i=0; i<jelentkezettek; ++i)
    {
        volte=-1;
        int j=0;
        for(j=0; j<i+1; j++)
        {
            if(latogatok[i].rendID==latogatokbyid[j])
            {
                volte=1;
                break;
            }
        }
        if(volte!=1 && latogatok[i].sorszam!=-1)
        {
            latogatokbyid[hanyadik]=latogatok[i].rendID;
            hanyadik++;
        }
    }

    FILE* f;
    int q;
    for(i=0; i<hanyadik; ++i)
    {
        if(latogatokbyid[i]!=-1)
        {
            int rendezvenyID = latogatokbyid[i];
            char filename[3];
            sprintf(filename, "%d", rendezvenyID);
            strcat(filename,".txt");
            f=fopen(filename, "w");
            if(f==NULL)
            {
                printf("Hiba a file megnyitasakor!");
                exit(1);
            }

            for(q=0; q<jelentkezettek; ++q)
            {
                if(latogatok[q].sorszam!=-1 && latogatok[q].rendID==latogatokbyid[i])
                {
                    fputs("Nev:",f);
                    fputs(latogatok[q].nev, f);
                    fputs(" Email:",f);
                    fputs(latogatok[q].mail, f);
                    fputs(" RendezvenyID:",f);
                    fprintf(f, "%d",latogatok[q].rendID);
                    fputs(" Sorszam:",f);
                    fprintf(f, "%d",latogatok[q].sorszam);
                    fputs(" Ido:",f);
                    fprintf(f, "%s", ctime(&latogatok[q].localtime));
                }
            }
            fclose(f);
        }
    }
    return 0;
}
