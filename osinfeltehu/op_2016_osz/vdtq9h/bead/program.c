/*
K�sz�tsen C programot, amely rendszerh�v�sok alkalmaz�s�val megoldja a k�vetkez� feladatot:

Egy ki�ll�t�sokat, rendezv�nyeket szervez� t�rsas�g a "King of Stands", elektronikus l�togat�i nyilv�ntart�st k�sz�tett. A l�togat�k a nyilv�ntart�sba jelentkez�skor megadj�k nev�ket, mail c�m�ket, a rendezv�ny azonos�t� sz�m�t! Az elektronikus rendszer a jelentkez�s nyugt�z�sak�nt visszaadja, hogy h�nyadik vend�gk�nt t�rt�nt meg a regisztr�ci�! Az alkalmaz�s adjon lehet�s�get vend�g jelentkez�shez, a jelentkezettek list�z�s�hoz, egy vend�g adatainak m�dos�t�s�hoz, t�rl�s�hez. Legyen lehet�s�g�nk egy �j rendezv�ny ind�t�s�hoz, egy rendezv�ny �sszes adat�nak t�rl�s�hez! A jelentkez�kn�l a kor�bbi adatok mellett r�gz�ts�k automatikusan a jelentkez�si id�t(rendszerid�) is(ezt nem kell bek�rni)!

K�sz�ts�nk C programot, amellyel a le�rt tev�kenys�g elv�gezhet�. Az adatokat f�jlban t�roljuk.

*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 256

void jelentkezes();
void menu();
void listazas();
void modositas();
void torles();
void rendezvenytorles();
void ujrendezveny();
void copy(const char *from_name, char *to_name);


int main()
{
	
	menu();
	
	return 0;
	
}



void menu()
{
	char menu[MAX_SIZE];	
	while(1)
	{
	printf("\nValassz menu pontot:\n\n");
		
	printf("=====================\n");
			printf("Listazas:\t\tl\n");
			printf("Jelentkezes:\t\tj\n");
			printf("Modositas:\t\tm\n");
			printf("Torles:\t\t\tt\n");
			printf("Torles(rendezveny):\trt\n");
			printf("Uj rendezveny:\t\tur\n");
			printf("kilepes:\t\tPress CTRL + C to exit\n");
	printf("=====================\n\n");
	scanf("%s",menu);
	
		if( strcmp(menu,"l") == 0 )
		{
			listazas();
		}
		else if( strcmp(menu,"j") == 0 )
		{
			jelentkezes();
		}
		else if( strcmp(menu,"m") == 0 )
		{
			modositas();
		}
		else if( strcmp(menu,"t") == 0 )
		{
			torles();
		}
		else if( strcmp(menu,"rt") == 0 )
		{
			rendezvenytorles();
		}
		else if(strcmp(menu,"ur") == 0)
		{
			ujrendezveny();
		}
	}	
	
}
void listazas()
{
	char line[MAX_SIZE];
	FILE *rendezveny;
	int rid;
	char crid[MAX_SIZE];				//character rid
	char filename[MAX_SIZE];
	
	printf("Kerlek add meg a listazni kivant rendezveny azonositojat!\n");
	scanf("%s",crid);

	sprintf(filename,"%s.txt",crid);	// rendezv�ny ID ut�n konkaten�ljuk a txt-t majd ezt nyitom meg
	
	rendezveny = fopen(filename,"r");
	if(rendezveny == NULL)
	{
		printf("Nem letezik a megadott rendezveny!");
		menu();
	}
	
	while(fgets(line,100,rendezveny) != NULL)
	{
		printf("%s",line);
	};
}

void jelentkezes()
{
	int sorszam;					// jelentkezettek sorsz�ma, kezdetben 1, ha nem �res a f�jl akkor pedig az utolso jelentkezett +1
	char nev[MAX_SIZE];
	char email[MAX_SIZE];
	int rid;									// rendezveny id
	char filename[MAX_SIZE];
	char crid[MAX_SIZE];
	char line[MAX_SIZE];
	FILE *rendezveny;
	
	printf("Kerlek add meg a nevet!\n");
	scanf("%s",nev);
	printf("Kerlek add meg az emailt!\n");
	scanf("%s",email);
	printf("Kerlek add meg a rendezveny azonositot!(szam)\n");
	scanf("%d",&rid);
	
	sprintf(crid,"%d",rid);					// characterr� konvert�ljuk a bek�rt ID-t	
	sprintf(filename,"%s.txt",crid);	// rendezv�ny ID ut�n konkaten�ljuk a txt-t majd ezt nyitom meg
	
	rendezveny = fopen(filename,"r");
	if(rendezveny == NULL)
	{
		printf("A rendezveny nem letezik!\n");
		fclose(rendezveny);
		menu();
	}
	rendezveny = fopen(filename,"a+");
	
			// felt�telezz�k, hogy jelentkezett emberek IDje szerint rendezett
			// megtudni milyen IDj� volt a legutols� jelentkezett ember (biztos van okosabb megold�s is...)
	char* _fgets;
	char read_nev[MAX_SIZE];
	char read_email[MAX_SIZE];
	char read_time[MAX_SIZE];
	time_t t = time(0);
	char *time = ctime(&t);
	
	
	for(int i = 0; time[i] != '\0'; i++)		// hogy egy stringgk�nt kezelje a d�tumot -> biztos van okosabb megold�s is
	{
		if(time[i] == ' ')
			time[i] = '_';
	}
	sorszam = 1;
	do
	{
			_fgets = fgets(line,sizeof(line),rendezveny);
			int read_id = 0;
			sscanf(line,"%d %s %s %s",&read_id,read_nev,read_email,read_time);
			if(_fgets == NULL && read_id != 0)
			{
				sorszam = read_id + 1;
			}
		
	}
	while(  _fgets != NULL);
	
	fprintf(rendezveny,"%d %s %s %s", sorszam,nev,email,time);
	printf("%s nevu jelentkezo a %d. sorszammal jelentkezett",nev,sorszam);
	
	fclose(rendezveny);
}

void modositas()
{
	char crid[MAX_SIZE];		// character rendezveny id
	char filename[MAX_SIZE];
	printf("Add meg annak rendezvenynek az azonositojat amelyikben modositani szeretnel!\n");
	scanf("%s",crid);
	FILE *rendezveny;
	FILE *helpfile;
	char line[MAX_SIZE];
	
	
	sprintf(filename,"%s.txt",crid);	// rendezv�ny ID ut�n konkaten�ljuk a txt-t majd ezt nyitom meg
	
	rendezveny = fopen(filename,"r");
	helpfile = fopen("helpfile.txt","w");
	
	if(rendezveny == NULL)
	{
		printf("A rendezveny nem letezik!\n");
		menu();
	}
	int sorszam;
	printf("Kerlek add meg annak a jelentkezonek az azonositojat akin modositani szeretnel\n");
	scanf("%d",&sorszam);
	

	int read_id = 0;	
	char read_nev[MAX_SIZE];
	char read_email[MAX_SIZE];
	char read_time[MAX_SIZE];	
	 
	
	while(fgets(line,sizeof(line),rendezveny) != NULL)
	{
		char menu[MAX_SIZE];

		sscanf(line,"%d %s %s %s",&read_id,read_nev,read_email,read_time);
		
		// amiken m�dos�tani lehet
		char new_nev[MAX_SIZE];
		char new_email[MAX_SIZE];
		

		
		if(read_id == sorszam)
		{
			printf("=============\n");
			printf("Mit szeretnel modositani? Nev(n),E-mail(e)\n");
			printf("=============\n");
			scanf("%s",menu);
			
			if(strcmp(menu,"e") == 0)
			{
				printf("Add meg az uj email cimet!\n");
				scanf("%s",new_email);
				fprintf(helpfile,"%d %s %s %s\n", sorszam,read_nev,new_email,read_time);
			}
			else if(strcmp(menu,"n") == 0)
			{
				printf("Add meg az uj nevet!\n");
				scanf("%s",new_nev);
				fprintf(helpfile,"%d %s %s %s\n", sorszam,new_nev,read_email,read_time);
			}
			else
			{
				modositas();
			}
		}
		else if( read_id != sorszam && read_id > 0 )
		{
			fputs(line,helpfile);
		}
	}
	if(read_id == 0)
	{
		fclose(rendezveny);
		fclose(helpfile);
		printf("A rendezvenyhez nem tartozott egy jelentkezo sem!\n");
		menu();
	}
	
	fclose(rendezveny);
	fclose(helpfile);
	
	copy("helpfile.txt",filename);	
}	



void torles()
{
	char crid[MAX_SIZE];		// character rendezveny id
	char filename[MAX_SIZE];
	printf("Add meg annak rendezvenynek az azonositojat amelyikbol torolni szeretnel!\n");
	scanf("%s",crid);
	FILE *rendezveny;
	FILE *helpfile;
	char line[MAX_SIZE];
	
	
	sprintf(filename,"%s.txt",crid);	// rendezv�ny ID ut�n konkaten�ljuk a txt-t majd ezt nyitom meg
	
	rendezveny = fopen(filename,"r");
	helpfile = fopen("helpfile.txt","w");
	
	if(rendezveny == NULL)
	{
		printf("A rendezveny nem letezik!\n");
		menu();
	}
	int sorszam;
	printf("Kerlek add meg annak a jelentkezonek az azonositojat akit torolni szeretnel!\n");
	scanf("%d",&sorszam);
	
	int read_id = 0;
	char read_nev[MAX_SIZE];
	char read_email[MAX_SIZE];
	char read_time[MAX_SIZE];	
	
	while(fgets(line,sizeof(line),rendezveny) != NULL)
	{
		sscanf(line,"%d %s %s %s",&read_id,read_nev,read_email,read_time);
		
		if( read_id != sorszam && read_id > 0 )
		{
			fputs(line,helpfile);
		}
	}
	
	if(read_id == 0)
	{
		fclose(rendezveny);
		fclose(helpfile);
		printf("A rendezvenyhez nem tartozott egy jelentkezo sem!\n");
		menu();
	}
	
	fclose(rendezveny);
	fclose(helpfile);
	
	copy("helpfile.txt",filename);	
}


void copy(const char *from_name, char *to_name)
{
	FILE *from;
	FILE *to;
	
	from = fopen(from_name,"r");
	to = fopen(to_name,"w");
	char line[MAX_SIZE];
	char read_nev[MAX_SIZE];
	char read_email[MAX_SIZE];
	char read_time[MAX_SIZE];	
	int read_id;
	
	while(fgets(line,sizeof(line),from) != NULL)
	{
		sscanf(line,"%d %s %s %s",&read_id,read_nev,read_email,read_time);
		fprintf(to,"%d %s %s %s\n",read_id,read_nev,read_email,read_time);
	}	
	
	fclose(from);
	fclose(to);
	
	remove("helpfile.txt");
}

void rendezvenytorles()
{
	FILE *rendezveny;
	char crid[MAX_SIZE];				//character rendezveny id
	char filename[MAX_SIZE];
	
	printf("Kerlek add meg a torolni kivant rendezveny azonositojat!\n");
	scanf("%s",crid);

	sprintf(filename,"%s.txt",crid);	// rendezv�ny ID ut�n konkaten�ljuk a txt-t majd ezt nyitom meg
	
	rendezveny = fopen(filename,"r");
	if(rendezveny == NULL)
	{
		printf("Nem letezik a megadott rendezveny!");
		menu();
	}
	fclose(fopen(filename, "w"));	// torles
	
}

void ujrendezveny()
{
		int rid; 		// rendezveny id -> felhaszn�l� felel�ss�ge sorrend kialak�t�sa stb
		char crid[10];
		char filename[MAX_SIZE];
		FILE *ujrendezveny;
		printf("Kerlek add meg a rendezveny azonositojat!\n");
		scanf("%d",&rid);
		sprintf(crid,"%d",rid);	// characterr� konvert�ljuk a bek�rt ID-t
		
		sprintf(filename,"%s.txt",crid);	// rendezv�ny ID ut�n konkaten�ljuk a txt-t majd ezt nyitom meg
		
			
		ujrendezveny = fopen(filename,"r");
		if(ujrendezveny == NULL)
		{
			ujrendezveny = fopen(filename,"wb");
			fclose(ujrendezveny);
			printf("Uj rendezveny letrehozva a kovetkezo azonositoval: %d\n\n",rid);
			menu();
		}
		else
		{
			printf("A rendezveny mar letezik!\n\n");
			fclose(ujrendezveny);
			menu();
		}
}

















