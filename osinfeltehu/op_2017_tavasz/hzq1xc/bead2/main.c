#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

typedef int bool;
#define true 1
#define false 0

void feladatKiir(){
  printf("\n1. opcio: Uj kerdes hozzaadasa.\n");
  printf("2. opcio: Letezo kerdes modositasa.\n");
  printf("3. opcio: Kerdes torlese.\n");
  printf("4. opcio: Kerdesek es valaszaik listazzasa.\n");
  printf("5. opcio: Vegso konzultacio osszeallitasa.\n");
  printf("0. opcio: Program befejezese.\n ");
}

bool isNumber(char *input) {
  int i;
  for (i = 0; input[i] != '\0'; i++)
    if (isalpha(input[i]))
      return false;
  return true;
}

int beolvasOpcio(int tol, int ig){
  char input[10];
  int opcio;
  bool l=true;
  while(l){
    printf("\nKerem valaszon szamot(%d-%d): ",tol,ig);
    scanf("%s", &input);
    if( isNumber(input)){
      opcio=atoi(input);
      if(opcio>=tol && opcio<=ig){
        l=false;
      }else{
        printf("A valasztot szam nincs a megfelelo tartomanyban");
      }
    }else{
      printf("Az opcio szam!\n");
    }
  }
  return opcio;
}



void kerdesekSzama(int *kerdesszam){
  char* ut="./kerdesek/kerdesekszama";
  FILE* f;
  f=fopen(ut,"rb");
  if(f==NULL){
    perror("Hiba tortent a fajl megnyitasakor!");
    exit(1);
  }
  char line[160];
  while (!feof(f)){
    fgets(line,sizeof(line),f);
   // printf("%s",line);
  }
  *kerdesszam=atoi(line);
  fclose(f);
}

void kerdesekSzamModositas(int* kerdesszam){
  //printf("\n%d\n %d\n\n",*kerdesszam,a);
  char* ut="./kerdesek/kerdesekszama";
  FILE* f;
  f=fopen(ut,"w");
  if(f==NULL){
    perror("Hiba tortent a fajl megnyitasakor!");
    exit(1);
  }
  fprintf(f, "%d", *kerdesszam);
  fclose(f);
}

struct kerdes{
  char szoveg[1024];
  int valaszdb;
  char valasz1[128];
  char valasz2[128];
  char valasz3[128];
  char valasz4[128];
};

void beolvasenter(char* szoveg){
    int i;
    char c;
    scanf("%s", szoveg);
    i = strlen(szoveg);
    do{
      scanf("%c", &c);
      szoveg[i++] = c;
    } while(c!='\n');
    szoveg[i-1]=0;
    //printf("\n\n%s\n\n",szoveg);
}

void kerdesfajbair(int *kerdesszam, struct kerdes asd){
  char str[1000]="./kerdesek/";
  sprintf(str, "%s%d",str, *kerdesszam);
  time_t curtime;
    time(&curtime);

  // printf("Current time = %s", ctime(&curtime));

  FILE *fp;
  fp = fopen(str, "w");
  if(fp==NULL){
    perror("Hiba tortent a kerdes letrehozasakor!");
    exit(1);
  }
  if(asd.valaszdb==2){
    fprintf(fp, "%s\n%d\n%s\n%s\n%s",asd.szoveg,asd.valaszdb,asd.valasz1,asd.valasz2,ctime(&curtime));
  }else if(asd.valaszdb==3){
    fprintf(fp, "%s\n%d\n%s\n%s\n%s\n%s",asd.szoveg,asd.valaszdb,asd.valasz1,asd.valasz2,asd.valasz3,ctime(&curtime));
  }else{
    fprintf(fp, "%s\n%d\n%s\n%s\n%s\n%s\n%s",asd.szoveg,asd.valaszdb,asd.valasz1,asd.valasz2,asd.valasz3,asd.valasz4,ctime(&curtime));
  }
  fclose(fp);
}

