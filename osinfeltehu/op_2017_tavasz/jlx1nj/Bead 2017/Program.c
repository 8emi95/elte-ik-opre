/*
A Magyar Labdajátékok Szervezete (MLSZ) nemzeti konzultációt hirdet a labdajátékok felvirágoztatására!
Első lépésként meg kell alkotni azt a kérdéssort és hozzá kérdésenként azt a minimum kettő, maximum 4
választási lehetőséget amelyet majd a labdajátékok jobbítása iránt felelősséget érzők kitölthetnek.
A készítendő program adjon lehetőséget a kérdéssor megalkotására. Tudjunk új kérdést felvenni, kérdést
módosítani és kérdést törölni. Módosításhoz, törléshez kínálja fel választásra a kérdést és válaszait
a program! Tudjunk emellett természetesen kérdéseket és válaszaikat listázni! A kérdésekhez tároljuk
automatikusan annak felvételi dátumát (év,hó,nap,óra,perc,másodperc). 
Végezetül a program adjon lehetőséget a kérdések véglegesítésére, azaz annak meghatározására, hogy mely
kérdések szerepeljenek a végső konzultációs kérdőíven!
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 512

void menu();
void listazas();
void uj();
void torles();
void modositas();
void veglegesites();
void kerdessor();

int main(int argc,char *argv[])
{
	FILE *File;  //fileok létrehozása ha nem léteznek
	File = fopen("questions.txt","r");
	if(File == NULL)
	{
		File = fopen("questions.txt","w");
	}
	fclose(File);
	File = fopen("qid.txt","r");
	if(File == NULL)
	{
		File = fopen("qid.txt","w");
	}
	fclose(File);
	File = fopen("aid.txt","r");
	if(File == NULL)
	{
		File = fopen("aid.txt","w");
	}
	fclose(File);
	
	menu();
	
	return 0;
}

void menu()			//Menu
{
	char menu[MAX_SIZE];	
	while(1 < 2)
	{
		printf("\nNyomj egy gombot: (help: h)\t");
		scanf("%s",menu);
		
		if( strcmp(menu,"h") == 0 )
		{
			printf("LISTAZAS:\tl\n");
			printf("UJ:\t\tu\n");
			printf("MODOSITAS:\tm\n");
			printf("TORLES:\t\tt\n");
			printf("VEGLEGESITES:\tv\n");
			printf("KERDESSOR:\tk\n");
			printf("KILEPES:\tPress CTRL + C to exit\n");
		}
		if( strcmp(menu,"l") == 0 )
		{
			listazas();
		}
		else if( strcmp(menu,"u") == 0 )
		{
			uj();
		}
		else if( strcmp(menu,"m") == 0 )
		{
			modositas();
		}
		else if( strcmp(menu,"t") == 0 )
		{
			torles();
		}
		else if( strcmp(menu,"v") == 0 )
		{
			veglegesites();
		}
		else if( strcmp(menu,"k") == 0 )
		{
			kerdessor();
		}
	}	
}

void listazas()		//LISTÁZÁS
{
	FILE *questions;
	FILE *qid;
	FILE *aid;
	
	questions = fopen("questions.txt","r");
	qid = fopen("qid.txt","r");
	aid = fopen("aid.txt","r");
		
	char line[MAX_SIZE]; //aktualis sor
	int qid1; //elso filebol kiolvasott kérdés azonosító
	int qid2 = 0; //2-3 filebol kiolvasott kérdés azonosító
	int numAns; //Válaszok száma
	int i = 0;  //Aktuális válasz sorszáma
	char datetime[MAX_SIZE]; //dátum
	char question[MAX_SIZE]; // kérdés
	while(fgets(line,sizeof(line),questions) != NULL)
	{
		
		strncpy(datetime, "", sizeof(datetime));
		sscanf(line,"%d %d %512c",&qid1,&numAns,datetime);
		strtok(datetime, "\n");
		
		char ans[numAns][MAX_SIZE]; //kérdéshez tartozó válaszok tömbje
		
		fseek(qid,0,SEEK_SET); //qid file elejére ugrás
		while(fgets(line,sizeof(line),qid) != NULL && qid1 != qid2)
		{
			strncpy(question, "", sizeof(question));
			sscanf(line,"%d %512c",&qid2,question);
			strtok(question, "\n");
			strncpy(line, "", sizeof(line));
		};
		if ( qid1 == qid2)
		{
			fseek(aid,0,SEEK_SET); //aid file elejére ugrás
			i = 0;
			while(fgets(line,sizeof(line),aid) != NULL && i != numAns)
			{
				strncpy(ans[i],"", sizeof(ans[i]));
				sscanf(line,"%d %512c",&qid2,ans[i]);
				strtok(ans[i], "\n");
				if(qid1 == qid2)
				{
					i++;
				}
				strncpy(line, "", sizeof(line));
			};
			if ( i == numAns)
			{
				printf("\nKERDES:\n");
				printf("%d %s ( %s )\n",qid1,question,datetime);
				printf("VALASZOK:\n");
				i = 0;
				while( i != numAns)
				{
					printf("%s\n",ans[i]);
					++i;
				}
			}
		}
		strncpy(line, "", sizeof(line));
	};

	fclose(questions);
	fclose(qid);
	fclose(aid);
}

void uj()			//Új
{
	FILE *q; // question file megnyítása a legnagyobb azonosító kinyerésére
	q = fopen("questions.txt","r");
	
	char line[MAX_SIZE]; 
	int qid1 = 0;
	while(fgets(line,sizeof(line),q) != NULL){}; //file utolsó soráig megyünk
	sscanf(line,"%d",&qid1);
	fclose(q);

	
	FILE *questions;
	FILE *qid;
	FILE *aid;
	questions = fopen("questions.txt","a+");
	qid = fopen("qid.txt","a+");
	aid = fopen("aid.txt","a+");

	int numAns; //A válaszok száma
	char question[MAX_SIZE]; //a kérdés
	
	printf("Add meg a kerdes adatait:\n");
	printf("Kerdes:\t");
	strncpy(question, "", sizeof(question));
    scanf(" %[^\n]",&question);
	printf("Valaszok szama(2-4):\t");
	scanf("%d",&numAns);
	char ans[numAns][MAX_SIZE]; //a kérdéshez tartozó válaszok tömbje
	int i = 0;
	while(i != numAns)
	{
		printf("%d. valasz:",i+1);
		strncpy(ans[i],"", sizeof(ans[i]));
		scanf(" %[^\n]",&ans[i]);
		++i;
	}
	
	time_t timer; //másodpercig jelső idő típús
    char buffer[26];
    time(&timer); //idő beállítása
    strftime(buffer, 26, "%Y %m %d %H %M %S", localtime(&timer)); //struct tm típusra konvertáljuk a timer változót és szétdaraboljuk a bufferbe
	if (qid1 != 0)
	{
		fprintf(questions,"\n");
		fprintf(qid,"\n");
	}
	fprintf(questions,"%d %d %s",qid1+1,numAns,buffer);
	fprintf(qid,"%d %s",qid1+1,question);
	i=0;
	while(i != numAns)
	{
		if (qid1 != 0 || i != 0)
		{
			fprintf(aid,"\n");
		}
		fprintf(aid,"%d %s",qid1+1,ans[i]);
		++i;
	}
	
	fclose(questions);
	fclose(qid);
	fclose(aid);
}

void modositas() 	//MÓDOSÍTÁS
{
	listazas();
	
	FILE *questions;
	FILE *qid;
	FILE *aid;
	FILE *helpfile;
	
	questions = fopen("questions.txt","r");
	qid = fopen("qid.txt","r");
	aid = fopen("aid.txt","r");
	helpfile = fopen("helpfile.txt","w"); 
	
	char line[MAX_SIZE]; //beolvasott sor
	int qid1 = 0; // aktualis azonosito
	int qid2; //modositando kerdes azonositoja
	int numAns; //valaszok szama
	int numAns2; //uj valaszok szama
	char mod[MAX_SIZE]; //mit modositunk
	char datetime[MAX_SIZE]; //datum
	char question[MAX_SIZE]; //kerdes
	char ans[MAX_SIZE]; //Valasz
	int j = 0; //elso sor-e
	int i; //hanyadik valasznak
	
	printf("Adja meg annak a kerdesnek a azonositojat, amelyiket modositani szeretne:\t");
	scanf("%d",&qid2);
	printf("Adja meg mit szeretne modositani rajta(k - Kerdest, n - valaszok szamat, v - valaszokat):\t");
	scanf("%s",mod);
	
	if( strcmp(mod,"k") == 0 )
	{

		while(fgets(line,sizeof(line),qid) != NULL)
		{
			
			strncpy(question, "", sizeof(question));
			sscanf(line,"%d %512c",&qid1,question);
			strtok(question, "\n");
			if(j != 0)
			{
				fprintf(helpfile,"\n");
				
			}
			if( qid1 != qid2 )
			{
				fprintf(helpfile,"%d %s",qid1,question);
				++j;
			}
			else
			{
				printf("Regi kerdes:\t %s \n",question);
				printf("Adja meg az uj kerdest:\t");
				scanf(" %[^\n]s", question);
				fprintf(helpfile,"%d %s",qid1,question);
				++j;
			}
			strncpy(line, "", sizeof(line));
		}
		fclose(qid);
		fclose(helpfile);
		remove("qid.txt");
		rename("helpfile.txt","qid.txt");
		
		qid1 = 0;
		j = 0;
		helpfile = fopen("helpfile.txt","w");
		
		while(fgets(line,sizeof(line),questions) != NULL)
		{
			strncpy(datetime, "", sizeof(datetime));
			sscanf(line,"%d %d %512c",&qid1,&numAns,datetime);
			strtok(datetime, "\n");
			if(j != 0)
			{
				fprintf(helpfile,"\n");
			}
			if( qid1 != qid2 )
			{
				fprintf(helpfile,"%d %d %s",qid1,numAns,datetime);
				++j;
			}
			else
			{
				printf("Adja meg a valaszok szamat(2-4):\t");
				scanf("%d", &numAns);
				numAns2 = numAns;
				printf("2");
				time_t timer; //másodpercig jelső idő típús
				char buffer[26];
				time(&timer);
				strftime(buffer, 26, "%Y %m %d %H %M %S", localtime(&timer));
				printf("3");
				fprintf(helpfile,"%d %d %s",qid1,numAns,buffer);
				++j;
			}
			strncpy(line, "", sizeof(line));
		};
		fclose(questions);
		fclose(helpfile);
		remove("questions.txt");
		rename("helpfile.txt","questions.txt");
		
		qid1 = 0;
		j = 0;
		i = 0;
		helpfile = fopen("helpfile.txt","w");
		
		while(fgets(line,sizeof(line),aid) != NULL)
		{
			strncpy(ans,"", sizeof(ans));
			sscanf(line,"%d %512c",&qid1,ans);
			strtok(ans, "\n");
			if(j != 0)
			{
				fprintf(helpfile,"\n");
			}
			if( qid1 != qid2 )
			{
				fprintf(helpfile,"%d %s",qid1,ans);
				++j;
			}
			else
			{
				j = 0;
				while ( i != numAns2)
				{
					if(j != 0)
					{
						fprintf(helpfile,"\n");
					}
					printf("Adja meg a %d-ik valaszt:\t", i+1);
					scanf(" %[^\n]s", ans);
					fprintf(helpfile,"%d %s",qid1,ans);
					++j;
					++i;
				}
			}
			strncpy(line, "", sizeof(line));
		};
		fclose(aid);
		fclose(helpfile);
		remove("aid.txt");
		rename("helpfile.txt","aid.txt");
	}
	else if( strcmp(mod,"n") == 0 )
	{
		while(fgets(line,sizeof(line),questions) != NULL)
		{
			strncpy(datetime, "", sizeof(datetime));
			sscanf(line,"%d %d %512c",&qid1,&numAns,datetime);
			strtok(datetime, "\n");
			if(j != 0)
			{
				fprintf(helpfile,"\n");
			}
			if( qid1 != qid2 )
			{
				fprintf(helpfile,"%d %d %s",qid1,numAns,datetime);
				++j;
			}
			else
			{
				printf("Regi valaszok szama: %d\n",numAns);
				printf("Adja meg az uj valaszok szamat(2-4):\t");
				scanf("%d", &numAns);
				numAns2 = numAns;
				time_t timer;
				char buffer[26];
				time(&timer);
				strftime(buffer, 26, "%Y %m %d %H %M %S", localtime(&timer));
				fprintf(helpfile,"%d %d %s",qid1,numAns,buffer);
				++j;
			}
			strncpy(line, "", sizeof(line));
		};
		fclose(questions);
		fclose(helpfile);
		remove("questions.txt");
		rename("helpfile.txt","questions.txt");
		
		qid1 = 0;
		j = 0;
		i = 0;
		helpfile = fopen("helpfile.txt","w");
		
		while(fgets(line,sizeof(line),aid) != NULL)
		{
			strncpy(ans,"", sizeof(ans));
			sscanf(line,"%d %512c",&qid1,ans);
			strtok(ans, "\n");
			if(j != 0)
			{
				fprintf(helpfile,"\n");
			}
			if( qid1 != qid2 )
			{
				fprintf(helpfile,"%d %s",qid1,ans);
				++j;
			}
			else
			{
				j = 0;
				while ( i != numAns2)
				{
					if(j != 0)
					{
						fprintf(helpfile,"\n");
					}
					printf("Adja meg a %d-ik valaszt:\t", i+1);
					scanf(" %[^\n]s", ans);
					fprintf(helpfile,"%d %s",qid1,ans);
					++j;
					++i;
				}
			}
			strncpy(line, "", sizeof(line));
		};
		fclose(aid);
		fclose(helpfile);
		remove("aid.txt");
		rename("helpfile.txt","aid.txt");
	}
	else if( strcmp(mod,"v") == 0 )
	{
		while(fgets(line,sizeof(line),questions) != NULL && qid1 != qid2)
		{
			 sscanf(line,"%d %d",&qid1,&numAns);
		}
		
		printf("A valaszok szama: %d", numAns);
		printf("Regi valaszok:\n");
		i = 1;
		while(fgets(line,sizeof(line),aid) != NULL)
		{
			strncpy(ans,"", sizeof(ans));
			sscanf(line,"%d %512c",&qid1,ans);
			strtok(ans, "\n");
			if(j != 0 && qid1 != qid2)
			{
				fprintf(helpfile,"\n");
			}
			if( qid1 != qid2 )
			{
				fprintf(helpfile,"%d %s",qid1,ans);
				++j;
			}
			else
			{
				printf("Az %d-ik valasz: %s \n",i,ans);
				if( i == numAns)
				{
					printf("Adja meg az uj valaszokat:\n");
					i = 0;
					j = 0;
					while( i != numAns)
					{
						if(j != 0)
						{
							fprintf(helpfile,"\n");
						}
						printf("Az %d-ik valaszt:\t",i+1);
						scanf(" %[^\n]s", ans);
						fprintf(helpfile,"%d %s",qid1,ans);
						++i;
						++j;
					}
				}
				++i;
			}
			strncpy(line, "", sizeof(line));
		};
		fclose(aid);
		fclose(helpfile);
		remove("aid.txt");
		rename("helpfile.txt","aid.txt");

		
		helpfile = fopen("helpfile.txt","w");
		j=0;
		fseek(questions,0,SEEK_SET);
		while(fgets(line,sizeof(line),questions) != NULL)
		{
			strncpy(datetime, "", sizeof(datetime));
			sscanf(line,"%d %d %512c",&qid1,&numAns,datetime);
			strtok(datetime, "\n");
			if(j != 0)
			{
				fprintf(helpfile,"\n");
			}
			if( qid1 != qid2 )
			{
				fprintf(helpfile,"%d %d %s",qid1,numAns,datetime);
				++j;
			}
			else
			{
				time_t timer;
				char buffer[26];
				time(&timer);
				strftime(buffer, 26, "%Y %m %d %H %M %S", localtime(&timer));
				fprintf(helpfile,"%d %d %s",qid1,numAns,buffer);
				++j;
			}
			strncpy(line, "", sizeof(line));
		};
		fclose(questions);
		fclose(helpfile);
		remove("questions.txt");
		rename("helpfile.txt","questions.txt");
	}
}

void torles()		//TÖRLÉS
{
	listazas();
	int id; // a törölni kívánt kérdés azonosítója
	printf("\nAdd meg annak kerdesnek az azonositojat, amelyiket torolni szeretned:\t");
	scanf("%d",&id);
	
	FILE *questions;
	FILE *helpfile; //segédfile ebbe kerülnek az adatok a tölöt sort kivéve
	FILE *qid;
	FILE *aid;	
	
	questions = fopen("questions.txt","r");
	helpfile = fopen("helpfile.txt","w");
	qid = fopen("qid.txt","r");
	aid = fopen("aid.txt","r");
	
	char ans[MAX_SIZE]; //válasz
	char datetime[MAX_SIZE]; //dátum
	char line[MAX_SIZE]; // aktuális sor
	char question[MAX_SIZE]; //kérdés
	int j = 0; // első sor--e
	int qid1;  // aktuális kérdés azonosítója
	int numAns; // válaszok száma
	
	while(fgets(line,sizeof(line),questions) != NULL)
	{
		strncpy(datetime, "", sizeof(datetime));
		sscanf(line,"%d %d %512c",&qid1,&numAns,datetime);
		strtok(datetime, "\n");
		if (j != 0)
		{
			fprintf(helpfile,"\n");
		}
		if( qid1 != id )
		{
			fprintf(helpfile,"%d %d %s",qid1,numAns,datetime);
			++j;
		}
		strncpy(line, "", sizeof(line));
	}
	
	fclose(questions);
	fclose(helpfile);
	remove("questions.txt");
	rename("helpfile.txt","questions.txt");

	helpfile = fopen("helpfile.txt","w");
	j = 0;
	while(fgets(line,sizeof(line),qid) != NULL)
	{
		
		strncpy(question, "", sizeof(question));
		sscanf(line,"%d %512c",&qid1,question);
		strtok(question, "\n");
		if(j != 0)
		{
			fprintf(helpfile,"\n");
		}
		if( qid1 != id )
		{
			fprintf(helpfile,"%d %s",qid1,question);
			++j;
		}	
		strncpy(line, "", sizeof(line));
	}
	
	fclose(qid);
	fclose(helpfile);
	remove("qid.txt");
	rename("helpfile.txt","qid.txt");

	helpfile = fopen("helpfile.txt","w");
	j = 0;
	while(fgets(line,sizeof(line),aid) != NULL)
	{
		
		strncpy(ans, "", sizeof(ans));
		sscanf(line,"%d %512c",&qid1,ans);
		strtok(ans, "\n");
		if ( j != 0 )
		{
			fprintf(helpfile,"\n");
		}
		if( qid1 != id )
		{
			fprintf(helpfile,"%d %s",qid1,ans);
			++j;
		}
		strncpy(line, "", sizeof(line));
	}
	
	fclose(aid);
	fclose(helpfile);
	remove("aid.txt");
	rename("helpfile.txt","aid.txt");
}

void veglegesites() //VÉGLEGESÍTÉS
{
	listazas();
	FILE *vegleges;
	vegleges = fopen("Vegleges.txt","w");
	int qid = -1;
	int i = 0;
	printf("\nAdja meg azon kerdesek azonositojat ,amelyek benne lesznek a vegleges kerdoivbe:\n");
	while ( qid != 0 )
	{
		if (qid > 0)
		{
			if (i != 0)
			{
				fprintf(vegleges,"\n");
			}
			fprintf(vegleges,"%d",qid);
			++i;
		}
		printf("Ha vegzett a 0-as sorszamot adja meg:\t");
		scanf("%d",&qid);
		
	}
	
	fclose(vegleges);
}

void kerdessor()	//A vegleges kérdéssor kiírása
{
	FILE *vegleges;
	FILE *questions;
	FILE *qid;
	FILE *aid;
	
	vegleges = fopen("Vegleges.txt","w");
	questions = fopen("questions.txt","r");
	qid = fopen("qid.txt","r");
	aid = fopen("aid.txt","r");
		
	char line[MAX_SIZE]; //aktualis sor
	int qid1; //elso filebol kiolvasott kérdés azonosító
	int qid2 = 0; //2-3 filebol kiolvasott kérdés azonosító
	int numAns; //Válaszok száma
	int i = 0;  //Aktuális válasz sorszáma
	char datetime[MAX_SIZE]; //dátum
	char question[MAX_SIZE]; // kérdés
	
	while(fgets(line,sizeof(line),vegleges) != NULL)
	{
		sscanf(line,"%d",&qid1);
		
		fseek(questions,0,SEEK_SET);
		while(fgets(line,sizeof(line),questions) != NULL && qid1 != qid2)
		{
			strncpy(datetime, "", sizeof(datetime));
			sscanf(line,"%d %d %512c",&qid2,&numAns,datetime);
			strtok(datetime, "\n");
		}	
		char ans[numAns][MAX_SIZE]; //kérdéshez tartozó válaszok tömbje
		if ( qid1 == qid2)
		{	
			qid2 = 0;
			fseek(qid,0,SEEK_SET); //qid file elejére ugrás
			while(fgets(line,sizeof(line),qid) != NULL && qid1 != qid2)
			{
				strncpy(question, "", sizeof(question));
				sscanf(line,"%d %512c",&qid2,question);
				strtok(question, "\n");
				strncpy(line, "", sizeof(line));
			};
			if ( qid1 == qid2)
			{
				fseek(aid,0,SEEK_SET); //aid file elejére ugrás
				i = 0;
				while(fgets(line,sizeof(line),aid) != NULL && i != numAns)
				{
					strncpy(ans[i],"", sizeof(ans[i]));
					sscanf(line,"%d %512c",&qid2,ans[i]);
					strtok(ans[i], "\n");
					if(qid1 == qid2)
					{
						i++;
					}
					strncpy(line, "", sizeof(line));
				};
				if ( i == numAns)
				{
					printf("\nKERDES:\n");
					printf("%s ( %s )\n",question,datetime);
					printf("VALASZOK:\n");
					i = 0;
					while( i != numAns)
					{
						printf("%s\n",ans[i]);
						++i;
					}
				}
			}
			strncpy(line, "", sizeof(line));
		};
	};
	fclose(questions);
	fclose(qid);
	fclose(aid);
}