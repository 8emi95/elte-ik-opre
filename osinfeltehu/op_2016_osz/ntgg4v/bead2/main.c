#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>
#include <unistd.h> //pipe

#define VISITORDATAFILE "visitordata"
#define EVENTDATAFILE "eventdata"

struct Visitor {
  char name[1000];
  char mail[1000];
  int id;
  char time[100];
};

struct Event {
  char name[1000];
  int id;
};

void clear(void) {
  while (getchar() != '\n');
}

void copyVisitorFile(char * fromFileName, char * toFileName) {
  FILE * to = fopen(toFileName, "wb");
  if (to == NULL) {
    perror("File opening error\n");
    return;
  }
  FILE * from = fopen(fromFileName, "rb");
  if (from == NULL) {
    perror("File opening error\n");
    return;
  }
  struct Visitor v;
  while (fread( & v, sizeof(v), 1, from)) {
    fwrite( & v, sizeof(v), 1, to);
  }
  fclose(to);
  fclose(from);
  remove(fromFileName);
}

void copyEventFile(char * fromFileName, char * toFileName) {
  FILE * to = fopen(toFileName, "wb");
  if (to == NULL) {
    perror("File opening error\n");
    return;
  }
  FILE * from = fopen(fromFileName, "rb");
  if (from == NULL) {
    perror("File opening error\n");
    return;
  }
  struct Event e;
  while (fread( & e, sizeof(e), 1, from)) {
    fwrite( & e, sizeof(e), 1, to);
  }
  fclose(to);
  fclose(from);
  remove(fromFileName);
}

void createNewVisitor(int * n) {
  struct Visitor v;
  printf("Nev: ");
  scanf("%s", v.name);
  printf("E-mail: ");
  scanf("%s", v.mail);
  printf("Rendezveny azonosito: ");
  scanf("%d", & v.id);
  clear();

  time_t rawtime;
  struct tm * timeinfo;
  time( & rawtime);
  timeinfo = localtime( & rawtime);
  strftime(v.time, 80, "%c", timeinfo);

  FILE * file = fopen(VISITORDATAFILE, "ab");
  if (file == NULL) {
    perror("File opening error\n");
    return;
  }
  if (file != NULL) {
    fwrite( & v, sizeof(v), 1, file);
    ( * n) ++;
    fclose(file);
  }
  printf("Az uj latogato sorszama: %d\n", * n);
}

//b != 0 -> kiírja a beolvasott adatokat
int readFromFile(int b) {
  int db = 0;
  FILE * f;
  f = fopen(VISITORDATAFILE, "rb");
  if (f == NULL) {
    return 0;
  }
  struct Visitor v;
  if(b) printf("\nSorszam\tNev\tE-mail\tRendezveny\tJelentkezes\n");
  while (fread( & v, sizeof(v), 1, f)) {
	db++;
    if (b) printf("%d\t%s\t%s\t%d\t%s\n",db, v.name, v.mail, v.id, v.time);
  }
  fclose(f);
  return db;
}

void deleteVisitor() {
  int db = readFromFile(1);
  int n = 0;
  
  printf("Sorszam: ");
  scanf("%d", & n);
  clear();
  if (n > db || n < 1) {
    printf("Hibas sorszam!\n");
    return;
  }

  FILE * f;
  f = fopen(VISITORDATAFILE, "rb");
  if (f == NULL) {
    perror("File opening error\n");
    return;
  }
  FILE * tmp;
  tmp = fopen("tmp", "wb");
  if (tmp == NULL) {
    perror("File opening error\n");
    return;
  }
  struct Visitor v;
  int i = 0;
  while (fread( & v, sizeof(v), 1, f)) {
    i++;
    if (i != n) fwrite( & v, sizeof(v), 1, tmp);
  }
  fclose(f);
  fclose(tmp);
  copyVisitorFile("tmp", VISITORDATAFILE);
}

void modifyVisitor() {
  int db = readFromFile(1);
  int n = 0;
  
  printf("Sorszam: ");
  scanf("%d", &n);
  clear();
  if (n > db || n < 1) {
    printf("Hibas sorszam!\n");
    return;
  }
  
  struct Visitor modifiedVisitor;
  printf("Nev: ");
  scanf("%s", modifiedVisitor.name);
  printf("E-mail: ");
  scanf("%s", modifiedVisitor.mail);
  printf("Rendezveny azonosito: ");
  scanf("%d", & modifiedVisitor.id);
  clear();
  FILE * f;

  f = fopen(VISITORDATAFILE, "rb");
  if (f == NULL) {
    perror("File opening error\n");
    return;
  }
  FILE * tmp;
  tmp = fopen("tmp", "wb");
  if (tmp == NULL) {
    perror("File opening error\n");
    return;
  }
  struct Visitor v;
  int i = 0;
  while (fread( & v, sizeof(v), 1, f)) {
    i++;
    if (i == n) {
      strcpy(modifiedVisitor.time, v.time);
      v = modifiedVisitor;
    }
    fwrite( & v, sizeof(v), 1, tmp);
  }
  fclose(f);
  fclose(tmp);
  copyVisitorFile("tmp", VISITORDATAFILE);
}

void createNewEvent() {
  struct Event e;
  printf("Nev: ");
  scanf("%s", e.name);
  printf("Rendezveny azonosito: ");
  scanf("%d", & e.id);
  clear();

  FILE * file = fopen(EVENTDATAFILE, "ab");
  if (file != NULL) {
    fwrite( & e, sizeof(e), 1, file);
    fclose(file);
  }
}