void ujkerdes(int *kerdesszam){
  (*kerdesszam)++;
  //printf("%d\n",(*kerdesszam));
  kerdesekSzamModositas((kerdesszam));
  struct kerdes asd;
  printf("Kerem a kerdes szoveget: ");
  beolvasenter(asd.szoveg);
  asd.valaszdb=beolvasOpcio(2,4);
    printf("\nKerem adja meg az elso valaszt: ");
    beolvasenter(asd.valasz1);
    printf("\nKerem adja meg az masodik valaszt: ");
    beolvasenter(asd.valasz2);
  if(asd.valaszdb==3){
    printf("\nKerem adja meg az harmadik valaszt: ");
    beolvasenter(asd.valasz3);
  }else if(asd.valaszdb==4){
    printf("\nKerem adja meg az harmadik valaszt: ");
    beolvasenter(asd.valasz3);
    printf("\nKerem adja meg az negyedik valaszt: ");
    beolvasenter(asd.valasz4);
  }
  kerdesfajbair(kerdesszam, asd);
}

//2. opci--------------------------------------------------------------------------------------------------

void enterlevag(char* str){
    int i=strlen(str);
    if(str[i-1]=='\n')str[i-1]=0;
}

struct kerdes kerdesbeolvas(int mod,struct kerdes asd){
    char str[1000]="./kerdesek/",c[20];
    sprintf(str, "%s%d",str, mod);
    FILE *fp;
    fp = fopen(str, "r");
    if(fp==NULL){
        perror("Hiba tortent a kerdes megkeresesekor!");
        exit(1);
    }
    fgets(asd.szoveg, sizeof(asd.szoveg), fp);
    fgets(c, sizeof(c), fp);
    fgets(asd.valasz1, sizeof(asd.valasz1), fp);
    fgets(asd.valasz2, sizeof(asd.valasz2), fp);
    asd.valaszdb=atoi(c);
    if(asd.valaszdb==3){
        fgets(asd.valasz3, sizeof(asd.valasz3), fp);
    }else if(asd.valaszdb==4){
        fgets(asd.valasz3, sizeof(asd.valasz3), fp);
        fgets(asd.valasz4, sizeof(asd.valasz4), fp);
    }
    fclose(fp);
    enterlevag(asd.szoveg);
    enterlevag(asd.valasz1);
    enterlevag(asd.valasz2);
    printf("%d. kerdes\n%s\n%s\n%s\n",mod,asd.szoveg,asd.valasz1,asd.valasz2);
    if(asd.valaszdb>2){
        enterlevag(asd.valasz3);
        printf("%s\n",asd.valasz3);
    }if(asd.valaszdb==4){
        enterlevag(asd.valasz4);
        printf("%s\n",asd.valasz4);
    }
    return asd;
};

