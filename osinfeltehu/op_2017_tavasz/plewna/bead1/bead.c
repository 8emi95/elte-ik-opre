#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#define SIZE 64
#define KERDESSOR "kerdessor.txt"
#define KONZULTACIO "konzultacio.txt"
#define TEMP "temp.txt"

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
		printf("8 - Kilepes\n");
		printf("Kerem valasszon: ");
		char commandarray[5];
		fgets(commandarray, sizeof(commandarray), stdin);
		int command;
		sscanf(commandarray, "%d", &command);
		if(command == 8){
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
	}
	return 0;
}