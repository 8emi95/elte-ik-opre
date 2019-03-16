#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h> //ctime
#include <unistd.h>

int getLines(char fileName[]){
	FILE* myfile = fopen(fileName, "r");
	if (myfile == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	int ch, lines = 0;

	do 
	{
		ch = fgetc(myfile);
		if(ch == '\n')
			lines++;
	} while (ch != EOF);

	if(ch != '\n' && lines != 0) 
		lines++;

	fclose(myfile);

	return lines;
}

void UjKerdes(){
	
	char kerdes[120];
	int vszam=1000;
	time_t most = time(0);
	//if( access( "tmpKerdesek", F_OK ) != -1 ) {
	FILE *f = fopen("tmpKerdesek", "a");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		return;
	}
	
	printf("Kerem adja meg a kerdest!\n");
	fflush(stdin);
	fgets(kerdes,120,stdin);
	char tmpV[10];
	do{
		printf("Kerem adja a valaszok szamat!(2-4)\n");
		scanf("%s",tmpV);
		vszam=atoi(tmpV);
	}while (vszam < 2 || vszam > 4);
	
	char valaszok[4][120];
	switch(vszam){
		case 2:
		strcpy(valaszok[2],"nincs\n");
		strcpy(valaszok[3],"nincs\n");
		break;
		case 3:
		strcpy(valaszok[3],"nincs\n");
		break;
		default:
		break;
	}
	
	int tmp=0;
	char tmpValasz[120];
	while(vszam != 0){
		vszam--;
		printf("Kerem adja meg a %i. valaszt!\n",tmp+1);
		fflush(stdin);
		fgets(tmpValasz,120,stdin);
		strcpy(valaszok[tmp],tmpValasz);
		tmp++;
	}
	
	int sorszam=getLines("tmpKerdesek")/7+1;
	//printf("%i",sorszam);
	fprintf(f,"%i. %s1)%s2)%s3)%s4)%s%s\n",sorszam,kerdes,valaszok[0],valaszok[1],valaszok[2],valaszok[3],ctime(&most));
	printf("Sikeres muvelet!");
	fclose(f);
	//} else {
	//	printf("Nincs ilyen rendezveny!");
	//	return;
	//}

}

void Kilistazas(){
	//char azon[30];
	//printf("Kerem adja meg a rendezveny azonositojat!\n");
	//scanf("%s",azon);
	FILE *f = fopen("tmpKerdesek","r");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		return;
	}
	putchar('\n');
	char ch;
	do{
		ch=getc(f);
		putchar(ch);
	}while(ch != EOF);
	//putchar('\n');
	fclose(f);
}

