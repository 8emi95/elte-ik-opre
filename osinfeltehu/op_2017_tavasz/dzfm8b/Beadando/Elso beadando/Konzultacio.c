//DZFM8B

/*
FELADAT

A Magyar Labdajátékok Szervezete (MLSZ) nemzeti konzultációt hirdet a labdajátékok felvirágoztatására! Első lépésként meg kell alkotni azt a kérdéssort és hozzá kérdésenként azt a minimum kettő, maximum 4 választási lehetőséget amelyet majd a labdajátékok jobbítása iránt felelősséget érzők kitölthetnek. A készítendő program adjon lehetőséget a kérdéssor megalkotására. Tudjunk új kérdést felvenni, kérdést módosítani és kérdést törölni. Módosításhoz, törléshez kínálja fel választásra a kérdést és válaszait a program! Tudjunk emellett természetesen kérdéseket és válaszaikat listázni! A kérdésekhez tároljuk automatikusan annak felvételi dátumát (év,hó,nap,óra,perc,másodperc). 
Végezetül a program adjon lehetőséget a kérdések véglegesítésére, azaz annak meghatározására, hogy mely kérdések szerepeljenek a végső konzultációs kérdőíven!
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>// Fajl letezese miatt

#define FileName "Kerdesek.txt"
#define VeglegesFile "Vegleges.txt"

void kerdesFelvetele();
void listazas();
void modositas();
void torles();
void veglegesit();
void veglegesitettekListazasa();
void menu(); 


int main(int argc,char* argv[]){
	
		menu();
		
	return 0;
	
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
		printf("\t0 - Kilepes\n");
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
	
	printf("Koszonom, hogy  hasznalta viszontlatasara :) \n\n");
	
}
void kerdesFelvetele(){
	
	printf("Uj kerdes felvetele\n\n");
	char kerdes[512];
	int valasz_db;
	char valasz[512];
	
	FILE * fp=fopen(FileName,"a+");
	
	
	printf("Adja meg a kerdest:\n ->");
	scanf(" %[^\t\n]s",&kerdes);
	
	fprintf(fp,"%s","KERDES:");
	fprintf(fp,"%s\t",kerdes);
	
	do{
	printf("Adja meg a valasz lehetosegeket(2-4)\n -> ");
	scanf("%d",&valasz_db);
	}while(valasz_db <2  ||valasz_db >4);
	
	fprintf(fp,"%s","VALASZOK:");
	
	int i;
	for(i=0;i<valasz_db;i=i+1){
		printf("Adja meg a.(z) ");
		printf("%d",i+1);
		printf(" valaszt\n ->");
		
		scanf(" %[^\t\n]s",&valasz);
		
		fprintf(fp,"%s\t",valasz);
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
