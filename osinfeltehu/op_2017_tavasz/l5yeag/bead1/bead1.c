#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 512

void menu();
void ujKerdes();
void kerdesModositas();
void kerdesTorlese();
void kerdesekListazasa();
void veglegesites();
int getValaszokSzama();

int main(int argc, char* argv[])
{
	FILE *file = fopen("temp.txt", "ab+"); //create file if not exists
	fclose(file);
	menu();
    return 0;
}

void menu(){
	char m[BUFFERSIZE];
	int valasztas;
	do{
		system("clear");
		printf("\t1 - Uj kerdes felvetele\n");
		printf("\t2 - Meglevo kerdes modositasa\n");
		printf("\t3 - Kerdes Torlese\n");
		printf("\t4 - Kerdesek listazasa\n");
		printf("\t5 - Kerdoiv veglegesitese\n");
		printf("\t0 - Kilepes (vagy nyomja meg a CTRL+C - t)\n");
		printf("\n\nValasszon a menupontok kozul -> ");
		scanf("%s",m);
		valasztas = atoi(m);
		system("clear");
		if(valasztas == 1){
			ujKerdes();
		}
		else if(valasztas == 2){
			kerdesModositas();
		}
		else if(valasztas== 3){
			kerdesTorlese();
		}
		else if(valasztas == 4){
			kerdesekListazasa();
		}
		else if(valasztas == 5){
			veglegesites();
		}
		printf("\nNyomjon meg egy gombot!\n");
		system("read");
	}while(valasztas!=0);
}

void ujKerdes(){
	printf("Uj kerdes hozzaadsa\n");
	char kerdes[BUFFERSIZE];
	int valaszokSzama;
	printf("Adja meg a kerdest:\n -> ");
   	scanf(" %[^\t\n]s",&kerdes);
	printf("A megadott kerdes: %s\n",kerdes);
	valaszokSzama=getValaszokSzama();
	char valaszok[valaszokSzama][BUFFERSIZE];
	int i;
	for (i=0; i<valaszokSzama; i = i+ 1){
		printf("Adja meg a(z) %d. valaszt:\n ->",i+1);
   		scanf(" %[^\t\n]s",&valaszok[i]);
		printf("A beirt valasz: %s \n", valaszok[i]);
	}
	FILE* data = fopen("temp.txt","a+");
	switch(valaszokSzama){
		case 2:
			fprintf(data,"%s\t%s\t%s\t%s\t%s\n",kerdes,valaszok[0],valaszok[1],__DATE__,__TIME__);
		break;
		case 3:
			fprintf(data,"%s\t%s\t%s\t%s\t%s\t%s\n",kerdes,valaszok[0],valaszok[1],valaszok[2],__DATE__,__TIME__);
		break;
		case 4:
			fprintf(data,"%s\t%s\t%s\t%s\t%s\t%s\t%s\n",kerdes,valaszok[0],valaszok[1],valaszok[2],valaszok[3],__DATE__,__TIME__);
		break;
	}
	fclose(data);
}

