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
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX_SIZE 512

void menu();
void listazas();
void uj();
void torles();
void modositas();
void veglegesites();
void kerdessor();
void kitoltes(char *Varos);

int main(int argc,char *argv[])	{
	FILE *File;  //fileok létrehozása ha nem léteznek
	File = fopen("questions.txt","r");
	if(File == NULL)	{
		File = fopen("questions.txt","w");
	}
	fclose(File);
	File = fopen("qid.txt","r");
	if(File == NULL)	{
		File = fopen("qid.txt","w");
	}
	fclose(File);
	File = fopen("aid.txt","r");
	if(File == NULL)	{
		File = fopen("aid.txt","w");
	}
	fclose(File);
	
	if (argc == 2)	{
		menu(argv[1]);
	}
	else if(argc > 2)	{
		printf("Tul Sok parameter!\n");
		exit(1);
	}
	else	{
		printf("1 parametert meg kell adni!\n");
		exit(1);
	}
	return 0;
}

void menu(char *Varos)	{ 			//Menu
	char menu[MAX_SIZE];	
	while(1 < 2)	{
		printf("\nNyomj egy gombot: (help: h)\t");
		scanf("%s",menu);
		
		if( strcmp(menu,"h") == 0 )		{
			printf("LISTAZAS:\tl\n");
			printf("UJ:\t\tu\n");
			printf("MODOSITAS:\tm\n");
			printf("TORLES:\t\tt\n");
			printf("VEGLEGESITES:\tv\n");
			printf("KERDESSOR:\tk\n");
			printf("KILEPES:\tPress CTRL + C to exit\n");
		}
		if( strcmp(menu,"l") == 0 )		{
			listazas();
		}
		else if( strcmp(menu,"u") == 0 )	{
			uj();
		}
		else if( strcmp(menu,"m") == 0 )	{
			modositas();
		}
		else if( strcmp(menu,"t") == 0 )	{
			torles();
		}
		else if( strcmp(menu,"v") == 0 )	{
			veglegesites();
			kitoltes(Varos);
		}
		else if( strcmp(menu,"k") == 0 )	{
			kerdessor();
		}
	}	
}

