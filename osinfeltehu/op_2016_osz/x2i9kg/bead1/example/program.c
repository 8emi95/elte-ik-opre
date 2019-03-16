/*
K�sz�tsen C programot, amely zenesz�mok adatait tartja nyilv�n, 
megval�s�tva a list�z�st, besz�r�st, m�dos�t�st, t�rl�st illetve 
kereshet�s�geket. Az adatokat (szerz�, c�m, sz�m hossza, m�faja) 
bin�ris f�jlban t�rolja. A megold�s legyen helytakar�kos, azaz a 
szerz�k nev�t illetve a m�fajok megnevez�s�t egy-egy sorsz�mmal 
helyettes�tse az adatf�jlban. A n�v-sorsz�m p�rokat pedig m�sik 
f�jlban t�rolja. 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIZE 512

void menu();
void listazas();
void beszuras();
void modositas();
void torles();
void kereses();
void copyfile();

int main(int argc,char *argv[])
{
    menu();
	
	return 0;
}

void menu()
{
	char menu[MAX_SIZE];	
	while(1 < 2)
	{
		printf("\nNyomj egy gombot: (help: h)\t");
		scanf("%s",menu);
		
		if( strcmp(menu,"h") == 0 )
		{
			printf("LISTAZAS:\tl\n");
			printf("BESZURAS:\tb\n");
			printf("MODOSITAS:\tm\n");
			printf("TORLES:\t\tt\n");
			printf("KERESES:\tk\n");
			printf("KILEPES:\tPress CTRL + C to exit\n");
		}
		if( strcmp(menu,"l") == 0 )
		{
			listazas();
		}
		else if( strcmp(menu,"b") == 0 )
		{
			beszuras();
		}
		else if( strcmp(menu,"m") == 0 )
		{
			modositas();
		}
		else if( strcmp(menu,"t") == 0 )
		{
			torles();
		}
		else if( strcmp(menu,"k") == 0 )
		{
			kereses();
		}
	}	
}

void listazas()		//LIST�Z�S
{
	FILE *musics;
	FILE *mid;
	FILE *eid;
	
	musics = fopen("music.txt","r");
	mid = fopen("mid.txt","r");
	eid = fopen("eid.txt","r");
		
	char kiir[MAX_SIZE];
	printf("\nZENESZAMOK ADATAI:\n");
	while(fgets(kiir,100,musics) != NULL)
	{
		printf("%s",kiir);
	};
	
	printf("\n\nID\tELOADO:\n");
	while(fgets(kiir,100,eid) != NULL)
	{
		printf("%s",kiir);
	};
	
	printf("\n\nID\tMUFAJ:\n");
	while(fgets(kiir,100,mid) != NULL)
	{
		printf("%s",kiir);
	};
	
	fclose(musics);
}

void kereses()
{
	printf("\nMit akarsz keresni?\n");
	printf("Eloado keresese: (e)\n");
	printf("Zeneszam keresese: (z)\n");
	printf("Mufaj keresese: (m)\n");
	
	char menu[MAX_SIZE];
	scanf("%s",menu);
	if( strcmp(menu,"e") == 0)		//EL�AD� KERES�S
	{
		FILE *eID;
		eID = fopen("eid.txt","r");
		
		int in;	//bek�rt ID
		printf("Add meg az eloado ID-jet:\t");
		scanf("%d",&in);

		char sor[MAX_SIZE];
		int id;	//el�ad� ID-je
		char nev[50];	//el�ad� neve
		while(fgets(sor,100,eID) != NULL)
		{
			sscanf(sor,"%d %s",&id,nev);
			if( id == in )	
			{
				printf("A keresett eloado:\t%d %s",id,nev);	
				printf("\n");
				
				FILE *musics;
				musics = fopen("music.txt","r");	
				char m_sor[MAX_SIZE];	
				while(fgets(m_sor,100,musics) != NULL)	
				{
					int sorszam;
					sscanf(m_sor,"%d",&sorszam);	
					if( sorszam == in )	
					{
						printf("%s",m_sor);
					}
				}
				fclose(musics);
			}
		};
		fclose(eID);
	}
	else if( strcmp(menu,"z") == 0 )	
	{
		FILE *musics;
		musics = fopen("music.txt","r");
		
		char cim[MAX_SIZE];	
		printf("Add meg a szam cimet:\t");
		scanf("%s",cim);

		char sor[MAX_SIZE];
		char c[MAX_SIZE];	
		int id;
		while(fgets(sor,100,musics) != NULL)
		{
			sscanf(sor,"%d %s",&id,c);
			if( strcmp(cim,c) == 0 )	
			{
				printf("A keresett dal:\n");	
				printf("%s",sor);
			}
		};
		fclose(musics);
	}
	else if( strcmp(menu,"m") == 0 )	//M�FAJ KERES�S
	{
		FILE *mID;
		mID = fopen("mid.txt","r");
		
		int in;	//bek�rt ID
		printf("Add meg a mufaj ID-jet:\t");
		scanf("%d",&in);

		char sor[MAX_SIZE];
		
		char mnev[50];	//sza�m c�me
		int mid;
		while(fgets(sor,100,mID) != NULL)
		{
			sscanf(sor,"%d %s",&mid,mnev);
			if( mid == in )	
			{
				printf("A keresett mufaj:\t%d %s",in,mnev);	
				printf("\n");
				
				FILE *musics;
				musics = fopen("music.txt","r");	
				int id;	//el�ad� ID-je
				char cim[MAX_SIZE];
				int min;	//perc
				int sec;	//m�sodperc
				char m_sor[MAX_SIZE];	//music f�jlban l�v� sor
				while(fgets(m_sor,100,musics) != NULL)	
				{
					int sorszam;
					sscanf(m_sor,"%d %s %d:%d %d",&id,cim,&min,&sec,&sorszam);	//sorb�l beolvasom az adatokat (nekem az utols� kell)
					if( sorszam == in )	
					{
						printf("%s",m_sor);
					}
				}
				fclose(musics);
			}
		};
		fclose(mID);
	}
	else
	{
		printf("Rossz gombot nyomtal!\n");
		kereses();
	}
}

void beszuras()		//BESZ�R�S
{
	FILE *musics;
	musics = fopen("music.txt","a+");
	
	int eid;	//el�ad� ID-je
	char cim[MAX_SIZE];	//sz�m c�me
	int min;	//perc
	int sec;	//m�sodperc
	int mid; 	//m�faj ID-je
	
	printf("Add meg a zeneszam adatait:");
	printf("\nEloado azonositoja:\t");
	scanf("%d",&eid);
	printf("\nSzam cime:\t");
	scanf("%s",cim);
	printf("\nPerc:\t");
	scanf("%d",&min);
	printf("\nMasodperc:\t");
	scanf("%d",&sec);
	printf("\nMufaj azonositoja:\t");
	scanf("%d",&mid);
	
	fprintf(musics,"\n%d %s %d:%d %d",eid,cim,min,sec,mid);
	
	fclose(musics);
}

void torles()		//T�RL�S
{
	char cim[MAX_SIZE];
	printf("Add meg annak szamnak a cimet, amelyiket torolni szeretned:\t");
	scanf("%s",cim);
	
	FILE *music;
	FILE *helpfile;
	music = fopen("music.txt","r");
	helpfile = fopen("helpfile.txt","w");
	char line[MAX_SIZE];
	
	while(fgets(line,sizeof(line),music) != NULL)
	{
		int id;
		char title[MAX_SIZE];
		sscanf(line,"%d %s",&id,title);
		if( strcmp(title,cim) != 0 )
		{
			fputs(line,helpfile);
		}
	}
	
	fclose(music);
	fclose(helpfile);
	
	copy_file("helpfile.txt","music.txt");
}

void copy_file(const char *from,const char *to)
{
	FILE *fr;
	FILE *t;
	fr = fopen(from,"r");
	t = fopen(to,"w");
	
	char line[MAX_SIZE];
	int eid;
	char cim[MAX_SIZE];
	int min;
	int sec;
	int mid;
	while(fgets(line,sizeof(line),fr) != NULL)
	{
		sscanf(line,"%d %s %d:%d %d",&eid,cim,&min,&sec,&mid);
		fprintf(t,"%d %s %d:%d %d\n",eid,cim,min,sec,mid);
	}
	
	fclose(fr);
	fclose(t);
	
	remove("helpfile.txt");
}

void modositas()		//M�DOS�T�S
{
	char cim[MAX_SIZE];
	printf("Add meg annak szamnak a cimet, amelyiket modositani szeretned:\t");
	scanf("%s",cim);
	
	FILE *music;
	FILE *helpfile;
	music = fopen("music.txt","r");
	helpfile = fopen("helpfile.txt","w");
	char line[MAX_SIZE];
	
	while(fgets(line,sizeof(line),music) != NULL)
	{
		int id;
		char title[MAX_SIZE];
		int mins;
		int secs;
		int mid;
		sscanf(line,"%d %s %d:%d %d",&id,title,&mins,&secs,&mid);
		
		//Bek�rt uj adatok
		int newID;
		char newTITLE[MAX_SIZE];
		int newMIN;
		int newSEC;
		int newMID;
		
		if( strcmp(title,cim) == 0 )
		{
			char menu[MAX_SIZE];
			
			printf("Mit szeretnel modositani?\n");
			printf("Eloado: (e)\n");
			printf("Szam c�m: (c)\n");
			printf("Sz�m hossza: (h)\n");
			printf("Mufaj: (m)\n");
			
			scanf("%s",menu);
			
			if( strcmp(menu,"e") == 0 )
			{
				printf("Add meg az uj eloado ID-jet:\t");
				scanf("%d",&newID);
				fprintf(helpfile,"%d %s %d:%d %d\n",newID,title,mins,secs,mid);
			}
			else if( strcmp(menu,"c") == 0 )
			{
				printf("Add meg az uj cimet:\t");
				scanf("%s",newTITLE);
				fprintf(helpfile,"%d %s %d:%d %d\n",id,newTITLE,mins,secs,mid);
			}
			else if( strcmp(menu,"h") == 0 )
			{
				printf("Add meg a percet:\t");
				scanf("%d",&newMIN);
				printf("\nAdd meg a masodpercet:\t");
				scanf("%d",&newSEC);
				fprintf(helpfile,"%d %s %d:%d %d\n",id,title,newMIN,newSEC,mid);
			}
			else if( strcmp(menu,"m") == 0 )
			{
				printf("Add meg az uj mufaj ID-jet:\t");
				scanf("%d",&newMID);
				fprintf(helpfile,"%d %s %d:%d %d\n",id,title,mins,secs,newMID);
			}
			else
			{
				modositas();
			}
		}
		else
		{
			fputs(line,helpfile);
		}
	}
	
	fclose(music);
	fclose(helpfile);
	
	copy_file("helpfile.txt","music.txt");	
}