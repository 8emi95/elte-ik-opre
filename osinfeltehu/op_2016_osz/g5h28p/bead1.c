
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

 
 /*
Egy kiállításokat, rendezvényeket szervező társaság a "King of Stands",
elektronikus látogatói nyilvántartást készített. A látogatók a
nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, a rendezvény
azonosító számát! Az elektronikus rendszer a jelentkezés nyugtázásaként
visszaadja, hogy hányadik vendégként történt meg a regisztráció! Az
alkalmazás adjon lehetőséget vendég jelentkezéshez, a jelentkezettek
listázásához, egy vendég adatainak módosításához, törléséhez.
Legyen lehetőségünk egy új rendezvény indításához, egy rendezvény összes
adatának törléséhez! A jelentkezőknél a korábbi adatok mellett rögzítsük
automatikusan a jelentkezési időt(rendszeridő) is(ezt nem kell bekérni)!
 */
 
struct Latogato
{
    char vezeteknev[25];
    char keresztnev[25];
    char email[50];
    char rendezvenyAzon[25];
    int datum;
};
 
struct Rendezveny
{
    char azon[25];
    char nev[50];
    int jelentkezokszama;
};
 
void Menu();
void latogatokListazasa();
void ujRendezveny();
void rendezvenyTorles();
void rendezvenyListazasa();
void ujLatogato();
void latogatoAdatokModositasa();
void latogatoAdatokTorlese();

void kiir(char filename[40]);
void torles_line(char filename[40], int delete_line);
void torles_string(char filename[40], char str[512]);
bool keres(char filename[40], char str[512]);


int main()
{
    Menu();
    return 0;
}
 
 
void Menu()
{
    int menu_szam;
 
    do
    {
        printf("\n\nValassz az alabbi menupontok kozul!\n\n");
        
        printf("1. Uj rendezveny\n");
        printf("2. Rendezveny torlese\n");
        printf("3. Rendezvenyek listazasa\n\n");
 
        printf("4. Uj jelentkezo rogzitese\n");
        printf("5. Jelentkezok listazasa\n");
        printf("6. Jelentkezo adatok modositasa\n");
        printf("7. Jelentkezo adatok torlese\n");
        printf("8. Kilepes\n\n");
 
 
        scanf("%i", &menu_szam);
 
        switch(menu_szam)
        {
            case 1: ujRendezveny();
            break;
 
            case 2: rendezvenyTorles();
            break;
 
            case 3: rendezvenyListazasa();
            break;
 
            case 4: ujLatogato();
            break;
            
            case 5: latogatokListazasa();
            break;
 
            case 6: latogatoAdatokModositasa();
            break;
 
            case 7: latogatoAdatokTorlese();
            break;
        }
    }while(menu_szam != 8);
 
}

void ujRendezveny()
{  
    struct Rendezveny rendezveny;
   
    FILE* f;
    
    f = fopen("rendezveny.db", "a");
   
    printf("%s","\nAdja meg az Rendezveny nevet!\n");
    scanf("%s", rendezveny.nev);

    printf("%s","\nAdja meg az Rendezveny azonositojat!\n");
    scanf("%s", rendezveny.azon);

    if(keres("rendezveny.db", rendezveny.azon))
    {
        printf("%s\n", "Mar van ilyen azon alatt redezveny.\n");
    }
    else
    {
        fprintf(f, "%s %s %i\n", rendezveny.azon, rendezveny.nev, 0);
    }
    fclose(f);
}

void rendezvenyTorles()
{
    char str[25];
    kiir("rendezveny.db");
    printf("Andja meg a rendezveny azonositojat: ");
    scanf("%s", str);
    torles_string("rendezveny.db", str);
    kiir("latogato.db");
    torles_string("latogato.db", str);
    kiir("rendezveny.db");
    kiir("latogato.db");
}

void rendezvenyListazasa()
{
    printf("\nRendezvenyek:\n\n");
    kiir("rendezveny.db");
}

void ujLatogato()
{
    struct Latogato latogato;
    
    FILE* f;
    f = fopen("latogato.db", "a");
 
    printf("\nAdja meg a nevet! (vezeteknev & keresztnev): ");
    scanf("%s%s", latogato.vezeteknev, latogato.keresztnev);
 
    printf("%s", "Adja meg az email cimet!\n");
    scanf("%s", latogato.email);
 
    printf("%s", "Adja meg a rendezveny azonositot!: ");
    scanf("%s", latogato.rendezvenyAzon);
 
    int my_time = time(NULL);
    latogato.datum = my_time;
    char tmp[512]; 
    strcpy(tmp, latogato.rendezvenyAzon);
    strcat(tmp, " ");
    strcat(tmp, latogato.vezeteknev);
    strcat(tmp, " ");
    strcat(tmp, latogato.keresztnev);
    strcat(tmp, " ");
    strcat(tmp, latogato.email);
    if((keres("latogato.db", tmp)) || !(keres("rendezveny.db", latogato.rendezvenyAzon)))
    {
            printf("%s\n","Mar van ilyen adatok alatt latogato vagy nem letezo rendezveny.\n");
    }
    else
    {
        fprintf(f, "%s %s %s %s %i \n", latogato.rendezvenyAzon, latogato.vezeteknev, latogato.keresztnev, latogato.email, latogato.datum);
    }
    fclose(f);
    
    FILE *fileptr1, *fileptr2;
    char temp[512];
    int latogatoszam;
    fileptr2 = fopen("replica.db", "w");
    if((fileptr1 = fopen("rendezveny.db", "r")) == NULL) {
         printf("Ures rendezveny file\n");
    }

    while(fgets(temp, 512, fileptr1) != NULL) {
        if((strstr(temp, latogato.rendezvenyAzon)) != NULL) {
            int i = 0;
            char *p = strtok (temp, " ");
            char *array[3];
            while (p != NULL)
            {
                array[i++] = p;
                p = strtok (NULL, " ");
            }
            latogatoszam = atoi(array[2]) + 1;
            fprintf(fileptr2, "%s %s %i\n", array[0],array[1],latogatoszam);
            printf("\nA %s szamu rendezvenyre, %s %s jelentkezo a %i. helyen regisztralt.\n",latogato.rendezvenyAzon, latogato.vezeteknev, latogato.keresztnev,latogatoszam);
        }
        else fprintf(fileptr2, "%s", temp);
    }
    fclose(fileptr1);
    fclose(fileptr2);
    remove("rendezveny.db");
    
    rename("replica.db", "rendezveny.db");
}

