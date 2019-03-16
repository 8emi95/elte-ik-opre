typedef int bool;
#define true 1
#define false 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

#define MAX_SIZE 256

struct kerdes
{
    char kerdes[MAX_SIZE];
    int valasznum;
    char valaszok[4][MAX_SIZE];
    bool vegleges;
    char* ido;
};

struct kerdes kerdessor[10000];
int kerdesnum = 0;
int vkerdesnum = 0;

void KerdesFelvetel(bool modosit, int sorszam)
{
    char tmp;
	printf("Kerem adja meg a kerdest: ");
	fgets(kerdessor[sorszam].kerdes,MAX_SIZE,stdin);
	if ((strlen(kerdessor[sorszam].kerdes)>0) && (kerdessor[sorszam].kerdes[strlen (kerdessor[sorszam].kerdes) - 1] == '\n'))
    {
          kerdessor[sorszam].kerdes[strlen (kerdessor[sorszam].kerdes) - 1] = '\0';
    }
    bool sikertelen = true;
    while (sikertelen)
    {

        printf("Kerem adja meg a valaszok szamat: ");
        char vnum[MAX_SIZE];
        fgets(vnum,MAX_SIZE,stdin);
        if ((strlen(vnum)>0) && (vnum[strlen (vnum) - 1] == '\n'))
        {
              vnum[strlen (vnum) - 1] = '\0';
        }

        char* end;
        int n;
        n = strtol(vnum,&end, 10);
        if (n == 2 || n == 3 || n == 4)
        {
            sikertelen = false;
            kerdessor[sorszam].valasznum = n;
        }
        if (sikertelen)
        {
            printf("A valaszok szama 2 es 4 kozott lehet!\n");
        }
    }
    int j;
    for (j = 0; j < kerdessor[sorszam].valasznum ; j = j +1)
    {
        printf("Kerem adja meg a %d. valaszt: ", j+1);
        fgets(kerdessor[sorszam].valaszok[j],MAX_SIZE,stdin);
        if ((strlen(kerdessor[sorszam].valaszok[j])>0) && (kerdessor[sorszam].valaszok[j][strlen (kerdessor[sorszam].valaszok[j]) - 1] == '\n'))
        {
            kerdessor[sorszam].valaszok[j][strlen (kerdessor[sorszam].valaszok[j]) - 1] = '\0';
        }
    }
    kerdessor[sorszam].vegleges = false;
    if (!modosit)
    {
        time_t rawtime;
        struct tm* timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        kerdessor[sorszam].ido = asctime(timeinfo);
        printf("\n");
        printf("Ez az ");
        printf("%d",sorszam+1);
        printf(". kerdes\n");

    }


}

void KerdesModositas()
{
    int sorszam = KerdesKivalasztas();
    KerdesFelvetel(true, sorszam-1);
}

void KerdesTorles()
{
    int sorszam = KerdesKivalasztas();
    kerdessor[sorszam-1] = kerdessor[kerdesnum-1];
    kerdessor[kerdesnum-1] = kerdessor[sorszam-1];
    kerdesnum = kerdesnum -1;
    printf("Torles sikeres!\n");
}

void KerdesListazas(bool v)
{
    int i;
    for (i = 0; i < kerdesnum; i = i + 1)
    {
        if (kerdessor[i].vegleges || !v)
        {
            printf("%d.%s \n",i+1,kerdessor[i].kerdes);
            int j;
            for (j = 0; j < kerdessor[i].valasznum-1; j = j +1)
            {
                printf("%s | ",kerdessor[i].valaszok[j]);
            }
            printf("%s \n",kerdessor[i].valaszok[kerdessor[i].valasznum-1]);
        }
        printf("\n");
	}
}

int KerdesKivalasztas()
{
    bool sikertelen = true;
    while (sikertelen)
    {

        printf("Kerem adja meg a kerdes szamat: ");
        char knum[MAX_SIZE];
        fgets(knum,MAX_SIZE,stdin);
        if ((strlen(knum)>0) && (knum[strlen (knum) - 1] == '\n'))
        {
              knum[strlen (knum) - 1] = '\0';
        }

        char* end;
        int ret;
        ret = strtol(knum,&end, 10);
        if (ret > 0 && ret <= kerdesnum)
        {
            return ret;
        }
        if (sikertelen)
        {
            printf("Nincs ilyen sorszamu kerdes!\n");
        }
    }

}

void Veglegesites()
{
    bool sikertelen;
    while (true)
    {
        printf("Ha nem akar tobb kerdest veglegesiteni, akkor adjon meg 0-t.");
        printf("Kerem adja meg a veglegesiteni kivant kerdes sorszamat:");
        char knum[MAX_SIZE];
        fgets(knum,MAX_SIZE,stdin);
        if ((strlen(knum)>0) && (knum[strlen (knum) - 1] == '\n'))
        {
              knum[strlen (knum) - 1] = '\0';
        }

        char* end;
        int ret;
        ret = strtol(knum,&end, 10);
        if (ret > 0 && ret <= kerdesnum)
        {
            sikertelen = false;
            kerdessor[ret-1].vegleges = true;
        }
        else if (ret == 0)
        {
            break;
        }
        if (sikertelen)
        {
            printf("Nincs ilyen sorszamu kerdes!\n");
        }
    }


}

