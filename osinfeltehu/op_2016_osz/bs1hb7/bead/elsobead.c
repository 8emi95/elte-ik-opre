#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>


bool rendker(int, int[], int*, bool);

struct reg{
  char nev[128];
  char mail[128];
  int raz;
  time_t regi;
};

struct rl{
  int azon;
  char nev[128];
  int kezdet;
  char helyszin[128];
  
};

void regisztracio(struct reg* tmp,struct rl* list, int n){
  printf("Nev: ");
  char tnev[128];
  scanf("%[^\n]%*c",tmp->nev);
  
  printf("\nMail: ");
  char tmail[128];
  scanf("%[^\n]%*c",tmp->mail);
  
  int taz;
  bool tbool;
  do{
  printf("\nRendezveny azonosito: ");
  scanf("%i",&taz);
  
  tbool = rendker(taz ,list, &n, false);
  }while(!tbool); 
  tmp->raz=taz;
  tmp->regi= time(0);
}

bool rendker(int az, struct rl  *rlist, int *n,  bool del){
  int i=0;
  //printf("%i\n", *n);
  for( i=0; i<*n;i++){
    //printf("%i",*rlist);
    if(az==(*rlist->azon)){
      if(del){
        int j=0;
        //printf("%i\n", (*n)-1);
        for(j=i;j<(*n)-1;j++){
          //printf("%i, %i\n",i,j);
          rlist[j]=rlist[j+1];
        }
        (*n)--;
        return true;
      }
      else{
        return true;
      }
    
    }
    rlist++;
  }
  return false;
}
      
int nevker(struct reg jlist[],int n, char* nev){
  int i=0;
  for(i=0;i<n;i++){
    if(strcmp(jlist[i].nev,nev)==0)
      return i;
  }
  return -1;
}

void etorol(struct reg *jlist, int *n, int azon){
  int i=0;
  for(i=0;i<*n;){
   // printf("|%i\n",jlist[i].raz);
    if(jlist[i].raz==azon){
      int j=i;
      for(j=i;j<(*n)-1;j++){
        jlist[j]=jlist[j+1];
      }
      (*n)--;
    }else
      i++;
  }
}

int menu(){
  printf("1: Vendeg jelentkezes.\n");
  printf("2: Vendegek listazasa.\n");
  printf("3: Vendeg adatainak modositasa.\n");
  printf("4: Vendeg torlese.\n");
  printf("5: Uj rendezven felvetele.\n");
  printf("6: Rendezveny torlese.\n");
  printf("7: Mentes es kilepes.\n");
  int ret;
  scanf("%i%*c", &ret);
  return ret;
}
  



int main(){
  struct rl rlist[128];
  int rn=0;
  struct reg jlist[1024];
  int jn=0;
  char tnev[128];
  int tn;
  int i;
  int taz=0;
  int handlerl = open("./rendezvenyek", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
  int handlejl = open("./jelentkezok", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
  //beolvasas
  if(handlerl <0&&handlejl<0)
  {
    printf("Fajl hiba.");
    return;
  }
  
  while(read(handlerl, &rlist[rn], sizeof(rlist[rn]))>0){
    rn++;
  }
  
  while(read(handlejl, &jlist[jn], sizeof(jlist[jn]))>0){
    jn++;
  }
  close(handlerl);
  close(handlejl);
    
 while(true){
 pid_t child1 =fork();
 if(child1>0)
 { 
  switch(menu()){
    case 1:
      
      
      regisztracio(&jlist[jn], &rlist, rn);
      printf("%i. jelentkezes\n", ++jn);
      break;
    case 2:
      printf("Nev,	Mail,	Razon,	Regisztracios ido\n");
      
      for(i=0;i<jn;i++)
        printf("%s,	%s,	%i,	%s\n", jlist[i].nev, jlist[i].mail, jlist[i].raz, ctime(&jlist[i].regi));
      break;
    case 3:
      printf("Irja be a modositando vendeg nevet:");
      
      scanf("%[^\n]%*c", tnev);
      tn= nevker(jlist, jn, tnev);
      if(tn<0)
        printf("Nincs ilyen nevu vendeg.\n");
      else
        regisztracio(&jlist[tn], rlist, rn);
      break;
    case 4:
      printf("Irja be a torlendo vendeg nevet:");
      
      scanf("%[^\n]%*c",tnev);
      tn=nevker(jlist, jn, tnev);
      if(tn<0)
        printf("Nincs ilyen nevu vendeg.\n");
      else{
        int i=0;
        for(i=tn; i<jn-1;i++)
          jlist[i]=jlist[i+1];
        jn--;
      }
      printf("\n");
      break;
    case 5:
      printf("Rendezveny neve: ");
      scanf("%[^\n]%*c",rlist[rn]->nev);
      printf("Rendezveny azonosito: ");
      taz=0;
      scanf("%i", &taz);
      
      rlist[rn]->azon=taz;
      int ido=0;
      printf("Kezdestel hatralevo ido(masodperc): ");
      scanf("%i%*c",ido);
      rlist[rn]->kezdet= time(0)+(ido);
      printf("Helyszin: ");
      scanf("%[^\n]%*c",rlist[rn]->helyszin); 
      rn++;
      break;
    case 6:
      taz=0;
      printf("Rendezveny azonosito: ");
      scanf("%i", &taz);
      //printf("f\n");
      if(rendker(taz, &rlist, &rn, true)){
        //printf("tuljut\n");
        etorol(&jlist, &jn, taz);
      }else
        printf("Nincs ilyen azonositoju rendezveny\n");
      break;
    case 7:
      //kiiras
      remove("./rendezvenyek");
      remove("./jelentkezok");
      int handlerl= open("./rendezvenyek", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
      int handlejl= open("./jelentkezok", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
      
      for(i=0;i<rn; i++)
        write(handlerl, &rlist[i], sizeof(rlist[i]));
            
      for(i=0;i<jn;i++)
        write(handlejl, &jlist[i], sizeof(jlist[i])); 
      
      close(handlerl);
      close(handlejl);
      return 0;
  }
}else{
  //child1
  while(true){
    int u=0;
    for(u=0;u<rn;u++){
      if(rlist[u]->kezdet==time(0))
      {
        int pipefd[2];
        if(pipe(pipefd)==-1){
          perror("Hiba a pipe nyitaskor!");
          exit(EXIT_FAILURE);
        }
        
        pid_t child2=fork();
        if(child2>0)
        {
          //child1
          
          write(pipefd[1], rlist[u]->helyszin, 129);
          pause();
          for(i=0;i<jn;i++)
          {
            if(jlist[i].raz==rlist[u])
              write(pipefd[1],jlist[i].nev,129);
          }
          write(pipefd[1],'\0',129);
           
          
        }else{
        //child2
          
          char h[128];
          read(piepfd[0],h, 129);
          printf("A partner megerkezett a helyszinre: %s", h);
          kill(getppid(), SIGTERM);
          printf("Resztvevok: ");
          char vl[128][128];
          int vn=0;
          read(piepfd[0],vl[vn],129);
          while(h!='/0'){
            printf("%s	",vl[vn]);
            vn++;
            read(pepefd[0],vl[vn],129);
          }
           
        
        
        }
        
      }
    }  
  }
}  
}
  
  
}
  
  
  