//make a copy of file given in argv[1] to file given in argv[2] 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errnoT
#include <time.h>

/**
Egy kiállításokat, rendezvényeket szervezõ társaság a "King of Stands", 
elektronikus látogatói nyilvántartást készített. A látogatók a 
nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, 
a rendezvény azonosító számát! Az elektronikus rendszer a jelentkezés 
nyugtázásaként visszaadja, hogy hányadik vendégként történt meg a regisztráció! 
Az alkalmazás adjon lehetõséget vendég jelentkezéshez, a jelentkezettek 
listázásához, egy vendég adatainak módosításához, törléséhez. 
Legyen lehetõségünk egy új rendezvény indításához, egy rendezvény 
összes adatának törléséhez! A jelentkezõknél a korábbi adatok mellett 
rögzítsük automatikusan a jelentkezési idõt(rendszeridõ) is(ezt nem kell bekérni)!
*/

typedef struct event {
  int id;
  char* name;
  //time_t date;
} event;

typedef struct visitor {
  char* name;
  char* email;
  int event_id;
  //time_t reg_date;
} visitor;

void printEvents(){
	FILE *other;
	int f;
	char c;
	f=open("events.txt",O_RDONLY); 
	//other = fopen("events.txt","r");
	while (read(f,&c,sizeof(c))){ 
		if(c == ';'){
			printf("\t");
		}else{
			printf("%c",c);
		} 
	} 
	/*int id; 
	char name; 
	int year = 0; 
	int month = 0; 
	int day = 0; 
	while(!feof(other)){
		fscanf(&id, "%i",sizeof(int),other);
		fscanf(name, "%s",sizeof(10),other);
		printf("%i\t%s\t%i-%i-%i\n",id, name,year,month,day);
	}*/
	close(f);
}

int deleteEvent(id){
	int f;
	FILE *temp;
	int i = 0;
	int actualId = 0;
	char c;
	char delimiter = ';';
	f=open("events.txt",O_RDONLY); 
	//temp=open("temp.txt",O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	temp=fopen("temp.txt","w");
	//if (fscanf(ifp, "%s %d", username, &score) != 2)
    //break;
  	//fprintf(ofp, "%s %d", username, score+10);
	while (read(f,&c,sizeof(c))){ //karakterenként olvassuk 
		if(i == 0 && c == delimiter){
			i++;
			printf("actualId: %i \n", actualId);
			if(id != actualId && i == 1){
          		fprintf(temp,"%i;", actualId);
			}
		}
		if(i == 0 && c != delimiter){
			if(actualId == 0){
				actualId = atoi(&c);
			}else{
				actualId = 10 * actualId + atoi(&c);
			}
		}
		if(i > 0 && id != actualId){
			fprintf(temp, "%c", c);
			//write(temp,&c,sizeof(c))!=sizeof(c);
			if(c == '\n'){
				i = 0;
				actualId = 0;
			}   
		} else if( i > 0 && id == actualId){
			i = 0;
			actualId = 0;
			fprintf(temp, "\n" );
		}
	}
	close(temp); 
	close(f);
	//remove("events.csv");
	//rename("temp.csv", "events.csv");
}

void appendEvents()
{/*
	char line[1024];

scanf("%[^\n]", line);*/
	FILE *events;
	int id; //ezt még nem tudjuk, ki kéne deríteni
	char name[20];
	int year;
	int month;
	int day;
	events=fopen("events.txt","a");
	printf("Rendezveny azonositoja: ");
	scanf("%i", &id);
	printf("Mi a rendezveny neve? : ");
	scanf("%s", &name);
	printf("Melyik evben lesz? : ");
	scanf("%i",&year);
	printf("Melyik honapban lesz? : ");
	scanf("%i",&month);
	printf("Melyik napon lesz? : ");
	scanf("%i",&day);
	printf("%i;%s;%i-%i-%i\n",id, name,year,month,day);
	fprintf(events, "%i;%s;%i-%i-%i\n",id, name,year,month,day);
	close(events);
}

int getNonUsedId()
{
	FILE *events;
	int id = 1; 
	int aid = 0; 
	events=fopen("events.txt","r");
	//char *strtok(char *str, const char *delim)
	//Breaks string str into a series of tokens separated by delim.

	return id;
}

int main(int argc,char** argv){
	int menu = 100;
	int deleteId = 0;
	int autoincrement = getNonUsedId();

	while(menu > 0){
		switch(menu){
			case 100:
				break;
			case 1:
				printEvents();
				printf("\n");
				break;
			case 2:
				appendEvents();
				break;
			case 3:
				printf("Melyik rendezvenyt szeretned torolni? \nAzonosito:");
				scanf("%i", &deleteId);
        printf("kivalasztott id: %i", deleteId);
				deleteEvent(deleteId);
        break;
			case 4:
        break;
			case 5:
        break;
			case 6: 
        break;
			case 7:
        break;
			default:
				printf("valami baj van");
				break;
		}
    printf("King of Stands - rendezveny nyilvantarto\n\n");
		printf("1 - Rendezvenyek listaja,\n");
		printf("2 - Uj rendezveny letrehozasa\n");
		printf("3 - Rendezveny torlese\n");
		printf("4 - Jelentkezes rendezvenyre\n");	
		printf("5 - Latogatok listaja\n");
		printf("6 - Latogato szerkesztese\n");
		printf("7 - Latogato torlese\n");
		printf("0 - Kilepes\n\n");  
		printf("Valasztas: ");
		scanf ("%i",&menu);
	}
  
	return 0;
}