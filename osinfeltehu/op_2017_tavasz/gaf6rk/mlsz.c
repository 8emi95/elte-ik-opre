#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_SIZE 512
#define KERDES_FORMAT "%s;%d;%s;%s;%s;%s;\n"
#define TMP_FILE "tmp.txt"
#define KERDES_FILE "form.txt"

struct Kerdes {
   char   kerdes[150];
   char   valasz1[50];
   char   valasz2[50];
   char   valasz3[50];
   char   valasz4[50];
   time_t   ido;
};
typedef struct Kerdes KerdesS ;

void menu();
void listazas();
void hozzaadas();
void modositas();
void torles();
void kiir();
void beolvas();
void konzultacio();
void kerdes_hozzaad(KerdesS* v);
char* readLineFromConsole();
void RemoveSpaces(char* source);

struct Kerdes kerdessor[MAX_SIZE];

int kerdesek_szama = 0;

int main(int argc,char *argv[])
{
  beolvas();

  menu();

  return 0;
}

void menu()
{
  char menu[MAX_SIZE];
  while(1 < 2)
  {
    printf("\nNyomj egy gombot: (help: h)\t");
    strcpy(&menu, readLineFromConsole());

    if( strcmp(menu,"h") == 0 )
    {
      printf("Kerdesek listazasa:\t1\n");
      printf("Uj kerdes hozzaadasa:\t2\n");
      printf("Kerdes modositasa:\t3\n");
      printf("Kerdes torlese:\t\t4\n");
      printf("Konzultacio inditasa\t\t5\n");
      printf("Kilepes:\tPress CTRL + C to exit\n");
    }
    else if( strcmp(menu,"1") == 0 )
    {
      listazas();
    }
    else if( strcmp(menu,"2") == 0 )
    {
      hozzaadas();
    }
    else if( strcmp(menu,"3") == 0 )
    {
      modositas();
    }
    else if( strcmp(menu,"4") == 0 )
    {
      torles();
    }
    else if( strcmp(menu,"5") == 0 )
    {
      konzultacio();
    }
  }
}

void konzultacio()
{
  char varos[MAX_SIZE];
  printf("\nMelyik varosba menjen a kerdezobiztos:\t");
  strcpy(varos, readLineFromConsole());

  int status=0;

  int pipe_file[2];
  int Xpipe = pipe(pipe_file);
  if (Xpipe < 0)
  {
    perror("pipe(pipe_file) error!");
    exit(-1);
  }

  pid_t child = fork();
  if (child < 0) 
  {
    perror ("fork() error!");
    exit(-1);
  }

  /* parent process */
  if (0 != child) 
  {
    printf("Elindult PARENT\n");

    char buffer[MAX_SIZE];
    write(pipe_file[1], varos, MAX_SIZE);
    
    /* read ok msg */
    read(pipe_file[0], buffer, MAX_SIZE);
    printf("%s\n", buffer);

    srand(time(NULL));
    int r1 = rand() % kerdesek_szama;
    int r2 = rand() % kerdesek_szama;
    while(r1 == r2) {
      r2 = rand() % kerdesek_szama;
    }

    int r3 = rand() % kerdesek_szama;
    while(r1 == r3 || r2 == r3) {
      r3 = rand() % kerdesek_szama;
    }

    write(pipe_file[1], &kerdessor[r1], MAX_SIZE);
    write(pipe_file[1], &kerdessor[r2], MAX_SIZE);
    write(pipe_file[1], &kerdessor[r3], MAX_SIZE);

    waitpid(child,&status,0); 

    int eredmeny[12];
    read(pipe_file[0], &eredmeny, MAX_SIZE);

    printf("%s\n %s : %d\n %s : %d\n %s : %d\n %s : %d\n", kerdessor[r1].kerdes, kerdessor[r1].valasz1, eredmeny[0], kerdessor[r1].valasz2, eredmeny[1], kerdessor[r1].valasz3, eredmeny[2], kerdessor[r1].valasz4, eredmeny[3]);
    printf("%s\n %s : %d\n %s : %d\n %s : %d\n %s : %d\n", kerdessor[r2].kerdes, kerdessor[r2].valasz1, eredmeny[4], kerdessor[r2].valasz2, eredmeny[5], kerdessor[r2].valasz3, eredmeny[6], kerdessor[r2].valasz4, eredmeny[7]);
    printf("%s\n %s : %d\n %s : %d\n %s : %d\n %s : %d\n", kerdessor[r3].kerdes, kerdessor[r3].valasz1, eredmeny[8], kerdessor[r3].valasz2, eredmeny[9], kerdessor[r3].valasz3, eredmeny[10], kerdessor[r3].valasz4, eredmeny[11]);
  }
  /* child process */
  else
  {
    printf("Elindult GYEREK\n");

    char buffer[MAX_SIZE];
    read(pipe_file[0], buffer, MAX_SIZE);

    char* ok = "Megerkeztem.";
    write(pipe_file[1], ok, strlen(ok) + 1);

    struct Kerdes kerdesek[3];
    read(pipe_file[0], &kerdesek[0], MAX_SIZE);
    read(pipe_file[0], &kerdesek[1], MAX_SIZE);
    read(pipe_file[0], &kerdesek[2], MAX_SIZE);

    srand(time(NULL));
    int polgarok = rand() % (21 - 10) + 10;
    int valaszok[12];

    for (int i = 0; i < 12; i++)
    {
      valaszok[i] = 0;
    } 

    for (int i = 0; i < polgarok; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        struct Kerdes k = kerdesek[j];
        int hatar = 2;

        if(!strcmp(k.valasz3, ""))
        {
          hatar = 3;

          if (!strcmp(k.valasz4, ""))
          {
            hatar = 4;
          }
        }

        int p = rand() % (hatar);

        valaszok[(j * 4) + p]++;

        printf("%d * 4 + %d\n", j, p);

        for (int x = 0; x < 12; x++)
        {
          printf("%d", valaszok[x]);
        }
        printf("\n");
      }
    }
    write(pipe_file[1], &valaszok, MAX_SIZE);

    exit(0);
  }
}