void Beolvasas()
{
    FILE* fp;
    fp = fopen("kerdessor.txt", "r");
    char num[MAX_SIZE];
    fgets(num, MAX_SIZE, fp);
    if (strlen(num) == 0)
    {
        return;
    }
    kerdesnum = atoi(num);
    int i;
    char adat[MAX_SIZE];
    for (i = 0; i < kerdesnum; i = i + 1)
    {
        fgets(kerdessor[i].kerdes,MAX_SIZE,fp);
        if ((strlen(kerdessor[i].kerdes)>0) && (kerdessor[i].kerdes[strlen (kerdessor[i].kerdes) - 1] == '\n'))
        {
          kerdessor[i].kerdes[strlen (kerdessor[i].kerdes) - 1] = '\0';
        }
        fgets(num, MAX_SIZE, fp);
        kerdessor[i].valasznum = atoi(num);
        int j;
        for (j = 0; j < kerdessor[i].valasznum; j = j+ 1)
        {
            fgets(kerdessor[i].valaszok[j],MAX_SIZE,fp);
            if ((strlen(kerdessor[i].valaszok[j])>0) && (kerdessor[i].valaszok[j][strlen (kerdessor[i].valaszok[j]) - 1] == '\n'))
            {
                kerdessor[i].valaszok[j][strlen (kerdessor[i].valaszok[j]) - 1] = '\0';
            }
        }
        fgets(num, MAX_SIZE, fp);
        kerdessor[i].vegleges = atoi(num);
        fgets(adat,MAX_SIZE,fp);
        kerdessor[i].ido = malloc(256*sizeof(char));
        int k = 0;
        int l = 0;
        while(adat[l] != '\n')
        {
            kerdessor[i].ido[k] = adat[l];
            k = k + 1;
            l = l + 1;
        }
        kerdessor[i].ido[k] = '\n';
        kerdessor[i].ido[k+1] = '\0';
    }
    fclose(fp);
}

void Kiiras()
{
    if (kerdesnum)
    {
        FILE* fp;
        fp = fopen("kerdessor.txt", "w");
        fprintf(fp,"%d\n",kerdesnum);
        int i;
        for (i = 0; i < kerdesnum; i = i +1)
        {

            fprintf(fp,"%s\n", kerdessor[i].kerdes);
            fprintf(fp,"%d\n", kerdessor[i].valasznum);
            int j = 0;
            for (j = 0; j < kerdessor[i].valasznum; j = j + 1)
            {
               fprintf(fp,"%s\n", kerdessor[i].valaszok[j]);
            }
            fprintf(fp,"%d\n", kerdessor[i].vegleges);
            fprintf(fp,"%s", kerdessor[i].ido);


        }
        fclose(fp);

    }

}

