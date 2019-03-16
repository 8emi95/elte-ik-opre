#include <stdio.h>
#include <string.h>
#include <time.h>
#include "kos.h"

#define MAX_RENDEZVENY 256

char fn_rendezvenyek[] = "rendezveny.data";
char fn_vendegek[] = "vendeg.data";
char fn_tmp[] = "tmp.data";

struct rendezveny_t rendezvenyek[MAX_RENDEZVENY];
int rendezvenyek_n = 0;
int rendezveny_max_id = 0;

void empty_stdin() {
  char c;
  while ((c=getchar())!='\n' && c!=EOF) ;
}

void beolvas_rendezvenyek() {
  FILE * f;
  f = fopen(fn_rendezvenyek, "r");
  //printf("%d %d\n", rendezvenyek_n, rendezveny_max_id);
  if (f) {
    while (!(feof(f))) {
      if (fread(&(rendezvenyek[rendezvenyek_n]), sizeof(struct rendezveny_t), 1, f)) {
        if (rendezveny_max_id<rendezvenyek[rendezvenyek_n].id) {
          rendezveny_max_id = rendezvenyek[rendezvenyek_n].id;
        }
        rendezvenyek_n++;
      }
      //printf(".");
    }
    fclose(f);
  }
  //printf("%d %d\n", rendezvenyek_n, rendezveny_max_id);
}

void kiir_rendezvenyek() {
  FILE * f;
  f = fopen(fn_rendezvenyek, "w");
  fwrite(rendezvenyek, sizeof(struct rendezveny_t), rendezvenyek_n, f);
  fclose(f);
}

void uj_rendezveny() {
  printf("Kérem adja meg az új rendezvény nevét: ");
  //struct rendezveny_t rendezveny;
  char * buffer = NULL;
  size_t buf_size = 0;
  buf_size = getline(&buffer, &buf_size, stdin);
  buffer[buf_size-1] = '\0';
  strncpy(rendezvenyek[rendezvenyek_n].nev, buffer, 255);
  rendezvenyek[rendezvenyek_n].id = ++rendezveny_max_id;
  rendezvenyek[rendezvenyek_n].vendegek = 0;
  //strcpy(rendezvenyek[rendezvenyek_n].nev, rendezveny
  //rendezvenyek_n++;
  FILE * f;
  f = fopen(fn_rendezvenyek, "a");
  fwrite(&(rendezvenyek[rendezvenyek_n]), sizeof(struct rendezveny_t), 1, f);
  rendezvenyek_n++;
  fclose(f);
  printf("Új rendezvény felvitele megtörtént. Azonosítója: %d\n", rendezveny_max_id);
}

void lista_rendezvenyek() {
  int i;
  printf("Rendezvények száma: %d\n", rendezvenyek_n);
  printf("----------\n");
  for (i=0; i<rendezvenyek_n; i++) {
    printf("%d - %s\n", rendezvenyek[i].id, rendezvenyek[i].nev);
  }
  printf("----------\n");
}

/*void mod_rendezveny() {
  int n,i;
  printf("Kerem adja meg a rendezveny azonositojat: ");
  scanf("%d", &n);
  for (i=0; i<rendezvenyek_n; i++) {
    if (rendezvenyek[i].id == n)
      break;
  }
  if (i==rendezvenyek_n) {
    printf("Ismeretlen azonosito");
  } else {
    printf("A rendeyveny regi neve: %s", &(rendezvenyek[i].nev));
  }
}*/

