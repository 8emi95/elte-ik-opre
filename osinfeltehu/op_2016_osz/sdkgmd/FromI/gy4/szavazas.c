#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <sys/types.h> //waitpid
#include <errno.h> //perror 
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

struct adat
{
  int azonosito;
  int ervenyes;
};

pid_t childs[2];
int fd[2];

int szavazokszama;

void handler( int );
void elnok();
void pecsetelo();
void ellenorzo();


int main( int argc, char** argv )
{
  signal( SIGUSR1, handler );
  szavazokszama = atoi( argv[1] );

  if( mkfifo( "ellenor.fifo", S_IRUSR|S_IWUSR ) ) { perror( "Fifo letrehoozasi hiba\n" ); exit( 1 ); } 

  if( pipe( fd ) < 0 ) { perror( "Hibas a cso letrehozas\n" ); exit( EXIT_FAILURE ); }
   
  pid_t ellen = fork();
  printf( "ellen pid now: %i \n", ellen );
  if( ellen > 0 )
  {
    pid_t pecset = fork();
    printf( "pecset pid now: %i \n", pecset );
    if( pecset > 0 )
    {
      elnok();
      wait( NULL );
      wait( NULL );
    }
    else 
    {
      pecsetelo();
    }  
  }
  else 
  {
    ellenorzo();
  }
   
  return 0;
}


void ellenorzo()
{
  kill( getppid(), SIGUSR1 ); //munka ksz vagyok 
  printf( "Ellenor vagyok - kesz a munkara\n" );

  int azonosito;
  struct adat a;
  
  int  f = open( "ellenor.fifo", O_WRONLY );
  if( f < 0 ) { perror( "Fifo hiba\n" ); exit( EXIT_FAILURE ); }

  close( fd[1] );
  
  while( read( fd[0], &azonosito, sizeof( int ) ) )
  {
    printf( "%i, ", azonosito );
    a.azonosito = azonosito; 
    a.ervenyes = ( ( rand() % 100 ) > 20 );
printf("\nEllenorzo vagyok, irok a fifoba.\n");
    write( f, &a, sizeof( struct adat ) );
    fflush( stdout );   
  }
  close( f );
  close( fd[0] );
  printf( "\nEllenor vagyok csobol olvastam, irtam \n" ); 
}

void pecsetelo()
{
  kill( getppid(), SIGUSR1 ); //munkara kesz
  printf( "Pecsetelo vagyok - kesz a munkara\n " );

  struct adat a;
  int ervenyesek = 0; 
  int ervenytelenek = 0;
 
  int f = open( "ellenor.fifo", O_RDONLY );
  if( f < 0 ) { perror( "Fifo hiba\n" ); exit( EXIT_FAILURE ); }

  int i;
  for( i = 0; i < szavazokszama; i++ )
  {
    read( f, &a, sizeof( struct adat ) );
printf( "\nPecsetelo vagyok: olvastam a fifobol: %i, %i\n", a.azonosito, a.ervenyes );
    if (a.ervenyes)  ervenyesek++;
    else             ervenytelenek++;
    //printf("fifobol %i\n",ervenyesek);
  }

  printf( "\nPecsetelo vagyok, %i ervenyes, %i ervenytelen szavazat van\n", ervenyesek, ervenytelenek ); 
  close( f );
  unlink( "ellenor.fifo" );
}

void elnok()
{
   pause();
   pause();
   printf( "Elnok vagyok - mindenki kesz\n" );
   //mindenki kesz volt munkara, csobe a genera adatokat 
   int i;
   int azonosito;
   printf( "Elnok vagyok...azonositokat gyartottam\n" );
   close( fd[0] );
   for( i=0; i < szavazokszama; i++ )
   {
       azonosito=rand() % 10000;
printf( "\nElnok vagyok: irok a csöbe: %i\n", azonosito );
       write( fd[1], &azonosito, sizeof( int ) );
       printf( "%i ,",azonosito );
   }
   printf( "Elnok vagyok, beirtam az azonositokat a csobe\n" );
   close( fd[1] );
 }

void handler( int sn )
{
  ;
}
