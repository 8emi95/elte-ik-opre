#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h> //open
#include <sys/stat.h>


#define MERET (200)

struct Kerdes{
    char kerdes_szovege[MERET];
    char valaszok[4][MERET];
    int valaszok_szama;
    time_t felvetel_datuma;
};
typedef struct Kerdes Kerdes;

void menuszoveg()
{
    printf("\nValassz az alabbiak kozul!\n");
    printf("1. Uj kerdes letrehozasa\n");
    printf("2. Kerdes modositasa\n");
    printf("3. Kerdes torlese\n");
    printf("4. Kerdesek listazasa\n");
    printf("5. Kerdoiv veglegesitese\n");
    printf("6. Kilepes\n\n");
}

void ujkerdes(struct Kerdes* k)
{
	printf("Adja meg a kerdest: ");
	fflush(stdin);
	scanf("%s[^\n]",k->kerdes_szovege);

	do {
        printf("Adja meg, hogy hany valaszt szeretne megadni [2-4]: ");
        scanf("%d",&k->valaszok_szama);
	} while(k->valaszok_szama > 4 || k->valaszok_szama < 2);

	int i;
	for (i = 0; i<k->valaszok_szama;++i)
    {
        printf("Irja be a %i. valaszt: ",i+1);
        scanf("%s",&k->valaszok[i]);
    }

	k->felvetel_datuma = time(0);

}

void veglegeskerdesek(struct Kerdes* vk)
{
	FILE * input;
	input = fopen("output.txt", "r");
	fscanf(input, "%s", &vk[0].kerdes_szovege);
	close( input);
}

void valasz_modositasa (struct Kerdes * k)
{
    int melyik;
    do {
        printf("Hanyas valaszt szeretne modositani?[1-%i]\n", k->valaszok_szama);
        scanf("%d",&melyik);
    } while(melyik < 1 || melyik > k->valaszok_szama);

    scanf("%s",&k->valaszok[melyik-1]);
}

void modositas(struct Kerdes* k, int kerdesszam)
{
     if(kerdesszam>=1)
    {
        int melyik;
        int m;
        do {
            printf("Melyik kerdest szeretne modositani [1-%i]: ", kerdesszam);
            scanf("%d",&melyik);
            m=melyik-1;
        } while(melyik < 1 || melyik > kerdesszam);

        char opcio;
        do{
            printf("A kerdest vagy valamelyik valaszt szeretne modositani? [k/v]");
            scanf("%s",&opcio);
        }while(opcio!='k' && opcio!='v');

        if(opcio=='k')
        {
            scanf("%s",k[m].kerdes_szovege);
            printf("A kerdes modositasa megtortent \n");
        }
        else
        {
            valasz_modositasa(&k[m]);
            printf("A valasz modositasa megtortent \n");
        }
    }
    else
    {
        printf("Nincs kerdes, amit modositani lehetne.\n");
    }

}

void valasz_torlese (struct Kerdes * k)
{
    int melyik;
    do {
        printf("Hanyas valaszt szeretne torolni?[1-%i]\n", k->valaszok_szama);
        scanf("%d",&melyik);
    } while(melyik < 1 || melyik > k->valaszok_szama);

    int i;
    int j;
    for(i = melyik - 1;i<(k->valaszok_szama)-1;i++)
    {
        for (j = 0; j < MERET; j++)
        {
            k->valaszok[i][j] = k->valaszok[i+1][j];
        }
    }

    k->valaszok_szama--;
}

void torles(struct Kerdes * k, int *kerdesszam) {
    if(*kerdesszam>=1)
    {
        int melyik;
        int m;
        do {
            printf("Melyik kerdest szeretne torolni [1-%i]: ", *kerdesszam);
            scanf("%d",&melyik);
            m=melyik-1;
        } while(melyik < 1 || melyik > *kerdesszam);

        char opcio;
        do{
            printf("Az egesz kerdest, vagy csak valaszt szeretne torolni? [k/v]");
            scanf("%s",&opcio);
        }while(opcio!='k' && opcio!='v');

        if(opcio=='k')
        {
            int i;
            for(i = m;i<(*kerdesszam)-1;i++)
            {
                k[i] = k[i+1];
            }
            --(*kerdesszam);
            printf("A kerdes torlese megtortent \n");
        }
        else
        {
            if(k[m].valaszok_szama>2)
            {
                valasz_torlese(&k[m]);
                printf("A valasz torlese megtortent \n");
            }
            else
            {
                printf("Minimum 2 valasznak lennie kell!\n");
            }
        }
    }
    else
    {
        printf("Nincs kerdes, amit torolni lehetne.\n");
    }
}