void torol_rendezveny() {
  int n,i;
  printf("Kerem adja meg a rendezveny azonositojat: ");
  scanf("%d", &n); empty_stdin();
  for (i=0; i<rendezvenyek_n; i++) {
    if (rendezvenyek[i].id == n)
      break;
  }
  if (i==rendezvenyek_n) {
    printf("Ismeretlen azonosito\n");
  } else {
    while (i<rendezvenyek_n-1) {
      memcpy(rendezvenyek+i, rendezvenyek+i+1, sizeof(struct rendezveny_t));
      i++;
    }
    rendezvenyek_n--;
    printf("Rendezveny torolve\n");
  }
  kiir_rendezvenyek();

  FILE * f;
  FILE * f2;
  f = fopen(fn_vendegek, "r");
  if (!f) {
    return;
  }
  f2 = fopen(fn_tmp, "w");
  struct vendeg_t vendeg;
  while (!feof(f)) {
    if (fread(&vendeg, sizeof(struct vendeg_t), 1, f)) {
      if (vendeg.rendezveny_id != n) {
        fwrite(&vendeg, sizeof(struct vendeg_t), 1, f2);
      }
    }
  }
  fclose(f);
  fclose(f2);
  remove(fn_vendegek);
  rename(fn_tmp, fn_vendegek);
  printf("A rendezvény törlése megtörtént\n");
}

void uj_vendeg() {
  struct vendeg_t vendeg;
  printf("Kérem adja meg az új vendég nevét: ");
  char * buffer = NULL;
  size_t buf_size = 0;
  buf_size = getline(&buffer, &buf_size, stdin);
  buffer[buf_size-1] = '\0';
  strncpy(vendeg.nev, buffer, 255);
  printf("Kérem adja meg az új vendég mail címét: ");
  buffer = NULL;
  buf_size = 0;
  buf_size = getline(&buffer, &buf_size, stdin);
  buffer[buf_size-1] = '\0';
  strncpy(vendeg.mail, buffer, 255);
  int n = 0;
  int i;
  do {
    printf("Kerem adja meg a rendezveny azonositojat: ");
    scanf("%d", &n); empty_stdin();
    for (i=0; i<rendezvenyek_n; i++) {
      if (rendezvenyek[i].id == n)
        break;
    }
    if (i==rendezvenyek_n) {
      printf("Ismeretlen azonosito\n");
      n = 0;
    }
  } while (n==0);
  vendeg.rendezveny_id = n;
  vendeg.hanyadik_vendeg = ++rendezvenyek[n].vendegek;
  vendeg.jelentkezes_ideje = time(NULL);
  FILE * f;
  f = fopen(fn_vendegek, "a");
  fwrite(&vendeg, sizeof(struct vendeg_t), 1, f);
  fclose(f);
  kiir_rendezvenyek;
  printf("A vendég felvitele megtörtént. Sorászma: %d\n", vendeg.hanyadik_vendeg);
}

void lista_vendegek() {
  int i;
  struct vendeg_t vendeg;
  struct tm * timeinfo;
  char timestr[32];
  FILE * f;
  f = fopen(fn_vendegek, "r");
  printf("Vendégek listája\n");
  printf("----------\n");
  int n = 0;
  if (f) {
    while (!(feof(f))) {
      if (fread(&vendeg, sizeof(struct vendeg_t), 1, f)) {
        for (i=0; i<rendezvenyek_n; i++) {
          if (rendezvenyek[i].id == vendeg.rendezveny_id)
            break;
        }
        timeinfo = localtime(&vendeg.jelentkezes_ideje);
        strftime(timestr, 31, "%Y-%m-%d %H-%M-%S", timeinfo);
        printf("%d: %s (%s)\n", ++n, vendeg.nev, vendeg.mail);
        printf("  %s - %s\n", timestr, rendezvenyek[i].nev);
        printf("----------\n");
      }
    }
    fclose(f);
  }
//  for (i=0; i<rendezvenyek_n; i++) {
//  printf("----------\n");
}

