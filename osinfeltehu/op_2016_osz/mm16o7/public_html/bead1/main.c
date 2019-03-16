#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_SIZE 200

void menuWrite(){
    printf("\nKing of Stands Menu:");
    printf("\n1 - Vendeg jelentekzes");
    printf("\n2 - Jelentkezettek listazasa");
    printf("\n3 - Vendeg adatmodositas");
    printf("\n4 - Vendeg torlese");
    printf("\n5 - Uj rendezveny");
    printf("\n6 - Rendezveny torlese");
    printf("\n0 - kilepes");
    printf("\nvalasszon menupontot: ");
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void fajlmasol(const char *alap,const char *cel){
    FILE *f1;
    FILE *f2;
    f1 = fopen(alap,"r");
    f2 = fopen(cel,"w");
	
    char sor[500];
    
    while(fgets(sor,sizeof(sor),f1) != NULL){
	fputs(sor,f2);
    }
	
    fclose(f1);
    fclose(f2);
	
    remove("temp.txt");
}

void run(){
    char ch;
    do{
        menuWrite();
        int hiba;

        do{
	    fflush(stdin);
	    hiba = 0;
            scanf("%c", &ch);
            if (ch!='1' && ch!='2' && ch!='3' && ch!='4' && ch!='5' && ch!='6' && ch!='0'){
                printf("\nHibas menupontbevitel, adja meg ujra: ");
                hiba=1;
            }
            fflush(stdin);
        }while(hiba==1);
        switch (ch) {
            case '1': {
                        FILE *rendezveny;

                        char rendAzon[MAX_SIZE];
                        char nev[MAX_SIZE];
                        char mail[MAX_SIZE];
                        time_t ido;

                        printf("\nAdja meg a rendezveny azonositojat: ");
                        scanf("%s",rendAzon);

                        rendezveny = fopen(concat(rendAzon,".txt"),"a+");

                        printf("\nVendeg neve: ");
                        scanf("%s",nev);
                        printf("\nVendeg email cime: ");
                        scanf("%s", mail);

                        time(&ido);

                        fprintf(rendezveny,"%s %s %s", nev, mail, ctime(&ido));

                        fclose(rendezveny);

			rendezveny = fopen(concat(rendAzon,".txt"),"r");

			int sorszam=0;
  			int c;
 			while((c = fgetc(rendezveny))!=EOF){
    				if((char)c=='\n'){
     					sorszam++;
   				}
  			}
			printf("\nA jelentkezes %d. helyen sikeresen megtortent!", sorszam);
			fclose(rendezveny);

                        break;
            }

            case '2':{
                        FILE *rendezveny;
                        char rendAzon[MAX_SIZE];

                        printf("\nAdja meg a rendezveny azonositojat: ");
                        scanf("%s",rendAzon);
			rendezveny = fopen(concat(rendAzon,".txt"),"r");

			char kiir[MAX_SIZE];
			printf("\nA %s rendezvenyre jelentekzettek listaja:\n",rendAzon);
			while(fgets(kiir,MAX_SIZE,rendezveny) != NULL){
				printf("%s",kiir);
			}
	
			fclose(rendezveny);
                        break;
            }

           case '3': {
			printf("\nAdja meg a rendezveny azonositojat: ");
			char rendAzon[MAX_SIZE];
			scanf("%s",rendAzon);

			printf("\nAdja meg a modositando vendeg nevet: ");
			char nevMod[MAX_SIZE];
			scanf("%s",nevMod);			

			FILE *rendezveny;
			FILE *temp;
			rendezveny = fopen(concat(rendAzon,".txt"),"r");
			temp = fopen("temp.txt","w");
			char sor[500];
	
			char nev[MAX_SIZE];

			char ujNev[MAX_SIZE];
			char ujMail[MAX_SIZE];
			time_t ujIdo;

			while(fgets(sor,sizeof(sor),rendezveny) != NULL){
			    sscanf(sor,"%s", nev);
		
			    if( strcmp(nevMod,nev) == 0 ){
			        printf("Add meg az uj nevet: ");
				scanf("%s",ujNev);
				printf("\nAdja meg az uj mail cimet: ");
				scanf("%s",ujMail);
				time(&ujIdo);
				fprintf(temp,"%s %s %s",ujNev,ujMail,ctime(&ujIdo));
				printf("\nAz adatmodositas sikeresen megtortent!");
			    }
			    else{
			        fputs(sor,temp);
			    }
			}
		
			fclose(rendezveny);
			fclose(temp);
	
			fajlmasol("temp.txt",concat(rendAzon,".txt"));	
                        break;
           }	

           case '4': {
			printf("\nAdja meg a rendezveny azonositojat: ");
                        char rendAzon[MAX_SIZE];
                        scanf("%s",rendAzon);

                        printf("\nAdja meg a torlendo vendeg nevet: ");
                        char nevMod[MAX_SIZE];
                        scanf("%s",nevMod);

                        FILE *rendezveny;
                        FILE *temp;
                        rendezveny = fopen(concat(rendAzon,".txt"),"r");
                        temp = fopen("temp.txt","w");
                        char sor[500];

                        char nev[MAX_SIZE];

                        while(fgets(sor,sizeof(sor),rendezveny) != NULL){
                            sscanf(sor,"%s", nev);

                            if( strcmp(nevMod,nev) != 0 ){
                                fputs(sor,temp);
                            }
                        }
			printf("\nA torles sikeresen megtortent!");			

                        fclose(rendezveny);
			fclose(temp);

			fajlmasol("temp.txt",concat(rendAzon,".txt"));
                        break;
           }

           case '5': {

                        FILE *rendezveny;
                        char rendAzon[MAX_SIZE];

                        printf("\nAdja meg a rendezveny azonositojat: ");
                        scanf("%s",rendAzon);

                        printf("\nA rendezveny letrehozasa sikerult!");
                        break;
            }

           case '6': {
                        FILE *rendezveny;
                        char rendAzon[MAX_SIZE];

                        printf("\nAdja meg a rendezveny azonositojat: ");
                        scanf("%s",rendAzon);

			remove(concat(rendAzon,".txt"));
			printf("\nAz esemény eltávolításra került!");

			break;
           }
        }
    }while (ch!='0');
}

int main()
{
    run();

    return 0;
}
