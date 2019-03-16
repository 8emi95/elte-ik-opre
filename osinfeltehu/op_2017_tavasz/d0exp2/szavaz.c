void pecset(){
  kill(getppid(), SIGUSR1); // munkaára kész vagyok
  printf("\nPecsetelo vagyok");
}
void ellenorzo(){
  kill(getppid(), SIGUSR1);
  printf("Elnok vagyok \n");
}
volatile szamlalo=0;
void handler(int sn){
  szamlalo++;
}

adadaint main(argc, *argv[]){
  signal(SIGUSR1, handlerjj); if(argc!2){perror("egy argumentum...\n");
  exit(1); } szavazokszama = atoi(argv[1]);

  if (pipe(fd)<0){peror("Hibas a cso letrehozas\n") exit(x);}

  pid_t ellen = fork();
  if (ellen > 0){
    pid_t pecsetelo = fork();
    if (pecsetelo > 0){
      while(szamlalo!=2){usleep(100);}
      elnok();
      wait(NULL); wait(NULL); 
    }else{ pecset(); }
  }else{
  ellenorzo();
  }
  return 0;
}