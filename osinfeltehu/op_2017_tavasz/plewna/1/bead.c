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
			free( target );
			
			FILE *kerdessor;
			FILE *temp;
			kerdessor = fopen(KERDESSOR, "r");
			temp = fopen(TEMP, "w");
			char sor[SIZE];
			while(fgets(sor, sizeof(sor), kerdessor) != NULL)
			{
				while(sscanf("%s", sor)==1){
					fputs(sor, temp);
				}
				/*sscanf(sor, "%s %s", r_azon, r_nev);
				if (strcmp(azon, r_azon) != 0)
				{
					fputs(sor, temp);
				}*/
			}
			fclose(kerdessor);
			fclose(temp);
			
			/*kerdessor = fopen(KERDESSOR, "w");
			temp = fopen(TEMP, "r");
			int elso = 1;
			while(fgets(sor, sizeof(sor), temp) != NULL)
			{
				sscanf(sor, "%s %s", r_azon, r_nev);
				if(elso == 1){
					fprintf(kerdessor, "\n");
					elso = 0;
				} else {
					fprintf(kerdessor, "%s %s\n", r_azon, r_nev);
				}
			}
			fclose(temp);
			fclose(kerdessor);
			remove(TEMP);*/
			
			//char file[SIZE];
			//stpcpy(file, file_contents);
			/*char *file = "ebbol asdasdasdasd vedd ki";
			const char *removeText = "asdasdasdasd";
			printf( "%s\n", file );
			while( (file=strstr(file,removeText)) ) memmove(file,file+strlen(removeText),1+strlen(file+strlen(removeText)));
			printf( "%s\n", file );*/
			
			/*remove(KERDESSOR);
			FILE *write_file = fopen(KERDESSOR, "ab");
			if (write_file != NULL)
			{
				fputs(file_contents, write_file);
				fclose(write_file);
			}*/
		}
		else if(command == 5){
			
		}
		else if(command == 6){
			
		}
		else if(command == 7){
			
		}
	}
	return 0;
}