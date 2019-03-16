#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void menu_lehetosegek_kiirasa(){
    printf("Nyomd meg a megfelelo szamot, a kivant funkcio kivalasztasahoz:\n");
    printf("    1: Resztvevo regisztracio\n");
    printf("    2: Jelentkezettek listazasa\n");
    printf("    3: Vendeg adat modositasa\n");
    printf("    4: Egy vendeg adatainak torlese\n");
    printf("    5: Uj rendezveny hozzaadasa\n");
    printf("    6: Rendezveny osszes adatanak torlese\n");
    printf("    7: Rendezvenyek lebonyolitasa\n");
	printf("    0: Kilépés\n");
}

int vendNum(){
    FILE* file = fopen("vendeg.txt", "r");
    int ch, vendNum = 0;
    do {
        ch = fgetc(file);
        if(ch == '\n')
            vendNum++;
    } while (ch != EOF);
    fclose(file);
    return vendNum;
}

int rendNum(){
    FILE* file = fopen("rendezveny.txt", "r");
    int ch, n = 0;
    do {
        ch = fgetc(file);
        if(ch == '\n')
            n++;
    } while (ch != EOF);
    fclose(file);
    return n;
}

void vendegek_listazasa(){
    printf("Vendegek listazasa:\n");
    int c;
    FILE *file;
    file = fopen("vendeg.txt", "r");
    if (file) {
        while ((c = getc(file)) != EOF){
            putchar(c);
        }
    }
    fclose(file);
}

void vendegek_listazasa_sorszamozva(){
    printf("Vendegek:\n");
    int line_number = vendNum();
    FILE *file;
    file = fopen("vendeg.txt", "r");
    if (file) {
        int d;
        int j = 1;
        printf("1) ");
        int sorok_szama = 0;
        do {
            d = fgetc(file);
            putchar(d);
            if(d == '\n' && sorok_szama < line_number-1){
                    sorok_szama++;
                    j++;
                    printf("%d) ", j);
            }
        } while (d != EOF);
    }
    fclose(file);
    char tmp2[30];gets(tmp2);
}

void rendListSzammal(){
    printf("Rendezvenyek:\n");
    int line_number = rendNum();
    FILE *file;
    file = fopen("rendezveny.txt", "r");
    if (file) {
        int d;
        int j = 1;
        printf("1) ");
        int sorok_szama = 0;
        do {
            d = fgetc(file);
            putchar(d);
            if(d == '\n' && sorok_szama < line_number-1){
                    sorok_szama++;
                    j++;
                    printf("%d) ", j);
            }
        } while (d != EOF);
    }
    fclose(file);
    char tmp2[30];gets(tmp2);
}

char* rendezveny(char vendeg[]){
	char* token;
	token = strtok(vendeg,", ");
	int db = 1;
	while( db < 3){
		token = strtok(NULL,", ");
		db++;
	}
	return token;
}

char* getRendazonFromRendezveny(char rendezveny[])  {
	char* token;
	token = strtok(rendezveny,", ");
	int db = 1;
	while( db < 2){		
		token = strtok(NULL,", ");
		db++;
	}
	return token;
}

//1 ha igaz 0 ha hamis
int rendezvenyExists(char rendAzon[]){
		FILE *file;
		file = fopen("rendezveny.txt", "rb");
		char rendezveny[1000];
		if (file) {
			while (fgets(&rendezveny,sizeof(rendezveny),file)){
				if(strcmp(rendAzon,getRendazonFromRendezveny(rendezveny)) == 0){					
					fclose(file);
					return 1;
				}
			}
		}		
		fclose(file);
		return 0;
}

