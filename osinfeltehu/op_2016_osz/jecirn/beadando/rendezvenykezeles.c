#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#define RENDEZVENY "rendezveny.txt"
#define VENDEGEK "vendegek.txt"
#define RENDEZVENY_MAX 200
#define VENDEG_MAX 500

struct rendezveny
{
    unsigned id;
    char     nev[20];
    unsigned vendeg_db;
};

struct vendeg
{
    char     nev[50];
    char     email[50];
    unsigned rendezveny_id;
    time_t jelentkezes;
};

struct rendezveny rendezvenyek[RENDEZVENY_MAX];
struct vendeg vendegek[VENDEG_MAX];

int aktualis_rendezveny_szama;
int aktualis_vendeg_szama;
int generalt_max_id;

int main()
{
    printf("/////King of Stands\\\\\\\\\\\n");

    aktualis_rendezveny_szama = 0;
    aktualis_vendeg_szama = 0;
    generalt_max_id = 1;

    fajl_beolvasas();

    generalt_max_id = id_genenaral() + 1;

    menu();
    return 0;
}

void menu ()
{
    int szam,i=1,pontszam=110,felt=0;
    char kar[10]= {0},kar1;

    printf("\nMENU:");
    printf("\nRendezvenyek lekerese           1");
    printf("\nRendezveny letrehozasa          2");
    printf("\nRendezveny torlese              3");
    printf("\nVendeg hozzaadasa               4");
    printf("\nVendeg adatmodositasa           5");
    printf("\nVendeg torlese                  6");
    printf("\nJelentkezettek listazasa        7");
    printf("\nKilepes                         8");
    printf("\nA kivalasztott funkciot az ENTER megnyomasaval aktivalhatja: ");
    scanf("%s",kar);
    szam=strlen(kar);
    if(szam!=1)kar1='9';
    if(szam==1)kar1=kar[0];
    (void) getchar();

    switch (kar1)
    {
    case '1':
    {
        rendezvenyek_listazasa();
        vissza_a_menube();
        menu();
    }
    break;
    case '2':
    {
        rendezveny_letrehozas();
        vissza_a_menube();
        menu();
    }
    break;
    case '3':
    {
        rendezveny_torlese();

        vissza_a_menube();
        menu();
    }
    break;
    case '4':
    {
        vendeg_letrehozas();
        vissza_a_menube();
        menu();
    }
    break;
    case '5':
    {
        vendeg_modositasa();
        vissza_a_menube();
        menu();
    }
    break;
    case '6':
    {
        vendeg_torlese();
        vissza_a_menube();
        menu();
    }
    break;
    case '7':
    {
        jelentkezettek_listazasa();
        vissza_a_menube();
        menu();
    }
    break;
    case '8':
    {
        exit(0);
    }
    break;
    default:
    {
        printf("Hiba!Rossz szamot adott meg. Probalja ujra!\n");
    }
    }

}

void vissza_a_menube()
{
    printf("\nNyomjon egy entert a menube valo visszatereshez!");
    getchar();
}

void fajl_beolvasas()
{
    FILE *fp;
    fp=fopen(RENDEZVENY,"r+");
    if (fp == NULL)
    {
        fp=fopen(RENDEZVENY,"w+");
    }
    if(fp!=NULL)
    {
        fread(&aktualis_rendezveny_szama, sizeof(int), 1, fp);
        fread(rendezvenyek, sizeof(struct rendezveny), aktualis_rendezveny_szama, fp);
        fclose(fp);
    }

    FILE *fp2;
    fp2=fopen(VENDEGEK,"r+");
    if (fp2 == NULL)
    {
        fp2=fopen(VENDEGEK,"w+");
    }
    if(fp2!=NULL)
    {
        fread(&aktualis_vendeg_szama, sizeof(int), 1, fp2);
        fread(vendegek, sizeof(struct vendeg), aktualis_vendeg_szama, fp2);
        fclose(fp2);
    }
}

int id_genenaral()
{
    int i = 0;
    int max_id = 0;
    for (i; i < aktualis_rendezveny_szama; i++)
    {
        if (max_id < rendezvenyek[i].id)
        {
            max_id = rendezvenyek[i].id;
        }
    }

    return max_id;
}

void clear2(FILE *fp)
{
    char ch;
    while (ch != '\n' && (ch = fgetc(fp)) != EOF)
    {
    }
}

