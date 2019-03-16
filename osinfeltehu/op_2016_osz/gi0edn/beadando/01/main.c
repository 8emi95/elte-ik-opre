#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h> 
#include <string.h>
#include <time.h>


void add_visitor(int * n) {
  char nev[1000];
  char mail[1000];
  int id;
  char ido[100];
  printf("Nev: ");
  scanf("%s", nev);
  printf("E-mail: ");
  scanf("%s", mail);
  printf("Rendezveny azonosito: ");
  scanf("%d", &id);

  time_t rawtime;
  struct tm * timeinfo;
  time( & rawtime);
  timeinfo = localtime( & rawtime);
  strftime(ido, 80, "%c", timeinfo);


  FILE * file = fopen("visitors", "ab");
  if (file == NULL) {
    perror("File nyitasi hiba!\n");
    return;
  }
  if (file != NULL) {
  
	fprintf(file, "%s\t%s\t%i\t%s\n", nev,mail,id,ido);
    ( * n) ++;
    fclose(file);
  }
  printf("Az uj latogato a(z) %d sorszamot kapta\n", * n);
}

int list_visitors(int i) {
  int db = 0;
  FILE * f;
  f = fopen("visitors", "rb");
  if (f == NULL)
	  return 0;
  
	if(i==0)
		printf("\nSorszam\tNev\tE-mail\tRendezveny\tJelentkezes datuma\n");
  
	const size_t line_size = 300;
	char* line = malloc(line_size);
	while (fgets(line, line_size, f) != NULL)  {
	db++;
	if(i==0){
	printf("%d\t",db);	
    printf(line);
	}
	}
  
  
  
  fclose(f);
  return db;
}

void remove_visitor() {
  int db = list_visitors(0);
  int n = 0;
  
  printf("Sorszam: ");
  scanf("%d", & n);
  while (getchar() != '\n');
  if (n > db || n < 1) {
    printf("Hibas sorszam!\n");
    return;
  }

  FILE * f;
  f = fopen("visitors", "rb");
  if (f == NULL) {
    perror("File nyitasi hiba!\n");
    return;
  }
  FILE * tmp;
  tmp = fopen("tmp", "wb");
  if (tmp == NULL) {
    perror("File nyitasi hiba!\n");
    return;
  }
 
  
  {int i=0;
  const size_t line_size = 300;
	char* line = malloc(line_size);
	while (fgets(line, line_size, f) != NULL)  {
	i++;
	if (i != n) 
	fprintf(tmp, "%s", line);
	}
  }
  fclose(f);
  fclose(tmp);
  
  {
	    FILE * f;
  f = fopen("visitors", "wb");
  if (f == NULL) {
    perror("File nyitasi hiba!\n");
    return;
  }
  FILE * tmp;
  tmp = fopen("tmp", "rb");
   if (tmp == NULL) {
    perror("File nyitasi hiba!\n");
    return;
  }
	  
	const size_t line_size = 300;
	char* line = malloc(line_size);
	while (fgets(line, line_size, tmp) != NULL)  {
	fprintf(f, "%s", line);
	}
	fclose(f);
	fclose(tmp);
  }

  
 
}

void modify_visitor() {
  int db = list_visitors(0);
  int n = 0;
  
  printf("Sorszam: ");
  scanf("%d", &n);
  while (getchar() != '\n');
  if (n > db || n < 1) {
    printf("Hibas sorszam!\n");
    return;
  }
  
  char nev[1000];
  char mail[1000];
  int id;
  char ido[1000];
  printf("Nev: ");
  scanf("%s", nev);
  printf("E-mail: ");
  scanf("%s", mail);
  printf("Rendezveny azonosito: ");
  scanf("%d", &id);
   

  FILE * f;

  f = fopen("visitors", "rb");
  if (f == NULL) {
    perror("File nyitasi hiba!\n");
    return;
  }
  FILE * tmp;
  tmp = fopen("tmp", "wb");
  if (tmp == NULL) {
    perror("File nyitasi hiba!\n");
    return;
  }
  
 
  
  
  {int i=0;
  const size_t line_size = 300;
	char* line = malloc(line_size);
	while (fgets(line, line_size, f) != NULL)  {
	i++;
	if (i != n) {
	fprintf(tmp, "%s", line);
	}else{
		int a=1;
		char * temp = strtok (line,"\t\n");
		while (temp != NULL)
		{	
			if(a>3)
				strcat(ido,temp);
			temp = strtok (NULL, "\t\n");
			a++;
		}
		
		fprintf(tmp, "%s\t%s\t%i\t%s\n", nev,mail,id,ido);
	}
  }
  fclose(f);
  fclose(tmp);
  
  {
	    FILE * f;
  f = fopen("visitors", "wb");
  if (f == NULL) {
    perror("File nyitasi hiba!\n");
    return;
  }
  FILE * tmp;
  tmp = fopen("tmp", "rb");
   if (tmp == NULL) {
    perror("File nyitasi hiba!\n");
    return;
  }
	const size_t line_size = 300;
	char* line = malloc(line_size);
	while (fgets(line, line_size, tmp) != NULL)  {
	fprintf(f, "%s", line);
	}
	fclose(f);
	fclose(tmp);
  }
}
}

