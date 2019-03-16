#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void menu_lehetosegek_kiirasa(){
    printf("Nyomd meg a megfelelo szamot, a kivant funkcio kivalasztasahoz:\n");
    printf("    1: Vendeg jelentkezes rendezvenyre\n");
    printf("    2: Jelentkezettek listazasa\n");
    printf("    3: Egy vendeg adatainak modositasa\n");
    printf("    4: Egy vendeg adatainak torlese\n");
    printf("    5: Uj rendezveny hozzaadasa\n");
    printf("    6: Egy rendezveny osszes adatainak torlese\n");
    printf("    7: Kilepes a programbol\n");
    printf("    8: Rendezvenyek lebonyolítása\n");
}

int number_of_lines(){
    FILE* myfile = fopen("test.txt", "r");
    int ch, number_of_lines = 0;
    do {
        ch = fgetc(myfile);
        if(ch == '\n')
            number_of_lines++;
    } while (ch != EOF);
    fclose(myfile);
    return number_of_lines;
}

int number_of_lines_of_rendezvenyek(){
    FILE* myfile = fopen("rendezvenyek.txt", "r");
    int ch, number_of_lines = 0;
    do {
        ch = fgetc(myfile);
        if(ch == '\n')
            number_of_lines++;
    } while (ch != EOF);
    fclose(myfile);
    return number_of_lines;
}

void vendegek_listazasa(){
    printf("Vendegek listazasa:\n");
    int c;
    FILE *file;
    file = fopen("test.txt", "r");
    if (file) {
        while ((c = getc(file)) != EOF){
            putchar(c);
        }
    }
    fclose(file);
}

void vendegek_listazasa_sorszamozva(){
    //szamoljuk meg h hany sor van a fajlban
    printf("Vendegek:\n");
    int line_number = number_of_lines();
    FILE *file4;
    file4 = fopen("test.txt", "r");
    if (file4) {
        int d;
        int j = 1;
        printf("1) ");
        //szamoljuk a sorokat es csak addig sorszamozzunk:
        int sorok_szama = 0;
        do {
            d = fgetc(file4);
            putchar(d);
            if(d == '\n' && sorok_szama < line_number-1){
                    sorok_szama++;
                    j++;
                    printf("%d) ", j);
            }
        } while (d != EOF);
    }
    fclose(file4);
    char tmp2[30];gets(tmp2);
}