void modosit(int* kerdesszam){
    int mod=beolvasOpcio(1,*kerdesszam),opc=1,modos;
    struct kerdes asd;
    asd=kerdesbeolvas(mod, asd);
    while(opc){
        printf("\nMit szeretne modositani a kerdesen belul? \n 1. A kerdest modositan\n 2. Valamelyik valaszt modositani\n 3. Uj valaszt hozzadni\n 4. valaszt elvenni\n0. modositas befejezese\n");
        opc=beolvasOpcio(0,4);
        switch(opc) {
          case 1 :
            printf("Kerem az uj kerdest: ");
            beolvasenter(asd.szoveg);
          break;
          case 2 :
              modos=beolvasOpcio(1,asd.valaszdb);
              if(modos==1){
                printf("Ezt a valaszt valasztotta: %s\nKerem az uj valaszt: ",asd.valasz1);
                beolvasenter(asd.valasz1);
              }else if(modos==2){
                printf("Ezt a valaszt valasztotta: %s\nKerem az uj valaszt: ",asd.valasz2);
                beolvasenter(asd.valasz2);
              }else if(modos==3){
                printf("Ezt a valaszt valasztotta: %s\nKerem az uj valaszt: ",asd.valasz3);
                beolvasenter(asd.valasz3);
              }else{
                printf("Ezt a valaszt valasztotta: %s\nKerem az uj valaszt: ",asd.valasz4);
                beolvasenter(asd.valasz4);
              }
          break;
          case 3 :
            if(asd.valaszdb<4){
                asd.valaszdb++;
                if(asd.valaszdb==3){
                    printf("Kerem az uj valaszlehetoseget: ");
                    beolvasenter(asd.valasz3);
                }else{
                    printf("Kerem az uj valaszlehetoseget: ");
                    beolvasenter(asd.valasz4);
                }
            }else{
                printf("nem lehetseges tobb valaszt hozzadni mivel 4 a maximalis valasz szam!\n");
            }
          break;
          case 4 :
            if(asd.valaszdb>2){
                printf("A kerdes arra iranyul hogy hanyadik valaszt szeretne torolni!\n");
                modos=beolvasOpcio(1,asd.valaszdb);
                asd.valaszdb--;
                if(modos==1){
                    if(asd.valaszdb==2){
                        strncpy(asd.valasz1, asd.valasz3, strlen(asd.valasz3));
                    }else{
                        strncpy(asd.valasz1, asd.valasz4, strlen(asd.valasz4));
                    }
                }else if(modos==2){
                    if(asd.valaszdb=2){
                        strncpy(asd.valasz2, asd.valasz3, strlen(asd.valasz3));
                    }else{
                        strncpy(asd.valasz2, asd.valasz4, strlen(asd.valasz4));
                    }
                }else if(modos==3){
                    strncpy(asd.valasz3, asd.valasz4, strlen(asd.valasz4));
                }
            }else{
                printf("nem lehetseges tobb valaszt elvenni mivel 2 a minimalis valasz szam!\n");
            }
          break;
          case 0 :
            printf("Modositas befejezve!\n");
          break;
          default :
            printf("Ilyen opcio nem letezik!\n");
        }
    }
    kerdesfajbair(&mod, asd);
}

//torles ---------------------------------------
void kerdestorles(int* kerdesszam){
    int torol;
    printf("Kerem irja le a torolni kivant kerdes szamat!\n");
    torol=beolvasOpcio(1,*kerdesszam);
    struct kerdes asd;
    if(torol!=*kerdesszam){
        printf("A fajlt toroltuk és az alabbi adatok kerultek a %d helyrol a %d helyre!\n",*kerdesszam,torol);
        asd=kerdesbeolvas(*kerdesszam, asd);
        kerdesfajbair(&torol, asd);
    }
    --(*kerdesszam);
    kerdesekSzamModositas(kerdesszam);
}

void kerdeskiiro(int mod){
    struct kerdes asd;
    char str[1000]="./kerdesek/",c[20];
    sprintf(str, "%s%d",str, mod);
    FILE *fp;
    fp = fopen(str, "r");
    if(fp==NULL){
        perror("Hiba tortent a kerdes megkeresesekor!");
        exit(1);
    }
    fgets(asd.szoveg, sizeof(asd.szoveg), fp);
    fgets(c, sizeof(c), fp);
    fgets(asd.valasz1, sizeof(asd.valasz1), fp);
    fgets(asd.valasz2, sizeof(asd.valasz2), fp);
    asd.valaszdb=atoi(c);
    if(asd.valaszdb==3){
        fgets(asd.valasz3, sizeof(asd.valasz3), fp);
    }else if(asd.valaszdb==4){
        fgets(asd.valasz3, sizeof(asd.valasz3), fp);
        fgets(asd.valasz4, sizeof(asd.valasz4), fp);
    }
    fclose(fp);
    enterlevag(asd.szoveg);
    enterlevag(asd.valasz1);
    enterlevag(asd.valasz2);
    printf("%d. kerdes\n%s\n%s\n%s\n",mod,asd.szoveg,asd.valasz1,asd.valasz2);
    if(asd.valaszdb==3){
        enterlevag(asd.valasz3);
        printf("%s\n",asd.valasz3);
    }
    if(asd.valaszdb==4){
        enterlevag(asd.valasz4);
        printf("%s\n",asd.valasz4);
    }
};

void sumkerdeskiir(int* kerdesszam){
    int i;
    for(i=1; i<=*kerdesszam; ++i){
        kerdeskiiro(i);
    }
}