void Modositas(){
	char mit;
	printf("Kerem adja meg hogy a kerdest vagy a valaszt szeretne modositani!(k/v)\n");
	fflush(stdin);
	mit= fgetc(stdin);
	if(mit=='k'){
		Kilistazas();
		printf("Kerem adja meg hanyas sorszamu kerdest szeretne modositani!(0-val egyiket se)\n");
		char tmpS[10];
		scanf("%s",tmpS);
		if(tmpS[0] != '0'){
			int hanyas=atoi(tmpS);
			int felhSorszam=(atoi(tmpS)-1)*7+1;
			int hanySoros=getLines("tmpKerdesek");
			if(felhSorszam < hanySoros){
				FILE *fileptr1, *fileptr2;
				char dump[120];
				int i=7;
				//char masolando[felhSorszam-1][120];
				fileptr1 = fopen("tmpKerdesek", "r");
				fileptr2 = fopen("masolat","w");
				if (fileptr1 == NULL)
				{
					printf("Error opening file!\n");
					return;
				}
				char ch;
				while(felhSorszam!=1){
					/*fgets(masolando[i],120,fileptr1);
					felhSorszam--;
					i++;*/
					while(i!=0){
						ch=getc(fileptr1);
						while(ch != '\n'){
							putc(ch,fileptr2);
							ch=getc(fileptr1);
						}
						putc('\n',fileptr2);
						//printf("%i\n",felhSorszam);
						//printf("%i\n",i);
						
						felhSorszam--;
						i--;
					}
					i=7;
				}
				/*while(i!=0){
					fgets(dump,120,fileptr1);
					i--;
				}*/
				char kerdes[120];
				//int j=6;
				printf("Kerem adja meg mi legyen a kerdes!\n");
				fflush(stdin);
				fgets(kerdes,120,stdin);
				fprintf(fileptr2,"%i.%s",hanyas,kerdes);
				
				ch=getc(fileptr1);
				while(ch!='\n' && ch!=EOF){
					ch=getc(fileptr1);
				}
				
				ch=getc(fileptr1);
				while(ch != EOF){
					putc(ch,fileptr2);
					ch=getc(fileptr1);
					
				}
				
				//fprintf(fileptr2,);
				
				fclose(fileptr1);
				fclose(fileptr2);
				remove("tmpKerdesek");
				rename("masolat","tmpKerdesek");
				remove("masolat");
			}else{
				printf("Tul nagy sorszam!");
			}
		}
		printf("Sikeres modositas!\n");
	}
	if(mit=='v'){
		Kilistazas();
		printf("Kerem adja meg hanyas sorszamu kerdesben akar valaszt modositani!(0-val egyiket se)\n");
		char tmpS[10];
		scanf("%s",tmpS);
		if(tmpS[0] != '0'){
			int hanyas=atoi(tmpS);
			int felhSorszam=(atoi(tmpS)-1)*7+1;
			int hanySoros=getLines("tmpKerdesek");
			if(felhSorszam < hanySoros){
				FILE *fileptr1, *fileptr2;
				int i=7;
				//char masolando[felhSorszam-1][120];
				fileptr1 = fopen("tmpKerdesek", "r");
				fileptr2 = fopen("masolat","w");
				if (fileptr1 == NULL)
				{
					printf("Error opening file!\n");
					return;
				}
				char ch;
				while(felhSorszam!=1){
					/*fgets(masolando[i],120,fileptr1);
					felhSorszam--;
					i++;*/
					while(i!=0){
						ch=getc(fileptr1);
						while(ch != '\n'){
							putc(ch,fileptr2);
							ch=getc(fileptr1);
						}
						putc('\n',fileptr2);
						//printf("%i\n",felhSorszam);
						//printf("%i\n",i);
						
						felhSorszam--;
						i--;
					}
					i=7;
				}
				printf("Kerem adja meg hanyadik valaszt akarja modositani!\n");
				char tmpValasz[10];
				scanf("%s",tmpValasz);
				int hanyadik=atoi(tmpValasz);
				int tmphanyadik=hanyadik;
				if(hanyadik>4){
					printf("Tul nagy szamot adott! :S");
					return;
				}
				
				//ch=getc(fileptr1);
				while(tmphanyadik!=0){
					ch=getc(fileptr1);
					while(ch!='\n'){
						putc(ch,fileptr2);
						ch=getc(fileptr1);
					}
					putc('\n',fileptr2);
					tmphanyadik--;
				}
				printf("Kerem adja meg mire modositsuk!\n");
				char mire[120];
				fflush(stdin);
				fgets(mire,120,stdin);
				fprintf(fileptr2,"%i)%s",hanyadik,mire);
				
				ch=getc(fileptr1);
				while(ch!=EOF && ch!='\n'){
					ch=getc(fileptr1);
				}
				
				ch=getc(fileptr1);
				while(ch!=EOF){
					putc(ch,fileptr2);
					ch=getc(fileptr1);
				}
				
				fclose(fileptr1);
				fclose(fileptr2);
				remove("tmpKerdesek");
				rename("masolat","tmpKerdesek");
				remove("masolat");
			}else{
				printf("Tul nagy sorszam!");
			}
		}
		printf("Sikeres modositas!\n");
	}
}

