#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#define SIZE 64
#define KERDESSOR "kerdessor.txt"
#define KONZULTACIO "konzultacio.txt"
#define TEMP "temp.txt"

void handler(int signumber){
  printf("\nKerdezobiztos megerkezett\n");
}

int main()
{
	while(1){
		printf("1 - Kerdessor listazas\n");
		printf("2 - Kerdes listazas\n");
		printf("3 - Kerdes hozzadas\n");
		printf("4 - Kerdes torles\n");
		printf("5 - Kerdes modositas\n");
		printf("6 - Konzultacio veglegesites\n");
		printf("7 - Konzultacio listazas\n");
		printf("8 - MLSZ konzultacio\n");
		printf("9 - Kilepes\n");
		printf("Kerem valasszon: ");
		char commandarray[5];
		fgets(commandarray, sizeof(commandarray), stdin);
		int command;
		sscanf(commandarray, "%d", &command);
		if(command == 9){
			break;
		}
		else if(command == 1){
			if( access(KERDESSOR, F_OK) != -1 ) {
				int read_file;
				read_file = open(KERDESSOR,O_RDONLY);
				char c;
				while (read(read_file,&c,sizeof(c))){ 
					printf("%c",c);
				} 
				printf("\n");
				close(read_file);
			} else {
				printf("\nJelenleg nincs kerdessor\n\n");
			}
		}
		else if(command == 2){
			int len;
			char azon[SIZE];
			char *file_contents;
			char *first;
			while(1){
				printf("Kerdes sorszama: ");
				fgets(azon, sizeof(azon), stdin);
				
				long input_file_size;
				FILE *input_file = fopen(KERDESSOR, "rb");
				fseek(input_file, 0, SEEK_END);
				input_file_size = ftell(input_file);
				rewind(input_file);
				file_contents = malloc(input_file_size * (sizeof(char)));
				fread(file_contents, sizeof(char), input_file_size, input_file);
				fclose(input_file);
				
				char word[] = "\n";
				len = strlen(azon);
				if (len > 0 && azon[len-1] == '\n')
				azon[len-1] = '\0';
				strcat(word, "Sorszam: ");
				strcat(word, azon);
				strcat(word, "\n");
				first = word;
				
				if(strstr(file_contents, word) != NULL) break;
			}
			
			const char *s = &(*file_contents);
			const char *PATTERN1 = &(*first);
			const char *PATTERN2 = "\n\n";
			const char *newrow = "\n";
			char *target = NULL;
			char *start, *end;
			if ( (start = strstr( s, PATTERN1 )) )
			{
				start += strlen( PATTERN1 );
				if ( (end = strstr( start, PATTERN2 )) )
				{
					target = ( char * )malloc( end - start + 1 );
					memcpy( target, start, end - start );
					target[end - start] = '\0';
				}
			}
			char text[SIZE];
			stpcpy(text, first);
			strcat(text, target);
			strcat(text, newrow);
			printf( "%s\n", text );
			free( target );
		}
		else if(command == 3){
			FILE *write_file;
			int len;
			int db;
			char azon[SIZE];
			char kerdes[SIZE];
			char valaszok[SIZE];
			char valasz[SIZE];
			if( access(KERDESSOR, F_OK) != -1 ) {
				while(1){
					printf("Uj kerdes sorszama: ");
					fgets(azon, sizeof(azon), stdin);
					
					char *file_contents;
					long input_file_size;
					FILE *input_file = fopen(KERDESSOR, "rb");
					fseek(input_file, 0, SEEK_END);
					input_file_size = ftell(input_file);
					rewind(input_file);
					file_contents = malloc(input_file_size * (sizeof(char)));
					fread(file_contents, sizeof(char), input_file_size, input_file);
					fclose(input_file);
					
					char word[] = "\n";
					len = strlen(azon);
					if (len > 0 && azon[len-1] == '\n')
					azon[len-1] = '\0';
					strcat(word, "Sorszam: ");
					strcat(word, azon);
					strcat(word, "\n");
					
					if(strstr(file_contents, word) == NULL) break;
				}
				printf("Uj kerdes: ");
				fgets(kerdes, sizeof(kerdes), stdin);
				
				write_file = fopen(KERDESSOR, "a+");
				len = strlen(kerdes);
				if (len > 0 && kerdes[len-1] == '\n')
				kerdes[len-1] = '\0';
				fprintf(write_file,"Sorszam: %s\n%s\n", azon, kerdes);
				
				while(1){
					printf("Valaszok szama: ");
					fgets(valaszok, sizeof(valaszok), stdin);
					db = atoi(valaszok);
					if(db > 1 && db < 5) break;
				}
				int i;
				for( i = 0; i < db; i = i + 1 ){
					printf("Uj valasz: ");
					fgets(valasz, sizeof(valasz), stdin);
					fprintf(write_file,"- %s", valasz);
				}
				time_t rawtime;
				struct tm * timeinfo;
				time ( &rawtime );
				timeinfo = localtime ( &rawtime );
				fprintf(write_file, "Felvetel ideje: %s", asctime(timeinfo));
				fprintf(write_file,"\n");
				
				fclose(write_file);
			} else {
				printf("Uj kerdes sorszama: ");
				fgets(azon, sizeof(azon), stdin);
				printf("Uj kerdes: ");
				fgets(kerdes, sizeof(kerdes), stdin);
				
				write_file = fopen(KERDESSOR, "w");
				len = strlen(azon);
				if (len > 0 && azon[len-1] == '\n')
				azon[len-1] = '\0';
				len = strlen(kerdes);
				if (len > 0 && kerdes[len-1] == '\n')
				kerdes[len-1] = '\0';
				fprintf(write_file,"\nSorszam: %s\n%s\n", azon, kerdes);
				
				while(1){
					printf("Valaszok szama: ");
					fgets(valaszok, sizeof(valaszok), stdin);
					db = atoi(valaszok);
					if(db > 1 && db < 5) break;
				}
				int i;
				for( i = 0; i < db; i = i + 1 ){
					printf("Uj valasz: ");
					fgets(valasz, sizeof(valasz), stdin);
					fprintf(write_file,"- %s", valasz);
				}
				time_t rawtime;
				struct tm * timeinfo;
				time ( &rawtime );
				timeinfo = localtime ( &rawtime );
				fprintf(write_file, "Felvetel ideje: %s", asctime(timeinfo));
				fprintf(write_file,"\n");
				
				fclose(write_file);
			}
			printf("\nAz uj kerdest felvettuk\n\n");
		}
		else if(command == 4){
			int len;
			char azon[SIZE];
			char *file_contents;
			char *first;
			char *second;
			while(1){
				printf("Kerdes sorszama: ");
				fgets(azon, sizeof(azon), stdin);
				
				long input_file_size;
				FILE *input_file = fopen(KERDESSOR, "rb");
				fseek(input_file, 0, SEEK_END);
				input_file_size = ftell(input_file);
				rewind(input_file);
				file_contents = malloc(input_file_size * (sizeof(char)));
				fread(file_contents, sizeof(char), input_file_size, input_file);
				fclose(input_file);
				
				char word[] = "\n";
				char word2[] = "";
				len = strlen(azon);
				if (len > 0 && azon[len-1] == '\n')
				azon[len-1] = '\0';
				strcat(word, "Sorszam: ");
				strcat(word2, "Sorszam: ");
				strcat(word, azon);
				//strcat(word2, azon);
				strcat(word, "\n");
				//strcat(word2, "\n");
				first = word;
				second = word2;
				
				if(strstr(file_contents, word) != NULL) break;
			}
			
			const char *s = &(*file_contents);
			const char *PATTERN1 = &(*first);
			const char *PATTERN2 = "\n\n";
			char *target = NULL;
			char *start, *end;
			if ( (start = strstr( s, PATTERN1 )) )
			{
				start += strlen( PATTERN1 );
				if ( (end = strstr( start, PATTERN2 )) )
				{
					target = ( char * )malloc( end - start + 1 );
					memcpy( target, start, end - start );
					target[end - start] = '\0';
				}
			}
			free( target );
			
			FILE *kerdessor;
			FILE *temp;
			kerdessor = fopen(KERDESSOR, "r");
			temp = fopen(TEMP, "w");
			char sor[SIZE];
			while(fgets(sor, sizeof(sor), kerdessor) != NULL)
			{
				fputs(sor, temp);
			}
			fclose(kerdessor);
			fclose(temp);
			
			kerdessor = fopen(KERDESSOR, "w");
			temp = fopen(TEMP, "r");
			int check = 1;
			char text[SIZE] = "";
			strcat(text, second);
			
			/*printf( "---%s---", text );
			printf( "---Sorszam: 2\n---");
			if(strcmp(text,"Sorszam: 2\n") == 0){
				printf("igen");
			}*/
			
			while(fgets(sor, sizeof(sor), temp) != NULL)
			{
				if(check == 1){
					if(strcmp(sor,text) == 0){
						check = 0;
					}
					else {
						fputs(sor, kerdessor);
					}
				} else {
					if(strcmp(sor,"\n") == 0){
						check = 1;
					}
				}
			}
			fclose(temp);
			fclose(kerdessor);
			remove(TEMP);
			
			printf("\nSikeres torles\n\n");
		}
		else if(command == 5){
			int len;
			char azon[SIZE];
			int db;
			char kerdes[SIZE];
			char valaszok[SIZE];
			char valasz[SIZE];
			char teljesvalasz[SIZE];
			char *file_contents;
			char *first;
			char *second;
			while(1){
				printf("Kerdes sorszama: ");
				fgets(azon, sizeof(azon), stdin);
				
				long input_file_size;
				FILE *input_file = fopen(KERDESSOR, "rb");
				fseek(input_file, 0, SEEK_END);
				input_file_size = ftell(input_file);
				rewind(input_file);
				file_contents = malloc(input_file_size * (sizeof(char)));
				fread(file_contents, sizeof(char), input_file_size, input_file);
				fclose(input_file);
				
				char word[] = "\n";
				char word2[] = "";
				len = strlen(azon);
				if (len > 0 && azon[len-1] == '\n')
				azon[len-1] = '\0';
				strcat(word, "Sorszam: ");
				strcat(word2, "Sorszam: ");
				strcat(word, azon);
				//strcat(word2, azon);
				strcat(word, "\n");
				//strcat(word2, "\n");
				first = word;
				second = word2;
				
				if(strstr(file_contents, word) != NULL) break;
			}
			
			const char *s = &(*file_contents);
			const char *PATTERN1 = &(*first);
			const char *PATTERN2 = "\n\n";
			char *target = NULL;
			char *start, *end;
			if ( (start = strstr( s, PATTERN1 )) )
			{
				start += strlen( PATTERN1 );
				if ( (end = strstr( start, PATTERN2 )) )
				{
					target = ( char * )malloc( end - start + 1 );
					memcpy( target, start, end - start );
					target[end - start] = '\0';
				}
			}
			free( target );
			
			FILE *kerdessor;
			FILE *temp;
			kerdessor = fopen(KERDESSOR, "r");
			temp = fopen(TEMP, "w");
			char sor[SIZE];
			while(fgets(sor, sizeof(sor), kerdessor) != NULL)
			{
				fputs(sor, temp);
			}
			fclose(kerdessor);
			fclose(temp);
			
			kerdessor = fopen(KERDESSOR, "w");
			temp = fopen(TEMP, "r");
			int check = 1;
			int check2 = 1;
			char text[SIZE] = "";
			strcat(text, second);
			
			while(fgets(sor, sizeof(sor), temp) != NULL)
			{
				if(check == 1){
					fputs(sor, kerdessor);
					if(strcmp(sor,text) == 0){
						check = 0;
					}
				} else {
					if(check2 == 1){
						printf("Uj kerdes: ");
						fputs(fgets(kerdes, sizeof(kerdes), stdin), kerdessor);
						
						while(1){
							printf("Valaszok szama: ");
							fgets(valaszok, sizeof(valaszok), stdin);
							db = atoi(valaszok);
							if(db > 1 && db < 5) break;
						}
						int i;
						for( i = 0; i < db; i = i + 1 ){
							printf("Uj valasz: ");
							fgets(valasz, sizeof(valasz), stdin);
							stpcpy(teljesvalasz, "- ");
							strcat(teljesvalasz, valasz);
							fputs(teljesvalasz, kerdessor);
						}
						/*time_t rawtime;
						struct tm * timeinfo;
						time ( &rawtime );
						timeinfo = localtime ( &rawtime );
						fprintf(write_file, "Felvetel ideje: %s", asctime(timeinfo));
						fprintf(write_file,"\n");*/
						check2 = 0;
					} else {
						if(sor[0]!='-'){
							fputs(sor, kerdessor);
							check = 1;
						}
					}
				}
			}
			fclose(temp);
			fclose(kerdessor);
			remove(TEMP);
			
			printf("\nSikeres modositas\n\n");
		}
		else if(command == 6){
			if( access(KERDESSOR, F_OK) != -1 ) {
				remove(KONZULTACIO);
			}
			char mennyi[SIZE];
			printf("Konzultacioban szereplo kerdesek szama: ");
			fgets(mennyi, sizeof(mennyi), stdin);
			int mennyidb = atoi(mennyi);
			int i;
			for( i = 0; i < mennyidb; i = i + 1 ){
				int len;
				char azon[SIZE];
				char *file_contents;
				char *first;
				char *second;
				while(1){
					printf("Kerdes sorszama: ");
					fgets(azon, sizeof(azon), stdin);
					
					long input_file_size;
					FILE *input_file = fopen(KERDESSOR, "rb");
					fseek(input_file, 0, SEEK_END);
					input_file_size = ftell(input_file);
					rewind(input_file);
					file_contents = malloc(input_file_size * (sizeof(char)));
					fread(file_contents, sizeof(char), input_file_size, input_file);
					fclose(input_file);
					
					char word[] = "\n";
					char word2[] = "";
					len = strlen(azon);
					if (len > 0 && azon[len-1] == '\n')
					azon[len-1] = '\0';
					strcat(word, "Sorszam: ");
					strcat(word2, "Sorszam: ");
					strcat(word, azon);
					//strcat(word2, azon);
					strcat(word, "\n");
					//strcat(word2, "\n");
					first = word;
					second = word2;
					
					if(strstr(file_contents, word) != NULL) break;
				}
				
				const char *s = &(*file_contents);
				const char *PATTERN1 = &(*first);
				const char *PATTERN2 = "\n\n";
				char *target = NULL;
				char *start, *end;
				if ( (start = strstr( s, PATTERN1 )) )
				{
					start += strlen( PATTERN1 );
					if ( (end = strstr( start, PATTERN2 )) )
					{
						target = ( char * )malloc( end - start + 1 );
						memcpy( target, start, end - start );
						target[end - start] = '\0';
					}
				}
				free( target );
				
				FILE *kerdessor;
				FILE *temp;
				kerdessor = fopen(KERDESSOR, "r");
				temp = fopen(TEMP, "w");
				char sor[SIZE];
				while(fgets(sor, sizeof(sor), kerdessor) != NULL)
				{
					fputs(sor, temp);
				}
				fclose(kerdessor);
				fclose(temp);
				
				kerdessor = fopen(KONZULTACIO, "a");
				temp = fopen(TEMP, "r");
				int check = 1;
				char text[SIZE] = "";
				strcat(text, second);
				
				while(fgets(sor, sizeof(sor), temp) != NULL)
				{
					if(check == 1){
						if(strcmp(sor,text) == 0){
							fputs(sor, kerdessor);
							check = 0;
						}
					} else {
						fputs(sor, kerdessor);
						if(strcmp(sor,"\n") == 0){
							check = 1;
						}
					}
				}
				fclose(temp);
				fclose(kerdessor);
				remove(TEMP);
			}
			printf("\nSikeres osszeallitas\n\n");
		}
		else if(command == 7){
			if( access(KONZULTACIO, F_OK) != -1 ) {
				int read_file;
				read_file = open(KONZULTACIO,O_RDONLY);
				char c;
				while (read(read_file,&c,sizeof(c))){ 
					printf("%c",c);
				} 
				printf("\n");
				close(read_file);
			} else {
				printf("\nJelenleg nincs konzultacios kerdoiv\n\n");
			}
		}
		else if(command == 8){
			if( access(KONZULTACIO, F_OK) == -1 ) {
				printf("\nJelenleg nincs konzultacios kerdoiv\n\n");
			} else {
				int kerdesek_szama = 0;
				FILE *read_file;
				read_file = fopen(KONZULTACIO, "r");
				char sor[SIZE];
				while(fgets(sor, sizeof(sor), read_file) != NULL)
				{
					if(strcmp(sor, "\n") == 0) kerdesek_szama = kerdesek_szama + 1;
				}
				fclose(read_file);
				//printf("Szam: %d\n", kerdesek_szama);
				if(kerdesek_szama < 3){
					printf("\nA konzultacios kerdoiv nem tartalmaz legalabb 3 kerdest\n\n");
				} else {
					srand(time(NULL));
					int r = 0;
		  
					char varos[SIZE];
					printf("Melyik varosba kuldjuk: ");
					fgets(varos, sizeof(varos), stdin);
		  
					int pipe1[2];
					int pipe2[2];
					char szoveg[1000];

					if (pipe(pipe1) == -1 || pipe(pipe2) == -1) 
					{
						perror("Hiba a pipe nyitaskor!");
						exit(EXIT_FAILURE);
					}
					
					signal(SIGTERM,handler);
					
					pid_t child=fork();
					if (child<0) 
					{
						perror("Fork hiba");
						exit(1);
					}
					if (child>0) //szülő
					{ 
						//várunk hogy megérkezzen a kérdezőbiztos
						pause();
						
						//elküldjük a kérdéseket
						printf("MLSZ kivalasztja a kerdeseket\n");
						close(pipe1[0]);
						int r1=rand()%kerdesek_szama;
						int r2=rand()%kerdesek_szama;
						while(1)
						{
							if(r2 == r1){
								r2=rand()%kerdesek_szama;
							} else {
								break;
							}
						}
						int r3=rand()%kerdesek_szama;
						while(1)
						{
							if(r3 == r1 || r3 == r2){
								r3=rand()%kerdesek_szama;
							} else {
								break;
							}
						}
						int hol = 0;
						int elso = 0;
						int masodik = 0;
						int harmadik = 0;
						strcpy(szoveg, "");
						read_file = fopen(KONZULTACIO, "r");
						while(fgets(sor, sizeof(sor), read_file) != NULL)
						{
							if(strcmp(sor, "\n") == 0){ 
								hol = hol + 1;
								elso = 0;
								masodik = 0;
								harmadik = 0;
							}
							if(hol == r1 || hol == r2 || hol == r3){
								if(elso == 1 && masodik == 1 && harmadik == 0){
									if(hol != 0) strcat(szoveg, sor);
									harmadik = 1;
								}
								if(elso == 1 && masodik == 0){
									if(hol == 0) strcat(szoveg, sor);
									masodik = 1;
								}
								if(elso == 0) elso = 1;
								if(sor[0] == '-') strcat(szoveg, sor);
							}
						}
						fclose(read_file);
						//printf("\n\n\n%s\n\n\n", szoveg);
						write(pipe1[1], &szoveg, sizeof(szoveg));
						close(pipe1[1]);
						printf("MLSZ elkuldte a kerdeseket\n");
						fflush(NULL);
						
						int status;
						waitpid(child,&status,0);		
						
						//kiírjuk a konzultáció eredményét
						printf("MLSZ megkapta az eredmenyt\n\n");
						close(pipe2[1]);
						read(pipe2[0],szoveg,sizeof(szoveg));
						printf("%s\n",szoveg);
						close(pipe2[0]);
					}
					else //gyerek
					{
						//jelzünk hogy megérkezett a kérdezőbiztos  
						kill(getppid(),SIGTERM); 
						
						//szülő feltölti
						sleep(3);	
						
						//kérdőív beolvasása
						printf("Kerdezobiztos megkapja a kerdeseket\n");
						close(pipe1[1]);
						read(pipe1[0],szoveg,sizeof(szoveg));
						//printf("\n\n\n%s\n\n\n",szoveg);
						close(pipe1[0]);
						
						//eredmény számítás
						FILE *temp;
						temp = fopen(TEMP, "w");
						fprintf(temp,"%s",szoveg);
						fclose(temp);
						
						r = rand()%10;
						r = r + 10;
						int elso = 0;
						int masodik = 0;
						int harmadik = 0;
						int hol = 0;
						
						temp = fopen(TEMP, "r");
						while(fgets(sor, sizeof(sor), temp) != NULL)
						{
							if(sor[0] != '-') hol = hol + 1;
							if(sor[0] == '-' && hol == 1) elso = elso + 1;
							if(sor[0] == '-' && hol == 2) masodik = masodik + 1;
							if(sor[0] == '-' && hol == 3) harmadik = harmadik + 1;
						}
						fclose(temp);
						//printf("%i %i %i\n",elso,masodik,harmadik);
						
						int a; int b; int c; int d; int y;
						int elso_hol = 0; int masodik_hol = 0; int harmadik_hol = 0;
						hol = 0; char buffer[SIZE];
						strcpy(szoveg, "");
						temp = fopen(TEMP, "r");
						while(fgets(sor, sizeof(sor), temp) != NULL)
						{
							if(sor[0] != '-'){
								a = 0; b = 0; c = 0; d = 0;
								strcat(szoveg, sor);
								hol = hol + 1;
								int x = 0;
								if(hol == 1) x = elso;
								if(hol == 2) x = masodik;
								if(hol == 3) x = harmadik;
								int i;
								for(i = 0; i < r; i++){
									y = rand()%x;
									if(y == 0) a = a + 1;
									if(y == 1) b = b + 1;
									if(y == 2) c = c + 1;
									if(y == 3) d = d + 1;
								}
								a = (float)a / (float)r * 100;
								b = (float)b / (float)r * 100;
								c = (float)c / (float)r * 100;
								d = (float)d / (float)r * 100;
							}
							if(sor[0] == '-' && hol == 1){
								elso_hol = elso_hol + 1;
								if(elso_hol == 1) sprintf(buffer, "%d", a);
								if(elso_hol == 2) sprintf(buffer, "%d", b);
								if(elso_hol == 3) sprintf(buffer, "%d", c);
								if(elso_hol == 4) sprintf(buffer, "%d", d);
								strcat(szoveg, buffer);
								strcat(szoveg, "% ");
								strcat(szoveg, sor);
							}
							if(sor[0] == '-' && hol == 2){
								masodik_hol = masodik_hol + 1;
								if(masodik_hol == 1) sprintf(buffer, "%d", a);
								if(masodik_hol == 2) sprintf(buffer, "%d", b);
								if(masodik_hol == 3) sprintf(buffer, "%d", c);
								if(masodik_hol == 4) sprintf(buffer, "%d", d);
								strcat(szoveg, buffer);
								strcat(szoveg, "% ");
								strcat(szoveg, sor);
							}
							if(sor[0] == '-' && hol == 3){
								harmadik_hol = harmadik_hol + 1;
								if(harmadik_hol == 1) sprintf(buffer, "%d", a);
								if(harmadik_hol == 2) sprintf(buffer, "%d", b);
								if(harmadik_hol == 3) sprintf(buffer, "%d", c);
								if(harmadik_hol == 4) sprintf(buffer, "%d", d);
								strcat(szoveg, buffer);
								strcat(szoveg, "% ");
								strcat(szoveg, sor);
							}
						}
						fclose(temp);
						remove(TEMP);
						
						//eredmények visszaküldése
						printf("Kerdezobiztos elkuldi az eredmenyt\n");
						close(pipe2[0]);
						write(pipe2[1], &szoveg, sizeof(szoveg));
						close(pipe2[1]);
							
						//befejezi a működését
						printf("Kerdezobiztos befejezi a konzultaciot\n");
						kill(getpid(),SIGKILL);
					}
				}
			}
		}
	}
	return 0;
}