void vegsokerdoivir(char* a, int k /*struct kerdes asd*/){
    FILE *fp;
    fp = fopen("kerdoiv.txt", a);
    if(fp==NULL){
        perror("Hiba tortent a kerdes letrehozasakor!");
        exit(1);
    }
    fprintf(fp, "%d\n",k);
    /*if(asd.valaszdb==2){
        fprintf(fp, "%s\n%s\n%s\n",asd.szoveg,asd.valasz1,asd.valasz2);
    }else if(asd.valaszdb==3){
        fprintf(fp, "%s\n%s\n%s\n%s\n",asd.szoveg,asd.valasz1,asd.valasz2,asd.valasz3);
    }else{
        fprintf(fp, "%s\n%s\n%s\n%s\n%s\n",asd.szoveg,asd.valasz1,asd.valasz2,asd.valasz3,asd.valasz4);
      }*/
    fclose(fp);
}


void kerdoiv(int* kerdesszam){
    printf("irja be hogy melyik kerdes szerepeljen a vegso konzultacioban!\n");
    int k=beolvasOpcio(1,*kerdesszam);
    //struct kerdes asd;
    //asd=kerdesbeolvas(k, asd);
    char* a="w";
    vegsokerdoivir(a, k);
    printf("Szeretnel meg beleirni mas kerdest 1-igen, 0-nem: ");
    int opc=beolvasOpcio(0,1);
    a="a";
    while(opc){
        printf("irja be hogy melyik kerdes szerepeljen a vegso konzultacioban!\n");
        int k=beolvasOpcio(1,*kerdesszam);
        //asd=kerdesbeolvas(k, asd);
        vegsokerdoivir(a, k);
        printf("Szeretnel meg beleirni mas kerdest 1-igen, 0-nem: ");
        opc=beolvasOpcio(0,1);
    }
}

//2. beadandó---------------------------------------------------------------------------------------------------------------------------------------

struct  haromKerdes{
    int kerdesSzam[3];
    int valaszDb[3];
};

void beolvasKerdesekMenyisege(int* KerdesekMeny){
    *KerdesekMeny=0;
    FILE* fp;
    char buffer[20];
    fp = fopen("kerdoiv.txt", "r");
    if(fp==NULL){
        perror("Hiba tortent a kerdes letrehozasakor!");
        exit(1);
    }
    while(fgets(buffer, 20, (FILE*) fp)) {
        *KerdesekMeny+=1;
    }
    fclose(fp);
}

int randKerdes(int r){
    FILE* fp;
    char buffer[255];
    fp = fopen("kerdoiv.txt", "r");
    if(fp==NULL){
        perror("Hiba tortent a kerdes letrehozasakor!");
        exit(1);
    }
    int i=-1;
    while(i!=r) {
        fgets(buffer, 255, (FILE*) fp);
        i++;
    }
    fclose(fp);
    return atoi(buffer);
}

int kerdesDb(int szam){
    char str[1000]="./kerdesek/",c[20];
    sprintf(str, "%s%d",str, szam);
    FILE *fp;
    char szoveg[1024];
    fp = fopen(str, "r");
    if(fp==NULL){
        perror("Hiba tortent a kerdes megkeresesekor!");
        exit(1);
    }
    fgets(szoveg, sizeof(szoveg), fp);
    fgets(c, sizeof(c), fp);
    fclose(fp);
    return atoi(c);
}

struct haromKerdes veletlenKerdesek(struct haromKerdes asd){
    srand(time(NULL));
    int KerdesekMeny;
    beolvasKerdesekMenyisege(&KerdesekMeny);
    int i,r;
    for(i=0; i<3; ++i){
        r = rand()%KerdesekMeny;
        asd.kerdesSzam[i]= randKerdes(r);
        asd.valaszDb[i]=kerdesDb(asd.kerdesSzam[i]);
        //printf("%d %d\n",asd.kerdesSzam[i],asd.valaszDb[i]);
    }
    return asd;
}

void handler(int signumber){
  printf("Az Mlsz keszenal a %i signalon\n",signumber);
}

