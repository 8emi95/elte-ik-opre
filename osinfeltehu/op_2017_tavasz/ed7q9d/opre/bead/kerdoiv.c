//Read and print out the file given in the parameter 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>

struct kerdesek
{
 char kerdes[160];
 char elso[160];
 char masodik[160];
 char harmadik[160];
 char negyedik[160];
 char datum[160];
 char status[160];
};

void fajl_iras(struct kerdesek tomb[], int i){
    FILE* g;
    g = fopen("text.txt", "wb");
    if (g==NULL){perror("File megnyitasi hiba\n"); exit(1);}
   
    int l = 0;
    for(l; l<i; ++l){
        if(strcmp(tomb[l].kerdes,"T�R�LVE\n") != 0){
                fprintf(g, "%s%s%s%s%s%s%s", tomb[l].kerdes, tomb[l].elso, tomb[l].masodik, tomb[l].harmadik, tomb[l].negyedik, tomb[l].datum, tomb[l].status); 
        }      
    }
    fclose(g);
}

void lekerdez()
{
 char menu1[1];
 struct kerdesek tomb[160];
 int i = feltolt(tomb);
 int j=0;
 for (j;j<i;++j){
  printf("%s\n%s%s%s%s\n", tomb[j].kerdes,tomb[j].elso,tomb[j].masodik,tomb[j].harmadik,tomb[j].negyedik);
  printf("M�dos�tva: %s", tomb[j].datum);
  if (strcmp(tomb[j].status,"m�dos�that�\n") == 0)
  {
   printf("A %d. k�rd�s m�g nincs v�gleges�tve.\n\n",j+1);
  }
  else
  {
   printf("A %d. k�rd�s m�r v�gleges�tve van.\n\n",j+1);
  }
    }
 printf("K�rj�k a tov�bbl�p�shez nyomja meg a 0-�s gombot!");
 scanf("%s",menu1);
}

void hozzaad()
{
 char menu1[1];
 struct kerdesek tomb[160];
 int i = feltolt(tomb);
 printf("K�rem g�pelje be a k�rd�st!");
 scanf("%s", &tomb[i].kerdes);
 strcat(tomb[i].kerdes,"\n");
 printf("K�rem adja meg az els� v�laszlehet�s�get!");
 scanf("%s", &tomb[i].elso);
 strcat(tomb[i].elso,"\n");
 printf("K�rem adja meg a m�sodik v�laszlehet�s�get!");
 scanf("%s", &tomb[i].masodik);
 strcat(tomb[i].masodik,"\n");
 printf("K�rem adja meg a harmadik v�laszlehet�s�get! Ha nem k�v�n t�bb v�laszlehet�s�get megadni, nyomja meg a 0-�s gombot!");
 scanf("%s", &tomb[i].harmadik);
 strcat(tomb[i].harmadik,"\n");
 if(strcmp(tomb[i].harmadik,"0\n") == 0)
 {
  strcpy(tomb[i].harmadik,"\0");
  strcat(tomb[i].harmadik,"\n");
  strcpy(tomb[i].negyedik,"\0");
  strcat(tomb[i].negyedik,"\n");
 }
 else
 {
  printf("K�rem adja meg a negyedik v�laszlehet�s�get! Ha nem k�v�n t�bb v�laszlehet�s�get megadni, nyomja meg a 0-�s gombot!");
  scanf("%s", &tomb[i].negyedik);
  strcat(tomb[i].negyedik,"\n");
  if(strcmp(tomb[i].negyedik,"0\n") == 0)
  {
   strcpy(tomb[i].negyedik,"\0");
   strcat(tomb[i].negyedik,"\n");
  }
 }
 strcpy(tomb[i].status,"m�dos�that�\n"); 
 time_t timet;
 struct tm * timeinfo;
 time (&timet);
 timeinfo = localtime(&timet);
 strftime(tomb[i].datum,160,"%D %H:%M:%S",timeinfo);
 strcat(tomb[i].datum,"\n");
 ++i;
 fajl_iras(tomb, i);
 printf("K�rj�k a tov�bbl�p�shez nyomjon meg egy gombot!");
 scanf("%s",menu1);
}

