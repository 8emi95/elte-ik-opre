#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 512

void menu();
void listazas();
void beszuras();
void modositas();
void torles();
void copyfile();
void copy_file(const char *from,const char *to);

int main(int argc, char *argv[]){
	menu();
	return 0;
}
/*void timestamp()
{
    time_t ltime; 
    ltime=time(NULL); 
    printf("%s",asctime( localtime(&ltime) ) );
}*/

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
	}	
}

void listazas()		//LISTÁZÁS
{
	FILE *kerdesek;
	//FILE *valaszok;
	
	
	kerdesek = fopen("kerdesekvalaszok.txt","r");
	//valaszok = fopen("valaszok.txt","r");
	
	char kiir[MAX_SIZE];
		
	printf("\nKERDESEKVALASZOK:\n");
	while(fgets(kiir,100,kerdesek) != NULL)
	{
		printf("%s",kiir);
	};
	
	
	fclose(kerdesek);
}

void beszuras()		//BESZÚRÁS
{
	FILE *kerdesekvalaszok;
	kerdesekvalaszok = fopen("kerdesekvalaszok.txt","a+");
	
	int kerdesID;
	char kerdes[MAX_SIZE];	
	int darabValasz;
	char valasz1[MAX_SIZE];	
	char valasz2[MAX_SIZE];	
	char valasz3[MAX_SIZE];	
	char valasz4[MAX_SIZE];	
	time_t ltime;
	ltime=time(NULL);
	
	
	printf("\nKerdes ID: \t");
	scanf("%d", &kerdesID);
	
	printf("\nHany kérdést adsz meg (min: 2, max. 4):\t");
	scanf("%d",&darabValasz);
	
	printf("\nKérdés:\t");
	scanf("%s",&kerdes);
		
		
		if(darabValasz == 2){
			printf("Elso kerdes: \n");
			scanf("%s",&valasz1);
			printf("Masodik kerdes: \n");
			scanf("%s",&valasz2);
		}else if(darabValasz == 3){
			printf("Elso kerdes: \n");
			scanf("%s",&valasz1);
			printf("Masodik kerdes: \n");
			scanf("%s",&valasz2);
			printf("Harmadik kerdes: \n");
			scanf("%s",&valasz3);
		}else if(darabValasz == 4){
			printf("Elso kerdes: \n");
			scanf("%s",&valasz1);
			printf("Masodik kerdes: \n");
			scanf("%s",&valasz2);
			printf("Harmadik kerdes: \n");
			scanf("%s",&valasz3);
			printf("Negyedik kerdes: \n");
			scanf("%s",&valasz4);
		}
	
	if(darabValasz == 2){
			
		fprintf(kerdesekvalaszok,"\n%d %d %s %s %s %s",kerdesID, darabValasz ,kerdes,valasz1,valasz2, asctime( localtime(&ltime) ));
	}else if(darabValasz == 3){
		fprintf(kerdesekvalaszok,"\n%d %d %s %s %s %s %s",kerdesID,darabValasz, kerdes,valasz1,valasz2, valasz3, asctime( localtime(&ltime) ));
	}else if(darabValasz == 4){
		fprintf(kerdesekvalaszok,"\n%d %d %s %s %s %s %s %s",kerdesID, darabValasz, kerdes,valasz1,valasz2, valasz3, valasz4, asctime( localtime(&ltime) ));
	}
	
	
	fclose(kerdesekvalaszok);
}

void torles()		//TÖRLÉS
{
	char kerdesTorol[MAX_SIZE];
	printf("Add meg annak kerdest, amelyiket torolni szeretned:\t");
	scanf("%s",kerdesTorol);
	
	FILE *kerdesvalasz;
	FILE *helpfile;
	kerdesvalasz = fopen("kerdesekvalaszok.txt","r");
	helpfile = fopen("helpfile.txt","w");
	char line[MAX_SIZE];
	
	while(fgets(line,sizeof(line),kerdesvalasz) != NULL)
	{
		int id;
		char kerdes[MAX_SIZE];
		sscanf(line,"%d %s",&id,kerdes);
		if( strcmp(kerdes, kerdesTorol) != 0 )
		{
			fputs(line,helpfile);
		}
	}
	
	fclose(kerdesvalasz);
	fclose(helpfile);
	
	copy_file("helpfile.txt","kerdesekvalaszok.txt");
}

void copy_file(const char *from,const char *to)
{
	FILE *fr;
	FILE *t;
	fr = fopen(from,"r");
	t = fopen(to,"w");
	
	char line[MAX_SIZE];
	int kerdesID;
	int darabKerdes;
	char kerdes[MAX_SIZE];
	char valasz1[MAX_SIZE];	//szám címe
	char valasz2[MAX_SIZE];	//szám címe
	char valasz3[MAX_SIZE];	//szám címe
	char valasz4[MAX_SIZE];	//szám címe
	time_t ltime;
	
	while(fgets(line,sizeof(line),fr) != NULL)
	{
		sscanf(line,"%d %d",&kerdesID,&darabKerdes, kerdes);
		if(darabKerdes == 2){
			sscanf(line, "%s %s %s", valasz1, valasz2, ltime);
			fprintf(t,"%d %d %s %s %s %s\n",kerdesID, darabKerdes, kerdes, valasz1, valasz2, ltime);
		}else if(darabKerdes == 3){
			sscanf(line, "%s %s %s %s", valasz1, valasz2, valasz3, ltime);
			fprintf(t,"%d %d %s %s %s %s %s\n",kerdesID, darabKerdes, kerdes, valasz1, valasz2, valasz3, ltime);
		}else if(darabKerdes == 4){
			sscanf(line, "%s %s %s %s %s", valasz1, valasz2, valasz3, valasz4, ltime);
			fprintf(t,"%d %d %s %s %s %s %s %s\n",kerdesID, darabKerdes, kerdes, valasz1, valasz2, valasz3, valasz4, ltime);
		}
		
	}
	
	fclose(fr);
	fclose(t);
	
	remove("helpfile.txt");
}

