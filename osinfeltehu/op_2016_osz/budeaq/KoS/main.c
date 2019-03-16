#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define GUESTDATAFILE "guesdata"
#define EVENTDATAFILE "eventdata"

struct guest{
 int id_guest;
 char name[100];
 char mail[100];
 char time[100];
};

struct event{
 char name[500];
 int id_event;
};

int read_file(int num);
void copy_guestfile(char *oldfile, char *newfile);
void copy_eventfile(char *oldfile, char *newfile);
void edit_guest();
void remove_event();
void remove_guest();
void list_event();
void add_event();
void add_guest();
void menu();

int main(){
 menu();
 return 0;
}

void menu(){
    int select = 0;
    int count = read_file(0);
    do{
        printf("\nMenü:");
        printf("\nVendégek:");
        printf("\n 1) Jelentkezés");
        printf("\n 2) Listázás");
        printf("\n 3) Módosítás");
        printf("\n 4) Törlés");
        printf("\n\nRendezvény:");
        printf("\n 5) Új ");
        printf("\n 6) Listázás");
        printf("\n 7) Törlés");
        printf("\n\n 0) Kilépés");
        printf("\n\n:> Menüpont: ");
        scanf("%d", &select);
        switch(select){
            case 0 : exit(0);
            case 1 : add_guest(&count);    break;
            case 2 : read_file(1);         break;
            case 3 : edit_guest();         break;
            case 4 : remove_guest();       break;
            case 5 : add_event();          break;
            case 6 : list_event();         break;
            case 7 : remove_event();       break;
            default: break;
        }
    }while (true);
}

void edit_guest(){
  int count = read_file(1);
  int num = 0;
  
  printf("A módosítandó vendég sorszáma: ");
  scanf("%d", &num);
  system("clear");
  if (num > count || num < 1) {
    printf("Hibás sorszám!\n");
    return;
  }
  
  struct guest new_visitor;

  printf("Név: ");
  scanf(" %[^\n]", new_visitor.name);
  printf("E-mail: ");
  scanf("%s", new_visitor.mail);
  printf("Rendezveny azonositó: ");
  while (scanf("%d",&new_visitor.id_guest) != 1){
   system("clear");
   while (getchar() != '\n');
   printf("Vendég felvétele:\n");
   printf("Név: %s\n",new_visitor.name);
   printf("E-mail: %s\n",new_visitor.mail);
   printf("A rendezvény azonosító egy szám!");
   printf("\nRendezveny azonosító: "); 
  }

  system("clear");
  FILE *f;

  f = fopen(GUESTDATAFILE, "rb");
  if (f == NULL) {
    perror("Hiba a fájl megnyitásakor!\n");
    return;
  }
  FILE *tmp;
  tmp = fopen("tmp", "wb");
  if (tmp == NULL) {
    perror("Hiba a fájl megnyitásakor!\n");
    return;
  }
  struct guest visitor;
  int ind = 0;
  while(fread( &visitor, sizeof(visitor), 1, f)){
    ind++;
    if(ind == num){
      strcpy(new_visitor.time, visitor.time);
      visitor = new_visitor;
    }
    fwrite( &visitor, sizeof(visitor), 1, tmp);
  }
  fclose(f);
  fclose(tmp);
  copy_guestfile("tmp", GUESTDATAFILE);
}

void remove_event() {
  list_event(); 
  int num = 0;
  printf("\nAdja meg a törlendő esemény azonosítóját: ");
  scanf("%d", &num);
  system("clear");

  FILE *f;
  f = fopen(GUESTDATAFILE, "rb");
  if (f == NULL) {
    perror("Hiba a fájl megnyitásakor!\n");
    return;
  }
  FILE *tmp;
  tmp = fopen("tmp", "wb");
  if (tmp == NULL){
    perror("Hiba a fájl megnyitásakor!\n");
    return;
  }
  struct guest visitor;
  while (fread( &visitor, sizeof(visitor), 1, f)){
    if (visitor.id_guest != num) fwrite( &visitor, sizeof(visitor), 1, tmp);
  }
  fclose(f);
  fclose(tmp);
  copy_guestfile("tmp", GUESTDATAFILE);

  f = fopen(EVENTDATAFILE, "rb");
  if (f == NULL){
    perror("Hiba a fájl megnyitásakor!\n");
    return;
  }
  tmp = fopen("tmp", "wb");
  if (tmp == NULL){
    perror("Hiba afájl megnyitásakor!\n");
    return;
  }
  struct event event;
  while (fread( &event, sizeof(event), 1, f)){
    if (event.id_event != num) fwrite( &event, sizeof(event), 1, tmp);
  }
  fclose(f);
  fclose(tmp);
  copy_eventfile("tmp", EVENTDATAFILE);
}


void copy_eventfile(char *oldfile, char *newfile){
  FILE *new = fopen(newfile, "wb");
  if (new == NULL) {
    perror("Hiba a fájl megnyitásakor!\n");
    return;
  }
  FILE *old = fopen(oldfile, "rb");
  if (old == NULL) {
    perror("Hiba a fájl megnyitásakor!\n");
    return;
  }
  struct event event;
  while (fread( &event, sizeof(event), 1, old)) {
    fwrite( &event, sizeof(event), 1, new);
  }
  fclose(new);
  fclose(old);
  remove(oldfile);
}

