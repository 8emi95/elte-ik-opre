/*
A Magyar Labdajátékok Szervezete (MLSZ) nemzeti konzultációt hirdet a labdajátékok felvirágoztatására!
Első lépésként meg kell alkotni azt a kérdéssort és hozzá kérdésenként azt a minimum kettő,
maximum 4 választási lehetőséget amelyet majd a labdajátékok jobbítása iránt felelősséget érzők kitölthetnek.
A készítendő program adjon lehetőséget a kérdéssor megalkotására. Tudjunk új kérdést felvenni,
kérdést módosítani és kérdést törölni. Módosításhoz, törléshez kínálja fel választásra a kérdést és válaszait a program!
Tudjunk emellett természetesen kérdéseket és válaszaikat listázni!
A kérdésekhez tároljuk automatikusan annak felvételi dátumát (év,hó,nap,óra,perc,másodperc). 
Végezetül a program adjon lehetőséget a kérdések véglegesítésére, azaz annak meghatározására,
hogy mely kérdések szerepeljenek a végső konzultációs kérdőíven!
Készítsünk C programot, amellyel a leírt tevékenység elvégezhető. Az adatokat fájlban tároljuk.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 512

void menu();
//void rendezveny_inditasa();
void megadas();
void listazas();
void torles();
void modositas();
//void rendezveny_torlese();
void copy_file();
void copy_file_rend();

int main(int argc,char *argv[])
{
	
	menu();
	
	return 0;
}

void menu()
{
	char menup[MAX_SIZE];
	
	system("clear");
	
	while(1)
	{
		printf("===========================================================\n");
		printf("Udvozoli ont a Magyar Labdajatekok Szervezete\n");
		printf("Az alabb felsorolt lehetosegek allnak rendelkezesere:\n");
		printf("(1) \tKerdes-Valasz megadasa\n");
		printf("(2) \tKerdes-Valasz listazasa\n");
		printf("(3) \tKerdes-Valasz modositasa\n");
		printf("(4) \tKerdes-Valasz torlese\n");
		printf("===========================================================\n");
		printf("(5) \tKerdessor megalkotasa (Kerdessor.txt)\n");
		printf("===========================================================\n");
		//printf("(6) \tRendezveny torlese\n");
		printf("A program bezarasahoz nyomja meg CTRL + C kombinaciot\n>");
		scanf("%s",menup);
		printf("===========================================================\n");
			
			if( strcmp(menup,"1") == 0 )
			{
				megadas();
			}
			else if( strcmp(menup,"2") == 0 )
			{
				listazas();
				//char temp[MAX_SIZE];
				printf("\nFolytatashoz nyomjon ENTER-t\n>");
				while (getchar() != '\n');
				while (getchar() != '\n');
				//scanf("%c",temp);
				system("clear");
			}
			else if( strcmp(menup,"3") == 0 )
			{
				modositas();
				system("clear");
				printf("A modositas sikeresen befelyezodott\n");
			}
			else if( strcmp(menup,"4") == 0 )
			{
				torles();
				system("clear");
				printf("A torles sikeresen befelyezodott\n");
			}
			else if( strcmp(menup,"5") == 0 )
			{
				kerdessorMegalkotasa();
				printf("\nFolytatashoz nyomjon ENTER-t\n>");
				while (getchar() != '\n');
				while (getchar() != '\n');
				system("clear");
				printf("A Kerdessor letrehozasa sikeresen befelyezodott\n");
			}
			else
			{
				//char temp[MAX_SIZE];
				printf("Nem definialt menupont, folytatashoz nyomjon ENTER-t\n>");
				while (getchar() != '\n');
				while (getchar() != '\n');
				//scanf("%s",temp);
				menu();
			}
	}
}

void kerdessorMegalkotasa()
{
	FILE *kerdesek;
	FILE *kerdessor;
	
	kerdesek = fopen("kerdesek.txt","r");
	kerdessor = fopen("Kerdessor.txt","w");
	
	char line[MAX_SIZE];
	char name[MAX_SIZE];		//kérdés azonositoja
	char question[MAX_SIZE];	//kérdés
	char numberOfAnswer[10];	//válaszok száma 2-4
	char answer[MAX_SIZE];		//válasz
	char buffer[26];
	char definitive[10];
	int i;
	
	system("clear");
	printf("=============================================================================\n");
	printf("A Magyar Labdajátékok Szervezete altal 2017-ben kiallitott konzultacios urlap\n");
	printf("=============================================================================\n\n");
	fprintf(kerdessor,"=============================================================================\n");
	fprintf(kerdessor,"A Magyar Labdajátékok Szervezete altal 2017-ben kiallitott konzultacios urlap\n");
	fprintf(kerdessor,"=============================================================================\n\n");
	while(fgets(line,sizeof(line),kerdesek) != NULL)
	{
		sscanf(line,"%s %s %s %s %s %s",name,question,numberOfAnswer,answer,definitive,buffer);
		if(atoi(&definitive[0]) == 1){
			for(i = 0; i<strlen(question); i++)
			{
				if(question[i] == '#') question[i] = ' ';
			}
			for(i = 0; i<strlen(answer); i++)
			{
				if(answer[i] == ';') answer[i] = '\n';
			}
		printf("%s\n%s\n\n",question,answer);
		printf("=============================================================================\n\n");
		fprintf(kerdessor,"%s\n%s\n\n",question,answer);
		fprintf(kerdessor,"=============================================================================\n");
		}
		
	};
	
	fclose(kerdesek);
	fclose(kerdessor);
}

void megadas()
{
	FILE *kerdesek;
	kerdesek = fopen("kerdesek.txt","a+");
	
	char name[MAX_SIZE];		//kérdés azonositoja
	char question[MAX_SIZE];	//kérdés
	char numberOfAnswer[10];	//válaszok száma 2-4
	char answer[4][MAX_SIZE];	//válasz
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[26];
	char definitive[10];

	
	
	printf("Ugy tegye fel a kerdest, hogy a valaszra egy szóval tudjon felelni!\n");
	srand ( time(NULL) );
	int id = rand() % 1000 + 1;
	sprintf(name,"%ld", id);
	printf("\nKerdese:\t");
	fgets (question, MAX_SIZE, stdin);
	memset(question, 0, sizeof question);;
	fgets (question, MAX_SIZE, stdin);
	int i;
	for(i = 0; i<strlen(question); i++)
	{
		if(question[i] == ' ') question[i] = '#';
	}
	int idxToDel = strlen(question)-1; 
	memmove(&question[idxToDel], &question[idxToDel + 1], strlen(question) - idxToDel);
	printf("\nLehetseges valaszok szama (2-4):\t");
	scanf("%s",numberOfAnswer);
	int equal = 0;
	if(atoi(&numberOfAnswer[0]) == 2) equal = 1;
	if(atoi(&numberOfAnswer[0]) == 3) equal = 1;
	if(atoi(&numberOfAnswer[0]) == 4) equal = 1;
	while(equal == 0){
		
		printf("\nLehetseges valaszok szama (2-4):\t");
		scanf(" %s",numberOfAnswer);
		if(atoi(&numberOfAnswer[0]) == 2) equal = 1;
		if(atoi(&numberOfAnswer[0]) == 3) equal = 1;
		if(atoi(&numberOfAnswer[0]) == 4) equal = 1;
	}
	
	for(i = 0; i<atoi(&numberOfAnswer[0]); i++)
	{
		printf("\n%i. valasz\t",i+1);
		scanf("%s",answer[i]);
	}
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime(buffer, 26, "%Y-%m-%d#%H:%M:%S", timeinfo);
	
	char temp[MAX_SIZE] = "";
	strcat(temp, &answer[0]);
	
	for(i = 1; i<atoi(&numberOfAnswer[0]); i++){
		strcat(temp, ";");
		strcat(temp, answer[i]);
	}
	
	memset(definitive, 0, sizeof definitive);
	definitive[0] = '0';
	
	fprintf(kerdesek,"%s %s %s %s %s %s\n",name,question,&numberOfAnswer[0],temp,&definitive[0],buffer);
	fclose(kerdesek);
	
	system("clear");
	
}

void listazas()
{
	FILE *kerdesek;
	
	kerdesek = fopen("kerdesek.txt","r");
	char line[MAX_SIZE];
	char name[MAX_SIZE];		//kérdés azonositoja
	char question[MAX_SIZE];	//kérdés
	char numberOfAnswer[10];	//válaszok száma 2-4
	char answer[MAX_SIZE];		//válasz
	char buffer[26];
	char definitive[10];
	printf("\nAz eddig megadott kerdesek & valaszok:\n");
	printf("===========================================================\n\n");
	int i;
	while(fgets(line,sizeof(line),kerdesek) != NULL)
	{
		sscanf(line,"%s %s %s %s %s %s",name,question,numberOfAnswer,answer,definitive,buffer);
		for(i = 0; i<strlen(question); i++)
		{
			if(question[i] == '#') question[i] = ' ';
		}
		for(i = 0; i<strlen(answer); i++)
		{
			if(answer[i] == ';') answer[i] = '\n';
		}
		printf("%s\n%s\n\n",question,answer);
	};
	
	fclose(kerdesek);
}

void torles()
{
	FILE *kerdesek;
	
	kerdesek = fopen("kerdesek.txt","r");
	char line[MAX_SIZE];
	char name[MAX_SIZE];		//kérdés azonositoja
	char question[MAX_SIZE];	//kérdés
	char numberOfAnswer[10];	//válaszok száma 2-4
	char answer[MAX_SIZE];		//válasz
	char buffer[26];
	char definitive[10];
	printf("\nAz eddig megadott kerdesek & valaszok:\n");
	printf("===========================================================\n\n");
	int i;
	while(fgets(line,sizeof(line),kerdesek) != NULL)
	{
		sscanf(line,"%s %s %s %s %s %s",name,question,numberOfAnswer,answer,definitive,buffer);
		for(i = 0; i<strlen(question); i++)
		{
			if(question[i] == '#') question[i] = ' ';
		}
		for(i = 0; i<strlen(answer); i++)
		{
			if(answer[i] == ';') answer[i] = '\n';
		}
		printf("Azon: %s\n%s\n%s\n\n",name,question,answer);
	};
	
	fclose(kerdesek);
	
	char search[MAX_SIZE];
	printf("Addja meg annak a kerdesnek azonositojat, amit torolni szeretne:\t");
	scanf("%s",search);
	
	FILE *temp;
	kerdesek = fopen("kerdesek.txt","r");
	temp = fopen("temp.txt","w");
	//char line[MAX_SIZE];
	
	while(fgets(line,sizeof(line),kerdesek) != NULL)
	{
		char name[MAX_SIZE];		//kérdés azonositoja
		char question[MAX_SIZE];	//kérdés
		char numberOfAnswer[10];	//válaszok száma 2-4
		char answer[MAX_SIZE];		//válasz
		char buffer[26];
		char definitive[10];
		sscanf(line,"%s %s %s %s %s %s",name,question,numberOfAnswer,answer,definitive,buffer);
		if( strcmp(name,search) != 0 )
		{
			fputs(line,temp);
		}
	}
	
	fclose(kerdesek);
	fclose(temp);
	
	copy_file("temp.txt","kerdesek.txt");
}

void modositas()
{
	system("clear");
	
	FILE *kerdesek;
	
	kerdesek = fopen("kerdesek.txt","r");
	char line[MAX_SIZE];
	char name[MAX_SIZE];		//kérdés azonositoja
	char question[MAX_SIZE];	//kérdés
	char numberOfAnswer[10];	//válaszok száma 2-4
	char answer[MAX_SIZE];		//válasz
	char buffer[26];
	char definitive[10];
	printf("\nAz eddig megadott kerdesek & valaszok:\n");
	printf("===========================================================\n\n");
	int i;
	while(fgets(line,sizeof(line),kerdesek) != NULL)
	{
		sscanf(line,"%s %s %s %s %s %s",name,question,numberOfAnswer,answer,definitive,buffer);
		for(i = 0; i<strlen(question); i++)
		{
			if(question[i] == '#') question[i] = ' ';
		}
		for(i = 0; i<strlen(answer); i++)
		{
			if(answer[i] == ';') answer[i] = '\n';
		}
		printf("Azon: %s\n%s\n%s\n\n",name,question,answer);
	};
	
	fclose(kerdesek);
	
	char search[MAX_SIZE];
	printf("Addja meg annak a kerdesnek azonositojat, amit modositani szeretne:\t");
	scanf("%s",search);
	
	FILE *temp;
	kerdesek = fopen("kerdesek.txt","r");
	temp = fopen("temp.txt","w");
	//char line[MAX_SIZE];
	
	while(fgets(line,sizeof(line),kerdesek) != NULL)
	{
		char name[MAX_SIZE];		//kérdés azonositoja
		char question[MAX_SIZE];	//kérdés
		char numberOfAnswer[10];	//válaszok száma 2-4
		char answer[MAX_SIZE];		//válasz
		char buffer[26];
		char definitive[10];
		sscanf(line,"%s %s %s %s %s %s",name,question,numberOfAnswer,answer,definitive,buffer);
		
		char n_question[MAX_SIZE];  //Új kérdés
		char n_numberOfAnswer[10];	//Új válaszok száma 2-4
		char n_answer[4][MAX_SIZE];	//válasz	
		
		if( strcmp(search,name) == 0 )
		{
			char menum[MAX_SIZE];
			
			printf("Mit szeretne modositani?\n");
			printf("---------------------------\n");
			printf("Kerdes: (1)\n");
			printf("Valasz: (2)\n");
			printf("Veglegesites: (3)\n");
			printf("---------------------------\n");
			printf("Vissza/Mégsem (0)\n");
			
			scanf("%s",menum);
			
			if( strcmp(menum,"1") == 0 )
			{
				printf("Addja meg az uj kerdest:\t");
				fgets (n_question, MAX_SIZE, stdin);
				memset(n_question, 0, sizeof n_question);;
				fgets (n_question, MAX_SIZE, stdin);
				int i;
				for(i = 0; i<strlen(n_question); i++)
				{
					if(n_question[i] == ' ') n_question[i] = '#';
				}
				int idxToDel = strlen(n_question)-1; 
				memmove(&n_question[idxToDel], &n_question[idxToDel + 1], strlen(n_question) - idxToDel);
				fprintf(temp,"%s %s %s %s %s %s\n",name,n_question,&numberOfAnswer[0],answer,definitive,buffer);
			}
			else if( strcmp(menum,"2") == 0 )
			{
				printf("Addja meg az uj valaszok szamat (2-4):\t");
				scanf("%s",n_numberOfAnswer);
				int equal = 0;
				if(atoi(&n_numberOfAnswer[0]) == 2) equal = 1;
				if(atoi(&n_numberOfAnswer[0]) == 3) equal = 1;
				if(atoi(&n_numberOfAnswer[0]) == 4) equal = 1;
				while(equal == 0){
					printf("\nLehetseges valaszok szama (2-4):\t");
					scanf(" %s",n_numberOfAnswer);
					if(atoi(&n_numberOfAnswer[0]) == 2) equal = 1;
					if(atoi(&n_numberOfAnswer[0]) == 3) equal = 1;
					if(atoi(&n_numberOfAnswer[0]) == 4) equal = 1;
				}
				int i;
				for(i = 0; i<atoi(&n_numberOfAnswer[0]); i++)
				{
					printf("\n%i. valasz\t",i+1);
					scanf("%s",n_answer[i]);
				}
				char n_temp[MAX_SIZE] = "";
				strcat(n_temp, &n_answer[0]);
	
				for(i = 1; i<atoi(&n_numberOfAnswer[0]); i++){
					strcat(n_temp, ";");
					strcat(n_temp, n_answer[i]);
				}
				fprintf(temp,"%s %s %s %s %s %s\n",name,question,&n_numberOfAnswer[0],n_temp,&definitive[0],buffer);
			}
			else if( strcmp(menum,"3") == 0 )
			{
				sscanf(line,"%s %s %s %s %s %s",name,question,numberOfAnswer,answer,definitive,buffer);
				char questionOriginal[MAX_SIZE];
				memset(questionOriginal, 0, sizeof questionOriginal);
				strcat(questionOriginal, question);
				char answerOriginal[MAX_SIZE];
				memset(answerOriginal, 0, sizeof answerOriginal);
				strcat(answerOriginal, answer);
				int i;
				for(i = 0; i<strlen(question); i++)
				{
					if(question[i] == '#') question[i] = ' ';
				}
				for(i = 0; i<strlen(answer); i++)
				{
					if(answer[i] == ';') answer[i] = '\n';
				}
				printf("\n%s\n%s\n\n",question,answer);
				printf("\nBiztos veglegesiti ezt a Kerdes-Valasz parost? (y/n)");
				char yesNo[MAX_SIZE];
				int equal = 0;
				while(equal == 0){
					scanf("%s",yesNo);
					if( strcmp(&yesNo[0],"y") == 0 )
					{
						memset(definitive, 0, sizeof definitive);
						definitive[0] = '1';
						equal = 1;
						fprintf(temp,"%s %s %s %s %s %s\n",name,questionOriginal,&numberOfAnswer[0],answerOriginal,&definitive[0],buffer);
					}else if( strcmp(&yesNo[0],"n") == 0 ){
						fclose(kerdesek);
						fclose(temp);
						remove("temp.txt");
						menu();
					}else{
						//printf("%s\n",yesNo);
					}
				}
			}
			else if( strcmp(menum,"0") == 0 )
			{
				fclose(kerdesek);
				fclose(temp);
				remove("temp.txt");
				menu();
			}
			else
			{
				printf("Nem definialt menupont, folytatashoz nyomjon ENTER-t\n>");
				while (getchar() != '\n');
				while (getchar() != '\n');
				modositas();
			}
		}
		else
		{
			fputs(line,temp);
		}
	}
	
	fclose(kerdesek);
	fclose(temp);
	
	copy_file("temp.txt","kerdesek.txt");	
}

void copy_file(const char *from,const char *to)
{
	FILE *fr;
	FILE *t;
	fr = fopen(from,"r");
	t = fopen(to,"w");
	
	char line[MAX_SIZE];
	char name[MAX_SIZE];		//kérdés azonositoja
	char question[MAX_SIZE];	//kérdés
	char numberOfAnswer[10];	//válaszok száma 2-4
	char answer[MAX_SIZE];		//válasz
	char buffer[26];
	char definitive[10];
	while(fgets(line,sizeof(line),fr) != NULL)
	{
		sscanf(line,"%s %s %s %s %s %s",name,question,numberOfAnswer,answer,definitive,buffer);
		fprintf(t,"%s %s %s %s %s %s\n",name,question,numberOfAnswer,answer,definitive,buffer);
	}
	
	fclose(fr);
	fclose(t);
	
	remove("temp.txt");
}