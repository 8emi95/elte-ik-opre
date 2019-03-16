#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <fcntl.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>


struct rendezveny
{
  char nev[100];
  char helyszin[100];
  int azonosito;
  int vendegekszama;
};

struct vendeg
{
  char nev[100];
  char email[100];
  int rendezvenyazonosito;
  time_t jelentkezesiido;
};

void handler(int signumber)
{
  if (signumber == SIGUSR1)
  {
    printf("(Signal a gyermektol) A helyszinen van es varja a resztvevoket! \n");
  }
 // else if(signumber == SIGUSR2)
 // {
 //   printf("anyadgeci\n");
 // }

}


void menu(struct rendezveny t[100],struct vendeg v[100],int rendezvenysz, int emberkeszam,int rendezvenyazon)
{
//  system("clear");
  printf("\n\n");
  printf("1. Vendeg jelentkezes\n");
  printf("2. Jelentkezettek listazasa\n");
  printf("3. Vendeg adatainak modositasa\n");
  printf("4. Vendeg adatainak torlese\n");
  printf("5. Uj rendezveny inditasa\n");
  printf("6. Rendezveny adatainak torlese\n");
  printf("7. Rendezvenyek inditasa(2.beadando)\n");
  printf("8. Kilepes\n");

  int m;
  scanf("%d",&m);

  if(m==1)
  { 
    if(rendezvenysz==0)
    {
      printf("Nincs rendezveny amelyre jelentkezhetne!\n");
    }
    else
    {
      printf("Adja meg a jelentkezeshez szukseges adatokat!\n");
      printf("Nev:\n");    
      scanf("%s",v[emberkeszam].nev);
      printf("Email:\n");
      scanf("%s",v[emberkeszam].email);
    
      int ra;
      printf("Rendezveny azonosito:\n");
      scanf("%d",&ra);
    
      int maszekbool=0;
      int i;
      for(i=0;i<rendezvenysz;i++)
      {
        if( t[i].azonosito==ra )
        {
          maszekbool=1;
        }
      }

      while(maszekbool==0)
      {
        printf("Nem letezo rendezveny azonosito, adjon meg ujat, az alabbiak leteznek:\n");
        for(i=0;i<rendezvenysz;i++)
       {
          printf("%d %s\n",t[i].azonosito,t[i].nev);      
        }     

        scanf("%d",&ra);
        for(i=0;i<rendezvenysz;i++)
        {
          if( t[i].azonosito==ra )
          {
            maszekbool=1;
          }
        }
      }
 
      v[emberkeszam].rendezvenyazonosito=ra;
      v[emberkeszam].jelentkezesiido= time(NULL);
      emberkeszam++;
      printf("Sikeresen felvett vendeg!\n");
      printf("A felvett vendeg sorszama: %d\n",emberkeszam);
    }
    menu(t,v,rendezvenysz,emberkeszam,rendezvenyazon);
  }
  else if(m==2)
  {
    int i;
    printf("Jelentkezettek listaja\n");
    printf("Nev / Email / Rendezvenyazonosito / Jelentkezesi ido\n");
    for(i=0;i<emberkeszam;i++)
    {      //printf(ctime(&mytime));
      printf("%s / %s / %d / %s",v[i].nev,v[i].email,v[i].rendezvenyazonosito,ctime(&(v[i].jelentkezesiido)));
    }
    menu(t,v,rendezvenysz,emberkeszam,rendezvenyazon);
  }
  else if(m==3)
  {
    if(emberkeszam==0)
    {
      printf("Ures a vendegek listaja!\n");
    }
    else
    {   
      printf("Adja meg a vendeg nevet, akinek adatait modositani szeretne!\n");
      char nev[100];
      scanf("%s",nev);
      int i;
      int keresettnev=0;
      int maszekbool=0;

      for(i=0;i<emberkeszam;i++)
      {
        if(!strcmp(v[i].nev,nev))
        {
          maszekbool=1;
          keresettnev=i;
        }
      }

      while(maszekbool==0)
      {
        printf("Nincs ilyen nevu vendeg, az alabbiak leteznek:\n");
        for(i=0;i<emberkeszam;i++)
        {    
          printf("%s\n",v[i].nev);
        }
        printf("Adja meg a vendeg nevet ujra!\n");
        scanf("%s",nev);
        for(i=0;i<emberkeszam;i++)
       {
          if(!strcmp(v[i].nev,nev))
          {
            maszekbool=1;
            keresettnev=i;
          }
        }
      }

      printf("Milyen adatot szeretne modositani?(1. Nev, 2. Email, 3. Rendezvenyazonosito )\n");
      printf("Adja meg az adathoz tartozo sorszamot!\n");

      int adatszam;
      scanf("%d",&adatszam);  
      maszekbool=0;
      if(adatszam>0 && adatszam<4)
      {
        maszekbool=1;
      }

      while(maszekbool==0)
      {
        printf("Nincs ilyen sorszam, csak: 1 2 3. Adjon meg uj sorszamot!\n");
        scanf("%d",&adatszam);
        if(adatszam>0 && adatszam<4)
        {
          maszekbool=1;
        }  
      }

    if(adatszam==1)
    {
      printf("Adja meg az uj nevet:\n");
      scanf("%s",v[keresettnev].nev);
      printf("Vendeg neve modositva!\n");
    }
    else if(adatszam==2)
    {
      printf("Adja meg az uj email cimet:\n");
      scanf("%s",v[keresettnev].email);
      printf("Vendeg email cime modositva!\n");     
    }
    else if(adatszam==3)
    {
      printf("Adja meg az uj rendezvenyazonositot:\n");
      scanf("%d",v[keresettnev].rendezvenyazonosito);
      printf("Vendeg rendezvenyazonositoja modositva!\n");     
    }

  }

    menu(t,v,rendezvenysz,emberkeszam,rendezvenyazon);
  }
  else if(m==4)
  {
    if(emberkeszam==0)
    {
      printf("Ures a vendegek listaja!\n");
    }
    else
    {
      printf("Adja meg a vendeg nevet, akinek adatait torolni szeretne!\n");
      char nev[100];
      scanf("%s",nev);
      int i;
      int keresettnev=0;
      int maszekbool=0;
      for(i=0;i<emberkeszam;i++)
      {
        if(!strcmp(v[i].nev,nev))
        {
          maszekbool=1;
          keresettnev=i;
        }
      }

      while(maszekbool==0)
      {
        printf("Nincs ilyen nevu vendeg, az alabbiak leteznek:\n");
        for(i=0;i<emberkeszam;i++)
        {    
          printf("%s\n",v[i].nev);
        }
        printf("Adja meg a vendeg nevet ujra!\n");
        scanf("%s",nev);
        for(i=0;i<emberkeszam;i++)
        {
          if(!strcmp(v[i].nev,nev))
          {
            maszekbool=1;
            keresettnev=i;
          }
        }
      }

      for(i=keresettnev;i<emberkeszam-1;i++)
      {
        v[i]=v[i+1];
      }
      emberkeszam--;
      printf("Vendeg adatai torolve!\n");
    }

      menu(t,v,rendezvenysz,emberkeszam,rendezvenyazon);
  }
  else if(m==5)
  {
    printf("Adja meg az uj rendezveny nevet!\n");
    char nev[100];
    scanf("%s",nev);
    
    printf("Adja meg az uj rendezveny helyszinet!\n");
    char helyszin[100];
    scanf("%s",helyszin);
  
    strncpy(t[rendezvenysz].nev,nev,100);
    strncpy(t[rendezvenysz].helyszin,helyszin,100);

    rendezvenyazon++;
    t[rendezvenysz].azonosito=rendezvenyazon;
    rendezvenysz++;
    printf("Uj rendezveny felveve!\n");
    menu(t,v,rendezvenysz,emberkeszam,rendezvenyazon);
  }
  else if(m==6)
  {
    if(rendezvenysz==0)
    {
      printf("Ures a rendezvenyek listaja!\n");
    }
    else
    {
      printf("Adja meg a rendezveny nevet amelyet torolni szeretne!\n");
      char nev[100];
      scanf("%s",nev);
      int maszekbool=0;
      int i;
      int keresettrendezveny;
      for(i=0;i<rendezvenysz;i++)
      {
        if(!strcmp(t[i].nev,nev))
        {
          maszekbool=1;
          keresettrendezveny=i;
        }
      }

      while(maszekbool==0)
      {
        printf("Nincs ilyen nevu rendezveny!\n");
        printf("Az alabbiak leteznek:\n");
        for(i=0;i<rendezvenysz;i++)
        {
          printf("%s\n",t[i].nev);
        }
        printf("Adja meg a rendezveny nevet amelyet torolni szeretne!\n");           
        scanf("%s",nev);
        for(i=0;i<rendezvenysz;i++)
        {
          if(!strcmp(t[i].nev,nev))
          {
            maszekbool=1;
            keresettrendezveny=i;
          }
        }
      }

      int keresettrendazon=t[keresettrendezveny].azonosito;

      for(i=keresettrendezveny;i<rendezvenysz-1;i++)
      {
        t[i]=t[i+1];
      }

      rendezvenysz--;

      int j;
      for(i=0;i<emberkeszam;i++)
      {
        if(v[i].rendezvenyazonosito==keresettrendazon)
        {          
          for(j=i;j<emberkeszam-1;j++)
          {
            v[j]=v[j+1];
          }
          emberkeszam--;
          i--;
        }
      }
  
    printf("Rendezveny torolve!\n");
    }

    menu(t,v,rendezvenysz,emberkeszam,rendezvenyazon);
  }
  else if(m==7)
  {
    srand(time(NULL));

    if(emberkeszam==0)
    {
      printf("A rendezvenyek listaja ures VAGY letezik egy vagy tobb rendezveny de egyikre sem jelentkezett meg egy vendeg  sem vendeg!\n");
    }
    else
    {
      int ig=0;
      int jg=0;
      
      for(ig=0;ig<rendezvenysz;ig++)
      {

        t[ig].vendegekszama=0;
        for(jg=0;jg<emberkeszam;jg++)
        {
          if(v[jg].rendezvenyazonosito==t[ig].azonosito)
          {
            t[ig].vendegekszama++;
          }
        }      
      }


      for(ig=0;ig<rendezvenysz;ig++)
      {
        rand(); rand(); rand();
        printf("\n\n");
        sleep(1);

        if(t[ig].vendegekszama==0)
        {
          printf("%s rendezvenyenre nem jelentkeztek ezert elmarad!\n",t[ig].nev);
        }
        else
        {
          signal(SIGUSR1,handler);

          int pipefdparent[2];

          if (pipe(pipefdparent) == -1) 
          {
            perror("Hiba a pipe nyitaskor!");
            exit(EXIT_FAILURE);
          }

          int pipefdchild[2];

          if (pipe(pipefdchild) == -1) 
          {
            perror("Hiba a pipe nyitaskor!");
            exit(EXIT_FAILURE);
          }

          char helyszin[100];

          int pidchild = fork();
          if (pidchild<0)
          {
            printf("Hiba a forkolaskor\n");
            exit(1);
          }
          else if(pidchild==0) //gyerek kod
          {

            close(pipefdchild[0]);
            close(pipefdparent[1]);

            read(pipefdparent[0], &helyszin, sizeof(helyszin));
            printf("A rendezveny helyszine: %s \n",helyszin);
  
            kill(getppid(),SIGUSR1); 
    
            int a=0;
            read(pipefdparent[0], &a, sizeof(a));
//        struct vendeg vendegeklistaja[a];


            int i;
            char resztvevonev[a][100];
            printf("A resztvevok nevei: \n");

            for(i=0;i<a;i++)
            {          
              read(pipefdparent[0], &resztvevonev[i], sizeof(resztvevonev[i]));
              printf("%d. %s \n",i+1,resztvevonev[i]);
            }
  


            printf("Varom a resztvevoket...\n");
            printf("Levezenylem a rendezvenyt...\n");
 

            int r = rand() % 100 + 1;

 //     sleep(5);
//      printf("%d \n",r);

            write(pipefdchild[1], &r, sizeof(r));
            int prob=0;
            int probs[a];
            int cnt=0;

            for(i=0;i<a;i++)
            {
              probs[i] = rand() % 10;        
              if(probs[i]==0)
              {
                cnt++;
              }
            }

            write(pipefdchild[1], &cnt, sizeof(cnt));
      
            for(i=0;i<a;i++)
            {
              if(probs[i]==0)
              {
                write(pipefdchild[1], &(resztvevonev[i]), sizeof(resztvevonev[i]));
              }
            }
 

//      sleep(5);
      //kill(getppid(),SIGUSR1);
 //     fflush(NULL);
 
            exit(0);

          }
          else
          {

            close(pipefdparent[0]);
            close(pipefdchild[1]);     
            write(pipefdparent[1], t[ig].helyszin, sizeof(t[ig].helyszin));
            pause(); //waits till a signal arrive 

//    printf("Signal arrived\n",SIGUSR1);


            int i=0;
            int j=0;

            write(pipefdparent[1], &t[ig].vendegekszama, sizeof(t[ig].vendegekszama));

            for(i=0;i<emberkeszam;i++)
            {
              if(v[i].rendezvenyazonosito==t[ig].azonosito)
              {
                write(pipefdparent[1], &(v[i].nev), sizeof(v[i].nev));
              }           
            }

            int mennyirevoltsikeres=0;
//sleep(15);
            read(pipefdchild[0], &mennyirevoltsikeres, sizeof(mennyirevoltsikeres));
  
            printf("A rendezveny %d szazalekban volt sikeres!\n",mennyirevoltsikeres);

            int hanyannem=0;
            read(pipefdchild[0], &hanyannem, sizeof(hanyannem));
 

            char nevbuff[100];
            printf("Azok listaja akik nem jelentek meg: \n");
            for(i=0;i<hanyannem;i++)
            {
              read(pipefdchild[0], nevbuff, sizeof(nevbuff));
              printf("%s \n",nevbuff);
            }

            int status;
            wait(&status);
          }

        }

      }

    }


//    printf("Parent process ended\n");

      unlink("/tmp/pipechild");

      menu(t,v,rendezvenysz,emberkeszam,rendezvenyazon);


//a sleepek miért kellenek, kiazaa pause??? waiteket átnézni sigactiont megkérdezni
// pollingos pipeot megnézni

  }
  else if(m==8)
  {
    FILE *rendezvenyfkiir;
    rendezvenyfkiir = fopen("rendezveny.txt","w");

    fprintf(rendezvenyfkiir,"%d\n",rendezvenysz);
    fprintf(rendezvenyfkiir,"%d\n",emberkeszam);
    int i;

    for(i=0;i<rendezvenysz;i++)
    {
      fprintf(rendezvenyfkiir,"%s %s %d\n",t[i].nev,t[i].helyszin,t[i].azonosito);
    }

    for(i=0;i<emberkeszam;i++)
    {
      fprintf(rendezvenyfkiir,"%s %s %d %d\n",v[i].nev,v[i].email,v[i].rendezvenyazonosito,v[i].jelentkezesiido);
    }

    exit(0);
  }
  else
  {
    printf("Nincs ilyen menupont!\n");
    menu(t,v,rendezvenysz,emberkeszam,rendezvenyazon);
  }

  printf("\n\n");
}