void beolvasas_consolbol(char** chr, int maxlength)
{
    *chr = (char*)malloc(maxlength * sizeof(char));
    fgets(*chr, maxlength, stdin);
    int len = strlen(*chr);
    if(len == maxlength - 1)
    {
        clear2(stdin);
    }
    char* ujsor = strchr(chr, '\n');
    if(ujsor != NULL)
    {
        *ujsor = '\0';
    }
}

void vendeg_adatok_beolvasasa(char** name, char** mail, int* rend_id)
{
    printf("Kerem adja meg a nevet: ");
    beolvasas_consolbol(name, 50);
    printf("Kerem adja meg az e-mail cimet: ");
    beolvasas_consolbol(mail, 50);
    printf("Kerem adja meg a rendezveny azonositojat: ");
    char *seged_azon;
    beolvasas_consolbol(&seged_azon, RENDEZVENY_MAX);
    *rend_id = atoi(seged_azon);
}

void rendezveny_letrehozas()
{
    if(aktualis_rendezveny_szama < RENDEZVENY_MAX)
    {
        printf("Kerem adja meg a rendezveny nevet: ");
        gets(rendezvenyek[aktualis_rendezveny_szama].nev);
        rendezvenyek[aktualis_rendezveny_szama].id = generalt_max_id;
        rendezvenyek[aktualis_rendezveny_szama].vendeg_db = 0;
        aktualis_rendezveny_szama++;
        generalt_max_id++;
        printf("A rendezveny felveve az alabbi azonositval: %i\n", generalt_max_id - 1);

        FILE *fp;
        fp = fopen(RENDEZVENY, "w+");
        if (fp != NULL)
        {
            fwrite(&aktualis_rendezveny_szama, sizeof(int), 1, fp);
            fwrite(rendezvenyek, sizeof(struct rendezveny), aktualis_rendezveny_szama, fp);
            fclose(fp);
        }
    }
    else
    {
        printf("Az esemenyek szama elerte a maximumot!\n");
    }
}

int rendezveny_keresese(int azonosito)
{
    int i;
    for(i = 0; i < aktualis_rendezveny_szama; i++)
    {
        if (rendezvenyek[i].id == azonosito)
        {
            return i;
        }
    }
    return -1;
}

void rendezveny_torlese()
{
    printf("Kerem adja meg a torolni kivant rendezveny azonositojat: ");
    char *rend_id;
    beolvasas_consolbol(&rend_id, RENDEZVENY_MAX);
    int rend_azon = atoi(rend_id);
    int i = rendezveny_keresese(rend_azon);
    if(i != -1)
    {
        int j;
        for(j = 0; j < aktualis_vendeg_szama; j++)
        {
            if(vendegek[j].rendezveny_id == rendezvenyek[i].id)
            {
                int j = vendeg_keresese(vendegek[j].nev, vendegek[j].email, vendegek[j].rendezveny_id);
                if(j < aktualis_vendeg_szama)
                {
                    for(; (j < aktualis_vendeg_szama - 1) && (j < VENDEG_MAX-1); j++)
                    {
                        vendegek[j] = vendegek[j+1];
                    }
                    aktualis_vendeg_szama--;
                    j--;
                }
            }
        }
        for(; (i < aktualis_rendezveny_szama - 1) && (i < RENDEZVENY_MAX-1); i++)
        {
            rendezvenyek[i] = rendezvenyek[i+1];
        }
        aktualis_rendezveny_szama--;

        printf("Sikeres torles!\n");


        FILE *fp;
        fp = fopen(VENDEGEK, "w+");
        if (fp != NULL)
        {
            fwrite(&aktualis_vendeg_szama, sizeof(int), 1, fp);
            fwrite(vendegek, sizeof(struct vendeg), aktualis_vendeg_szama, fp);
            fclose(fp);
        }

        FILE *fp2;
        fp2 = fopen(RENDEZVENY, "w+");
        if (fp2 != NULL)
        {
            fwrite(&aktualis_rendezveny_szama, sizeof(int), 1, fp2);
            fwrite(rendezvenyek, sizeof(struct rendezveny), aktualis_rendezveny_szama, fp2);
            fclose(fp2);
        }


    }
    else
    {
        printf("\nNem talalhato a megadott rendezveny!\n");
    }
}