void Torles(){
	/*char azon[30];
	char adat[30];
	printf("Kerem adja meg a rendezveny azonositojat!\n");
	scanf("%s",azon);
	printf("Kerem adja meg a torlendo vendeg nevet vagy email cimet!\n");
	scanf("%s",adat);*/
	char mit;
	printf("Kerem adja meg hogy a kerdest vagy a valaszt szeretne torolni!(k/v)\n");
	fflush(stdin);
	mit= fgetc(stdin);
	if(mit=='k'){
		Kilistazas();
		printf("Kerem adja meg hanyas sorszamu kerdest szeretne torolni!(0-val egyiket se)\n");
		char tmpS[10];
		scanf("%s",tmpS);
		if(tmpS[0] != '0'){
			int hanyas=atoi(tmpS);
			int felhSorszam=(atoi(tmpS)-1)*7+1;
			int hanySoros=getLines("tmpKerdesek");
			if(felhSorszam < hanySoros){
				FILE *fileptr1, *fileptr2;
				char dump[120];
				int i=7;
				//char masolando[felhSorszam-1][120];
				fileptr1 = fopen("tmpKerdesek", "r");
				fileptr2 = fopen("masolat","w");
				if (fileptr1 == NULL)
				{
					printf("Error opening file!\n");
					return;
				}
				char ch;
				while(felhSorszam!=1){
					/*fgets(masolando[i],120,fileptr1);
					felhSorszam--;
					i++;*/
					while(i!=0){
						ch=getc(fileptr1);
						while(ch != '\n'){
							putc(ch,fileptr2);
							ch=getc(fileptr1);
						}
						putc('\n',fileptr2);
						//printf("%i\n",felhSorszam);
						//printf("%i\n",i);
						
						felhSorszam--;
						i--;
					}
					i=7;
				}
				while(i!=0){
					fgets(dump,120,fileptr1);
					i--;
				}
				char kerdes[120];
				int j=6;
				ch=getc(fileptr1);
				while(ch != EOF){
					if(hanyas >= 10){
						kerdes[0]=hanyas/10+'0';
						kerdes[1]=hanyas%10+'0';
						hanyas++;
						ch=getc(fileptr1);
						int u=2;
						ch=getc(fileptr1);
						while(ch!='\n'){
							kerdes[u]=ch;
							ch=getc(fileptr1);
							u++;
						}
						kerdes[u]=ch;
						kerdes[u+1]='\0';
						fprintf(fileptr2,"%s",kerdes);
					}else{
						kerdes[0]=hanyas+'0';
						hanyas++;
						int u=1;
						ch=getc(fileptr1);
						while(ch!='\n'){
							kerdes[u]=ch;
							ch=getc(fileptr1);
							u++;
						}
						kerdes[u]=ch;
						kerdes[u+1]='\0';
						fprintf(fileptr2,"%s",kerdes);
					}
					while(j!=0){
						ch=getc(fileptr1);
						while(ch!='\n'){
							putc(ch,fileptr2);
							ch=getc(fileptr1);
						}
						putc('\n',fileptr2);
						//printf("%i\n",j);
						j--;
					}
					j=6;
					ch=getc(fileptr1);
				}
				
				//fprintf(fileptr2,);
				
				fclose(fileptr1);
				fclose(fileptr2);
				remove("tmpKerdesek");
				rename("masolat","tmpKerdesek");
				remove("masolat");
			}else{
				printf("Tul nagy sorszam!");
			}
		}
		printf("Sikeres torles!\n");
	}
	if(mit=='v'){
		Kilistazas();
		printf("Kerem adja meg hanyas sorszamu kerdesben akar valaszt torolni!(0-val egyiket se)\n");
		char tmpS[10];
		scanf("%s",tmpS);
		if(tmpS[0] != '0'){
			int hanyas=atoi(tmpS);
			int felhSorszam=(atoi(tmpS)-1)*7+1;
			int hanySoros=getLines("tmpKerdesek");
			if(felhSorszam < hanySoros){
				FILE *fileptr1, *fileptr2;
				int i=7;
				//char masolando[felhSorszam-1][120];
				fileptr1 = fopen("tmpKerdesek", "r");
				fileptr2 = fopen("masolat","w");
				if (fileptr1 == NULL)
				{
					printf("Error opening file!\n");
					return;
				}
				char ch;
				while(felhSorszam!=1){
					/*fgets(masolando[i],120,fileptr1);
					felhSorszam--;
					i++;*/
					while(i!=0){
						ch=getc(fileptr1);
						while(ch != '\n'){
							putc(ch,fileptr2);
							ch=getc(fileptr1);
						}
						putc('\n',fileptr2);
						//printf("%i\n",felhSorszam);
						//printf("%i\n",i);
						
						felhSorszam--;
						i--;
					}
					i=7;
				}
				printf("Kerem adja meg hanyadik valaszt akarja torolni!\n");
				char tmpValasz[10];
				scanf("%s",tmpValasz);
				int hanyadik=atoi(tmpValasz);
				int tmphanyadik=hanyadik;
				if(hanyadik>4){
					printf("Tul nagy szamot adott! :S");
					return;
				}
				
				//ch=getc(fileptr1);
				while(tmphanyadik!=0){
					ch=getc(fileptr1);
					while(ch!='\n'){
						putc(ch,fileptr2);
						ch=getc(fileptr1);
					}
					putc('\n',fileptr2);
					tmphanyadik--;
				}
				fprintf(fileptr2,"%i)nincs\n",hanyadik);
				
				ch=getc(fileptr1);
				while(ch!=EOF && ch!='\n'){
					ch=getc(fileptr1);
				}
				
				ch=getc(fileptr1);
				while(ch!=EOF){
					putc(ch,fileptr2);
					ch=getc(fileptr1);
				}
				
				fclose(fileptr1);
				fclose(fileptr2);
				remove("tmpKerdesek");
				rename("masolat","tmpKerdesek");
				remove("masolat");
			}else{
				printf("Tul nagy sorszam!");
			}
		}
		printf("Sikeres torles!\n");
	}
}