void kiir()
{
  FILE *tmp;
  tmp = fopen(TMP_FILE,"w");

  for (int i = 0; i < kerdesek_szama; i++)
  {
    fprintf(tmp, KERDES_FORMAT, kerdessor[i].kerdes, kerdessor[i].ido, kerdessor[i].valasz1, kerdessor[i].valasz2, kerdessor[i].valasz3, kerdessor[i].valasz4);
  }

  fclose(tmp);

  remove(KERDES_FILE);
  rename(TMP_FILE, KERDES_FILE);
}

void beolvas()
{
  FILE *f;
  f = fopen(KERDES_FILE,"r");
  char line[MAX_SIZE];

  const char s[2] = ";";
  char *token;

  while(fgets(line,sizeof(line),f) != NULL)
  {
    struct Kerdes r;

    size_t idx  = 0;
    token = strtok(line, s);


    int hol = 0;

   while( token != NULL )
   {
      if (hol == 0)
      {
        strcpy(&r.kerdes, token);
      }
      if (hol == 1)
      {
        r.ido = atoi(token);
      }
      if (hol == 2)
      {
        strcpy(&r.valasz1, token);
      }
      if (hol == 3)
      {
        strcpy(&r.valasz2, token);
      }
      if (hol == 4)
      {
        strcpy(&r.valasz3, token);
      }
      if (hol == 5)
      {
        strcpy(&r.valasz4, token);
      }

      token = strtok(NULL, s);
      hol++;
   }

    kerdessor[kerdesek_szama++] = r;
  }

  fclose(f);
}

void listazas()   //LISTÁZÁS
{
  for (int i = 0; i < kerdesek_szama; i++)
  {
    printf("%d. %s:\n%s; %s; %s; %s;\n", i+1, kerdessor[i].kerdes, kerdessor[i].valasz1, kerdessor[i].valasz2, kerdessor[i].valasz3, kerdessor[i].valasz4);
  }
}

void kerdes_hozzaad(KerdesS* v)
{
  printf("Add meg a kerdest (min. 2 valaszlehetoseg, ha nincs 3 es 4 valasz akkor irj kotojelet");
  printf("\nKerdes:\t");
  strcpy(v->kerdes, readLineFromConsole());
  // scanf("%30[^\n]", v->kerdes);
  printf("\nValasz 1:\t");
  strcpy(v->valasz1, readLineFromConsole());
  // scanf("%s", v->valasz1);
  printf("\nValasz 2:\t");
  strcpy(v->valasz2, readLineFromConsole());
  // scanf("%s", v->valasz2);
  printf("\nValasz 3:\t");
  strcpy(v->valasz3, readLineFromConsole());
  // scanf("%s", v->valasz3);
  printf("\nValasz 4:\t");
  strcpy(v->valasz4, readLineFromConsole());
  // scanf("%s", v->valasz4);
}

void hozzaadas()    //BESZÚRÁS
{
  struct Kerdes v;

  kerdes_hozzaad(&v);

  v.ido = time(NULL);

  kerdessor[kerdesek_szama++] = v;

  kiir();

  printf("A kerdes mentve: %s. \n", v.kerdes);
}

char* readLineFromConsole()
{
        char    *buffer;
        size_t  n = 1024;
        buffer = malloc(n);
        getline(&buffer, &n, stdin);

        if ((strlen(buffer)>0) && (buffer[strlen (buffer) - 1] == '\n'))
        buffer[strlen (buffer) - 1] = '\0';

        return buffer;
}

void modositas() {
  int k;
  printf("Hanyadik kerdest akarod modositani:\t");
  k = atoi(readLineFromConsole()) - 1;

  for (int i = 0; i < kerdesek_szama; i++)
  {
    if( i == k )
    {
      kerdes_hozzaad(&kerdessor[i]);
    }
  }

  kiir();
}

void torles()   //TÖRLÉS
{

  int k;
  printf("Hanyadik kerdest akarod torolni:\t");
  k = atoi(readLineFromConsole()) - 1;

  if (k > kerdesek_szama) {
    printf("Nincs ilyen azonositoval kerdes: %d\n", k);

    return;
  }



  int siker = 1;

  for (int i = 0; i < kerdesek_szama; i++)
  {
    if( i == k )
    {
      for (int j = i; j < kerdesek_szama; j++){
          kerdessor[j] = kerdessor[j+1];
      }

      kerdesek_szama--;
      siker = 0;
    }
  }

  if (siker == 0) {
    kiir();
    printf("A kerdes torolve lett.\n");
  } else {
    printf("Nem sikerult a torles.\n");
  }
}