void add_event() {

char nev[1000];
  int id;
  printf("Nev: ");
  scanf("%s", nev);
  printf("Rendezveny azonosito: ");
  scanf("%d", & id);

  FILE * file = fopen("events", "ab");
  if (file != NULL) {
	fprintf(file, "%s\t%i\n", nev,id);
    fclose(file);
  }
}

void list_events() {
  FILE * f;
  printf("\nNev\tAzonosito\n");
  f = fopen("events", "rb");
  if (f == NULL) {
    return;
  }
  
  const size_t line_size = 300;
	char* line = malloc(line_size);
	while (fgets(line, line_size, f) != NULL)  {	
    printf(line);
	}
  
  fclose(f);
}

void remove_event() {
  int n;
  printf("Azonosito: ");
  scanf("%i", &n);
  while (getchar() != '\n');

  FILE * f;
  f = fopen("events", "rb");
  if (f == NULL) {
    perror("File nyitasi hiba!\n");
    return;
  }
  FILE * tmp;
  tmp = fopen("tmp", "wb");
  if (tmp == NULL) {
    perror("File nyitasi hiba!\n");
    return;
  }
  int a=0;
  int b=0;
   {
  const size_t line_size = 300;
	char* line = malloc(line_size);
	while (fgets(line, line_size, f) != NULL)  {
	a++;
	char * temp = strtok (line,"\t\n");
	temp = strtok (NULL, "\t\n");
	if(*temp == (n + '0'))
		b=a;
	
	}
  }
  fclose(f);
  fclose(tmp);
  
  {
	  FILE * f;
  f = fopen("events", "rb");
  if (f == NULL) {
    perror("File nyitasi hiba!\n");
    return;
  }
  FILE * tmp;
  tmp = fopen("tmp", "wb");
  if (tmp == NULL) {
    perror("File nyitasi hiba!\n");
    return;
  }
	  
	  
  const size_t line_size = 300;
	char* line = malloc(line_size);
	while (fgets(line, line_size, f) != NULL)  {
		fprintf(tmp, "%s", line);
	}
	
	fclose(f);
  fclose(tmp);
  }
  
  
  {
	    FILE * f;
  f = fopen("events", "wb");
  if (f == NULL) {
    perror("File nyitasi hiba!\n");
    return;
  }
  FILE * tmp;
  tmp = fopen("tmp", "rb");
   if (tmp == NULL) {
    perror("File nyitasi hiba!\n");
    return;
  }
	a=0;  
	const size_t line_size = 300;
	char* line = malloc(line_size);
	while (fgets(line, line_size, tmp) != NULL)  {
		a++;
		if(a!=b)
			fprintf(f, "%s", line);
	
	
	}
	fclose(f);
	fclose(tmp);
  }
  
  
  
  
}

void menu() {
  printf("1\tLatogato hozzaadasa\n");
  printf("2\tLatogatok listazasa\n");
  printf("3\tLatogato szerkesztese\n");
  printf("4\tLatogato torlese\n");
  printf("5\tRendezveny hozzaadasa\n");
  printf("6\tRendezvenyek listazasa\n");
  printf("7\tRendezveny torlese\n");
  printf("0\tKilepes\n");
}

int main() {
  char c;
  int db = list_visitors(1);
  menu();
  do {
    scanf("%c", & c);
    switch (c) {
    case '1':
      add_visitor(&db);
	  printf("\n");
	  menu();
      break;
    case '2':
      list_visitors(0);
	  printf("\n");
	  menu();
      break;
    case '3':
      modify_visitor();
	  printf("\n");
	  menu();
      break;
    case '4':
      remove_visitor();
	  printf("\n");
	  menu();
      break;
    case '5':
      add_event();
	  printf("\n");
	  menu();
      break;
    case '6':
      list_events();
	  printf("\n");
	  menu();
      break;
    case '7':
      remove_event();
	  printf("\n");
	  menu();
      break;
    }
    printf("\n");
  } while (c != '0');
  return 0;
}