void modositas()
{
 char menu1[1];
 int modosit_id=0;
 struct kerdesek tomb[160];
 int i = feltolt(tomb);
 int j = 0;
 printf("V�lassza ki a m�dos�tand� k�rd�st! G�pelje be a t�rlend� k�rd�s sorsz�m�t!\n\n");
 for (j;j<i;++j)
 {
  printf("%d.\t%s", j+1, tomb[j].kerdes);
 }
 scanf("%d",&modosit_id);
 --modosit_id;
 if(strcmp(tomb[modosit_id].status,"m�dos�that�\n") == 0)
 {
  printf("K�rem adja meg az els� v�laszlehet�s�get!");
  scanf("%s", &tomb[modosit_id].elso);
  strcat(tomb[modosit_id].elso,"\n");
  printf("K�rem adja meg a m�sodik v�laszlehet�s�get!");
  scanf("%s", &tomb[modosit_id].masodik);
  strcat(tomb[modosit_id].masodik,"\n");
  printf("K�rem adja meg a harmadik v�laszlehet�s�get! Ha nem k�v�n t�bb v�laszlehet�s�get megadni, nyomja meg a 0-�s gombot!");
  scanf("%s", &tomb[modosit_id].harmadik);
  strcat(tomb[modosit_id].harmadik,"\n");
  if(strcmp(tomb[modosit_id].harmadik,"0\n") == 0)
  {
   strcpy(tomb[modosit_id].harmadik,"\0");
   strcat(tomb[modosit_id].harmadik,"\n");
   strcpy(tomb[modosit_id].negyedik,"\0");
   strcat(tomb[modosit_id].negyedik,"\n");
  }
  else
  {
   printf("K�rem adja meg a negyedik v�laszlehet�s�get! Ha nem k�v�n t�bb v�laszlehet�s�get megadni, nyomja meg a 0-�s gombot!");
   scanf("%s", &tomb[modosit_id].negyedik);
   strcat(tomb[modosit_id].negyedik,"\n");
   if(strcmp(tomb[modosit_id].negyedik,"0\n") == 0)
   {
    strcpy(tomb[modosit_id].negyedik,"\0");
    strcat(tomb[modosit_id].negyedik,"\n");
   }
  }
 time_t timet;
 struct tm * timeinfo;
 time (&timet);
 timeinfo = localtime(&timet);
 strftime(tomb[modosit_id].datum,160,"%D %H:%M:%S",timeinfo);
 strcat(tomb[modosit_id].datum,"\n");

 fajl_iras(tomb, i);
 }
 else
 {
  printf("A k�rd�s nem m�dos�that�. L�pjen vissza a f�men�be a 0-�s gomb megnyom�s�val.");
  scanf("%s",menu1);
 }
}


void torles()
{
 char menu1[1];
 int torles_id=0;
 struct kerdesek tomb[160];
 int i = feltolt(tomb);
 int j = 0;
 printf("V�lassza ki a t�rlend� k�rd�st! G�pelje be a t�rlend� k�rd�s sorsz�m�t!\n\n");
 for (j;j<i;++j)
 {
  printf("%d.\t%s", j+1, tomb[j].kerdes);
 }
 scanf("%d",&torles_id);
 --torles_id;
 if(strcmp(tomb[torles_id].status,"m�dos�that�\n") == 0)
 {
  strcpy(tomb[torles_id].kerdes,"T�R�LVE\n");
  fajl_iras(tomb, i);
 }
 else
 {
 printf("A k�rd�s nem t�r�lhet�. L�pjen vissza a f�men�be a 0-�s gomb megnyom�s�val.");
 scanf("%s",menu1);
 }
}

void veglegesites()
{
 int veglegesit_id=0;
 struct kerdesek tomb[160];
 int i = feltolt(tomb);
 int j = 0;
 printf("V�lassza ki a m�dos�tand� k�rd�st! G�pelje be a t�rlend� k�rd�s sorsz�m�t!\n\n");
 for (j;j<i;++j)
 {
  printf("%d.\t%s", j+1, tomb[j].kerdes);
 }
 scanf("%d",&veglegesit_id);
 --veglegesit_id;
 strcpy(tomb[veglegesit_id].status,"v�gleges�tve\n");
 fajl_iras(tomb, i);
}

int feltolt(struct kerdesek tomb[]){
 
    FILE* f;
    f = fopen("text.txt", "r");
    if (f==NULL){perror("File megnyitasi hiba\n"); exit(1);}
    int i = 0;
    int j = 0;
    while(!feof(f)){
        if (j==0) 
	{        
         fgets(tomb[i].kerdes,sizeof(tomb[i].kerdes),f);
	 ++j;
	}
        else if (j==1)
        {
         fgets(tomb[i].elso,sizeof(tomb[i].elso),f);
	 ++j;
        }
        else if (j==2)
        {
         fgets(tomb[i].masodik,sizeof(tomb[i].masodik),f);
	 ++j;
        }
        else if (j==3)
        {
         fgets(tomb[i].harmadik,sizeof(tomb[i].harmadik),f);
	 ++j;
        }
	else if (j==4)
        {
         fgets(tomb[i].negyedik,sizeof(tomb[i].negyedik),f);
	 ++j;
        }
	else if (j==5)
        {
         fgets(tomb[i].datum,sizeof(tomb[i].datum),f);
	 ++j;
        }
        else if (j==6)
        {
         fgets(tomb[i].status,sizeof(tomb[i].status),f);
	 j=0;
	 ++i;
        }
    }
    fclose(f);
   
    return i;
}



void menu()
{
 char menu[1];
 while(1)
 {
  system("clear");
  printf("�dv�zli �nt a Nemzeti k�rd�ivk�sz�t� program! K�rj�k, v�lasszon az al�bbi lehet�s�gek k�z�l:\n\n");
  printf("1-es gomb: K�rd�sek list�z�sa\n");
  printf("2-es gomb: K�rd�s hozz�ad�sa\n");
  printf("3-as gomb: K�rd�s m�dos�t�sa\n");
  printf("4-es gomb: K�rd�s t�rl�se\n");
  printf("5-�s gomb: K�rd�s v�gleges�t�se\n");
  printf("6-os gomb: Kil�p�s\n");
  scanf("%s",menu);
  if(strcmp(menu,"1") == 0)
  {
   lekerdez();
  }
  if(strcmp(menu,"2") == 0)
  {
   hozzaad();
  }
  if(strcmp(menu,"3") == 0)
  {
   modositas();
  }
  if(strcmp(menu,"4") == 0)
  {
   torles();
  }
  if(strcmp(menu,"5") == 0)
  {
   veglegesites();
  }
  if(strcmp(menu,"6") == 0)
  {
   exit(1);
  }
 }
}



int main(){
 menu();
 return 0;
}