int main()
{



  struct rendezveny t[100];
  struct vendeg v[100];

  int rendezvenysz=0;
  int rendezvenyazon=0;
  char rendezvenyszc[100];
  int emberkeszam=0;

  FILE *rendezvenyf = fopen("rendezveny.txt","r");
  char line[200];


  if(rendezvenyf != NULL)
  { 
    if(fgets(rendezvenyszc,sizeof(rendezvenyszc),rendezvenyf ) != NULL)
    {
      rendezvenysz=atoi(rendezvenyszc);
      //printf("%d\n",rendezvenysz);
    }

    char test[100];


    if(fgets(test,sizeof(test),rendezvenyf ) != NULL)
    {
       if(!isdigit(test[0]))
       {

         int cnt=0;
	  int id;
	  char title[100];

         sscanf(test,"%s %d",title,&id);
         //printf("%d %s\n",id,title);
         strncpy(t[cnt].nev, title, 100);
         t[cnt].azonosito=id;
         cnt++;

         while(fgets(test,sizeof(test),rendezvenyf ) != NULL)
         {
           sscanf(test,"%s %d",title,&id);
           //printf("%d %s\n",id,title);
           strncpy(t[cnt].nev, title, 100);
           t[cnt].azonosito=id;
           cnt++;
         }

       }
       else
       {
         emberkeszam=atoi(test);                
       }

      int j=0;
      char line[100];
      int id;
      char title[100];
      char helyszin[100];
      while(j<rendezvenysz)
      {
        fgets(line,sizeof(line),rendezvenyf );
        sscanf(line,"%s %s %d",title,helyszin,&id);
        strncpy(t[j].nev, title, 100);
        strncpy(t[j].helyszin, helyszin, 100);
        t[j].azonosito=id;
        j++;
      }

      char nev[100];
      char email[100];
      int rendezvenyazonosito;
      char jelentkezesiido[100];

      j=0;
      time_t mytime;
      while(j<emberkeszam)
      {
        fgets(line,sizeof(line),rendezvenyf );
        sscanf(line,"%s %s %d %d",nev,email,&rendezvenyazonosito,&mytime);
  
    //mytime = time(NULL);
        //printf(ctime(&mytime));
        strncpy(v[j].nev,nev,100);
        strncpy(v[j].email,email,100);
        v[j].rendezvenyazonosito=rendezvenyazonosito;
        v[j].jelentkezesiido=mytime;

      //printf("%d %s\n",id,title);
      //strncpy(t[j].nev, title, 100);
      //t[j].azonosito=id;
        j++;
      }

    }

    fclose(rendezvenyf);
  }


  int i;
  for(i=0;i<rendezvenysz;i++)
  {
    if(t[i].azonosito>rendezvenyazon)
    {
       rendezvenyazon=t[i].azonosito;
    }
  }

  menu(t,v,rendezvenysz,emberkeszam,rendezvenyazon);

  
}