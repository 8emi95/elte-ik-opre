#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 512
#define FileName "in.txt"

void menu();
void beszuras();
void modositas();
void torles();
void listazas();
void listazas_telepules();

int main(int argc, char* argv[])
{
	menu();
    return 0;
}

void menu(){
	char m[BUFFERSIZE]; sscanf("-1","%s",m);
	while(strcmp(m,"0")!=0){
		system("clear");
		printf("\t1 - Beszuras\n");
		printf("\t2 - Modositas\n");
		printf("\t3 - Torles\n");
		printf("\t4 - Listazas\n");
		printf("\t5 - Listazas telepulesek\n\n");
		printf("\t0 - Kilepes (vagy nyomja meg a CTRL+C - t)\n");
		printf("\n\nValasszon a menupontok kozul -> ");
		scanf("%s",m);

		system("clear");
		if(strcmp(m,"1") == 0){
			beszuras();
		}
		else if(strcmp(m,"2") == 0){
			modositas();
		}
		else if(strcmp(m,"3") == 0){
			torles();
		}
		else if(strcmp(m,"4") == 0){
			listazas();
		}
		else if(strcmp(m,"5") == 0){
			listazas_telepules();
		}
		printf("\nNyomjon meg egy gombot!\n");
		system("read");
	}
}

void beszuras(){
	printf("Uj megfigyeles beszurasa\n\n");

	char megf[BUFFERSIZE],tel[BUFFERSIZE];
	char ny_db[BUFFERSIZE],ny_kor[BUFFERSIZE];

	printf("Adja meg a megfigyelo nevet:\n -> ");
	scanf("%s",megf);
	printf("Adja meg a megfigyeles helyet:\n -> ");
	scanf("%s",tel);
	printf("Adja meg a nyuszik darabszamat:\n -> ");
	scanf("%s",ny_db);
	printf("Adja meg a nyuszik korat (Fiatal / Felnott / Oreg):\n -> ");
	scanf("%s",ny_kor);

	FILE* data = fopen(FileName,"a+");
	fprintf(data,"%s\t%s\t%s\t%s\t%s %s\n",megf,tel,ny_db,ny_kor,__DATE__,__TIME__);
	fclose(data);
}

void modositas(){
	printf("Megfigyeles modositasa\n\n");

	char megf[64],tel[64],ny_kor[16];

	printf("Modositani kivant sor adatai:\n");
	printf("Adja meg a megfigyelo nevet:\n -> ");
	scanf("%s",megf);
	printf("Adja meg a megfigyeles helyet:\n -> ");
	scanf("%s",tel);
	printf("Adja meg a nyuszik korat (Fiatal / Felnott / Oreg):\n -> ");
	scanf("%s",ny_kor);

	char modosit[64]; int mod=0;
	printf("\nMit szeretne modositani?\n");
	printf("  1 - megfigyelo neve\n");
	printf("  2 - megfigyeles helyszine\n");
	printf("  3 - nyuszik darabszama\n");
	printf("  4 - nyuszik kora\n");
	scanf("%s",modosit);

	if(strcmp(modosit,"1")==0){
		printf("\n\nAdja meg az uj megfigyelo nevet: ");
		scanf("%s",modosit);
		mod=1;
	}
	else if(strcmp(modosit,"2")==0){
		printf("\n\nAdja meg a megfigyeles uj helyet: ");
		scanf("%s",modosit);
		mod=2;
	}
	else if(strcmp(modosit,"3")==0){
		printf("\n\nAdja meg a nyuszik uj darabszamat: ");
		scanf("%s",modosit);
		mod=3;
	}
	else if(strcmp(modosit,"4")==0){
		printf("\n\nAdja meg a nyuszik uj korat: ");
		scanf("%s",modosit);
		mod=4;
	}


	FILE* data, *newfile;
	data = fopen(FileName,"r");
	newfile = fopen("new.txt","w");
	char buf[BUFFERSIZE];

	while(fgets(buf,sizeof(buf),data)){
		char _megf[64],_tel[64],_ny_kor[16],_db[6];
		sscanf(buf,"%s %s %s %s",&_megf,&_tel,&_db,_ny_kor);
		if(strcmp(megf,_megf)==0 && strcmp(tel,_tel)==0 && strcmp(ny_kor,_ny_kor)==0){
			if(mod==1) 		fprintf(newfile,"%s\t%s\t%s\t%s\t%s %s\n",modosit,_tel,_db,_ny_kor,__DATE__,__TIME__);
			else if(mod==2) fprintf(newfile,"%s\t%s\t%s\t%s\t%s %s\n",_megf,modosit,_db,_ny_kor,__DATE__,__TIME__);
			else if(mod==3) fprintf(newfile,"%s\t%s\t%s\t%s\t%s %s\n",_megf,_tel,modosit,_ny_kor,__DATE__,__TIME__);
			else if(mod==4) fprintf(newfile,"%s\t%s\t%s\t%s\t%s %s\n",_megf,_tel,_db,modosit,__DATE__,__TIME__);
			printf("~ Modositva\n");
		}
		else
			fprintf(newfile,"%s",buf);
	}

	fclose(newfile);
	fclose(data);


	data=fopen(FileName,"w");
	newfile=fopen("new.txt","r");
	while(fgets(buf,sizeof(buf),newfile))
		fprintf(data,"%s",buf);
	fclose(newfile);
	fclose(data);
	remove("new.txt");
}

