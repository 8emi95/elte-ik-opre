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

void Jelentkezes(){
	
	char nev[30];
	char mail[30];
	char azon[30];
	time_t most = time(0);
	printf("Kerem adja meg a rendezveny azonositojat!\n");
	scanf("%s",azon);
	if( access( azon, F_OK ) != -1 ) {
		FILE *f = fopen(azon, "a");
		if (f == NULL)
		{
			printf("Error opening file!\n");
			return;
		}
		
		printf("Kerem adja meg a nevet!\n");
		scanf("%s",nev);
		printf("Kerem adja meg az email cimet!\n");
		scanf("%s",mail);
		
		int lines=getLines(azon);
		
		
		
		fprintf(f,"%s %s %s %s",nev,mail,azon,ctime(&most));
		printf("On a %i. jelentkezo!",lines);
		fclose(f);
	} else {
		printf("Nincs ilyen rendezveny!");
		return;
	}

}

void Kilistazas(){
	char azon[30];
	printf("Kerem adja meg a rendezveny azonositojat!\n");
	scanf("%s",azon);
	FILE *f = fopen(azon,"r");
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
	putchar('\n');
	fclose(f);
}

void Modositas(){
	char azon[30];
	char regiA[30];
	char ujA[30];
	char nev_e[1];
	printf("Kerem adja meg a rendezveny azonositojat!\n");
	scanf("%s",azon);

	
	FILE *fileptr1, *fileptr2;
    char ch;
    int delete_line, temp = 1,tmp=0,igaze=0;

    fileptr1 = fopen(azon, "r");
	if (fileptr1 == NULL)
	{
		printf("Error opening file!\n");
		return;
	}
	printf("Kerem adja meg, hogy nevet vagy emailt modosit-e?(nev eseten 1,email eseten 0)\n");
	scanf("%s",nev_e);
	printf("Kerem adja meg a modositando vendeg nevet vagy email cimet!\n");
	scanf("%s",regiA);
	printf("Adja meg mire szeretne modositani!\n");
	scanf("%s",ujA);
    ch = getc(fileptr1);
	while (ch != EOF)
		{
			//printf("%i    ",sizeof(adat)/sizeof(adat[0]));
			//printf("%i   ",tmp);
			//printf("%i",strlen(adat));
			if(strlen(regiA) == tmp){
				igaze=1;
				break;
			}
			if (ch == '\n'){
				temp++;
			}else if(ch == regiA[tmp]){
				tmp++;
			}else{
				tmp=0;
			}
			ch = getc(fileptr1);
		}
	if(igaze==1){
		delete_line=temp;
		temp=1;
		rewind(fileptr1);
		int megvolt=0;
		fileptr2 = fopen("masolat", "w");
		if(delete_line==1){
			int timp=1;
			ch = getc(fileptr1);
			while (ch != EOF)
			{
				if (ch == '\n')
				{
					temp++;
				}
				if (temp != delete_line)
				{
					if(timp==1){
						timp--;
					}else{
						putc(ch, fileptr2);
					}
				}else{
					if(megvolt==0){
						ch = getc(fileptr1);
						megvolt=1;
						if(nev_e[0]=='1'){
							int i=0;
							for(i=0;i<strlen(ujA);i++){
								putc(ujA[i],fileptr2);
							}
						}else{
							while(ch!='\n'){
								ch=getc(fileptr1);
							}
							int i=0;
							for(i=0;i<strlen(ujA);i++){
								putc(ujA[i],fileptr2);
							}
						}
					}
					putc(ch,fileptr2);
				}
				ch = getc(fileptr1);
			}
		}else{
			ch = getc(fileptr1);
			while (ch != EOF)
			{
				if (ch == '\n')
				{
					temp++;
				}
				if (temp != delete_line)
				{
					putc(ch, fileptr2);
				}else{
					if(megvolt==0){
						ch = getc(fileptr1);
						megvolt=1;
						if(nev_e[0]=='1'){
							int i=0;
							for(i=0;i<strlen(ujA);i++){
								putc(ujA[i],fileptr2);
							}
						}else{
							while(ch!='\n'){
								ch=getc(fileptr1);
							}
							int i=0;
							for(i=0;i<strlen(ujA);i++){
								putc(ujA[i],fileptr2);
							}
						}
					}
					putc(ch,fileptr2);
				}
				ch = getc(fileptr1);
			}
		}
		fclose(fileptr1);
		fclose(fileptr2);
		remove(azon);
		rename("masolat", azon);
	}else{
		printf("Nem volt ilyen adat!");
	}
    fclose(fileptr1);
}