void remove_guest() {
  int count = read_file(1);
  int num = 0;

  if (count == 0){
   return;
  }
 
  printf("\nAdja meg az eltávolítandó vendég sorszámát: ");
  scanf("%d", & num);
  system("clear");
  if(num > count || num < 1){
    printf("Rossz sorszám!\n");
    return;
  }

  FILE *f;
  f = fopen(GUESTDATAFILE, "rb");
  if (f == NULL) {
    perror("Hiba a fájl megnyitásakor!\n");
    return;
  }
  FILE *tmp;
  tmp = fopen("tmp", "wb");
  if (tmp == NULL) {
    perror("Hiba a fájl megnyitásakor!\n");
    return;
  }
  struct guest visitor;
  int ind = 0;
  while (fread( &visitor, sizeof(visitor), 1, f)) {
    ind++;
    if (ind != num) fwrite( &visitor, sizeof(visitor), 1, tmp);
  }
  fclose(f);
  fclose(tmp);
  copy_guestfile("tmp", GUESTDATAFILE);
}

void copy_guestfile(char *oldfile, char *newfile){
  FILE *new = fopen(newfile, "wb");
  if (new == NULL) {
    perror("Hiba a fájl megnyitásakor!\n");
    return;
  }
  FILE *old = fopen(oldfile, "rb");
  if (old == NULL) {
    perror("Hiba a fájl megnyitásakor!\n");
    return;
  }
  struct guest visitor;
  while (fread( & visitor, sizeof(visitor), 1, old)) {
    fwrite( & visitor, sizeof(visitor), 1, new);
  }
  fclose(new);
  fclose(old);
  remove(oldfile);
}

void add_guest(int *number){
 system("clear");
 
 struct guest visitor;
 
 printf("Vendég felvétele:\n");
 printf("Név: ");
 scanf( " %[^\n]" , visitor.name);
 printf("E-mail cím: ");
 scanf("%s" , visitor.mail);
 printf("Rendezvény azonosító: ");
  while (scanf("%d",&visitor.id_guest) != 1){
   system("clear");
   while (getchar() != '\n');
   printf("Vendég felvétele:\n");
   printf("Név: %s\n",visitor.name);
   printf("E-mail: %s\n",visitor.mail);
   printf("A rendezvény azonosító egy szám!");
   printf("\nRendezveny azonosító: "); 
 }

 time_t regtime;
 struct tm *timeinfo;
 time(&regtime);
 timeinfo = localtime(&regtime);
 strftime(visitor.time, 80, "%c", timeinfo);

 FILE *file = fopen(GUESTDATAFILE, "ab");
 if (file == NULL){
  perror("Probléma a fájl megnyitásakor!\n");
  return;
 }
 if (file != NULL){
  fwrite(&visitor,sizeof(visitor), 1, file);
  (*number)++;
  fclose(file);
 }
 system("clear");
 printf("A regisztrált vendég sorszáma: %d\n\n", *number);
}

void add_event(){
 struct event event;

 system("clear");
 printf("Esemény létrehozása:\n");
 printf("Név: ");
 scanf(" %[^\n]", event.name);
 printf("Rendezvény azonosító: ");
 while (scanf("%d",&event.id_event) != 1){
   system("clear");
   while (getchar() != '\n');
   printf("Esemény létrehozása:\n");
   printf("Név: %s\n",event.name);
   printf("Hibás rendezvény azonosító!");
   printf("\nRendezveny azonosító: "); 
 }

 system("clear");

 FILE *file = fopen(EVENTDATAFILE, "ab");
 if (file != NULL){
   fwrite( &event, sizeof(event), 1, file);
   fclose(file);
 }
}

int read_file(int num){
 system("clear");
 int count = 0;
 FILE *f;
 
 f = fopen(GUESTDATAFILE, "rb");
 if (f == NULL) {
   if(num) printf("\nNincsenek vendégek regisztrálva!");
  return 0;
 }
 if(num) printf("\nA regisztrált vendégek:");
 struct guest visitor;
 if(num) printf("\nSorszám\t\tNév\t\t\tE-mail\t\tRendezvény\t\tJelentkezés\n");
 while (fread( &visitor, sizeof(visitor), 1, f)){
  count++;
  if(num) printf("%d\t\t%s\t\t%s\t\t%d\t\t%s\n",count, visitor.name, visitor.mail, visitor.id_guest, visitor.time);
 }
 fclose(f);
 return count;
}

void list_event(){
  system("clear");
  printf("Az elérhető események:"); 
  FILE *f;

  f = fopen(EVENTDATAFILE, "rb");
  if (f == NULL) {
    printf("\nNincsenek események létrehozva!");
    return;
  }

  printf("\nNév\t\t\tAzonositó\n");
  struct event event;
  while (fread( &event, sizeof(event), 1, f)) {
    printf("%s\t\t\t%d\n", event.name, event.id_event);
  }
  fclose(f);
}