void listazas()	{		//LISTÁZÁS
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
	while(fgets(line,sizeof(line),questions) != NULL)	{
		
		strncpy(datetime, "", sizeof(datetime));
		sscanf(line,"%d %d %512c",&qid1,&numAns,datetime);
		strtok(datetime, "\n");
		
		char ans[numAns][MAX_SIZE]; //kérdéshez tartozó válaszok tömbje
		
		fseek(qid,0,SEEK_SET); //qid file elejére ugrás
		while(fgets(line,sizeof(line),qid) != NULL && qid1 != qid2)		{
			strncpy(question, "", sizeof(question));
			sscanf(line,"%d %512c",&qid2,question);
			strtok(question, "\n");
			strncpy(line, "", sizeof(line));
		};
		if ( qid1 == qid2)		{
			fseek(aid,0,SEEK_SET); //aid file elejére ugrás
			i = 0;
			while(fgets(line,sizeof(line),aid) != NULL && i != numAns)			{
				strncpy(ans[i],"", sizeof(ans[i]));
				sscanf(line,"%d %512c",&qid2,ans[i]);
				strtok(ans[i], "\n");
				if(qid1 == qid2)				{
					i++;
				}
				strncpy(line, "", sizeof(line));
			};
			if ( i == numAns)		{
				printf("\nKERDES:\n");
				printf("%d %s ( %s )\n",qid1,question,datetime);
				printf("VALASZOK:\n");
				i = 0;
				while( i != numAns)		{
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

void uj()	{			//Új

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
	while(i != numAns)	{
		printf("%d. valasz:",i+1);
		strncpy(ans[i],"", sizeof(ans[i]));
		scanf(" %[^\n]",&ans[i]);
		++i;
	}
	
	time_t timer; //másodpercig jelső idő típús
    char buffer[26];
    time(&timer); //idő beállítása
    strftime(buffer, 26, "%Y %m %d %H %M %S", localtime(&timer)); //struct tm típusra konvertáljuk a timer változót és szétdaraboljuk a bufferbe
	if (qid1 != 0)	{
		fprintf(questions,"\n");
		fprintf(qid,"\n");
	}
	fprintf(questions,"%d %d %s",qid1+1,numAns,buffer);
	fprintf(qid,"%d %s",qid1+1,question);
	i=0;
	while(i != numAns)	{
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

void modositas()	{ 	//MÓDOSÍTÁS

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
	
	if( strcmp(mod,"k") == 0 )	{

		while(fgets(line,sizeof(line),qid) != NULL)	{
			
			strncpy(question, "", sizeof(question));
			sscanf(line,"%d %512c",&qid1,question);
			strtok(question, "\n");
			if(j != 0)	{
				fprintf(helpfile,"\n");
				
			}
			if( qid1 != qid2 )	{
				fprintf(helpfile,"%d %s",qid1,question);
				++j;
			}
			else	{
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
		
		while(fgets(line,sizeof(line),questions) != NULL)	{
			strncpy(datetime, "", sizeof(datetime));
			sscanf(line,"%d %d %512c",&qid1,&numAns,datetime);
			strtok(datetime, "\n");
			if(j != 0)	{
				fprintf(helpfile,"\n");
			}
			if( qid1 != qid2 )	{
				fprintf(helpfile,"%d %d %s",qid1,numAns,datetime);
				++j;
			}
			else	{
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
		
		while(fgets(line,sizeof(line),aid) != NULL)	{
			strncpy(ans,"", sizeof(ans));
			sscanf(line,"%d %512c",&qid1,ans);
			strtok(ans, "\n");
			if(j != 0)	{
				fprintf(helpfile,"\n");
			}
			if( qid1 != qid2 )	{
				fprintf(helpfile,"%d %s",qid1,ans);
				++j;
			}
			else	{
				j = 0;
				while ( i != numAns2)	{
					if(j != 0)	{
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
	else if( strcmp(mod,"n") == 0 )	{
		while(fgets(line,sizeof(line),questions) != NULL)	{
			strncpy(datetime, "", sizeof(datetime));
			sscanf(line,"%d %d %512c",&qid1,&numAns,datetime);
			strtok(datetime, "\n");
			if(j != 0)	{
				fprintf(helpfile,"\n");
			}
			if( qid1 != qid2 )	{
				fprintf(helpfile,"%d %d %s",qid1,numAns,datetime);
				++j;
			}
			else	{
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
		
		while(fgets(line,sizeof(line),aid) != NULL)	{
			strncpy(ans,"", sizeof(ans));
			sscanf(line,"%d %512c",&qid1,ans);
			strtok(ans, "\n");
			if(j != 0)	{
				fprintf(helpfile,"\n");
			}
			if( qid1 != qid2 )	{
				fprintf(helpfile,"%d %s",qid1,ans);
				++j;
			}
			else	{
				j = 0;
				while ( i != numAns2)	{
					if(j != 0)	{
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
	else if( strcmp(mod,"v") == 0 )	{
		while(fgets(line,sizeof(line),questions) != NULL && qid1 != qid2)	{
			 sscanf(line,"%d %d",&qid1,&numAns);
		}
		
		printf("A valaszok szama: %d", numAns);
		printf("Regi valaszok:\n");
		i = 1;
		while(fgets(line,sizeof(line),aid) != NULL)	{
			strncpy(ans,"", sizeof(ans));
			sscanf(line,"%d %512c",&qid1,ans);
			strtok(ans, "\n");
			if(j != 0 && qid1 != qid2)	{
				fprintf(helpfile,"\n");
			}
			if( qid1 != qid2 )	{
				fprintf(helpfile,"%d %s",qid1,ans);
				++j;
			}
			else	{
				printf("Az %d-ik valasz: %s \n",i,ans);
				if( i == numAns)	{
					printf("Adja meg az uj valaszokat:\n");
					i = 0;
					j = 0;
					while( i != numAns)	{
						if(j != 0)	{
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
		while(fgets(line,sizeof(line),questions) != NULL)	{
			strncpy(datetime, "", sizeof(datetime));
			sscanf(line,"%d %d %512c",&qid1,&numAns,datetime);
			strtok(datetime, "\n");
			if(j != 0)	{
				fprintf(helpfile,"\n");
			}
			if( qid1 != qid2 )	{
				fprintf(helpfile,"%d %d %s",qid1,numAns,datetime);
				++j;
			}
			else	{
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

void torles()	{		//TÖRLÉS

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
	
	while(fgets(line,sizeof(line),questions) != NULL)	{
		strncpy(datetime, "", sizeof(datetime));
		sscanf(line,"%d %d %512c",&qid1,&numAns,datetime);
		strtok(datetime, "\n");
		if (j != 0)	{
			fprintf(helpfile,"\n");
		}
		if( qid1 != id )	{
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
	while(fgets(line,sizeof(line),qid) != NULL)	{
		
		strncpy(question, "", sizeof(question));
		sscanf(line,"%d %512c",&qid1,question);
		strtok(question, "\n");
		if(j != 0)	{
			fprintf(helpfile,"\n");
		}
		if( qid1 != id )	{
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
	while(fgets(line,sizeof(line),aid) != NULL)	{
		
		strncpy(ans, "", sizeof(ans));
		sscanf(line,"%d %512c",&qid1,ans);
		strtok(ans, "\n");
		if ( j != 0 )	{
			fprintf(helpfile,"\n");
		}
		if( qid1 != id )	{
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

void veglegesites()	{ 	//VÉGLEGESÍTÉS
	listazas();
	FILE *vegleges;
	vegleges = fopen("Vegleges.txt","w");
	int qid = -1;
	int i = 0;
	printf("\nAdja meg azon kerdesek azonositojat ,amelyek benne lesznek a vegleges kerdoivbe:\n");
	while ( qid != 0 )	{
		if (qid > 0)	{
			if (i != 0)		{
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

void kerdessor()	{	//A vegleges kérdéssor kiírása
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
	
	while(fgets(line,sizeof(line),vegleges) != NULL)	{
		sscanf(line,"%d",&qid1);
		
		fseek(questions,0,SEEK_SET);
		while(fgets(line,sizeof(line),questions) != NULL && qid1 != qid2)	{
			strncpy(datetime, "", sizeof(datetime));
			sscanf(line,"%d %d %512c",&qid2,&numAns,datetime);
			strtok(datetime, "\n");
		}	
		char ans[numAns][MAX_SIZE]; //kérdéshez tartozó válaszok tömbje
		if ( qid1 == qid2)	{	
			qid2 = 0;
			fseek(qid,0,SEEK_SET); //qid file elejére ugrás
			while(fgets(line,sizeof(line),qid) != NULL && qid1 != qid2)		{
				strncpy(question, "", sizeof(question));
				sscanf(line,"%d %512c",&qid2,question);
				strtok(question, "\n");
				strncpy(line, "", sizeof(line));
			};
			if ( qid1 == qid2)	{
				fseek(aid,0,SEEK_SET); //aid file elejére ugrás
				i = 0;
				while(fgets(line,sizeof(line),aid) != NULL && i != numAns)	{
					strncpy(ans[i],"", sizeof(ans[i]));
					sscanf(line,"%d %512c",&qid2,ans[i]);
					strtok(ans[i], "\n");
					if(qid1 == qid2)	{
						i++;
					}
					strncpy(line, "", sizeof(line));
				};
				if ( i == numAns)	{
					printf("\nKERDES:\n");
					printf("%s ( %s )\n",question,datetime);
					printf("VALASZOK:\n");
					i = 0;
					while( i != numAns)	{
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

void kitoltes(char *Varos)	{ //Kitoltes
	FILE 	*vegleges;
	FILE 	*questions;
	FILE 	*qid;
	FILE 	*aid;
	
	int  	fd1[2], fd2[2], nbytes, sizeV, msg[3];
	pid_t   childpid;
	char 	question[MAX_SIZE];
	int 	qid1, numAns[3], numppl, j, i;
	char 	line[MAX_SIZE];
	
	vegleges = fopen("Vegleges.txt","r");
	questions = fopen("questions.txt","r");
	qid = fopen("qid.txt","r");
	aid = fopen("aid.txt","r");
	pipe(fd1);
	pipe(fd2);
	
	sizeV = 0;
	while(fgets(line,sizeof(line),vegleges) != NULL)	{
		sizeV++;
	}
	fseek(vegleges,0,SEEK_SET);
	int Veg[sizeV];
	i = 0;
	while(fgets(line,sizeof(line),vegleges) != NULL)	{
		sscanf(line,"%d",&Veg[i]);
		i++;
	}
	
	if((childpid = fork()) == -1)
	{
			perror("fork");
			exit(1);
	}

	if(childpid == 0)
	{
			close(fd1[0]);
			close(fd2[1]);
			do {
				printf("Kerdezobiztos bejelentkezes: (1 - bejelentkezes/0 - vissza a menu-be)");
				scanf("%d", &msg[0]);
			}while(msg[0] != 0 && msg[0] != 1);
			write(fd1[1], &msg[0], sizeof(msg[0]));
			if(msg[0] == 0)
			{
				close(fd1[1]);
				close(fd2[0]);
				exit(0);
			}
			srand(time(NULL));
			numppl = rand() % 10 + 10;			
			i = 0;
			while ((nbytes = read(fd2[0], &msg[i], sizeof(msg[i])))>0)	{
				read(fd2[0], &numAns[i], sizeof(numAns[i]));
				i++;
			}
			i = 0;
			while(i < numppl)	{
				j = 0;
				while ( j < 3)	{
					int r = rand() % numAns[j];
					write(fd1[1], &j, sizeof(j));
					write(fd1[1], &r, sizeof(r));
					j++;
				}
				i++;
			}
			close(fd1[1]);
			exit(0);
	}
	else
	{
		close(fd1[1]);
		close(fd2[0]);			
		read(fd1[0], &msg[0], sizeof(msg[0]));
		if(msg[0] == 0)
		{
			close(fd1[1]);
			close(fd2[0]);
			return;
		}
		srand(time(NULL));
		msg[0] = rand() % sizeV;
		msg[1] = msg[0];
		while ( msg[0] == msg[1])	{
			msg[1] = rand() % sizeV;
		}
		msg[2] = msg[0];
		while ( msg[0] == msg[2] || msg[1] == msg[2])	{
			msg[2] = rand() % sizeV;
		}
		i = 0;
		while (i < 3)	{
			fseek(questions,0,SEEK_SET);
			while(fgets(line,sizeof(line),questions) != NULL && Veg[msg[i]] != qid1)	{
				sscanf(line,"%d %d",&qid1,&numAns[i]);
			}
			write(fd2[1], &Veg[msg[i]], sizeof(Veg[msg[i]]));
			write(fd2[1], &numAns[i], sizeof(numAns[i]));
			i++;
		}
		close(fd2[1]);
		int Answers[3][4];
		i = 0;
		j = 0;
		while( i < 3) {
			j = 0;
			while ( j < 4)	{
				Answers[i][j] = 0;
				j++;
			}
			i++;
		}
		
		while (read(fd1[0], &i, sizeof(i)) > 0) { // Read until it returns 0 (EOF) or -1 (error)
			read(fd1[0], &j, sizeof(j));
			Answers[i][j]++;
		}
		printf("\n %s varosbanban tartott konzultacio eredmenye:\n\n",Varos);
		i = 0;
		while(i < 3)	{
			fseek(qid,0,SEEK_SET); //qid file elejére ugrás
			while(fgets(line,sizeof(line),qid) != NULL && Veg[msg[i]] != qid1)		{
				strncpy(question, "", sizeof(question));
				sscanf(line,"%d %512c",&qid1,question);
				strtok(question, "\n");
				strncpy(line, "", sizeof(line));
			}
			printf("%s\n",question);
				
			int max = 0;
			j = 1;
			while(j < 4)	{
				if ( Answers[i][j] > Answers[i][max])	{
					max = j;
				}
				j++;
			}
			
			char ans[numAns[i]][MAX_SIZE];
			j = 0;
			fseek(aid,0,SEEK_SET); //aid file elejére ugrás
			while(fgets(line,sizeof(line),aid) != NULL && j != numAns[i])	{
				strncpy(ans[j],"", sizeof(ans[j]));
				sscanf(line,"%d %512c",&qid1,ans[j]);
				strtok(ans[j], "\n");
				if(Veg[msg[i]] == qid1)	{
					j++;
				}
				strncpy(line, "", sizeof(line));
			};
			printf("%s: %d db\n",ans[max], Answers[i][max]);
			i++;
		}	
	}
	fclose(vegleges);
	fclose(questions);
	fclose(qid);
	fclose(aid);
}