void VTorles(){
	char azon[30];
	char adat[30];
	printf("Kerem adja meg a rendezveny azonositojat!\n");
	scanf("%s",azon);
	printf("Kerem adja meg a torlendo vendeg nevet vagy email cimet!\n");
	scanf("%s",adat);
	
	FILE *fileptr1, *fileptr2;
    char ch;
    int delete_line, temp = 1,tmp=0,igaze=0;

    fileptr1 = fopen(azon, "r");
	if (fileptr1 == NULL)
	{
		printf("Error opening file!\n");
		return;
	}
    ch = getc(fileptr1);
	while (ch != EOF)
		{
			//printf("%i    ",sizeof(adat)/sizeof(adat[0]));
			//printf("%i   ",tmp);
			//printf("%i",strlen(adat));
			if(strlen(adat) == tmp){
				igaze=1;
				break;
			}
			if (ch == '\n'){
				temp++;
			}else if(ch == adat[tmp]){
				tmp++;
			}else{
				tmp=0;
			}
			ch = getc(fileptr1);
		}
	if(igaze==1){
		delete_line=temp;
		temp=1;
		rewind(fileptr1);

		fileptr2 = fopen("masolat", "w");
		if(delete_line==1){
			int timp=1;
			ch = getc(fileptr1);
			while (ch != EOF)
			{
				if (ch == '\n')
				{
					temp++;
				}
				if (temp != delete_line)
				{
					if(timp==1){
						timp--;
					}else{
						putc(ch, fileptr2);
					}
				}
				ch = getc(fileptr1);
			}
		}else{
			ch = getc(fileptr1);
			while (ch != EOF)
			{
				if (ch == '\n')
				{
					temp++;
				}
				if (temp != delete_line)
				{
					putc(ch, fileptr2);
				}
				ch = getc(fileptr1);
			}
		}
		fclose(fileptr1);
		fclose(fileptr2);
		remove(azon);
		rename("masolat", azon);
	}else{
		printf("Nem volt ilyen adat!");
	}
    fclose(fileptr1);
}

void UjRend(){
	char azon[30];
	printf("Kerem adja meg az uj rendezveny azonositojat!\n");
	scanf("%s",azon);
	if( access( azon, F_OK ) != -1 ) {
		printf("Mar van ilyen rendezveny!");
		return;
		
	} else {
		FILE *f = fopen(azon,"w");
		if (f == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		}
		fclose(f);
		printf("A rendezveny sikeresen letrejott!");
	}
	
}

void RTorles(){
	char azon[30];
	printf("Kerem adja meg a torlendo rendezveny azonositojat!\n");
	scanf("%s",azon);
	int ret = remove(azon);

	if(ret == 0) 
	{
	  printf("Rendezveny sikeresen torolve!");
	}
	else 
	{
	  printf("Error: Nem sikerult torolni a rendezvenyt!(Nincs ilyen)");
	}
}

void Menu(){
	int x;
	do{
		printf("\n-----------------------------\n");
		printf("Kerem valasszon a kovetkezo menupontok kozul:\n");
		printf("1.Rendezvenyre jelentkezes\n");
		printf("2.Jelentkezettek listazasa\n");
		printf("3.Vendeg adatmodositasa\n");
		printf("4.Vendeg torlese\n");
		printf("5.Rendezveny inditasa\n");
		printf("6.Rendezveny torlese\n");
		printf("0.Kilepes\n");
		printf("-----------------------------\n");
		scanf("%d", &x);
		switch(x) {
			case 1:
			  Jelentkezes();
			  break;
			case 2:
			  Kilistazas();
			  break;
			case 3:
			  Modositas();
			  break;
			case 4:
			  VTorles();
			  break;
			case 5:
			  UjRend();
			  break;
			case 6:
			  RTorles();
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