void reset( int *array, int size){
  srand(time(NULL));
  int r, r1, i; 
  r=(rand()%11+10);
  for(i=0;i<r;++i){
    r1=rand()%size;
    //printf("%i ",r1);
    array[r1]++;//=array[r1];
    //printf("%i ",array[r1]);
  }
}

int main(){
  int kerdesszam;
  kerdesekSzama(&kerdesszam);

  printf("Osszesen %d. kerdes van\n", kerdesszam);

  int feltetel=1;
  while(feltetel) {
    feladatKiir();
    feltetel=beolvasOpcio(0,5);
    switch(feltetel) {
      case 1 :
        ujkerdes(&kerdesszam);
      break;
      case 2 :
        sumkerdeskiir(&kerdesszam);
        modosit(&kerdesszam);
      break;
      case 3 :
        sumkerdeskiir(&kerdesszam);
        kerdestorles(&kerdesszam);
      break;
      case 4 :
        sumkerdeskiir(&kerdesszam);
      break;
      case 5 :
        sumkerdeskiir(&kerdesszam);
        kerdoiv(&kerdesszam);
      break;
      case 0 :
        printf("Befejezte a kerdoiv oszalitasat most folytatodik a feladat masodik resze!\n");
      break;
      default :
        printf("Ilyen opcio nem letezik!\n");
    }
  }
  //masodik resze a beadandónak----------------------------------------------------------------------------
//mlsz válasz 3 kérdést véletlenszerüen
  struct haromKerdes asd;
    
	
    
	signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
	
  
  
  int pid,fd;    
  printf("Fifo start!\n");    
  int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); 
  if (fid==-1){	
    printf("Error number: %i",errno);	
    exit(EXIT_FAILURE);   
  }    
  printf("Mkfifo vege, fork indul!\n");  
  int var=0;
  int status;
  pid_t child=fork();
  if (child>0)
  { 
    asd=veletlenKerdesek(asd);
    int kerdes[6]={asd.kerdesSzam[0], asd.valaszDb[0],asd.kerdesSzam[1], asd.valaszDb[1],asd.kerdesSzam[2],asd.valaszDb[2]};        
    //fd=open("fifo.ftc",O_WRONLY);
    //close(fd);
    kill(child,SIGTERM);
    int i;
    for(i=0;i<6;++i){
      fd=open("fifo.ftc",O_WRONLY);
      write(fd,&kerdes[i],sizeof(int));
      ///printf("%i\n", kerdes[i]);                
      //printf("irtam\n");
    }
    close(fd);
    struct kerdes asd;
    int felmeres[12];
    fd=open("fifo.ftc",O_RDONLY);
     printf("\n haho \n");
     
    read(fd,felmeres,12*sizeof(int));
   printf("\n haho \n");
   for(i=0; i<12;i=i+4){
      kerdesbeolvas(kerdes[i/4],asd);
      printf("\nA valasz %i-n valasztottak\n", felmeres[i]);
      printf("B valasz %i-n valasztottak\n", felmeres[i+1]);
      if(kerdes[((i/4)+1)]>=3){
        printf("C valasz %i-n valasztottak\n", felmeres[i+2]);
      }
      if(kerdes[((i/4)+1)]==4){
         printf("D valasz %i-n valasztottak\n", felmeres[i+3]);
       }
      printf("\n");
    }
                                                                                        
              
    unlink("fifo.ftc");
  }
  else 
  {
    //printf("var\n");
    pause();
    //printf("vege\n");
    int kadat[6],i,j;
    sleep(6);
    fd=open("fifo.ftc",O_RDONLY);	
    read(fd,kadat,6*sizeof(int));
    //for(i=0; i<6;++i){	
    //  printf("Ezt olvastam a csobol: %d \n",kadat[i]);	
    //}
    int felmeres[12]={0,0,0,0,0,0,0,0,0,0,0,0};
    for(i=1;i<6;i=i+2){
      reset( &felmeres[(i-1)*2], kadat[i]);
    }
    for(j=0;j<12;j++){
            write(fd,&felmeres[j],sizeof(int));
            printf("%i ",felmeres[j]);
    }              
    close(fd);
    
            
  }
  //---------------------------------------------------------------------------------------

  return 0;
}