void rendezvenyek_listazasa_sorszamozva(){
    //szamoljuk meg h hany sor van a fajlban
    printf("Rendezvenyek:\n");
    int line_number = number_of_lines_of_rendezvenyek();
    FILE *file4;
    file4 = fopen("rendezvenyek.txt", "r");
    if (file4) {
        int d;
        int j = 1;
        printf("1) ");
        //szamoljuk a sorokat es csak addig sorszamozzunk:
        int sorok_szama = 0;
        do {
            d = fgetc(file4);
            putchar(d);
            if(d == '\n' && sorok_szama < line_number-1){
                    sorok_szama++;
                    j++;
                    printf("%d) ", j);
            }
        } while (d != EOF);
    }
    fclose(file4);
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

void rendezvenyek_lebonyolitasa(char rendezveny_azonosito[]){
	//cso letrehozasa:
	int pipefd[2]; //egy cso, amibe ír a szülő, olvas a gyerek
	int pipefd2[2]; //egy cso, amibe ír a gyerek, olvas a szulo
	pid_t pid; //folyamat azonosító (h gyerek - v szulo)
	
	//megnyitjuk az egyik csovet:
	if(pipe(pipefd) == -1){
		perror("Hiba a cső megnyitásakor.");
		exit(EXIT_FAILURE);
	}
	
	if(pipe(pipefd2) == -1){
		perror("Hiba a cső megnyitásakor.");
		exit(EXIT_FAILURE);
	}
	
	//forkkal letrejon a gyerekfolyamat
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
			int random = rand() % 100;			
			read(pipefd[0],kapott_vendeg,1000);
			if(random > 10){
				printf("kapott_vendeg: %s\n", kapott_vendeg);
			}else{
				stpcpy(&vendegek[nem_erkezett_meg],kapott_vendeg);
				nem_erkezett_meg++;
			}
		}
		int r = rand() % 5 + 1;
		write(pipefd2[1],&r,sizeof(r));
		write(pipefd2[1],&nem_erkezett_meg,sizeof(nem_erkezett_meg));
		for(i = 0; i<nem_erkezett_meg;i++){
			write(pipefd2[1],&vendegek[i],1000);
		}
		
		exit(EXIT_SUCCESS);
		
	}else{
		int vendegek_szama = number_of_lines();
		char vendegek[vendegek_szama][1000];
		//szulofolyamat
		printf("Szulo folyamat indul.\n");

		int db = 0;
		FILE *file;
		file = fopen("test.txt", "rb");
		char vendeg[1000];
		read(pipefd2[0],vendeg,1000);
		printf("A gyerek uzenete: %s\n", vendeg);
		//beolvassuk az osszes vendeget egy tombbe
		if (file) {
			while (fgets(&vendeg,sizeof(vendeg),file)){
				if(strcmp(rendezveny_azonosito,rendezveny(vendeg)) == 0){
					stpcpy(&vendegek[db],vendeg);
					db++;
				}
				//putchar(c);
				//pipefd[0] - olvaso resz
				//pipefd[1] - iro resz
			}
		}
		fclose(file);
		//elkuldjuk a gyerekfolyamatnak a vendegek szamat, majd a vendegeket
		int i;
		write(pipefd[1],&db,sizeof(db));
		for(i = 0; i<db;i++){
			write(pipefd[1],&vendegek[i],1000);
		}
		
		wait();
		int r;
		read(pipefd2[0],&r,sizeof(r));
		printf("Rendezveny sikeressege(1-5): %d\n",r);
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
	srand(time(NULL));
    menu_lehetosegek_kiirasa();
    char menu = getchar( );
    switch(menu) {
        case '1' :
            printf("Vendeg jelentkezese rendezvenyre\n");
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
			//fprintf(fp,"%s, %s, %s, %s\n", nev, email, rend_azon, char_timeinfo); // write to file

            FILE *fp;
            fp = fopen("test.txt", "a+");
            //a sorszamot jobb, ha nem mentjuk el mert valtozhat:
            //fprintf(fp, "%d, %s, %s, %s, %s", number_of_lines+1 , nev, email, rend_azon, asctime (timeinfo)); // write to file
            fprintf(fp,"%s, %s, %s, %s\n", nev, email, rend_azon, char_timeinfo); // write to file
			//fprintf(fp,"%s, %s, %s", nev, email, rend_azon); // write to file
            fclose(fp);
            printf("%d. vendegkent regisztralt!\n", number_of_lines());
            break;
        case '2'  :
                vendegek_listazasa();
            break;
        case '3' :
            {
            printf("4: Egy vendeg adatainak modositasa\n");
            vendegek_listazasa_sorszamozva();

            int torlendo_sor = 0;
            printf("\nModositando vendeg sorszama: ");
            scanf ("%d", &torlendo_sor);

            //csak ervenyes indexu sort torlunk:
            int line_number = number_of_lines();
            printf("\nNUMBER OF LINES: %d\n", line_number);
            if(torlendo_sor>number_of_lines() || torlendo_sor<1){
                printf("Hibas sorszam!\n");
                return 0;
            }

            FILE *fileptr1, *fileptr2;
            char ch;
            int temp = 1;
            fileptr1 = fopen("test.txt", "r+");
            fileptr2 = fopen("test2.txt", "w+");
            ch = getc(fileptr1);
            while (ch != EOF){
                if (ch == '\n'){
                    temp++;
                }
                if(torlendo_sor == 1 && temp == 2){ //az elso enter utan bekerek adatokat, majd mindent masoljunk at:
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
                //kiveve a torlendo sort
                    if (temp != torlendo_sor){
                        putc(ch, fileptr2);
                    }
                    if( temp == torlendo_sor+1){ // ha a torlendo sor vegere ertem
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

            remove("test.txt");
            rename("test2.txt", "test.txt");
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

            //csak ervenyes indexu sort torlunk:
            int line_number = number_of_lines();
            if(torlendo_sor>number_of_lines() || torlendo_sor<1){
                printf("Hibas sorszam!\n");
                return 0;
            }

            FILE *fileptr1, *fileptr2;
            char ch;
            int temp = 1;
            fileptr1 = fopen("test.txt", "r+");
            fileptr2 = fopen("test2.txt", "w+");
            ch = getc(fileptr1);
            while (ch != EOF){
                if (ch == '\n'){
                    temp++;
                }
                if(torlendo_sor == 1){
                    if(temp == 2){ //az elso enter utan mindent masoljunk at:
                        ch = getc(fileptr1);
                        while(ch != EOF){
                            putc(ch, fileptr2);
                            ch = getc(fileptr1);
                        }
                    }
                }else{
                //kiveve a torlendo sort
                if (temp != torlendo_sor){
                    putc(ch, fileptr2);
                }
                }
                ch = getc(fileptr1);
            }
            fclose(fileptr1);
            fclose(fileptr2);

            remove("test.txt");
            rename("test2.txt", "test.txt");
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

            FILE *fp5;
            fp5 = fopen("rendezvenyek.txt", "a+");
            fprintf(fp5,"%s, %s, %s\n", nev, azon, ido); // write to file
            fclose(fp5);
            printf("%d. rendezveny regisztralva!\n", number_of_lines_of_rendezvenyek());
            }
            break;
        case '6'  :
            {
            printf("6: Rendezveny torlese\n");
            rendezvenyek_listazasa_sorszamozva();

            int torlendo_sor = 0;
            printf("\nTorlendo rendezveny sorszama: ");
            scanf ("%d", &torlendo_sor);

            //csak ervenyes indexu sort torlunk:
            int line_number = number_of_lines_of_rendezvenyek();
            if(torlendo_sor>line_number || torlendo_sor<1){
                printf("Hibas sorszam!\n");
                return 0;
            }

            FILE *fileptr1, *fileptr2;
            char ch;
            int temp = 1;
            fileptr1 = fopen("rendezvenyek.txt", "r+");
            fileptr2 = fopen("rendezvenyek2.txt", "w+");
            ch = getc(fileptr1);
            while (ch != EOF){
                if (ch == '\n'){
                    temp++;
                }
                if(torlendo_sor == 1){
                    if(temp == 2){ //az elso enter utan mindent masoljunk at:
                        ch = getc(fileptr1);
                        while(ch != EOF){
                            putc(ch, fileptr2);
                            ch = getc(fileptr1);
                        }
                    }
                }else{
                //kiveve a torlendo sort
                if (temp != torlendo_sor){
                    putc(ch, fileptr2);
                }
                }
                ch = getc(fileptr1);
            }
            fclose(fileptr1);
            fclose(fileptr2);

            remove("rendezvenyek.txt");
            rename("rendezvenyek2.txt", "rendezvenyek.txt");
            remove("rendezvenyek2.txt");

            char tmp2[30];gets(tmp2);
            printf("%d. sorszamu rendezvenyt toroltuk!", torlendo_sor);
            }
            break;
        case '7'  :
            exit(0);
            break;
		case '8'  :
			printf("Adja meg a rendezveny azonositojat: ");
			char rend[30];
			scanf("%s", rend);
            rendezvenyek_lebonyolitasa(rend);
            break;

        default :
            printf("Nem megfelelo szam lett le nyomva!");
    }
    return 0;
}