void vendeg_letrehozas()
{
    if(aktualis_vendeg_szama < VENDEG_MAX)
    {
        int rend_id;
        char* name;
        char* mail;
        vendeg_adatok_beolvasasa(&name, &mail, &rend_id);

        if (rendezveny_keresese(rend_id) == -1)
        {
            printf("A beirt azonosito nem letezik!\n");
        }
        else if (vendeg_keresese(name, mail, rend_id) < aktualis_vendeg_szama)
        {
            printf("Erre a rendezvenyre, ezen adatokkal mar regisztraltak!");
        }
        else
        {
            vendegek[aktualis_vendeg_szama].rendezveny_id = rend_id;
            strcpy(vendegek[aktualis_vendeg_szama].nev, name);
            strcpy(vendegek[aktualis_vendeg_szama].email, mail);
            vendegek[aktualis_vendeg_szama].jelentkezes = time(0);
            aktualis_vendeg_szama++;
            rendezvenyek[rendezveny_keresese(rend_id)].vendeg_db++;

            printf("Sikeresen regisztralt az esemenyre ezen sorszammal: %i\n", rendezvenyek[rendezveny_keresese(rend_id)].vendeg_db);

            FILE *fp;
            fp = fopen(VENDEGEK, "w+");
            if (fp != NULL)
            {
                fwrite(&aktualis_vendeg_szama, sizeof(int), 1, fp);
                fwrite(vendegek, sizeof(struct vendeg), aktualis_vendeg_szama, fp);
                fclose(fp);
            }

            FILE *fp2;
            fp2 = fopen(RENDEZVENY, "w+");
            if (fp2 != NULL)
            {
                fwrite(&aktualis_rendezveny_szama, sizeof(int), 1, fp2);
                fwrite(rendezvenyek, sizeof(struct rendezveny), aktualis_rendezveny_szama, fp2);
                fclose(fp2);
            }
        }
    }
    else
    {
        printf("A vendegek szama elerte a maximumot!\n");
    }
}

void vendeg_modositasa()
{
    int rend_id;
    char* name;
    char* mail;
    vendeg_adatok_beolvasasa(&name, &mail, &rend_id);
    int i = 0;
    i = vendeg_keresese(name, mail, rend_id);

    if (i >= aktualis_vendeg_szama)
    {
        printf("Nincs ilyen adatokkal rendelkezo vendeg a rendszerben!\n");
    }
    else
    {
        char menua;
        printf("Milyen adatot kivan modositani?\n");
        printf("a: Nev\n");
        printf("b: Email\n");
        printf("c: Rendezveny azonosito\n");
        printf("k: Megsem\n");
        printf("A valasztott menupont: ");
        scanf("%s", &menua);
        clear();
        switch(menua)
        {
        case 'a':
        {
            printf("Kerem adja meg ujra a nevet: ");
            gets(vendegek[i].nev);
        }
        break;
        case 'b':
        {
            printf("Kerem adja meg ujra az email cimet: ");
            gets(vendegek[i].email);
        }
        break;
        case 'c':
        {
            int seged_id = 0;
            printf("Kerem adja meg ujra a rendezvenyazonositot: ");
            scanf("%i", &seged_id);

            if (seged_id == vendegek[i].rendezveny_id) {
                printf("Az atjelentkezes sikertelen! Jelenleg is ezen rendezvenyen van!");
            } else {

                int k = rendezveny_keresese(seged_id);
                if (k == -1)
                {
                    printf("Nincs ilyen rendezveny!\n");
                }
                else
                {
                    int l = rendezveny_keresese(vendegek[i].rendezveny_id);
                    if (l != -1) {
                        rendezvenyek[l].vendeg_db--;
                    }
                    vendegek[i].rendezveny_id = seged_id;
                    rendezvenyek[k].vendeg_db++;
                }
            }
        }
        break;
        case 'k':
        {
            menu();
        }
        break;
        default:
        {
            printf("Hiba!Rossz szamot adott meg. Probalja ujra!\n");
        }
        }

        FILE *fp;
        fp = fopen(VENDEGEK, "w+");
        if (fp != NULL)
        {
            fwrite(&aktualis_vendeg_szama, sizeof(int), 1, fp);
            fwrite(vendegek, sizeof(struct vendeg), aktualis_vendeg_szama, fp);
            fclose(fp);
        }

        FILE *fp2;
        fp2 = fopen(RENDEZVENY, "w+");
        if (fp2 != NULL)
        {
            fwrite(&aktualis_rendezveny_szama, sizeof(int), 1, fp2);
            fwrite(rendezvenyek, sizeof(struct rendezveny), aktualis_rendezveny_szama, fp2);
            fclose(fp2);
        }
    }
}

