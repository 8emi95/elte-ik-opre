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
}

int number_of_lines(char* file_name){
    FILE* myfile = fopen(file_name, "r");
    int ch, count = 0;
	int charsOnCurrentLine = 0;
    
	while ((ch = fgetc(myfile)) != EOF) {
    if (ch == '\n') {
        count++;
        charsOnCurrentLine = 0;
    } else {
        charsOnCurrentLine++;
    }
	}
	if (charsOnCurrentLine > 0) {
		count++;
	}
	
    fclose(myfile);
	return count;
}

void vendegek_listazasa(){
    printf("Vendegek listazasa:\n");
    int c;
    FILE *file;
    file = fopen("vendegek.txt", "r");
    if (file) {
        while ((c = getc(file)) != EOF){
            putchar(c);
        }
    }
    fclose(file);
}

void file_listazasa_sorszamozva(char* file_name, char* msg){
    printf("%s\n",msg);
    int line_number = number_of_lines(file_name);
    FILE *file4;
    file4 = fopen(file_name, "r");
    if (file4) {
        int d;
        int j = 1;
        printf("1) ");
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

int main()
{
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
			
            FILE *fp;
            fp = fopen("vendegek.txt", "a+");
            fprintf(fp,"%s, %s, %s, %s\n", nev, email, rend_azon, char_timeinfo);
			fclose(fp);
            printf("%d. vendegkent regisztralt!\n", number_of_lines("vendegek.txt"));
            break;
        case '2'  :
                vendegek_listazasa();
            break;
        case '3' :
            {
            printf("3: Egy vendeg adatainak modositasa\n");
            file_listazasa_sorszamozva("vendeg.txt", "Vendegek: ");

            int torlendo_sor = 0;
            printf("\nModositando vendeg sorszama: ");
            scanf ("%d", &torlendo_sor);

            int line_number = number_of_lines("vendegek.txt");
            printf("\nNUMBER OF LINES: %d\n", line_number);
            if(torlendo_sor>line_number || torlendo_sor<1){
                printf("Hibas sorszam!\n");
                return 0;
            }

            FILE *fileptr1, *fileptr2;
            char ch;
            int temp = 1;
            fileptr1 = fopen("vendegek.txt", "r+");
            fileptr2 = fopen("vendegekCopy.txt", "w+");
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

            remove("vendegek.txt");
            rename("vendegekCopy.txt", "vendegek.txt");
            remove("vendegekCopy.txt");

            printf("%d. sorszamu vendeget modositottuk!", torlendo_sor);

            }
            break;
        case '4'  :
            printf("4: Egy vendeg adatainak torlese\n");
            file_listazasa_sorszamozva("vendegek.txt", "Vendegek: ");

            int torlendo_sor = 0;
            printf("Torlendo vendeg sorszama: ");
            scanf ("%d", &torlendo_sor);


            int line_number = number_of_lines("vendegek.txt");
            if(torlendo_sor>line_number || torlendo_sor<1){
                printf("Hibas sorszam!\n");
                return 0;
            }

            FILE *fileptr1, *fileptr2;
            char ch;
            int temp = 1;
            fileptr1 = fopen("vendegek.txt", "r+");
            fileptr2 = fopen("vendegekCopy.txt", "w+");
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

            remove("vendegek.txt");
            rename("vendegekCopy.txt", "vendegek.txt");
            remove("vendegekCopy.txt");

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
            printf("%d. rendezveny regisztralva!\n", number_of_lines("rendezvenyek.txt"));
            }
            break;
        case '6'  :
            {
            printf("6: Rendezveny torlese\n");
            file_listazasa_sorszamozva("rendezvenyek.txt", "Rendezvenyek: ");

            int torlendo_sor = 0;
            printf("\nTorlendo rendezveny sorszama: ");
            scanf ("%d", &torlendo_sor);
			
            int line_number = number_of_lines("rendezvenyek.txt");
            if(torlendo_sor>line_number || torlendo_sor<1){
                printf("Hibas sorszam!\n");
                return 0;
            }

            FILE *fileptr1, *fileptr2;
            char ch;
            int temp = 1;
            fileptr1 = fopen("rendezvenyek.txt", "r+");
            fileptr2 = fopen("rendezvenyekCopy.txt", "w+");
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

            remove("rendezvenyek.txt");
            rename("rendezvenyekCopy.txt", "rendezvenyek.txt");
            remove("rendezvenyekCopy.txt");

            char tmp2[30];gets(tmp2);
            printf("%d. sorszamu rendezvenyt toroltuk!", torlendo_sor);
            }
            break;
        case '7'  :
            exit(0);
            break;

        default :
            printf("Nem megfelelo menupont!");
    }
    return 0;
}
