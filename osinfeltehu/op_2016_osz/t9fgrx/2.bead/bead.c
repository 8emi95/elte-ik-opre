#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>

void sigint(int);

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
    struct Latogato latogatok[100]; //látogatók
    int kilep=1;	//kilepeshez
    int sorszam=1;	//hányaddiként jelentkezett
    int jelentkezettek=0; //eddig hányan jelentkeztek fel

    int i;
    for(i=0; i<100; ++i)
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
                    printf("A(z) %i. sorszamu jelentkezo email cimenel modositasa sikeresen megtortent :)\n", latogatok[index].sorszam);
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
                latogatok[index].rendID=-1;
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
            printf("A rendezvenyek adatai sikeresen torolve lettek!");
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

            while (fgets(line, sizeof(line), file))
            {
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
    }
    while(kilep==1);

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

    /*A "King of Stands" rendezvény szervezõ társaság, amint a rendezvény ideje eljön, megbízza egy partnerét(gyermekét) az esemény lebonyolítására.
    A gyermekfolyamat indulása után csõvezetéken megkapja, hogy hol kerül a rendezvény lebonyolításra!
    A helyszínre utazik, majd visszajelzi (jelzést küld vissza) a központnak, hogy a megadott helyszinen van, várja a résztvevõket,kész a rendezvény lebonyolítására.
    Ezután a rendezvényszervezõ csõvezetéken elküldi a helyszínen tartózkodó partnernek, hogy kik a résztvevõk. Ezt a partner képernyõre írja, majd várja a vendégeket és levezényli a rendezvényt!
    A rendezvény végén jelentést küld vissza, amiben tájékoztatja a fõszervezõt, hogy mennyire sikeres volt a rendezvény (véletlenszám), illetve a jelentkezettek közül végül is ki nem érkezett meg!
    (Minden résztvevõ 10% valószínûséggel nem érkezik meg!) Miután egy esemény lebonyolításra került, "King of Stands" újra megvizsgálja, melyik rendezvény a következõ!*/

    printf("Szeretne valamelyik esemenyt lebonyolitani? 0-Nem, 1-Igen\n");
    scanf("%s",valasz);

    if(atoi(valasz)==1)
    {
	signal(SIGINT,sigint);
	
        srand(time(NULL));
        int random = (rand() % 10)+1;

        int pipefd[2];
        int pipefd2[2];
        int pipefd3[2];

        char helyszin[100];
        char sz[200];
        char kapottNevek[1000];
        char nevek[1000];
        char src[30];
        int rend;

        printf("Rendezveny ID: ");
        scanf("%s", rendID);
        printf("Helyszin: ");
        scanf("%s",helyszin);
        rend=atoi(rendID);

        pid_t pid;

        if(pipe(pipefd) == -1 || pipe(pipefd2) || pipe(pipefd3))
        {
            perror("Hiba a pipe nyitasakor!");
            exit(EXIT_FAILURE);
        }
        pid=fork();
        if(pid == -1)
        {
            perror("Fork hiba!");
            exit(EXIT_FAILURE);
        }

        if(pid == 0)//child
        {

            sleep(1);
            read(pipefd[0],sz,sizeof(sz));
            /*printf("\nPartner: Megkaptam a helyszint: %s\n", sz);
            sleep(2);*/
            printf("Partner: Megerkeztem a helyszinre, johetnek a resztvevok adatai!\n");
            //sleep(2);
            printf("Partner: Megkaptam a resztvevok adatait, varom a vendegeket!\n");
            read(pipefd2[0],kapottNevek,sizeof(kapottNevek));
            printf("Adatok:\n%s", kapottNevek);
            //sleep(3);
            printf("Partner: A rendezvenynek vege, kuldom a sikeresseget(10-bol), es hogy ki nem erkezett meg!\n");
            char ran[2];
            sprintf(ran,"%d",random);
            strcat(nevek, "\nSikeresseg: ");
            strcpy(src, ran);
            strcat(nevek, src);
            strcat(nevek, "\n");
            strcat(nevek, "Hianyzottak listaja:\n");
            for(i=0; i<jelentkezettek; ++i)
            {
                if(latogatok[i].rendID==rend)
                {
                    random=(rand()%100)+1;
                    if(random<=10)
                    {
                        strcpy(src,latogatok[i].nev);
                        strcat(nevek, src);
                        strcat(nevek, " ");
                        strcpy(src, latogatok[i].mail);
                        strcat(nevek, src);
                        strcat(nevek, "\n");
                    }
                }
            }
            write(pipefd3[1],&nevek,sizeof(nevek));

            close(pipefd[0]);
            close(pipefd[1]);
            close(pipefd2[0]);
            close(pipefd2[1]);
            close(pipefd3[0]);
            close(pipefd3[1]);


        }
        else//parent
        {
	    signal(SIGINT, SIG_DFL);

            printf("King of Stand indul(szulo)\n");
            write(pipefd[1],&helyszin,sizeof(helyszin));
            printf("King of Stands: Helyszin elkuldve\n");
	    kill(pid,SIGINT);
            for(i=0; i<jelentkezettek; ++i)
            {
                if(latogatok[i].rendID==rend)
                {
                    strcpy(src,latogatok[i].nev);
                    strcat(nevek, src);
                    strcat(nevek, " ");
                    strcpy(src,latogatok[i].mail);
                    strcat(nevek, src);
                    strcat(nevek, "\n");
                }

            }
            write(pipefd2[1],&nevek,sizeof(nevek));
            printf("King of Stands: Nevek elkuldve\n");

            read(pipefd3[0],sz,sizeof(sz));
            printf("\nKing of Stands: Megkaptam a sikeresseget es a hianyzottak listajat! %s\n",sz);
            wait(NULL); //megvárom a gyereket, hogy végezzen
            printf("King of Stands(szulo) vegzett\n");
            printf("A mihamarabbi Viszontlatasra!\n");
            close(pipefd[0]);
            close(pipefd[1]);
            close(pipefd2[0]);
            close(pipefd2[1]);
            close(pipefd3[0]);
            close(pipefd3[1]);

        }
    }
    else
    {
        printf("A mihamarabbi Viszontlatasra!\n");
    }
    return 0;
}

void sigint(int signo) {
    signal(SIGINT,sigint); /* reset signal */
    printf("\nPartner: Megkaptam a helyszint!\n");
}