void vendeg_torlese()
{
    int rend_id;
    char* name;
    char* mail;
    vendeg_adatok_beolvasasa(&name, &mail, &rend_id);
    int i = 0;
    i = vendeg_keresese(name, mail, rend_id);

    if(i < aktualis_vendeg_szama)
    {
        int k = vendegek[i].rendezveny_id;
        int l = rendezveny_keresese(k);
        if (l == -1 )
        {
            printf("Hiba tortent! Nem letezik az adott azonosito!");
        }
        else
        {
            rendezvenyek[l].vendeg_db--;
        }
        for(; (i < aktualis_vendeg_szama - 1) && (i < VENDEG_MAX-1); i++)
        {
            vendegek[i] = vendegek[i+1];
        }
        (aktualis_vendeg_szama)--;
        FILE *fp;
        fp = fopen(VENDEGEK, "w+");
        if (fp != NULL)
        {
            fwrite(&aktualis_vendeg_szama, sizeof(int), 1, fp);
            fwrite(vendegek, sizeof(struct vendeg), aktualis_vendeg_szama, fp);
            fclose(fp);
        }

        FILE *fp2;
        fp2 = fopen(RENDEZVENY, "w+");
        if (fp2 != NULL)
        {
            fwrite(&aktualis_rendezveny_szama, sizeof(int), 1, fp2);
            fwrite(rendezvenyek, sizeof(struct rendezveny), aktualis_rendezveny_szama, fp2);
            fclose(fp2);
        }
        printf("Sikeres torles!\n");
    }
    else
    {
        printf("Nincs vendeg ezen adatokkal!\n");
    }
}

int vendeg_keresese(char* name, char* mail, unsigned rend_id)
{
    int i;
    for(i = 0; i < aktualis_vendeg_szama; i++)
    {
        if (name != NULL && mail != NULL && (strcmp(vendegek[i].nev, name) == 0 && strcmp(vendegek[i].email, mail) == 0 && vendegek[i].rendezveny_id == rend_id))
        {
            return i;
        }
    }
    return i;
}

void jelentkezettek_listazasa()
{
    int kivalaszt = 0;
    printf("Kerem adja meg melyik rendezveny vendegeit szeretne kilistazni: ");
    scanf("%i", &kivalaszt);

    int k = rendezveny_keresese(kivalaszt);
    if ( k == -1)
    {
        printf("A beirt azonosito nem letezik!\n");
    }
    else
    {
        printf("Jelentkezettek listaja:\n");
        printf("\tRendezveny azonosito: %i\n", rendezvenyek[k].id);
        printf("\tRendezveny neve: %s\n\n", rendezvenyek[k].nev);
        int i;
        for(i = 0; i < aktualis_vendeg_szama; i++)
        {
            if (vendegek[i].rendezveny_id == rendezvenyek[k].id)
            {
                char buff[100];
                strftime(buff, 100, "%Y-%m-%d %H:%M:%S", localtime(&vendegek[i].jelentkezes));
                printf("---------------\n");
                printf("Vendeg neve: %s", vendegek[i].nev);
                printf("Vendeg emaile: %s", vendegek[i].email);
                printf("Vendeg jelentkezesi ideje: %s\n", buff);
                printf("---------------");
            }
        }
    }


}

void rendezvenyek_listazasa()
{
    printf("\nRendezvenyek listaja:\n");
    int i;
    for(i = 0; i < aktualis_rendezveny_szama; i++)
    {
        printf("\t---------------\n");
        printf("\tRendezveny azonosito: %i\n", rendezvenyek[i].id);
        printf("\tRendezveny neve: %s\n", rendezvenyek[i].nev);
        printf("\tRendezveny vendegeinek szama: %i\n", rendezvenyek[i].vendeg_db);
        printf("\t---------------\n");
    }
    if (aktualis_rendezveny_szama == 0)
    {
        printf("Jelenleg nincs egyetlen rendezveny a rendszerben!\n");
    }
}

void clear()
{
    char ch;
    while((ch=getchar()!='\n')&&(ch!=EOF));
}