void kerdesModositas(){
	printf("Kerdes modositasa\n");
	kerdesekListazasa();
	printf("Adja meg a modositani kivant kerdes sorszamat\n");
	char kerdes[BUFFERSIZE];
	int kerdesSzam;
	scanf("%s",kerdes);
	kerdesSzam = atoi(kerdes);
	FILE *newfile=fopen("out.txt","w");
	FILE* data = fopen("temp.txt","r");
	char buf[BUFFERSIZE];
	int i=1;
	while(fgets(buf,sizeof(buf),data)){
		if(i==kerdesSzam){
			char kerdes[BUFFERSIZE];
			int valaszokSzama;
			printf("Adja meg az uj kerdest:\n -> ");
		   	scanf(" %[^\t\n]s",&kerdes);
			printf("A megadott kerdes: %s\n",kerdes);
			valaszokSzama=getValaszokSzama();
			char valaszok[valaszokSzama][BUFFERSIZE];
			int i;
			for (i=0; i<valaszokSzama; i = i+ 1){
				printf("Adja meg a(z) %d. valaszt:\n ->",i+1);
		   		scanf(" %[^\t\n]s",&valaszok[i]);
				printf("A beirt valasz: %s \n", valaszok[i]);
			}
		switch(valaszokSzama){
			case 2:
				fprintf(newfile,"%s\t%s\t%s\t%s\t%s\n",kerdes,valaszok[0],valaszok[1],__DATE__,__TIME__);
			break;
			case 3:
				fprintf(newfile,"%s\t%s\t%s\t%s\t%s\t%s\n",kerdes,valaszok[0],valaszok[1],valaszok[2],__DATE__,__TIME__);
			break;
			case 4:
				fprintf(newfile,"%s\t%s\t%s\t%s\t%s\t%s\t%s\n",kerdes,valaszok[0],valaszok[1],valaszok[2],valaszok[3],__DATE__,__TIME__);
			break;
		}
	}
	else{
			fprintf(newfile,"%s",buf);
		}
		i=i+1;
	}
	fclose(newfile);
	fclose(data);
	remove("temp.txt");
	rename("out.txt","temp.txt");
}

void kerdesTorlese(){
	printf("Kerdes torlese\n\n");
	char kerdes[BUFFERSIZE];
	int kerdesSzam;
	kerdesekListazasa();
	printf("Adja meg a torolni kivant kerdes sorszamat\n");
	scanf("%s",kerdes);
	kerdesSzam = atoi(kerdes);
	FILE *newfile=fopen("out.txt","w");
	FILE* data = fopen("temp.txt","r");
	char buf[BUFFERSIZE];
	int i=1;
	while(fgets(buf,sizeof(buf),data)){
		if(i==kerdesSzam){
			printf("%d. kerdes torolve\n",i);
		}else{
			fprintf(newfile,"%s",buf);
		}
		i=i+1;
	}
	fclose(newfile);
	fclose(data);
	remove("temp.txt");
	rename("out.txt","temp.txt");
}

void kerdesekListazasa(){
	printf("Kerdesek listazasa:\n");

	FILE* data = fopen("temp.txt","r");
	char buf[BUFFERSIZE];
	int i=1;
	while(fgets(buf,sizeof(buf),data)){
		printf("%d. kerdes: ",i);
		printf("%s \n",buf);
		i=i+1;
	}
	fclose(data);
}

void veglegesites(){
	printf("Kerdesek veglegesitese.\n");
	remove("kerdoiv.txt");
	FILE* data = fopen("temp.txt","r");
	FILE *newfile=fopen("kerdoiv.txt","w");
	char buf[BUFFERSIZE];
	char igenNem[BUFFERSIZE];
	int i=1;
	int j=0;
	while(fgets(buf,sizeof(buf),data)){
		printf("%d. kerdes: ",i);
		printf("%s \n",buf);
		printf("Szerepeljen a vegleges listaban? (i/n)""\n");
		scanf("%s",&igenNem);
		if(strcmp(igenNem, "i") == 0){
			j=j+1;
			fprintf(newfile,"%d. kerdes %s",j,buf);
		}
		i=i+1;
	}
	fclose(newfile);
	fclose(data);
}

int getValaszokSzama(){
	char valaszok[BUFFERSIZE];
	int valaszokSzama;
	int error=1;
	do{
		printf("Adja meg a valaszok szamat:\n -> ");
		scanf("%s",valaszok);
		valaszokSzama=atoi(valaszok);
		printf("A valaszok szama %d\n",valaszokSzama);
		if(valaszokSzama<2 || valaszokSzama>4){
			error=0;
			printf("Hiba a valaszoknak 2 es 4 kozott kell lenniuk \n");
		}else{
			error=1;
		}
	}while(error==0);
	return valaszokSzama;
}