void listEvents() {
  FILE * f;
  printf("\nNev\tAzonosito\n");
  f = fopen(EVENTDATAFILE, "rb");
  if (f == NULL) {
    return;
  }
  struct Event e;
  while (fread( & e, sizeof(e), 1, f)) {
    printf("%s\t%d\n", e.name, e.id);
  }
  fclose(f);
}

void deleteEvent() {
  int n = 0;
  printf("Azonosito: ");
  scanf("%d", & n);
  clear();

  FILE * f;
  f = fopen(VISITORDATAFILE, "rb");
  if (f == NULL) {
    perror("File opening error\n");
    return;
  }
  FILE * tmp;
  tmp = fopen("tmp", "wb");
  if (tmp == NULL) {
    perror("File opening error\n");
    return;
  }
  struct Visitor v;
  while (fread( & v, sizeof(v), 1, f)) {
    if (v.id != n) fwrite( & v, sizeof(v), 1, tmp);
  }
  fclose(f);
  fclose(tmp);
  copyVisitorFile("tmp", VISITORDATAFILE);

  f = fopen(EVENTDATAFILE, "rb");
  if (f == NULL) {
    perror("File opening error\n");
    return;
  }
  tmp = fopen("tmp", "wb");
  if (tmp == NULL) {
    perror("File opening error\n");
    return;
  }
  struct Event e;
  while (fread( & e, sizeof(e), 1, f)) {
    if (e.id != n) fwrite( & e, sizeof(e), 1, tmp);
  }
  fclose(f);
  fclose(tmp);
  copyEventFile("tmp", EVENTDATAFILE);
}

void start_process(int maxVisitor, int eventID){
	size_t l = sizeof(struct Visitor);
	int pipefd[2];
	int pipefd2[2];
	int db;
	int i;
	int ndb = 0;
	int r;
	struct Visitor v;
	
	pid_t pid;
	if(pipe(pipefd) == -1){
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	if(pipe(pipefd2) == -1){
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	pid = fork();
	if(pid == -1)
	{
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}
	
	//pipefd[0] : read, pipefd[1] : write
	if (pid == 0)
    {   // child process
		
		write(pipefd2[1],"Itt vagyok a helyszinen",sizeof("Itt vagyok a helyszinen"));
		
		read(pipefd[0], &db, sizeof(db));
		printf("Vendegek szama: %d\n", db);
		
		struct Visitor notArrived[db];

		for(i=0;i<db;i++){
			read(pipefd[0], &v,l);
			r = rand() % 100 + 1;
			if(r <= 10){
				notArrived[ndb] = v;
				ndb++;
			} else {
				printf("vendeg megerkezett: %s\n",v.name);
			}
		}
		r  = rand() % 101;
		
		write(pipefd2[1], &ndb, sizeof(ndb));
		write(pipefd2[1], &r, sizeof(r));
		for(i=0;i<ndb;i++){
			write(pipefd2[1], &notArrived[i],l);
		}

		printf("Gyerek befejezve!\n");
		exit(EXIT_SUCCESS);
    }
    else
    {   // parent process
		char* msg;
		read(pipefd2[0],&msg,1000);
		printf("Megerkezett a helyszinre, latogatok kuldese.\n");
		struct Visitor visitors[maxVisitor];
		
		db = 0;
		FILE * f;
		f = fopen(VISITORDATAFILE, "rb");
		if (f == NULL) {
			perror("File nyitasi hiba");
			exit(EXIT_FAILURE);
		}
		while (fread( & v, l, 1, f)) {
			if(v.id == eventID){
				visitors[db] = v;
				db++;	
			}
		}
		fclose(f);

		write(pipefd[1], &db, sizeof(db));
		for(i=0;i<db;i++){
			write(pipefd[1], &visitors[i],l);
		}
		
		wait(); // Várakozás a rendezvény bejefezésére
		read(pipefd2[0], &ndb, sizeof(ndb));
		read(pipefd2[0], &r, sizeof(r));
		printf("A rendezmeny sikeressége: %d\n", r);
		printf("Ennyi vendeg nem erkezett meg: %d\n", ndb);
		if(ndb>0) printf("Ok voltak azok:\n");
		
		for(i=0;i<ndb;i++){
			read(pipefd2[0], &v,l);
			printf("vendeg: %s\n",v.name);
		}
        printf("Rendezveny vege!\n\n");
    }
}

void loop_events(int db){
  FILE * f;
  f = fopen(EVENTDATAFILE, "rb");
  if (f == NULL) {
    return;
  }
  struct Event e;
  while (fread( & e, sizeof(e), 1, f)) {
    start_process(db,e.id);
  }
  fclose(f);
}


void writeMenu() {
  rand();
  printf("Uj latogato: 1\n");
  printf("Latogatok listazasa: 2\n");
  printf("Latogato adatainak szerkeztese: 3\n");
  printf("Latogato torlese: 4\n");
  printf("Uj rendezveny: 5\n");
  printf("Rendezveny torlese: 6\n");
  printf("Rendezvenyek listazasa: 7\n");
  printf("Rendezvenyek lebonyolitas: 8\n");
  printf("Kilepes: 9\n");
}

int main() {
  char c;
  srand(time(NULL));
  int db = readFromFile(0);
  do {
    writeMenu();
    scanf("%c", & c);
    clear();
    switch (c) {
    case '1':
	  db = readFromFile(0);
      createNewVisitor(&db);
      break;
    case '2':
      readFromFile(1);
      break;
    case '3':
      modifyVisitor();
      break;
    case '4':
      deleteVisitor();
      break;
    case '5':
      createNewEvent();
      break;
    case '6':
      deleteEvent();
      break;
    case '7':
      listEvents();
      break;
	case '8':
	  db = readFromFile(0);
	  loop_events(db);
	  break;
    }
    printf("\n");
  } while (c != '9');
  return 0;
}