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
        if(strcmp(tomb[l].kerdes,"TÖRÖLVE\n") != 0){
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
  printf("Módosítva: %s", tomb[j].datum);
  if (strcmp(tomb[j].status,"módosítható\n") == 0)
  {
   printf("A %d. kérdés még nincs véglegesítve.\n\n",j+1);
  }
  else
  {
   printf("A %d. kérdés már véglegesítve van.\n\n",j+1);
  }
    }
 printf("Kérjük a továbblépéshez nyomja meg a 0-ás gombot!");
 scanf("%s",menu1);
}

void hozzaad()
{
 char menu1[1];
 struct kerdesek tomb[160];
 int i = feltolt(tomb);
 printf("Kérem gépelje be a kérdést!");
 scanf("%s", &tomb[i].kerdes);
 strcat(tomb[i].kerdes,"\n");
 printf("Kérem adja meg az elsõ válaszlehetõséget!");
 scanf("%s", &tomb[i].elso);
 strcat(tomb[i].elso,"\n");
 printf("Kérem adja meg a második válaszlehetõséget!");
 scanf("%s", &tomb[i].masodik);
 strcat(tomb[i].masodik,"\n");
 printf("Kérem adja meg a harmadik válaszlehetõséget! Ha nem kíván több válaszlehetõséget megadni, nyomja meg a 0-ás gombot!");
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
  printf("Kérem adja meg a negyedik válaszlehetõséget! Ha nem kíván több válaszlehetõséget megadni, nyomja meg a 0-ás gombot!");
  scanf("%s", &tomb[i].negyedik);
  strcat(tomb[i].negyedik,"\n");
  if(strcmp(tomb[i].negyedik,"0\n") == 0)
  {
   strcpy(tomb[i].negyedik,"\0");
   strcat(tomb[i].negyedik,"\n");
  }
 }
 strcpy(tomb[i].status,"módosítható\n"); 
 time_t timet;
 struct tm * timeinfo;
 time (&timet);
 timeinfo = localtime(&timet);
 strftime(tomb[i].datum,160,"%D %H:%M:%S",timeinfo);
 strcat(tomb[i].datum,"\n");
 ++i;
 fajl_iras(tomb, i);
 printf("Kérjük a továbblépéshez nyomjon meg egy gombot!");
 scanf("%s",menu1);
}

void modositas()
{
 char menu1[1];
 int modosit_id=0;
 struct kerdesek tomb[160];
 int i = feltolt(tomb);
 int j = 0;
 printf("Válassza ki a módosítandó kérdést! Gépelje be a törlendõ kérdés sorszámát!\n\n");
 for (j;j<i;++j)
 {
  printf("%d.\t%s", j+1, tomb[j].kerdes);
 }
 scanf("%d",&modosit_id);
 --modosit_id;
 if(strcmp(tomb[modosit_id].status,"módosítható\n") == 0)
 {
  printf("Kérem adja meg az elsõ válaszlehetõséget!");
  scanf("%s", &tomb[modosit_id].elso);
  strcat(tomb[modosit_id].elso,"\n");
  printf("Kérem adja meg a második válaszlehetõséget!");
  scanf("%s", &tomb[modosit_id].masodik);
  strcat(tomb[modosit_id].masodik,"\n");
  printf("Kérem adja meg a harmadik válaszlehetõséget! Ha nem kíván több válaszlehetõséget megadni, nyomja meg a 0-ás gombot!");
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
   printf("Kérem adja meg a negyedik válaszlehetõséget! Ha nem kíván több válaszlehetõséget megadni, nyomja meg a 0-ás gombot!");
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
  printf("A kérdés nem módosítható. Lépjen vissza a fõmenübe a 0-ás gomb megnyomásával.");
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
 printf("Válassza ki a törlendõ kérdést! Gépelje be a törlendõ kérdés sorszámát!\n\n");
 for (j;j<i;++j)
 {
  printf("%d.\t%s", j+1, tomb[j].kerdes);
 }
 scanf("%d",&torles_id);
 --torles_id;
 if(strcmp(tomb[torles_id].status,"módosítható\n") == 0)
 {
  strcpy(tomb[torles_id].kerdes,"TÖRÖLVE\n");
  fajl_iras(tomb, i);
 }
 else
 {
 printf("A kérdés nem törölhetõ. Lépjen vissza a fõmenübe a 0-ás gomb megnyomásával.");
 scanf("%s",menu1);
 }
}

void veglegesites()
{
 int veglegesit_id=0;
 struct kerdesek tomb[160];
 int i = feltolt(tomb);
 int j = 0;
 printf("Válassza ki a módosítandó kérdést! Gépelje be a törlendõ kérdés sorszámát!\n\n");
 for (j;j<i;++j)
 {
  printf("%d.\t%s", j+1, tomb[j].kerdes);
 }
 scanf("%d",&veglegesit_id);
 --veglegesit_id;
 strcpy(tomb[veglegesit_id].status,"véglegesítve\n");
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
  printf("Üdvözli Önt a Nemzeti kérdõivkészítõ program! Kérjük, válasszon az alábbi lehetõségek közül:\n\n");
  printf("1-es gomb: Kérdések listázása\n");
  printf("2-es gomb: Kérdés hozzáadása\n");
  printf("3-as gomb: Kérdés módosítása\n");
  printf("4-es gomb: Kérdés törlése\n");
  printf("5-ös gomb: Kérdés véglegesítése\n");
  printf("6-os gomb: Kilépés\n");
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