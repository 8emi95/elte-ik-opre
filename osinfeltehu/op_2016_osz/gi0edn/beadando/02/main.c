#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

void add_visitor(int * n) {
  char nev[1000];
  char mail[1000];
  int id;
  char ido[100];
  printf("Nev ( '_'-sal elvalasztva ): ");
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
  printf("Nev ( '_'-sal elvalasztva ): ");
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
  printf("Nev ( '_'-sal elvalasztva ): ");
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
  printf("8\tRendezvenyek lebonyolitas\n");
  printf("0\tKilepes\n");
}

///////////////////////////////2.beadandó

void process(int max_db, int id){	
	struct Visitor {
	  char name[1000];
	  char mail[1000];
	  int id;
	  char time[100];
	};
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
		return;
	}
	if(pipe(pipefd2) == -1){
		return;
	}	
	pid = fork();
	if (pid == 0)
    {// child process		
		write(pipefd2[1],"Megadott helyszinen vagyok",sizeof("Megadott helyszinen vagyok"));		
		read(pipefd[0], &db, sizeof(db));		
		printf("Vendegek listaja:");
		struct Visitor tomb[db];
		for(i=0;i<db;i++){
			read(pipefd[0], &v,l);
			printf(" %s",v.name);
			r = rand() % 100 + 1;
			if(r <= 10){
				tomb[ndb] = v;
				ndb++;
			}
		}
		printf("\n");
		r  = rand() % 101;
		write(pipefd2[1], &ndb, sizeof(ndb));
		write(pipefd2[1], &r, sizeof(r));
		for(i=0;i<ndb;i++){
			write(pipefd2[1], &tomb[i],l);
		}
		exit(EXIT_SUCCESS);
    }
    else
    {// parent process
		char* msg;
		read(pipefd2[0],&msg,1000);
		printf("A megadott helyszinen van, varja a resztvevoket!\n");
		struct Visitor visitors_tomb[max_db];
		db = 0;
		FILE * f;
		f = fopen("visitors", "rb");
		if (f == NULL) {
			return;
		}		
		const size_t line_size = 300;
		char* line = malloc(line_size);
		while (fgets(line, line_size, f) != NULL)  {
			char * temp = strtok (line,"\t");
			strcpy(v.name,temp);
			temp = strtok (NULL, "\t");
			strcpy(v.mail,temp);
			temp = strtok (NULL, "\t");
			v.id=atoi(temp);	  
			temp = strtok (NULL, "\t");
			strcpy(v.time,temp);
			if(v.id == id){
				visitors_tomb[db] = v;
				db++;	
			}
		}
		fclose(f);	
		write(pipefd[1], &db, sizeof(db));
		for(i=0;i<db;i++){
			write(pipefd[1], &visitors_tomb[i],l);
		}		
		wait(); 
		read(pipefd2[0], &ndb, sizeof(ndb));
		read(pipefd2[0], &r, sizeof(r));
		printf("Rendezmeny sikeressege: %d", r);
		if(ndb>0) 
			printf("\nNem jelentek meg:");		
		for(i=0;i<ndb;i++){
			read(pipefd2[0], &v,l);
			printf(" %s",v.name);
		}
		printf("\n");
        printf("Rendezveny vege!\n\n");
    }
}

void start_events(int db){	
  FILE * f;
  f = fopen("events", "rb");
  if (f == NULL) {
    return;
	}  
  const size_t line_size = 300;
	char* line = malloc(line_size);
	while (fgets(line, line_size, f) != NULL)  {		
		int id;
		char * temp = strtok (line,"\t");
		temp = strtok (NULL, "\t");
		id = atoi(temp);
		process(db,id);
	}
  fclose(f);
}

int main() {
	srand(time(NULL));
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
	case '8':
	  start_events(db);
	  printf("\n");
	  menu();
	  break;
    }
    printf("\n");
  } while (c != '0');
  return 0;
}