void Veglegesites(){
	//char azon[30];
	//printf("Kerem adja meg az uj rendezveny azonositojat!\n");
	//scanf("%s",azon);
	time_t most = time(0);
	Kilistazas();
	int felhSorszam;
	char tmpS[10];
	printf("Hanyas sorszamu kerdest szeretne veglegesiteni?(0-val egyiket se)\n");
	scanf("%s",tmpS);
	if(tmpS[0] != '0'){
		felhSorszam=(atoi(tmpS)-1)*7+1;
		
		int hanySoros=getLines("tmpKerdesek");
		if(felhSorszam < hanySoros){
			char masolando[5][120];
			char dump[120];
			FILE *g = fopen("tmpKerdesek","r");
			if (g == NULL)
			{
				printf("Error opening file!\n");
				return;
			}
			/*char ch;
			do{
				ch=getc(g);
				putchar(ch);
			}while(ch != EOF);*/
			while(felhSorszam != 1){
				fgets(dump,120,g);
				felhSorszam--;
			}
			fgets(masolando[0],120,g);
			fgets(masolando[1],120,g);
			fgets(masolando[2],120,g);
			fgets(masolando[3],120,g);
			fgets(masolando[4],120,g);
			fclose(g);
			
			int i;
			for(i=1;i<5;i++){
				if(strstr(masolando[i],"nincs\n") != NULL){
					strcpy(masolando[i],"\n");
				}
			}
			
			int sorSz=getLines("vegKerdesek")/7+1;
			masolando[0][0]=sorSz/10+ '0';
			masolando[0][1]=sorSz%10+ '0';
			//masolando[0][2]='.';
			
			FILE *f = fopen("vegKerdesek","a");
			if (f == NULL)
			{
				printf("Error opening file!\n");
				exit(1);
			}
			char ch;
			do{
				ch=getc(g);
				//putchar(ch);
			}while(ch != EOF);
			fprintf(f,"%s%s%s%s%s%s\n",masolando[0],masolando[1],masolando[2],masolando[3],masolando[4],ctime(&most));
			
			fclose(f);
			printf("Kerdes veglegesitve!");
		}else{
			printf("Tul nagy szamot irt be!");
		}
	}
}

void VegLista(){
	FILE *f = fopen("vegKerdesek","r");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		return;
	}
	putchar('\n');
	char ch;
	do{
		ch=getc(f);
		putchar(ch);
	}while(ch != EOF);
	//putchar('\n');
	fclose(f);
}

void Menu(){
	int x;
	do{
		printf("\n-----------------------------\n");
		printf("Kerem valasszon a kovetkezo menupontok kozul:\n");
		printf("1.Uj kerdes\n");
		printf("2.Kerdesek listazasa\n");
		printf("3.Kerdes modositasa\n");
		printf("4.Kerdes torlese\n");
		printf("5.Kerdes veglegesitese\n");
		printf("6.Veglegesitett lista\n");
		printf("0.Kilepes\n");
		printf("-----------------------------\n");
		scanf("%d", &x);
		switch(x) {
			case 1:
			  UjKerdes();
			  break;
			case 2:
			  Kilistazas();
			  break;
			case 3:
			  Modositas();
			  break;
			case 4:
			  Torles();
			  break;
			case 5:
			  Veglegesites();
			  break;
			case 6:
			  VegLista();
			  break;
			case 0:
			  
			  break;
			default :
			printf("Rossz bemenet!!\n");
			  break;
		}
	}while(x!=0);
}

int main(int argc,char** argv){
	Menu();
}