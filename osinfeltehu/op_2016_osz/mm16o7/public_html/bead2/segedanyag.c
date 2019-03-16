/*FORK HASZNÁLAT*/

//itt a szuloben inditjuk a forkot,meg nincs gyermek 
pid=fork() 
/*a szuloben es a letrehozott gyermekben is itt visszatert a fork()*/ 
//A gyermek szamara nullaval tert vissza 
if (pid==0) 
{ 
  /*IDE A GYERMEK FOLYAMAT JUTHAT CSAK EL*/ 
  //itt vegzi el a gyermek a feladatat 
  //mondjuk varakozik egy kapcsolatra es kiszolgalja 
  clientsock=accept(listensock, &clientaddr, &size); 
  kiszolgal(clientsock); 
  //es nem felejt el meghalni 
  exit(0); 
  //A gyermek KILEPETT 
} 
//IDE A SZULO JUTOTT EL 
if (pid<0) 
{ 
  //nem sikerült a fork 
  perror("fork"); 
  exit(1); 
} 

/*PIPE HASZNÁLAT*/

int main (void) 
{ 
  pid_t pid; 
  int mypipe[2]; 
  /*pipe létrehozása*/ 
  pipe (mypipe) 
  /* child process letrehozasa */ 
  pid = fork (); 
  if (pid == 0) 
  { 
    char c; 
    /*CHILD*/ 
    //ezt a parent hasznalja, itt bezarhatjuk 
    close(mypipe[1]); 
    //olvas, amig a szülő be nem zárja a pipe másik végét 
    while (read(mypipe[0],&c,1)) 
    { 
      putchar(c); 
    } 
    exit(EXIT_SUCCESS); 
   } 
  else 
  { 
    /*PARENT*/ 
    //ezt a veget a child hasznalja 
    close(mypipe[0]); 
    //irunk a pipe-ra, a child majd kiolvassa 
    write(mypipe[1],"hello!\n",strlen("hello, world!\n")); 
    write(mypipe[1],"bye!\n",strlen("bye, world!\n")); 
    exit(EXIT_SUCCESS); 
  } 
} 