void rendezvenyek_lebonyolitasa(char rendezveny_azonosito[]){
	srand(time(NULL));
	int pipefd[2];
	int pipefd2[2];
	pid_t pid;
	

	if(pipe(pipefd) == -1){
		perror("Hiba a cső megnyitásakor.");
		exit(EXIT_FAILURE);
	}
	
	if(pipe(pipefd2) == -1){
		perror("Hiba a cső megnyitásakor.");
		exit(EXIT_FAILURE);
	}
	
	pid = fork();
	if(pid == -1){
		perror("Hiba a forknal.");
		exit(EXIT_FAILURE);
	}
	
	if(pid == 0){
		//gyerekfolyamat
		sleep(1);
		write(pipefd2[1],"Megerkeztem",1000);
		
		int i;
		int db = 0;
		read(pipefd[0],&db,sizeof(db));
		printf("DB : %d\n",db);
		char kapott_vendeg[1000];
		char vendegek[db][1000];
		int nem_erkezett_meg = 0;
		for(i = 0; i<db;i++){
			int random = rand() % 10;			
			read(pipefd[0],kapott_vendeg,1000);
			if(random > 1){
				printf("kapott_vendeg: %s\n", kapott_vendeg);
			}else{
				stpcpy(&vendegek[nem_erkezett_meg],kapott_vendeg);
				nem_erkezett_meg++;
			}
		}
		int r = rand() % 100 + 1;
		write(pipefd2[1],&r,sizeof(r));
		write(pipefd2[1],&nem_erkezett_meg,sizeof(nem_erkezett_meg));
		for(i = 0; i<nem_erkezett_meg;i++){
			write(pipefd2[1],&vendegek[i],1000);
		}
		
		exit(EXIT_SUCCESS);
		
	}else{
		int vendegek_szama = vendNum();
		char vendegek[vendegek_szama][1000];
		//szulofolyamat
		printf("Szulo folyamat indul.\n");

		int db = 0;
		FILE *file;
		file = fopen("vendeg.txt", "rb");
		char vendeg[1000];
		read(pipefd2[0],vendeg,1000);
		printf("A gyerek uzenete: %s\n", vendeg);
		if (file) {
			while (fgets(&vendeg,sizeof(vendeg),file)){
				if(strcmp(rendezveny_azonosito,rendezveny(vendeg)) == 0){
					stpcpy(&vendegek[db],vendeg);
					db++;
				}
			}
		}
		fclose(file);
		int i;
		write(pipefd[1],&db,sizeof(db));
		for(i = 0; i<db;i++){
			write(pipefd[1],&vendegek[i],1000);
		}
		
		wait();
		int r;
		read(pipefd2[0],&r,sizeof(r));
		printf("Rendezveny sikeressege: %d%\n",r);
		read(pipefd2[0],&db,sizeof(db));
		printf("Nem erkzett meg: %d\n",db);
		for(i = 0; i<db;i++){	
			read(pipefd2[0],vendeg,1000);
			printf("vendeg: %s\n", vendeg);
		}
		
	}
	
}

