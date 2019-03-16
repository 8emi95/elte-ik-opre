
/*
(DZFM8B)
A Magyar Labdajátékok Szervezete (MLSZ) nemzeti konzultációt hirdetett a labdajátékok felvirágoztatására! Első lépésként megalkotjuk azt a kérdéssort és hozzá kérdésenként azt a minimum kettő, maximum 4 választási lehetőséget amelyet majd a labdajátékok jobbítása iránt felelősséget érzők kitölthetnek. Miután véglegesítettük a kérdéseket az MLSZ (szülő) kérdezőbiztost(gyerek) küld a paraméterként megadott labdajáték centrumba, városba.

Az MLSZ véletlenszerűen kiválaszt 3 kérdést, amit csővezetéken kiküld a kérdezőbiztosnak azután, hogy az bejelentkezett az MLSZ-hez, hogy készen áll a feladat végrehajtására. A kérdezőbiztos véletlenszerűen választ 10 és 20 közti konzultálni kívánó polgárt, akik kitöltik a kérdőívet (véletlenszerűen). Az eredményt visszaküldi csővezetéken az MLSZ-be, majd a kérdezőbiztos befejezi a konzultációt. Az MLSZ fogadja az eredményt és a képernyőre írja a konzultáció eredményét.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>// Fajl letezese miatt


#define FileName "Kerdesek.txt"
#define VeglegesFile "Vegleges.txt"


typedef int bool;
#define true 1
#define false 0


//1 Beadandóhoz
void kerdesFelvetele();
void listazas();
void modositas();
void torles();
void veglegesit();
void veglegesitettekListazasa();
void menu(); 

//2 Beadandóhoz
void fork_pipe ();

const char* KonzultaciosKitoltese();
char** str_split(char* a_str, const char a_delim);
int getRandom();
int countlines(char *filename);
void handler(int signumber);

int main(int argc,char* argv[]){
	menu();
	printf("Koszonom, hogy  hasznalta viszontlatasara :) \n\n");
	return 0;
}


void fork_pipe(){
	srand(time(NULL));
	signal(SIGUSR1,handler);

	char kerdes[3][512];
	char valaszok[3][512];	
	int pipefd[3][2];
	pid_t pid;
	char text [512];
	

	int i=0;
	for (i=0; i<3; ++i){
		if(pipe(pipefd[i])==-1){perror("Pipe fail! \n");exit(1);}
	}

	pid = fork();	
	if(pid==0){
		kill(getpid(),SIGUSR1);	
		sleep(3);
		
		
		int k;
	//Olvassuk a kérdéseket
		for(k=0;k<3;k++){	
			read(pipefd[k][0], kerdes[k], sizeof(kerdes[k]));
			close(pipefd[k][0]);
			printf("KERDEZŐ -> Ez az (%i). kérdés: %s\n",k+1,kerdes[k]);
		}
		printf("---||---\n");

	//Visszakuldjuk a valaszt
		char valasz[512];	
		int j;
		for (j=0; j<3; ++j){
			stpcpy(valasz,KonzultaciosKitoltese(getRandom(),kerdes[j]));
			write(pipefd[j][1],valasz,sizeof(valasz));
			close(pipefd[j][1]);
		}
		
		printf("Kerdező -> Elvégeztem a konzultációt. Elkuldtem az eredményt\n");
		printf("~Child process ended~ \n");
	
	 
	 }else if(pid>0){
		   
		//3 Kérdés kiválasztás majd átküldése. Figyeltem arra hogy ugyanazt a kérdést ne küldhessem át kétszer az MLSZ;
		int kuldo_tomb[3];
		int i;
		int kuldott_szam;
		for(i=0;i<3;i++){			
				kuldott_szam=rand()%countlines(VeglegesFile)+1;
				bool jo_szam=false;
				bool volt_mar;
				int j;
					while(!jo_szam){
						j=0;					
						volt_mar=false;					
						while(j<=sizeof(kuldo_tomb)/sizeof(int) && !volt_mar){						
							if(kuldo_tomb[j]==kuldott_szam){							
								volt_mar=true;
							}						
							++j;
						}
						if(volt_mar==true){
							kuldott_szam=rand()%countlines(VeglegesFile)+1;
							jo_szam=false;
						}else{
							kuldo_tomb[i]=kuldott_szam;
							jo_szam=true;
						}
					}
				
						FILE*fp=fopen(VeglegesFile,"r");
						char buf[512];
						char line[512];
						int count=1;
						while(fgets(buf,sizeof(buf),fp)){
							if(count==kuldo_tomb[i]){
								int i;
								for(i=0;i<512;i++){
									line[i]=buf[i];
								}
							}
							count++;
						}		
				//A kiválasztott kérdéseket elküldjük
				write(pipefd[i][1],line,sizeof(line));
				close(pipefd[i][1]);
				
		}
		printf("---||---\n");
		printf("MLSZ->Elküldtem a kérdéseket\n");
		printf("---||---\n");
		wait(NULL);//Gyerekre varakozas
		int k;
		//Olvassuk a kérdéseket
		
		for(k=0;k<3;k++){	
			read(pipefd[k][0], valaszok[k], sizeof(valaszok[k]));
			close(pipefd[k][0]);
		
			printf("MLSZ -> Az (%i). konzultacio eredménye: %s\n",k+1,valaszok[k]);
			printf("---||---\n");
		}
			
		printf("~Parent process ended~\n");

	 }
	 
	 
	 // Olvassuk a választ
	 
	 
	 printf("VÉGE\n");
	
}

void menu(){
	int menupont = 0;

	do{
		
		printf("\n\n");
		printf("\t1 - Kerdes felvetele\n");
		printf("\t2 - Kerdes modositasa\n");
		printf("\t3 - Torles\n");
		printf("\t4 - Listazas\n");
		printf("\t5 - Kerdes veglegesitese\n");
		printf("\t6 - Vegleges kerdesek listazasa\n\n");
		printf("\t0 - Konzultacio indítása\n");
				printf("\n\nValasszon a menupontok kozul -> ");
		scanf("%d", &menupont);
		
		switch(menupont) {
			case 1:
					kerdesFelvetele();
				break;
			case 2:
			   modositas();
				break;
			case 3:
			    torles();
				break;
			case 4:
				listazas();
				break;
            case 5:
				veglegesit();
				break;
			case 6:
				veglegesitettekListazasa();
				break;
		}	
	}while(menupont!=0);
	fork_pipe();

	
}
void kerdesFelvetele(){
	
	printf("Uj kerdes felvetele\n\n");
	char kerdes[512];
	int valasz_db;
	char valasz[512];
	
	FILE * fp=fopen(FileName,"a+");
	
	
	printf("Adja meg a kerdest:\n ->");
	scanf(" %[^\t\n]s",&kerdes);
	

	fprintf(fp,"%s;",kerdes);
	
	do{
	printf("Adja meg a valasz lehetosegeket(2-4)\n -> ");
	scanf("%d",&valasz_db);
	}while(valasz_db <2  ||valasz_db >4);
	
	
	
	int i;
	for(i=0;i<valasz_db;i=i+1){
		printf("Adja meg a.(z) ");
		printf("%d",i+1);
		printf(" valaszt\n ->");
		
		scanf(" %[^\t\n]s",&valasz);
		
		fprintf(fp,"%s;",valasz);
	}
	
	fprintf(fp,"%s%s\t%s\n","DATUM:",__DATE__,__TIME__);
	
	fclose(fp);

}

void torles(){
	listazas();
	
	printf("Kerdes torlese \n\n");
	
	
	if(access(FileName,F_OK)!=-1){
		
		
	
	char kerdes[512];
	
	printf("Adja meg a torolni kivant kerdest \n ->");
	scanf(" %[^\t\n]s",&kerdes);
	
	
	  char line[1024] ;
   FILE* data = fopen(FileName, "r") ;
   FILE* new = fopen("new.txt","a+");
   while (fgets(line , sizeof(line) , data )!= NULL)
   {
      if (strstr(line , kerdes )!= NULL)
      {
         printf("%s","Torolte ezt a sort: ");
		 printf("%s",line);
		
      }else{
		    fprintf(new,"%s",line);
		  
	  }
	 
   }
	fclose(data);
	fclose(new);
	
	
	char buffer[512];
	data=fopen(FileName,"w");
	new=fopen("new.txt","r");
	while(fgets(buffer,sizeof(buffer),new))
		fprintf(data,"%s",buffer);
	fclose(new);
	fclose(data);
	remove("new.txt");

printf(" ~Torles megtortent~ \n");
	}else{
		
		printf("Nincsenek meg kerdesek\n\n");
		
	}
}


void modositas(){
	
	printf("Kerdes modositasa \n\n");
	listazas();
	
	if(access(FileName,F_OK)!=-1){
		

	char kerdes[512];
	printf("Adja meg a kerdest\n ->");
	scanf(" %[^\t\n]s",&kerdes);
	
	char uj_Kerdes[512];
	printf("Adja meg az uj kerdest\n ->");
	scanf(" %[^\t\n]s",&uj_Kerdes);
	
	  char *buff_ptr, *find_ptr;
	
	FILE *data, *newfile;
	 size_t find_len = strlen(kerdes);
	data=fopen(FileName,"r");
	newfile = fopen("new.txt","w");
	
	char buffer[512];
	
	while(fgets(buffer,512,data)){
		
		 buff_ptr = buffer;
		 while ((find_ptr = strstr(buff_ptr,kerdes))){
			 
				 while(buff_ptr < find_ptr){
					 
					  fputc((int)*buff_ptr++,newfile);
				 }
            fputs(uj_Kerdes,newfile);
            buff_ptr += find_len;
			 
		 }
		  fputs(buff_ptr,newfile);
	}

fclose(newfile);
fclose(data);


	data=fopen(FileName,"w");
	newfile=fopen("new.txt","r");
	while(fgets(buffer,sizeof(buffer),newfile))
		fprintf(data,"%s",buffer);
	fclose(newfile);
	fclose(data);
	remove("new.txt");

printf(" ~Modositas megtortent~ \n");
	}else{
				printf("Nincsenek meg kerdesek\n\n");
	}

}
  
void listazas(){
	printf("\n");
	printf("Kerdesek es valaszok listazasa\n\n");
	if(access(FileName,F_OK)!=-1){
			
		FILE*fp=fopen(FileName,"r");
		
		char buf[512];
		while(fgets(buf,sizeof(buf),fp))
		{
			printf("%s",buf);
		}
	}else{
		
	printf("Nincsenek kerdest\n\n");
	}

}

void veglegesit(){
	printf("Kerdesek veglegesitese\n\n");
	
	listazas();
	
	if(access(FileName,F_OK)!=-1){
		
	char kerdes[512];
	printf("Melyik kerdest szeretne veglegesiteni?\n ->");
	scanf(" %[^\t\n]s",&kerdes);
	
		
		
	char line[1024] ;
   FILE* data = fopen(FileName, "r") ;
   FILE* new = fopen("new.txt","a+");
   FILE* vegleges=fopen(VeglegesFile,"a+");
   while (fgets(line , sizeof(line) , data )!= NULL)
   {
      if (strstr(line , kerdes )!= NULL)
      {
        fprintf(vegleges,"%s",line);
		
      }else{
		    fprintf(new,"%s",line);
		  
	  }
	 
   }
	fclose(data);
	fclose(new);
	fclose(vegleges);

	char buffer[512];
	data=fopen(FileName,"w");
	new=fopen("new.txt","r");
	while(fgets(buffer,sizeof(buffer),new))
		fprintf(data,"%s",buffer);
	fclose(new);
	fclose(data);
	remove("new.txt");

printf(" ~ Veglegesites megtortent~ \n");
	}else{
	printf("Nincsenek meg kerdesek\n\n");
	}
}



void veglegesitettekListazasa(){
	printf("\n");
	printf("Veglegesitett kerdesek listazasa\n\n");
	
	if(access(VeglegesFile,F_OK)!=-1){
		
			FILE*fp=fopen(VeglegesFile,"r");
		
		char buf[512];
		while(fgets(buf,sizeof(buf),fp))
		{
			printf("%s",buf);
		}
	}else{
		printf("%s","Nem letezik a fajl! Lehet meg nincsen kerdese amit veglegesiteni tudna\n\n");
	}

}



const char* KonzultaciosKitoltese(int random_people,char kapott_kerdes[512]){
	
		char** tokens;	
		int emberek=random_people;
		int darab;
		int lines;
		static char kitoltott[512];
		
		lines=0;	
		strcpy(kitoltott, "");//kiürítem hogy ne egymás után írja az eredményt
		strcat(kitoltott,"Mennyi embert kérdeztunk meg:");			
	
		char str[15];
		//konvert int to string;
		sprintf(str, "%d", emberek);
		strcat(kitoltott, str);
		strcat(kitoltott,  "\n");
	
	
		tokens = str_split(kapott_kerdes, ';');
		int counter;		
		if (tokens)
		{
			int i;
			for (i = 0; *(tokens + (i+1)); i++)
			{
				counter=i;			
				if(counter==0){
					strcat(kitoltott,"Kérdés:");					
					strcat(kitoltott,  *(tokens + i));
					strcat(kitoltott,  "\n");
						
				}else{				
					int x;				
					darab=0;					
					int igen_vagy_nem;					
						for(x=0;x<emberek;++x){							
							//egyettért-e ezzel a válasszal a random kiválasztott ember
							igen_vagy_nem=rand()%2;
							if(igen_vagy_nem==1){							
								darab=darab+1;
							}	
						}		
							strcat(kitoltott,"VÁLASZ:");
							strcat(kitoltott,  *(tokens + i));
							//ahányan igennel válaszoltak azok már másra nem szavazhatnak ezért őket elvesszük.Így lehet olyan aki egyik kérdésre sem szavaz igenel. Az ő szavazata érvénytelen lesz mint egy rendes konzultációnál.
							emberek-=darab;						
		
					
					char str[15];
					//konvert int to string;
					sprintf(str, "%d", darab);
					strcat(kitoltott, ":");
					strcat(kitoltott, str);
					strcat(kitoltott,  "\n");						
				}
				free(*(tokens + i));									
			}				
			strcat(kitoltott,"Érvénytelen kitöltések" );
			char str[15];
			//konvert int to string;
			sprintf(str, "%d", emberek);
			strcat(kitoltott, ":");
			strcat(kitoltott, str);
			
			free(tokens);
		}
	return kitoltott;
}


//Random  kiválasztjuk a 10-20 embert
int getRandom(){
  int random=(rand()%10)+10; 
  return random;

	return 0;
}

//Segéd fv_ delimiter mentén splitelés
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

 
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

 
    count += last_comma < (a_str + strlen(a_str) - 1);

   
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

int countlines(char *filename)
{
                                   
 FILE*fp=fopen(VeglegesFile,"r");
 int line=0;
		
		char buf[512];
		while(fgets(buf,sizeof(buf),fp))
		{
			
			line+=1;
		}
		
	return line;
  
  
}

void handler(int signumber){
	printf("----|||----\n");
  printf("A Kérdező megérkezett\n",signumber);
  	printf("----|||----\n");
}
