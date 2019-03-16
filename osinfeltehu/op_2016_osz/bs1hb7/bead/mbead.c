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

bool rendker(int, struct rl*, int*, bool);

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

bool rendker(int az, struct rl* rlist, int* n,  bool del){
  int i=0;
  //printf("%i\n", *n);
  for( i=0; i<*n;i++){
    //printf("%i",*rlist);
    if(az==(rlist[i].azon)){
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
  printf("7: Rendezveny szervezese.\n");
  printf("8: Mentes es kilepes.\n");
  int ret;
  scanf("%i%*c", &ret);
  return ret;
}
  
void handler(int signumber){
	//printf("wowshand %i\n",signumber);
}



int main(){
	signal(SIGTERM, handler);
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
      scanf("%[^\n]%*c",rlist[rn].nev);
      printf("Rendezveny azonosito: ");
      taz=0;
      scanf("%i", &taz);
      
      rlist[rn].azon=taz;
      int ido=0;
      printf("Kezdestel hatralevo ido(masodperc): ");
      scanf("%i%*c",&ido);
      (rlist[rn]).kezdet=((int) time(NULL))+ido;
      
      printf("Helyszin: ");
      scanf("%[^\n]%*c",rlist[rn].helyszin); 
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
      while(true){
        printf("%i\n",rn);
	char h[128];
	srand(time(NULL));
	int u=0;
	int w=0;
	int i=0;
    for(u=0;u<rn;u++){
		if(rlist[u].kezdet-(int)time(0)<0)
		{
			if(rendker(rlist[u].azon, &rlist, &rn, true)){
        
				etorol(&jlist, &jn, rlist[u].azon);
			}
		}
		printf("%s rendezvenyig %i masodperc van hatra\n",rlist[u].nev, rlist[u].kezdet-(int)time(0));
      if(rlist[u].kezdet==(int)time(0))
      {
        int pipefd[2];
		//printf("pipe\n");
        if(pipe(pipefd)==-1){
          printf("Hiba a pipe nyitaskor!\n");
          exit(EXIT_FAILURE);
        }
        
        pid_t child2=fork();
		//printf("fork\n");
        if(child2==0)
        {
          //child1
          //printf("child\n");
          
          read(pipefd[0],h, 129);
          printf("A partner megerkezett a helyszinre: %s\n", h);
          kill(getppid(), SIGTERM);
		  
          printf("Resztvevok: \n");
          char vl[128][128];
          int vn=0;
		  //printf("olvasaselott\n");
		  
          read(pipefd[0],vl[vn],129);
		  
		  //printf("child olvasta: %s\n",vl[vn]);
          while(vl[vn][0]!='#'){
            printf("%s	\n",vl[vn]);
            vn++;
            read(pipefd[0],vl[vn],129);
          }
		  //printf("olvascikvege::%s\n",vl[0]);
		  int random=(int)rand()/100000000;
		  
		  char rnd[16]="asd";
		  sprintf(rnd,"%d",random);
		  
		  sleep(1);
		  write(pipefd[1],rnd,129);
		  //printf("%i::%s\n",random,rnd);
			
		  for(w=0;w<vn;w++){
			  
			  if(rand()/100000000<10)
				  write(pipefd[1],vl[w],129);
		  }
		  write(pipefd[1],"#",129);
		  //printf("nulgyerektolkuldve\n");
		  
          close(pipefd[0]);
		  close(pipefd[1]);
		  exit(0);
		
          
        }else{
			//szulo
			//printf("szulo\n");
          write(pipefd[1], rlist[u].helyszin,  129);
          pause();
		  //printf("pauseutan\n");
          for(i=0;i<jn;i++)
          {
			 //printf("kereses\n");
            if(jlist[i].raz==rlist[u].azon)
              write(pipefd[1],jlist[i].nev,129);
          }
          write(pipefd[1],"#",129);
		  //printf("nullkuldve\n");
		  sleep(1);
		  
          read(pipefd[0],h,129);
		  //printf("beolvasva\n");
	  printf("A rendezveny %s% volt sikeres.\n", h);
	  printf("Meg nem jelent vendegek: \n");
	  
	  read(pipefd[0],h,129);
          while(h[0]!='#'){
            printf("%s	\n",h);
            read(pipefd[0],h,129);
          }
           close(pipefd[0]);
		   close(pipefd[1]);
        
          
        
        }
		if(rendker(rlist[u].azon, &rlist, &rn, true)){
        
        etorol(&jlist, &jn, rlist[u].azon);
		}
		
        
      }
    }
    if(rn==0) break;
	sleep(1);
	

  }
	break;
    case 8:
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
} 

  
}  

  
  
  