
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
 
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
    char nev[50];
    char email[50];
    int rendezvenyAzon;
    int datum;
 
};
 
struct Rendezveny
{
    int azon;
};
 
void Menu();
void latogatokListazasa();
void ujRendezveny();
void rendezvenyTorles();
void rendezvenyInformacioLatogatoListaval();
void ujLatogato();
void latogatoAdatokModositasa();
void latogatoAdatokTorlese();
 
int main()
{
    Menu();
}
 
 
void Menu()
{
    int menu_szam;
 
    do
    {
        printf("Valassz az alabbi menupontok kozul!\n\n");
        
        printf("1. Jelentkezesek listazasa\n");
        printf("2. Uj jelentkezes\n");
        printf("3. Rendezveny torlese\n");
        printf("4. Rendezvenyinformacio az adott jelentkezesokkel\n\n");
 
        printf("5. Jelentkezo rogzitese\n");
        printf("6. Jelentkezo adatok modositasa\n");
        printf("7. Jelentkezo adatok torlese\n");
        printf("8. Kilepes\n");
 
 
        scanf("%i", &menu_szam);
 
        switch(menu_szam)
        {
            case 1: latogatokListazasa();
            break;
 
            case 2: ujRendezveny();
            break;
 
            case 3: rendezvenyTorles();
            break;
 
            case 4: rendezvenyInformacioLatogatoListaval();
            break;
 
            case 5: ujLatogato();
            break;
 
            case 6: latogatoAdatokModositasa();
            break;
 
            case 7: latogatoAdatokTorlese();
            break;
        }
    }while(menu_szam != 8);
 
}
 
void latogatokListazasa()
{
    int rendezvenyek_szama;
    int osszes_rendezveny = 50;
    struct Rendezveny rendezveny[50];
    FILE* fo;
    fo = fopen("rendezvenyek.db", "ab+");
    rendezvenyek_szama = fread(rendezveny,sizeof(struct Rendezveny),osszes_rendezveny,fo);
    if (rendezveny == NULL || rendezvenyek_szama == 0) {
            printf(" Nincs semmi az adatbázisban, ezért nem lehet olvasni\n");
            exit(1);
        }
   
    int i=0;
    for(i; i< rendezvenyek_szama; i++)
    {
        printf("%i", rendezveny[i].azon);
       
    }
    return;
}
void ujRendezveny()
{  
    struct Rendezveny rendezveny;
    char* input = (char*)malloc(50 * sizeof(char));
    FILE* f;
    
    f = fopen("rendezveny.db", "a");
   
    printf("%s","\nAdja meg az Rendezveny nevet!\n");
    scanf("%d", &rendezveny.azon);    
   
    fprintf(f, "%i \n", rendezveny.azon);
    fclose(f);
    free(input);
   
}
void rendezvenyTorles()
{
 
}
void rendezvenyInformacioLatogatoListaval()
{
 
}
void ujLatogato()
{
    struct Latogato latogato;
    char* input = (char*)malloc(50 * sizeof(char));
    FILE* f;
    f = fopen("latogato.db", "a");
 
    printf("%s","\nAdja meg a nevet!\n");
    scanf("%s", latogato.nev);
 
    printf("%s", "Adja meg az email cimet!\n");
    scanf("%s", latogato.email);
 
    printf("%s", "Adja meg a rendezveny azonositot!\n");
    scanf("%d", &latogato.rendezvenyAzon);
 
    int my_time = time(NULL);
    latogato.datum = my_time;
    fprintf(f, "%s %s %i  %i \n", latogato.nev, latogato.email, latogato.rendezvenyAzon, latogato.datum);
    fclose(f);
    free(input);
}
void latogatoAdatokModositasa()
{
 
}
void latogatoAdatokTorlese()
{
 
}