int main()
{
	
	char menu='1';
	while(menu!='0')
	{
    menu_lehetosegek_kiirasa();
    menu = getchar( );
    switch(menu) {
		srand(time(NULL));
        case '1' :
            printf("Resztvevo regisztracio\n");
            char tmp[30];gets(tmp);

            printf("Nev: \n");char nev[30];gets(nev);
            printf("E-mail: \n");char email[30];gets(email);
            printf("Rendezveny azonosito: \n");char rend_azon[30];gets(rend_azon);

			if(rendezvenyExists(rend_azon)==1)
			{
            time_t rawtime;
            struct tm * timeinfo;
            time ( &rawtime );
            timeinfo = localtime ( &rawtime );
			char char_timeinfo[100];
			strftime (char_timeinfo,80,"%c",timeinfo);

            FILE *fp;
            fp = fopen("vendeg.txt", "a+");
            fprintf(fp,"%s, %s, %s, %s\n", nev, email, rend_azon, char_timeinfo);
            fclose(fp);
            printf("%d. vendegkent regisztralt!\n", vendNum());
			}
			else {
				printf("Nincs Ilyen Rendezveny!\n");
			}
            break;
        case '2'  :
                vendegek_listazasa();
            break;
        case '3' :
            {
            printf("4: Vendeg adat modositasa\n");
            vendegek_listazasa_sorszamozva();

            int torlendo_sor = 0;
            printf("\nModositando vendeg sorszama: ");
            scanf ("%d", &torlendo_sor);

            int line_number = vendNum();
            printf("\nNUMBER OF LINES: %d\n", line_number);
            if(torlendo_sor>vendNum() || torlendo_sor<1){
                printf("Hibas sorszam!\n");
                return 0;
            }

            FILE *fileptr1, *fileptr2;
            char ch;
            int temp = 1;
            fileptr1 = fopen("vendeg.txt", "r+");
            fileptr2 = fopen("test2.txt", "w+");
            ch = getc(fileptr1);
            while (ch != EOF){
                if (ch == '\n'){
                    temp++;
                }
                if(torlendo_sor == 1 && temp == 2){
                    char tmp[30];gets(tmp);
                    printf("Nev: \n");char nev[30];gets(nev);
                    printf("E-mail: \n");char email[30];gets(email);
                    printf("Rendezveny azonosito: \n");char rend_azon[30];gets(rend_azon);
                    
					time_t rawtime;
					struct tm * timeinfo;
					time ( &rawtime );
					timeinfo = localtime ( &rawtime );
					char char_timeinfo[100];
					strftime (char_timeinfo,80,"%c",timeinfo);
					
					fprintf(fileptr2,"%s, %s, %s, %s\n", nev, email, rend_azon, char_timeinfo); // write to file
					
                    ch = getc(fileptr1);
                    while(ch != EOF){
                        putc(ch, fileptr2);
                        ch = getc(fileptr1);
                    }
                }else{
                    if (temp != torlendo_sor){
                        putc(ch, fileptr2);
                    }
                    if( temp == torlendo_sor+1){ 
                        char tmp[30];gets(tmp);
                        printf("Nev: \n");char nev[30];gets(nev);
                        printf("E-mail: \n");char email[30];gets(email);
                        printf("Rendezveny azonosito: \n");char rend_azon[30];gets(rend_azon);
                        
						time_t rawtime;
						struct tm * timeinfo;
						time ( &rawtime );
						timeinfo = localtime ( &rawtime );
						char char_timeinfo[100];
						strftime (char_timeinfo,80,"%c",timeinfo);
						
						fprintf(fileptr2,"%s, %s, %s, %s\n", nev, email, rend_azon, char_timeinfo); // write to file
                        ch = getc(fileptr1);
                        while(ch != EOF){
                            if (ch == '\n'){
                                temp++;
                            }
                            if(torlendo_sor != temp){
                                putc(ch, fileptr2);
                            }
                            ch = getc(fileptr1);
                        }
                    }
                }
                ch = getc(fileptr1);
            }
            fclose(fileptr1);
            fclose(fileptr2);

            remove("vendeg.txt");
            rename("test2.txt", "vendeg.txt");
            remove("test2.txt");

            printf("%d. sorszamu vendeget modositottuk!", torlendo_sor);

            }
            break;
        case '4'  :
            printf("4: Egy vendeg adatainak torlese\n");
            vendegek_listazasa_sorszamozva();

            int torlendo_sor = 0;
            printf("Torlendo vendeg sorszama: ");
            scanf ("%d", &torlendo_sor);

            int line_number = vendNum();
            if(torlendo_sor>vendNum() || torlendo_sor<1){
                printf("Hibas sorszam!\n");
                return 0;
            }

            FILE *fileptr1, *fileptr2;
            char ch;
            int temp = 1;
            fileptr1 = fopen("vendeg.txt", "r+");
            fileptr2 = fopen("test2.txt", "w+");
            ch = getc(fileptr1);
            while (ch != EOF){
                if (ch == '\n'){
                    temp++;
                }
                if(torlendo_sor == 1){
                    if(temp == 2){ 
                        ch = getc(fileptr1);
                        while(ch != EOF){
                            putc(ch, fileptr2);
                            ch = getc(fileptr1);
                        }
                    }
                }else{
                if (temp != torlendo_sor){
                    putc(ch, fileptr2);
                }
                }
                ch = getc(fileptr1);
            }
            fclose(fileptr1);
            fclose(fileptr2);

            remove("vendeg.txt");
            rename("test2.txt", "vendeg.txt");
            remove("test2.txt");

            char tmp2[30];gets(tmp2);
            printf("%d. sorszamu vendeget toroltuk!", torlendo_sor);

            break;
        case '5' :
            {
            printf("Uj rendezveny hozzaadasa:\n");
            char tmp[30];gets(tmp);

            printf("Rendezveny neve: \n");char nev[30];gets(nev);
            printf("Rendezveny azonositoja: \n");char azon[30];gets(azon);
            printf("Rendezveny idopontja: \n");char ido[30];gets(ido);

            FILE *file;
            file = fopen("rendezveny.txt", "a+");
            fprintf(file,"%s, %s, %s\n", nev, azon, ido); // write to file
            fclose(file);
            printf("%d. rendezveny regisztralva!\n", rendNum());
            }
            break;
        case '6'  :
            {
            printf("6: Rendezveny torlese\n");
            rendListSzammal();

            int torlendo_sor = 0;
            printf("\nTorlendo rendezveny sorszama: ");
            scanf ("%d", &torlendo_sor);

            int line_number = rendNum();
            if(torlendo_sor>line_number || torlendo_sor<1){
                printf("Hibas sorszam!\n");
                return 0;
            }

            FILE *fileptr1, *fileptr2;
            char ch;
            int temp = 1;
            fileptr1 = fopen("rendezveny.txt", "r+");
            fileptr2 = fopen("rendezveny2.txt", "w+");
            ch = getc(fileptr1);
            while (ch != EOF){
                if (ch == '\n'){
                    temp++;
                }
                if(torlendo_sor == 1){
                    if(temp == 2){ 
                        ch = getc(fileptr1);
                        while(ch != EOF){
                            putc(ch, fileptr2);
                            ch = getc(fileptr1);
                        }
                    }
                }else{
                if (temp != torlendo_sor){
                    putc(ch, fileptr2);
                }
                }
                ch = getc(fileptr1);
            }
            fclose(fileptr1);
            fclose(fileptr2);

            remove("rendezveny.txt");
            rename("rendezveny2.txt", "rendezveny.txt");
            remove("rendezveny2.txt");

            char tmp2[30];gets(tmp2);
            printf("%d. sorszamu rendezvenyt toroltuk!", torlendo_sor);
            }
            break;
		case '7'  :
			printf("Adja meg a rendezveny azonositojat: ");
			char rend[30];
			scanf("%s", rend);
            rendezvenyek_lebonyolitasa(rend);
            break;
		case '0'  :
			break;
    }
	}
	return 0;
}
