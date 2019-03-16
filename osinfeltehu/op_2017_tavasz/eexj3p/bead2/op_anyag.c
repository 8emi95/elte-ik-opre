#include <stdlib.h>
#include <stdio.h>
struct szamlistaelem {
  int szerzo;
  char cim[30];
  int mufaj;
  int hossz;
  struct szamlistaelem *next;
};
struct roviditeselem {
 int sorsz;
 char jelentes[30];
 struct roviditeselem *next;
};
struct szamlistaelem *szamok = 0; 
struct roviditeselem *roviditesek = 0;
int is_the_same(char a[], char b[]) {
 int i;
 for (i = 0; i < 30; i++) if (a[i] != b[i]) return 0;
 return 1;
}
char* dekod(int mit) {
 struct roviditeselem *seged = roviditesek;
  
 if (seged != 0) {
 
   while ((seged->next != 0) && (seged->sorsz != mit)) seged = seged->next;
   return seged->jelentes;
 }
 
 return "Proba"; 
}
void addrovidites(char jelentes[], int sorsz) {
 struct roviditeselem *elem;
 elem = malloc( sizeof(struct roviditeselem) );
 elem->next = roviditesek;
 elem->sorsz = sorsz;
 int i;
 for (i = 0; i < 30; i++) elem->jelentes[i] = jelentes[i];
 roviditesek = elem;
}
int ujrovidites(char mit[]) {
 int i;
 int sorsz;
 int l = 0;
 struct roviditeselem *seged = roviditesek;
 struct roviditeselem *elozo = roviditesek;
 
 if (seged != 0) {
 
   while ((seged->next != 0) && (l == 0)) {
    l = 1; 
    for (i = 0; i < 30; i++) if (seged->jelentes[i] != mit[i]) l = 0;
    elozo = seged;
    seged = seged->next;
   }
   if (l == 1) return elozo->sorsz;
 }
 
 if (roviditesek == 0) sorsz = 1;
 else sorsz = roviditesek->sorsz + 1;
 addrovidites(mit, sorsz);
 return sorsz;
}
//Listázás:
void listazas() {
 printf("\nA rendszerben talalhato zenefajlok:\n\n");
 struct szamlistaelem *seged = szamok;
  
 if (seged != 0) {
 int i = 1;
   do {
    printf("\n%d. ", i);
    printf("Szerzo: %s", dekod(seged->szerzo));
    printf(" Cim: %s", seged->cim);
    printf(" Mufaj: %s", dekod(seged->mufaj));
    printf(" Hossz: %d", seged->hossz);
    i++;
    seged = seged->next;
   } while (seged != 0);
 }
 else printf("Ures lista \n");
 
}
void addelem(char szerzo[], char cim[], char mufaj[], int hossz) {
 int i;  
 struct szamlistaelem *elem;
 elem = malloc( sizeof(struct szamlistaelem) );
 elem->next = szamok;
 elem->szerzo = ujrovidites(szerzo);
 for (i = 0; i < 30; i++) elem->cim[i] = cim[i];
 elem->mufaj = ujrovidites(mufaj);
 elem->hossz = hossz;
 szamok = elem;
}
//Beszúrás:
void beszuras() {
 char szerzo[30];
 char cim[30];
 char mufaj[30];
 int hossz; //mp
 printf("\nSzam beszurasa:\n");
 printf("Add meg a szam szerzojet:");
 
 scanf("%[^\n]s", szerzo);
 getchar();
 printf("\nAdd meg a szam cimet:\n");
 scanf("%[^\n]s", &cim);
 getchar();
 printf("\nAdd meg a szam mufajat:\n");
 scanf("%[^\n]s", &mufaj);
 getchar();
 printf("\nAdd meg a szam hosszat (masodpercben):\n");
 scanf("%d", &hossz);
 getchar();
 
 addelem(szerzo, cim, mufaj, hossz);
 
 printf("\nA bejegyzes beszurva!\n");
}
//Keresés:
void kereses() {
 char cim[30];
 printf("\nAdd meg a szam cimet:\n");
 scanf("%[^\n]s", &cim);
 getchar();
 
 printf("\nA rendszerben talalhato ilyen cimu zenefajlok:\n\n");
 struct szamlistaelem *seged = szamok;
 int db = 0; 
 int l = 1;
 int i;
 if (seged != 0) {
   do {
    if (is_the_same(cim, seged->cim) == 1) {
     printf("Szerzo: %s", dekod(seged->szerzo));
     printf(" Cim: %s", seged->cim);
     printf(" Mufaj: %s", dekod(seged->mufaj));
     printf(" Hossz: %d", seged->hossz);
     db++;
    }
    seged = seged->next;
   } while (seged != 0);
 }
 
 
 
}
//Módosítás:
void modositas() {
 char szerzo[30];
 char cim[30];
 char mufaj[30];
 int hossz; //mp
 int melyik;
 listazas();
 printf("\nAdd meg hanyadik bejegyzest szeretned modositani! ");
 scanf("%d", &melyik);
 getchar();
 printf("\nMost pedig add meg az uj parametereket:\n");
 printf("Add meg a szam szerzojet: ");
 scanf("%[^\n]s", &szerzo);
 getchar();
 printf("\nAdd meg a szam cimet: ");
 scanf("%[^\n]s", &cim);
 getchar();
 printf("\nAdd meg a szam mufajat: ");
 scanf("%[^\n]s", &mufaj);
 getchar();
 printf("\nAdd meg a szam hosszat (masodpercben): ");
 scanf("%d", &hossz);
 getchar();
 
 struct szamlistaelem *seged = szamok;
  
 int i = 1;
   while (seged != 0 && i != melyik) {
    seged = seged->next;
    i++;
   }
   if (seged == 0) printf("Nincs ilyen sorszamu elem a listaban!\n");
   else {
    seged->szerzo = ujrovidites(szerzo);
    for (i = 0; i < 30; i++) seged->cim[i] = cim[i];
    seged->mufaj = ujrovidites(mufaj);
    seged->hossz = hossz;
   }
 
 printf("\nA bejegyzes modositva!\n");
}
//Törlés:
void torles() {
 int melyik;
 listazas();
 printf("\nAdd meg hanyadik bejegyzest szeretned torolni! ");
 scanf("%d", &melyik);
 getchar();
 
 struct szamlistaelem *seged = szamok;
 struct szamlistaelem *elozo = 0;
  
 int i = 1;
   while (seged != 0 && i != melyik) {
    elozo = seged;
    seged = seged->next;
    i++;
   }
   if (seged == 0) printf("Nincs ilyen sorszamu elem a listaban!\n");
   else {
    if (elozo == 0) szamok = seged->next;
    else elozo->next = seged->next;
    free(seged);
   }
 
 printf("\nA bejegyzes torolve!\n");
}
//Adatok betöltése
void betoltes() {
if( access("rovidites.bin", 0 ) != -1 ) {
 FILE *roviditesfile;
 roviditesfile = fopen("rovidites.bin", "r");
 if (roviditesfile != NULL) {
  struct roviditeselem seged;
  while (fread(&seged, sizeof(seged), 1, roviditesfile)) {
   addrovidites(seged.jelentes, seged.sorsz);
   
  }
 }
 fclose(roviditesfile);
if( access("szamlista.bin", 0 ) != -1 ) {
 FILE *szamlistafile;
 szamlistafile = fopen("szamlista.bin", "rb");
 if (szamlistafile != NULL) {
  struct szamlistaelem seged2;
  while (fread(&seged2, sizeof(seged2), 1, szamlistafile)) {
   addelem(dekod(seged2.szerzo), seged2.cim, dekod(seged2.mufaj), seged2.hossz);
  }
 }
 fclose(szamlistafile);
}
}
}
//Adatok kimentése
void mentes() {
 FILE *roviditesfile;
 roviditesfile = fopen("rovidites.bin", "w");
 struct roviditeselem *seged = roviditesek;
 while (seged != 0) {
  fwrite(seged, sizeof(*seged), 1, roviditesfile);
  seged = seged->next;
 }
 fclose(roviditesfile);
 
 FILE *szamlistafile;
 szamlistafile = fopen("szamlista.bin", "w");
 struct szamlistaelem *seged2 = szamok;
 while (seged2 != 0) {
  fwrite(seged2, sizeof(*seged2), 1, szamlistafile);
  seged2 = seged2->next;
 }
 fclose(szamlistafile);
}
int main() {
 betoltes();
 printf("Ez a program zeneszamok adatait tartja nyilvan!\n");
 int vege; 
 do {
  printf("\n\n---------------\nMenu\n\n");
  printf("1. Listazas\n");
  printf("2. Kereses\n");
  printf("3. Beszuras\n");
  printf("4. Modositas\n");
  printf("5. Torles\n");
  printf("6. Kilepes");
  printf("\n\n---------------\n");
  scanf("%d", &vege);
  getchar();
  switch (vege) {
   case 1:
    //Listazas
    listazas();
   break;
   case 2:
    //Kereses
    kereses();
   break;
   case 3:
    //Beszuras
    beszuras();
   break;
   case 4:
    //Modositas
    modositas();
   break;
   case 5:
    //Torles
    torles();
   break;
   case 6:
   break;
   default:
    printf("\n\nNem letezo menupont!\n");
   break;
  }
 } while (vege != 6);
 mentes();
 return 0;
}