void mod_vendeg() {
  FILE * f;
  f = fopen(fn_vendegek, "r+");
  if (!f) {
    printf("Nincs vendég a rendszerben\n");
    return;
  }
  fseek(f, 0, SEEK_END);
  int vendegek_n = ftell(f) / sizeof(struct vendeg_t);
  if (vendegek_n==0) {
    printf("Nincs vendég a rendszerben\n");
    fclose(f);
    return;
  }
  int n,i;
  printf("Kerem adja meg a vendeg sorszamat (1-%d): ", vendegek_n);
  scanf("%d", &n); empty_stdin();
  if (n<1 || n>vendegek_n) {
    printf("Ismeretlen sorszám.\n");
    fclose(f);
    return;
  }
  fseek(f, (n-1)*sizeof(struct vendeg_t), SEEK_SET);
  struct vendeg_t vendeg;
  fread(&vendeg, sizeof(struct vendeg_t), 1, f);
  printf("Kérem adja meg a vendég nevét (%s): ", vendeg.nev);
  char * buffer = NULL;
  size_t buf_size = 0;
  buf_size = getline(&buffer, &buf_size, stdin);
  buffer[--buf_size] = '\0';
  if (buf_size>0) {
    strncpy(vendeg.nev, buffer, 255);
  }
  printf("Kérem adja meg a vendég mail címét (%s): ", vendeg.mail);
  buffer = NULL;
  buf_size = 0;
  buf_size = getline(&buffer, &buf_size, stdin);
  buffer[--buf_size] = '\0';
  if (buf_size>0) {
    strncpy(vendeg.mail, buffer, 255);
  }
  fseek(f, (n-1)*sizeof(struct vendeg_t), SEEK_SET);
  fwrite(&vendeg, sizeof(struct vendeg_t), 1, f);
  printf("A módosítás megtörtént\n");
  fclose(f);
}

void torol_vendeg() {
  FILE * f;
  FILE * f2;
  f = fopen(fn_vendegek, "r");
  if (!f) {
    printf("Nincs vendég a rendszerben\n");
    return;
  }
  fseek(f, 0, SEEK_END);
  int vendegek_n = ftell(f) / sizeof(struct vendeg_t);
  if (vendegek_n==0) {
    printf("Nincs vendég a rendszerben\n");
    fclose(f);
    return;
  }
  int n,i;
  printf("Kerem adja meg a vendeg sorszamat (1-%d): ", vendegek_n);
  scanf("%d", &n); empty_stdin();
  if (n<1 || n>vendegek_n) {
    printf("Ismeretlen sorszám.\n");
    fclose(f);
    return;
  }
  fseek(f, 0, SEEK_SET);
  f2 = fopen(fn_tmp, "w");
  struct vendeg_t vendeg;
  for (i=1; i<n; i++) {
    fread(&vendeg, sizeof(struct vendeg_t), 1, f);
    fwrite(&vendeg, sizeof(struct vendeg_t), 1, f2);
  }
  fread(&vendeg, sizeof(struct vendeg_t), 1, f);
  while (!feof(f)) {
    if (fread(&vendeg, sizeof(struct vendeg_t), 1, f)) {
      fwrite(&vendeg, sizeof(struct vendeg_t), 1, f2);
    }
  }
  printf("A törlés megtörtént\n");
  fclose(f);
  fclose(f2);
  remove(fn_vendegek);
  rename(fn_tmp, fn_vendegek);
}

void menu() {
  char * opcio = NULL;
  size_t size = 0;
  do {
    opcio = NULL;
    size = 0;
    printf("a) rendezvenyek listaja\n");
    printf("b) uj rendezveny\n");
//    printf("c( rendezveny modositasa\n");
    printf("d) rendezveny torlese\n");
    printf("e) vendegek listaja\n");
    printf("f) uj vendeg\n");
    printf("g) vendeg modositasa\n");
    printf("h) vendeg torlese\n");
    printf("x) kilepes\n");
    printf("Kerem valasson: ");
    getline(&opcio, &size, stdin);
    switch (opcio[0]) {
      case 'a': lista_rendezvenyek(); break;
      case 'b': uj_rendezveny(); break;
//      case 'c': mod_rendezveny(); break;
      case 'd': torol_rendezveny(); break;
      case 'e': lista_vendegek(); break;
      case 'f': uj_vendeg(); break;
      case 'g': mod_vendeg(); break;
      case 'h': torol_vendeg(); break;
    }
  } while (opcio[0]!='x');
}

void main(int argc, char** argv) {
  printf("King Of Stands - vendégnyilvántartás\n");
  printf("------------------------------------\n");
  beolvas_rendezvenyek();
  menu();
}
