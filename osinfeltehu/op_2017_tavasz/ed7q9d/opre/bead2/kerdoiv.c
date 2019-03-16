//Read and print out the file given in the parameter 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>
#include <unistd.h>

struct kerdesek
{
 char kerdes[160];
 char elso[160];
 char masodik[160];
 char harmadik[160];
 char negyedik[160];
 char temp[160];
 char datum[160];
 char status[160];
};

struct veglegesitettek
{
	int kerdes_sorszam;
	int valaszok_szama;
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
 fgets(tomb[i].temp,160,stdin);
 printf("K�rem g�pelje be a k�rd�st!");
 fgets(tomb[i].kerdes,160,stdin);
 printf("K�rem adja meg az els� v�laszlehet�s�get!");
 fgets(tomb[i].elso,160,stdin);
 printf("K�rem adja meg a m�sodik v�laszlehet�s�get!");
 fgets(tomb[i].masodik,160,stdin);
 printf("K�rem adja meg a harmadik v�laszlehet�s�get! Ha nem k�v�n t�bb v�laszlehet�s�get megadni, nyomja meg a 0-�s gombot!");
 fgets(tomb[i].harmadik,160,stdin);
 printf("K�rem adja meg a negyedik v�laszlehet�s�get! Ha nem k�v�n t�bb v�laszlehet�s�get megadni, nyomja meg a 0-�s gombot!");
 fgets(tomb[i].negyedik,160,stdin);
 printf("%s", tomb[i].harmadik);
 if(strcmp(tomb[i].harmadik,"0\n") == 0)
 {
  strcpy(tomb[i].harmadik,"\n");
  strcpy(tomb[i].negyedik,"\n");
 }
 else
 {
  //fgets(tomb[i].negyedik,160,stdin);
  //printf("K�rem adja meg a negyedik v�laszlehet�s�get! Ha nem k�v�n t�bb v�laszlehet�s�get megadni, nyomja meg a 0-�s gombot!");
  if(strcmp(tomb[i].negyedik,"\n") == 0 || (strcmp(tomb[i].negyedik,"0\n") == 0))
  {
   strcpy(tomb[i].negyedik,"\n");
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
 //printf("K�rj�k a tov�bbl�p�shez nyomjon meg egy gombot!");
 //scanf("%s",menu1);
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
  fgets(tomb[modosit_id].elso,160,stdin);
  printf("K�rem adja meg az els� v�laszlehet�s�get!");
  fgets(tomb[modosit_id].elso,160,stdin);
  printf("K�rem adja meg a m�sodik v�laszlehet�s�get!");
  fgets(tomb[modosit_id].masodik,160,stdin);
  printf("K�rem adja meg a harmadik v�laszlehet�s�get! Ha nem k�v�n t�bb v�laszlehet�s�get megadni, nyomja meg a 0-�s gombot!");
  fgets(tomb[modosit_id].harmadik,160,stdin);
  printf("K�rem adja meg a negyedik v�laszlehet�s�get! Ha nem k�v�n t�bb v�laszlehet�s�get megadni, nyomja meg a 0-�s gombot!");
  fgets(tomb[modosit_id].negyedik,160,stdin);
  if(strcmp(tomb[modosit_id].harmadik,"0\n") == 0)
  {
   strcpy(tomb[modosit_id].harmadik,"\n");
   strcpy(tomb[modosit_id].negyedik,"\n");
  }
  else
  {
   //printf("K�rem adja meg a negyedik v�laszlehet�s�get! Ha nem k�v�n t�bb v�laszlehet�s�get megadni, nyomja meg a 0-�s gombot!");
   //fgets(tomb[modosit_id].negyedik,160,stdin);
   if(strcmp(tomb[modosit_id].negyedik,"0\n") == 0 || (strcmp(tomb[modosit_id].negyedik,"\n") == 0))
   {
    strcpy(tomb[modosit_id].negyedik,"\n");
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
 printf("V�lassza ki a m�dos�tand� k�rd�st! G�pelje be a v�gleges�tend� k�rd�s sorsz�m�t!\n\n");
 for (j;j<i;++j)
 {
  printf("%d.\t%s", j+1, tomb[j].kerdes);
 }
 scanf("%d",&veglegesit_id);
 --veglegesit_id;
 strcpy(tomb[veglegesit_id].status,"v�gleges�tve\n");
 fajl_iras(tomb, i);
}

int digit_to_int(char d)
{
 char str[2];

 str[0] = d;
 str[1] = '\0';
 return (int) strtol(str, NULL, 10);
}

void konzultacio()
{
	char menu1[1];
	struct kerdesek tomb[160];
	int i = feltolt(tomb);
	struct veglegesitettek tomb_vegleges[160];
	int kikuldottek[3];
	int j = 0;
	int db = 0;
	for (j;j<i;++j)
	{
		if (strcmp(tomb[j].status,"v�gleges�tve\n") == 0)
		{
			tomb_vegleges[db].kerdes_sorszam = j;
			if (strcmp(tomb[j].harmadik,"\n") == 0)
			{
				tomb_vegleges[db].valaszok_szama = 2;
				++db;
			}
			else if (strcmp(tomb[j].harmadik,"\n") != 0 && strcmp(tomb[j].negyedik,"\n") == 0)
			{
				tomb_vegleges[db].valaszok_szama = 3;
				++db;
			}
			else
			{
				tomb_vegleges[db].valaszok_szama = 4;
				++db;
			}
		}
	}
	if (db < 3)
	{
		printf("Nincs el�g k�rd�s a megval�s�t�shoz");
		exit(EXIT_SUCCESS);
	}
	else
	{
		int pipe_parenttochild[2];
		int pipe_childtoparent[2];
		pid_t pid;
		char buffer1[100];
		char buffer2[100];
		if (pipe(pipe_parenttochild) == -1 || pipe(pipe_childtoparent) == -1)
		 {
			  perror("Hiba a pipe nyitaskor!");
			  exit(EXIT_FAILURE);
		 }
		 pid = fork(); // creating parent-child processes
		 if (pid == -1)
		 {
			  perror("Fork hiba");
			  exit(EXIT_FAILURE);
		 }

		 if (pid == 0)
		 { // child process
			  close(pipe_parenttochild[1]); //Usually we close the unused write end
			  close(pipe_childtoparent[0]);
			  write(pipe_childtoparent[1], "V�rom a k�rd�seket!",20);
			  fflush(NULL);
			  sleep(3);
			  read(pipe_parenttochild[0],buffer2,sizeof(buffer2));
			  //printf("Gyerek olvasta uzenet: %s\n",buffer2);
			  int r0 = (rand() % 11) + 10;
			  int r = digit_to_int(buffer2[0]);
			  int r1 = digit_to_int(buffer2[1]);
			  int r2 = digit_to_int(buffer2[2]);
			  memset(&buffer2,0,sizeof(buffer2));
			  sprintf(buffer2, "%d", r0);
			  write(pipe_childtoparent[1], buffer2,2);
			  j = 0;
			  for (j;j<r0;++j) 
			  {
				sleep(3);
				memset(&buffer2,0,sizeof(buffer2));
				sprintf(buffer2, "%d%d%d", (rand() % r), (rand() % r1), (rand() % r2));
				write(pipe_childtoparent[1], buffer2,4);
			  }
			  close(pipe_childtoparent[1]);		  // finally we close the used read end
			  close(pipe_parenttochild[0]);
		 }
		 else
		 { // szulo process

			  close(pipe_childtoparent[1]);
			  close(pipe_parenttochild[0]); //Usually we close unused read end
			  sleep(3);
			  read(pipe_childtoparent[0],buffer1,sizeof(buffer1));
			  printf("K�rdez�biztos �zenet: %s\n",buffer1);
			  int r = (rand() % db);
			  int r2 = (rand() % db);
			  int r3 = (rand() % db);
			  kikuldottek[0] = tomb_vegleges[r].kerdes_sorszam;
			  kikuldottek[1] = tomb_vegleges[r2].kerdes_sorszam;
			  kikuldottek[2] = tomb_vegleges[r3].kerdes_sorszam;
			  memset(&buffer1,0,sizeof(buffer1));
			  sprintf(buffer1, "%d%d%d", tomb_vegleges[r].valaszok_szama, tomb_vegleges[r2].valaszok_szama, tomb_vegleges[r3].valaszok_szama);
			  write(pipe_parenttochild[1], buffer1, 4);
			  sleep(3);
			  memset(&buffer1,0,sizeof(buffer1));
			  read(pipe_childtoparent[0],buffer1,sizeof(buffer1));
			  int max = atoi(buffer1);
			  i=0;
			  for (i;i<max;++i)
			  {
				  sleep(3);
				  memset(&buffer1,0,sizeof(buffer1));
				  read(pipe_childtoparent[0],buffer1,sizeof(buffer1));
				  printf("%d. szem�ly\n\n", i+1);
				  printf("%s", tomb[kikuldottek[0]].kerdes);
				  int a = digit_to_int(buffer1[0]);
				  if (a == 0) {
					printf("%s\n\n", tomb[kikuldottek[0]].elso);
				  }
				  else if(a == 1) {
					printf("%s\n\n", tomb[kikuldottek[0]].masodik);
				  }
				  else if(a == 2) {
					printf("%s\n\n", tomb[kikuldottek[0]].harmadik);
				  }
				  else if(a == 3) {
					printf("%s\n\n", tomb[kikuldottek[0]].negyedik);
				  }
				  printf("%s", tomb[kikuldottek[1]].kerdes);
				  int a2 = digit_to_int(buffer1[1]);
				  if (a2 == 0) {
					printf("%s\n\n", tomb[kikuldottek[1]].elso);
				  }
				  else if(a2 == 1) {
					printf("%s\n\n", tomb[kikuldottek[1]].masodik);
				  }
				  else if(a2 == 2) {
					printf("%s\n\n", tomb[kikuldottek[1]].harmadik);
				  }
				  else if(a2 == 3) {
					printf("%s\n\n", tomb[kikuldottek[1]].negyedik);
				  }
				  printf("%s", tomb[kikuldottek[2]].kerdes);
				  int a3 = digit_to_int(buffer1[2]);
				  if (a == 0) {
					printf("%s\n\n", tomb[kikuldottek[2]].elso);
				  }
				  else if(a == 1) {
					printf("%s\n\n", tomb[kikuldottek[2]].masodik);
				  }
				  else if(a == 2) {
					printf("%s\n\n", tomb[kikuldottek[2]].harmadik);
				  }
				  else if(a == 3) {
					printf("%s\n\n", tomb[kikuldottek[2]].negyedik);
				  }
			  }
			  close(pipe_childtoparent[0]); // Closing write descriptor
			  close(pipe_parenttochild[1]);
			  wait(); // waiting for child process (not necessary)
		 }
		 exit(EXIT_SUCCESS);
	}
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
  //system("clear");
  printf("�dv�zli �nt a Nemzeti k�rd�ivk�sz�t� program! K�rj�k, v�lasszon az al�bbi lehet�s�gek k�z�l:\n\n");
  printf("1-es gomb: K�rd�sek list�z�sa\n");
  printf("2-es gomb: K�rd�s hozz�ad�sa\n");
  printf("3-as gomb: K�rd�s m�dos�t�sa\n");
  printf("4-es gomb: K�rd�s t�rl�se\n");
  printf("5-�s gomb: K�rd�s v�gleges�t�se\n");
  printf("6-os gomb: Konzult�ci� megkezd�se\n");
  printf("7-es gomb: Kil�p�s\n");
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
   konzultacio();
  }
  if(strcmp(menu,"7") == 0)
  {
   exit(1);
  }
 }
}



int main(){
 menu();
 return 0;
}