void kerdes_kiir(const struct Kerdes * k) {
	printf("%s\n",k->kerdes_szovege);
	int i = 0;
	while(i < k->valaszok_szama) {
		printf("%i. %s\n",i+1,k->valaszok[i]);
		i++;
	}
	printf("Felvetel datuma: %s \n",ctime(&k->felvetel_datuma));
}

void listazas(const struct Kerdes * k, int kerdesszam)
{
	int i = 0;
	for(i=0;i<kerdesszam;++i)
    {
        kerdes_kiir(&k[i]);
    }
}

void veglegesites(struct Kerdes * k, int kerdesszam, struct Kerdes** vk, int vegleges_szama)
{
    int i;
    FILE *f=fopen("kerdoiv.txt","w");
    if(f==NULL)
    {
        printf("HIBA A FAJL MEGNYITASAKOR!\n");
        return;
    }
    int l = 0;
    for(i=0;i<kerdesszam;++i)
    {
        printf("Ez a kerdes szerepeljen a vegleges kerdoivben?\n");
        kerdes_kiir(&k[i]);

        char v;
        do{
            printf("[i/n]:");
            scanf("%s",&v);
        }while(v!='i' && v!='n');

        if(v=='i')
        {
            fprintf(f,"%s\n",k[i].kerdes_szovege);
			
	    *vk = (struct Kerdes *) realloc(*vk,(vegleges_szama+1) * sizeof(struct Kerdes));
	    strcpy(vk[l]->kerdes_szovege, k[i].kerdes_szovege);
	    vegleges_szama++;
	    l++;
            int j = 0;
	    vk[l]->valaszok_szama = k[i].valaszok_szama;
            while(j < k[i].valaszok_szama) {
                char* asd = ("%i. %s\n",j+1,k[i].valaszok[j]);
		strcpy(vk[l]->valaszok[j],k[i].valaszok[j]);
                fprintf(f,"%i. %s\n",j+1,k[i].valaszok[j]);
		
                j++;
            }
            fprintf(f,"Felvetel datuma: %s \n",ctime(&k[i].felvetel_datuma));
	    vk[l]->felvetel_datuma = k[i].felvetel_datuma;
        }
    }
    fclose(f);
}

int main()
{
    printf("MLSZ nemzeti konzultacio - kerdoiv letrehozasa\n");
    Kerdes* k;
    Kerdes* vk;
    int vegleges_szama = 0;
    int kerdesek_szama = 0;
    int vege = 0;
    while(vege!=6)
    {
        menuszoveg();
        scanf("%d",&vege);
        switch(vege)
        {
        case 1:
            if(kerdesek_szama == 0) {
				k = (struct Kerdes *) malloc(sizeof(struct Kerdes));
			} else {
				k = (struct Kerdes *) realloc(k,(kerdesek_szama+1) * sizeof(struct Kerdes));
			}
            ujkerdes(&k[kerdesek_szama]);
            ++kerdesek_szama;
            break;
        case 2:
            modositas(k,kerdesek_szama);
            break;
        case 3:
            torles(k,&kerdesek_szama);
            break;
        case 4:
            listazas(k,kerdesek_szama);
            break;
        case 5:
  	    vk = (struct Kerdes *) malloc(sizeof(struct Kerdes));
            veglegesites(k,kerdesek_szama, &vk, vegleges_szama);
printf("%i", vegleges_szama);
            printf("A kerdoiv elkeszult. Koszonjuk, hogy programunkat hasznalta!\nViszont latasra!\n");
            //vege=6;
	    listazas(vk, 1);
            break;
        case 6:
            vege=6;
            break;
        default:
            printf("Nincs ilyen menupont!\n");
            break;
        }
    }

    return 0;
}