void latogatokListazasa()
{
    printf("\nLatogatok:\n\n");
    kiir("latogato.db");
}

void latogatoAdatokModositasa()

{
    int sor = 0;
    char filename[40] ="latogato.db";
    kiir(filename);
    printf("Add meg a modosítani kivant sort\n");
    scanf("%d", &sor);
    FILE *fileptr1, *fileptr2;


    fileptr1 = fopen(filename, "r");
    
    fileptr2 = fopen("replica.db", "w");
    static char buff[100000];
    int linenumber = 0;
    FILE *fp = fopen (filename, "r");

        while (fgets (buff, sizeof (buff), fp) != NULL)
        {
            if (++linenumber == sor)
            {
                int k,i = 0;
                char *p = strtok (buff, " ");
                char *array[5];
                char temp[40];
                while (p != NULL)
                {
                    array[i++] = p;
                    p = strtok (NULL, " ");
                }
                for (i = 0; i < 5; ++i) printf("%s ", array[i]);
                printf("\nAdd meg a mododsitani kivant erteket (Razon,Vnev,Knev,Email): ");
                scanf("%d", &k);
                switch(k)
                {
                    case 1: {scanf("\n%s", temp); array[0]=temp;}
                    break;
                    case 2: {scanf("\n%s", temp); array[1]=temp;}
                    break;
                    case 3: {scanf("\n%s", temp); array[2]=temp;}
                    break;
                    case 4: {scanf("\n%s", temp); array[3]=temp;}
                    break;
                }
                fprintf(fileptr2, "%s %s %s %s %s", array[0],array[1],array[2],array[3],array[4]);
            }
            else fprintf(fileptr2, "%s", buff);
        }
        fclose (fp);

    fclose(fileptr1);
    fclose(fileptr2);
    remove(filename);
    
    rename("replica.db", filename);
    kiir(filename);
}

void latogatoAdatokTorlese()
{
    int delete_line = 0;

    kiir("latogato.db");
    printf("\nAdja meg a torolni kivant sort:");
    scanf("%d", &delete_line);
    torles_line("latogato.db", delete_line);
    printf("\n\nTorles utani allapot\n\n");
    kiir("latogato.db");
}

void kiir(char filename[40])
{
    static char buff[100000];
    int linenumber = 0;
    FILE *fp = fopen (filename, "r");
    printf("\n%2s\n", filename);
    if (fp != NULL) {
        while (fgets (buff, sizeof (buff), fp) != NULL) {
            printf ("%4d: %s", ++linenumber, buff);
        }
        fclose (fp);
    }
    else printf("Ures/nemletezo file!\n");
}

void torles_line(char filename[40], int delete_line)
{
    FILE *fileptr1, *fileptr2;
    
    fileptr1 = fopen(filename, "r");
    fileptr2 = fopen("replica.db", "w");
    static char buff[100000];
    int linenumber = 0;
    FILE *fp = fopen (filename, "r");

        while (fgets (buff, sizeof (buff), fp) != NULL)
        {
            if (++linenumber != delete_line) fprintf(fileptr2, "%s", buff); 
        }
        fclose (fp);

    fclose(fileptr1);
    fclose(fileptr2);
    remove(filename);
    
    rename("replica.db", filename);
}

void torles_string(char filename[40], char str[512])
{
    FILE *fileptr1, *fileptr2;
    int find_result = 0;
    char temp[512];
    fileptr2 = fopen("replica.db", "w");
    if((fileptr1 = fopen(filename, "r")) == NULL) {
         printf("Ures file\n");
    }

    while(fgets(temp, 512, fileptr1) != NULL) {
        if((strstr(temp, str)) != NULL) {
            find_result++;
        }
        else fprintf(fileptr2, "%s", temp);
        
    }

    if(find_result == 0) {
        printf("\nNincs talalat.\n");
    }
    
    fclose(fileptr1);
    fclose(fileptr2);
    remove(filename);
    
    rename("replica.db", filename);
}

bool keres(char filename[40], char str[512])
{
    char temp[512];
    FILE *file;
    if((file = fopen(filename, "r")) == NULL) {
         return false;
    }

    while(fgets(temp, 512, file) != NULL) {
        if((strstr(temp, str)) != NULL) {
            return true;
        }
    }
    return false;
    fclose(file);
}