void torles(){
	printf("Megfigyeles torlese\n\n");

	char megf[64],tel[64],ny_kor[16];

	printf("Adja meg a megfigyelo nevet:\n -> ");
	scanf("%s",megf);
	printf("Adja meg a megfigyeles helyet:\n -> ");
	scanf("%s",tel);
	printf("Adja meg a nyuszik korat (Fiatal / Felnott / Oreg):\n -> ");
	scanf("%s",ny_kor);

	FILE* data, *newfile;
	data = fopen(FileName,"r");
	newfile = fopen("new.txt","w");
	char buf[BUFFERSIZE];

	char _megf[64],_tel[64],_ny_kor[16],tmp[128];
	while(fgets(buf,sizeof(buf),data)){
		sscanf(buf,"%s %s %s %s",&_megf,&_tel,&tmp,_ny_kor);
		if(strcmp(megf,_megf)==0 && strcmp(tel,_tel)==0 && strcmp(ny_kor,_ny_kor)==0)
			printf("~ Torolve\n");
		else
			fprintf(newfile,"%s",buf);
	}
	fclose(newfile);
	fclose(data);


	data=fopen(FileName,"w");
	newfile=fopen("new.txt","r");
	while(fgets(buf,sizeof(buf),newfile))
		fprintf(data,"%s",buf);
	fclose(newfile);
	fclose(data);
	remove("new.txt");
}

void listazas(){
	char m[BUFFERSIZE];
	printf("Mit szeretnel kilistazni?\n");
	printf("  1 - minden\n");
	printf("  2 - adott megfigyelo szerint\n");
	printf("  3 - adott telepules szerint\n");
	printf(" -> ");
	scanf("%s",m);

	FILE* data = fopen(FileName,"r");
	char buf[BUFFERSIZE];

	if(strcmp(m,"1")==0) {
		while(fgets(buf,sizeof(buf),data))
			printf("%s",buf);
	}
	else if(strcmp(m,"2")==0) {
		printf("\nKi megfigyeleseit szeretned latni?\n");
		printf(" -> ");
		scanf("%s",m);
		printf("\n");

		char megf[BUFFERSIZE],tmp[BUFFERSIZE];
		while(fgets(buf,sizeof(buf),data)){
			sscanf(buf,"%s %s",&megf,&tmp);
			if(strcmp(megf,m)==0)
				printf("%s",buf);
		}
	}
	else if(strcmp(m,"3")==0) {
		printf("\nMelyik telepulesen tortent megfigyeleseket szeretned latni?\n");
		printf(" -> ");
		scanf("%s",m);
		printf("\n\n");

		char tel[BUFFERSIZE],tmp[32];
		while(fgets(buf,sizeof(buf),data)){
			sscanf(buf,"%s %s",&tmp,&tel);
			if(strcmp(tel,m)==0)
				printf("%s",buf);
		}
	}
	fclose(data);
}

void listazas_telepules(){
	printf("Nyuszik szama telepuleseken adott ev:honapra\n\n");
	
	char month[BUFFERSIZE];
	printf("Adja meg az evszamot:\n");
	printf(" -> ");
	scanf("%s",month);
	int year = atoi(month);
	
	printf("Adja meg a honapot (Febr, Mar stb.. alakban):\n");
	printf(" -> ");
	scanf("%s",month);	
		
	FILE* data = fopen(FileName,"r");
	char buf[BUFFERSIZE];
	
	printf("\n");
	while(fgets(buf,sizeof(buf),data)){
		char _megf[64],_tel[64],_ny_kor[16],_month[8];
		int _year,_db;
		sscanf(buf,"%s %s %d %s %s %s %d",&_megf,&_tel,&_db,&_ny_kor,&_month,&_year,&_year);
		
		if(strcmp(month,_month)==0 && _year==year)
			printf("%s %s %d %s\n",_megf,_tel,_db,_ny_kor);
	}
	fclose(data);
}