void modositas()		//MÓDOSÍTÁS
{
	char eredetiKerdes[MAX_SIZE];
	printf("Add meg azt a kérdést, amelyiket modositani szeretned:\t");
	scanf(eredetiKerdes);
	
	FILE *kerdesvalasz;
	FILE *helpfile;
	kerdesvalasz = fopen("kerdesekvalaszok.txt","r");
	helpfile = fopen("helpfile.txt","w");
	char line[MAX_SIZE];
	
	while(fgets(line,sizeof(line), kerdesvalasz) != NULL)
	{
		int kerdesID;
		int darabValasz;
		char kerdes[MAX_SIZE];
		char valasz1[MAX_SIZE];
		char valasz2[MAX_SIZE];
		char valasz3[MAX_SIZE];
		char valasz4[MAX_SIZE];
		time_t ltime;
		sscanf(line,"%d %d %s ",&kerdesID, &darabValasz,kerdes);
		
		if(darabValasz == 2){
			sscanf(line, "%s %s %s", valasz1, valasz2, ltime);
		}
		else if(darabValasz == 3){
			sscanf(line, "%s %s %s %s", valasz1, valasz2, valasz3, ltime);
		}
		else if(darabValasz == 4){
			sscanf(line, "%s %s %s %s %s", valasz1, valasz2, valasz3, valasz4, ltime);
		}
		
		int newKerdesID;
		int newDarabValasz;
		char newKerdes[MAX_SIZE];
		char newValasz1[MAX_SIZE];
		char newValasz2[MAX_SIZE];
		char newValasz3[MAX_SIZE];
		char newValasz4[MAX_SIZE];
		
		
		if( strcmp(kerdes,eredetiKerdes) == 0 )
		{
			char menu[MAX_SIZE];
			
			printf("Mit szeretnel modositani?\n");
			printf("Kérdést: (k)\n");

			if(darabValasz == 2){
				printf("Első választ: (1)\n");
				printf("Második választ: (2)\n");
			}
			else if(darabValasz == 3){
				printf("Első választ: (1)\n");
				printf("Második választ: (2)\n");
				printf("Harmadik választ: (3)\n");

			}
			else if(darabValasz){
				
				printf("Első választ: (1)\n");
				printf("Második választ: (2)\n");
				printf("Harmadik választ: (3)\n");
				printf("Negyedik választ: (4)\n");
			}
			
			
			scanf("%s",menu);
			
			if( strcmp(menu,"k") == 0 )
			{
				printf("Add meg az uj kerdes ID-t:\t");
				scanf("%d",&newKerdesID);
				printf("Add meg az uj kerdest:\t");
				scanf("%s",&newKerdes);
				fprintf(helpfile,"%d %d %s %s %s %s %s %s\n",newKerdesID,darabValasz, newKerdes,valasz1, valasz2, valasz3, valasz4, asctime( localtime(&ltime) ));
			}
			else if( strcmp(menu,"1") == 0 )
			{
				printf("Add meg az uj választ:\t");
				scanf("%s", newValasz1);
				fprintf(helpfile,"%d %d %s %s %s %s %s %s\n",kerdesID,darabValasz, kerdes, newValasz1, valasz2, valasz3, valasz4, asctime( localtime(&ltime) ));
			}
			else if( strcmp(menu,"2") == 0 )
			{
				printf("Add meg az új választ:\t");
				scanf("%s",&newValasz2);
				fprintf(helpfile,"%d %d %s %s %s %s %s %s\n",kerdesID,darabValasz, kerdes, valasz1, newValasz2, valasz3, valasz4, asctime( localtime(&ltime) ));
			}
			else if( strcmp(menu,"3") == 0 )
			{
				printf("Add meg az új választ:\t");
				scanf("%s",&newValasz3);
				fprintf(helpfile,"%d %d %s %s %s %s %s %s\n",kerdesID,darabValasz, kerdes, valasz1, valasz2, newValasz3, valasz4, asctime( localtime(&ltime) ));
			}
			else if( strcmp(menu,"4") == 0 )
			{
				printf("Add meg az új választ:\t");
				scanf("%s",&newValasz4);
				fprintf(helpfile,"%d %d %s %s %s %s %s %s\n",kerdesID,darabValasz, kerdes, valasz1, valasz2, valasz3, newValasz4, asctime( localtime(&ltime) ));
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
	
	fclose(kerdesvalasz);
	fclose(helpfile);
	
	copy_file("helpfile.txt","kerdesekvalaszok.txt");	
}