void SignalHandler(int signo)
{
    if(signo == SIGINT)
    {
        printf("Kerdezobiztos: Megerkeztem a helyszinre.\n");
    }

}


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Hibas parameterszam");
        exit(1);
    }
    Beolvasas();
	printf("Udvozoljuk!\n");
	printf("\n");
	char funkcio = '0';
	bool veglegesitve = true;
    while (veglegesitve)
	{
		printf("Uj kerdes felvetele - u\n");
		printf("Kerdes modositasa - m\n");
		printf("Kerdes torlese - t\n");
		printf("Kerdesek listazasa - l\n");
		printf("Kerdesek veglegesitese - v\n");
		printf("Vegleges kerdessor kilistazasa - k\n");
		printf("Kilepes - x\n");
		printf("Kerem usse be hasznalni kivant funkcio betujelet : ");
		scanf(" %c", &funkcio);
		printf("\n");
		char tmp;
		tmp = getchar();
		bool hibasbemenet = false;
		if (tmp != '\n')
        {
            hibasbemenet = true;
        }
		while (tmp != '\n')
		{
			tmp = getchar();
		}
        tmp = 'a';
        if (hibasbemenet)
        {
            hibasbemenet = false;
            printf("Hiba! Az on altal megadott betujelhez nem tartozik funkcio\n");
            continue;
        }
		funkcio = toupper(funkcio);
		switch (funkcio)
		{
			case 'U':
				kerdesnum = kerdesnum + 1;
				KerdesFelvetel(false, kerdesnum-1);
				break;
			case 'M':
			    KerdesListazas(false);
				KerdesModositas();
				break;
			case 'T':
			    KerdesListazas(false);
				KerdesTorles();
				break;
			case 'L':
				KerdesListazas(false);
				break;
			case 'V':
			    KerdesListazas(false);
				Veglegesites();
				veglegesitve = false;
				break;
            case 'K':
                KerdesListazas(true);
                break;
			case 'X':
				printf("Viszontlatasra!");
				Kiiras();
				int i;
				for (i = 0; i < kerdesnum; i = i + 1)
                {
                    free(kerdessor[i].ido);
                }
				exit(0);
			default:
				printf("Hiba! Az on altal megadott betujelhez nem tartozik funkcio\n");
				break;
		}
		printf("\n");
	}
	for (int i = 0; i < kerdesnum; i++ )
    {
        if (kerdessor[i].vegleges != true)
        {
            kerdessor[i] = kerdessor[kerdesnum-1];
            kerdessor[kerdesnum-1] = kerdessor[i];
            kerdesnum = kerdesnum -1;
            i--;
        }
    }
	printf("Kikuldom a kerdezobiztost. Uticel : %s.\n",argv[1]);
	//KerdesListazas(false);
	pid_t   child;
    int     fd[2],fd2[2], nbytes;
    char    readbuffer[MAX_SIZE];
    signal (SIGINT, SignalHandler);
    char nyugta[MAX_SIZE] = "nyugta";

    if(pipe(fd) == -1)
    {
        perror("pipe\n");
        exit(1);
    }
    if(pipe(fd2) == -1)
    {
        perror("pipe\n");
        exit(1);
    }
    if((child = fork()) == -1)
    {
       perror("fork\n");
       exit(1);
    }
    //gyerek
    if (child == 0)
    {
        close(fd[1]);
        close(fd2[0]);
        int k1,k2,k3;
        kill(getppid(), SIGINT);

        nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
        readbuffer[nbytes+1] =  0;
        k1 = atoi(readbuffer);
        write(fd2[1], nyugta, sizeof(nyugta));

        nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
        readbuffer[nbytes+1] =  0;
        k2 = atoi(readbuffer);
        write(fd2[1], nyugta, sizeof(nyugta));

        nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
        readbuffer[nbytes+1] =  0;
        k3 = atoi(readbuffer);
        write(fd2[1], nyugta, sizeof(nyugta));
        //printf("%d %d %d\n",k1,k2,k3);

        srand(time(NULL));
        int kitoltok = rand() % 11 + 10;
        //printf("%d\n", kitoltok);
        int eredmenyek[3][4];
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                eredmenyek[i][j] = 0;
            }
        }
        for (int i = 0; i < kitoltok; i++)
        {
            eredmenyek[0][rand() % kerdessor[k1].valasznum]++;
            eredmenyek[1][rand() % kerdessor[k2].valasznum]++;
            eredmenyek[2][rand() % kerdessor[k3].valasznum]++;
        }
        char send[MAX_SIZE];
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                snprintf(send,MAX_SIZE,"%d",eredmenyek[i][j]);
                write(fd2[1], send, strlen(send)+1);
                read(fd[0],readbuffer,sizeof(readbuffer));
            }
        }


        close(fd[0]);
        close(fd2[1]);
        exit(0);
    }

    //szulo
    if (child > 0)
    {
        close(fd[0]);
        close(fd2[1]);
        pause();
        srand(time(NULL));

        int k1 = rand() % kerdesnum;
        char send[MAX_SIZE];
        snprintf(send,MAX_SIZE,"%d",k1);
        write(fd[1], send, strlen(send)+1);
        //sleep(1);
        read(fd2[0],readbuffer,sizeof(readbuffer));
        int k2;
        do
        {
            k2 = rand() % kerdesnum;
        }
        while (k2 == k1);
        snprintf(send,MAX_SIZE,"%d",k2);
        write(fd[1], send, strlen(send)+1);
        //sleep(1);
        read(fd2[0],readbuffer,sizeof(readbuffer));
        int k3;
        do
        {
            k3 = rand() % kerdesnum;
        }
        while (k2 == k3 || k3 == k1);
        snprintf(send,MAX_SIZE,"%d",k3);
        write(fd[1], send, strlen(send)+1);
        read(fd2[0],readbuffer,sizeof(readbuffer));
        //printf("%d %d %d",k1,k2,k3);
        int eredmenyek[3][4];

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                nbytes = read(fd2[0], readbuffer, sizeof(readbuffer));
                readbuffer[nbytes+1] =  0;
                eredmenyek[i][j] = atoi(readbuffer);
                write(fd[1], nyugta, sizeof(nyugta));
            }
        }
        printf("MLSZ: Eredmenyek:\n");
        printf("\n");
        printf("1. %s\n",kerdessor[k1].kerdes);
        for (int i = 0; i < kerdessor[k1].valasznum; i++)
        {
            printf("%s: %d\n", kerdessor[k1].valaszok[i], eredmenyek[0][i]);
        }
        printf("\n");
        printf("2. %s\n",kerdessor[k2].kerdes);
        for (int i = 0; i < kerdessor[k2].valasznum; i++)
        {
            printf("%s: %d\n", kerdessor[k2].valaszok[i], eredmenyek[1][i]);
        }
        printf("\n");
        printf("3. %s\n",kerdessor[k3].kerdes);
        for (int i = 0; i < kerdessor[k3].valasznum; i++)
        {
            printf("%s: %d\n", kerdessor[k3].valaszok[i], eredmenyek[2][i]);
        }

        close(fd[1]);
        close(fd2[0]);
    }

	Kiiras();
    return 0;
}
