#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#define SIZE 64
#define RENDEZVENY "rendezveny.txt"
#define TEMP "temp.txt"
#define PIPE "npipe"

void handler(int signumber){}

int main()
{
	while(1){
		printf("1 - Uj rendezveny\n");
		printf("2 - Rendezvenyek listazasa\n");
		printf("3 - Rendezveny torlese\n");
		printf("4 - Jelentkezes rendezvenyre\n");
		printf("5 - Jelentkezettek listazasa\n");
		printf("6 - Vendeg adatainak modositasa\n");
		printf("7 - Vendeg torlese\n");
		printf("8 - Esemeny lebonyolitasa\n");
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
			FILE *write_file;
			int new_file;
			int len;
			char c;
			char azon[SIZE];
			char r_nev[SIZE];
			if( access(RENDEZVENY, F_OK) != -1 ) {
				while(2){
					printf("Uj rendezveny azonositoszama: ");
					fgets(azon, sizeof(azon), stdin);
					char sent[] = "";
					int sent_file = open(RENDEZVENY, O_RDONLY);
					while (read(sent_file,&c,sizeof(c))){
						strcat(sent, (char[2]) { (char) c, '\0' });
					}
					close(sent_file);
					char word[] = "\n";
					len = strlen(azon);
					if (len > 0 && azon[len-1] == '\n')
					azon[len-1] = '\0';
					strcat(word, azon);
					strcat(word, " ");
					if(strstr(sent, word) == NULL) break;
				}
				printf("Uj rendezveny neve: ");
				fgets(r_nev, sizeof(r_nev), stdin);
				
				write_file = fopen(RENDEZVENY, "a+");
				len = strlen(r_nev);
				if (len > 0 && r_nev[len-1] == '\n')
				r_nev[len-1] = '\0';
				fprintf(write_file,"%s %s\n", azon, r_nev);
				fclose(write_file);
				
				char new_file_name[15];
				stpcpy(new_file_name, azon);
				strcat(new_file_name, ".txt");
				new_file = open(new_file_name,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
				write(new_file,"1\n",2*sizeof(c));
				close(new_file);
			} else {
				printf("Uj rendezveny azonositoszama: ");
				fgets(azon, sizeof(azon), stdin);
				printf("Uj rendezveny neve: ");
				fgets(r_nev, sizeof(r_nev), stdin);
				
				write_file = fopen(RENDEZVENY, "w");
				len = strlen(azon);
				if (len > 0 && azon[len-1] == '\n')
				azon[len-1] = '\0';
				len = strlen(r_nev);
				if (len > 0 && r_nev[len-1] == '\n')
				r_nev[len-1] = '\0';
				fprintf(write_file,"\n%s %s\n", azon, r_nev);
				fclose(write_file);
				
				char new_file_name[SIZE];
				stpcpy(new_file_name, azon);
				strcat(new_file_name, ".txt");
				new_file = open(new_file_name,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
				write(new_file,"1\n",2*sizeof(c));
				close(new_file);
			}
			printf("\nAz uj rendezvenyt felvettuk\n\n");
		}
		else if(command == 2){
			if( access(RENDEZVENY, F_OK) != -1 ) {
				int read_file;
				read_file = open(RENDEZVENY,O_RDONLY);
				char c;
				while (read(read_file,&c,sizeof(c))){ 
					printf("%c",c);
				} 
				printf("\n");
				close(read_file);
			} else {
				printf("\nJelenleg nincs egy rendezveny sem\n\n");
			}
		}
		else if(command == 3){
			char c;
			char azon[SIZE];
			char r_azon[SIZE];
			char r_nev[SIZE];
			while(3){
				printf("Torlendo rendezveny azonositoszama: ");
				fgets(azon, sizeof(azon), stdin);
				char sent[] = "";
				int sent_file = open(RENDEZVENY, O_RDONLY);
				while (read(sent_file,&c,sizeof(c))){
					strcat(sent, (char[2]) { (char) c, '\0' });
				}
				close(sent_file);
				char word[] = "\n";
				int len = strlen(azon);
				if (len > 0 && azon[len-1] == '\n')
				azon[len-1] = '\0';
				strcat(word, azon);
				strcat(word, " ");
				if(strstr(sent, word) != NULL) break;
			}
			
			FILE *rendezveny;
			FILE *temp;
			rendezveny = fopen(RENDEZVENY, "r");
			temp = fopen(TEMP, "w");
			char sor[SIZE];
			while(fgets(sor, sizeof(sor), rendezveny) != NULL)
			{
				sscanf(sor, "%s %s", r_azon, r_nev);
				if (strcmp(azon, r_azon) != 0)
				{
					fputs(sor, temp);
				}
			}
			fclose(rendezveny);
			fclose(temp);
			
			rendezveny = fopen(RENDEZVENY, "w");
			temp = fopen(TEMP, "r");
			int elso = 1;
			while(fgets(sor, sizeof(sor), temp) != NULL)
			{
				sscanf(sor, "%s %s", r_azon, r_nev);
				if(elso == 1){
					fprintf(rendezveny, "\n");
					elso = 0;
				} else {
					fprintf(rendezveny, "%s %s\n", r_azon, r_nev);
				}
			}
			fclose(temp);
			fclose(rendezveny);
			remove(TEMP);
			
			char delete_file_name[SIZE];
			stpcpy(delete_file_name, azon);
			strcat(delete_file_name, ".txt");
			remove(delete_file_name); 
			printf("\nSikeres torles\n\n");
		}
		else if(command == 4){
			char c;
			char azon[SIZE];
			char vnev[SIZE];
			char knev[SIZE];
			char email[SIZE];
			int  len;
			while(4){
				printf("Rendezveny azonositoszama: ");
				fgets(azon, sizeof(azon), stdin);
				char sent[] = "";
				int sent_file = open(RENDEZVENY, O_RDONLY);
				while (read(sent_file,&c,sizeof(c))){
					strcat(sent, (char[2]) { (char) c, '\0' });
				}
				close(sent_file);
				char word[] = "\n";
				len = strlen(azon);
				if (len > 0 && azon[len-1] == '\n')
				azon[len-1] = '\0';
				strcat(word, azon);
				strcat(word, " ");
				if(strstr(sent, word) != NULL) break;
			}
			printf("Jelentkezo vezetek neve: ");
			fgets(vnev, sizeof(vnev), stdin);
			len = strlen(vnev);
			if (len > 0 && vnev[len-1] == '\n')
			vnev[len-1] = '\0';
			printf("Jelentkezo kereszt neve: ");
			fgets(knev, sizeof(knev), stdin);
			len = strlen(knev);
			if (len > 0 && knev[len-1] == '\n')
			knev[len-1] = '\0';
			printf("Jelentkezo email cime: ");
			fgets(email, sizeof(email), stdin);
			len = strlen(email);
			if (len > 0 && email[len-1] == '\n')
			email[len-1] = '\0';
			
			FILE *lista;
			char lista_file_name[SIZE];
			stpcpy(lista_file_name, azon);
			strcat(lista_file_name, ".txt");
			lista = fopen(lista_file_name, "r");
			char sorszamarray[SIZE];
			fgets(sorszamarray, sizeof(sorszamarray), lista);
			int sorszam;
			sscanf(sorszamarray, "%d", &sorszam);
			fclose(lista);
			
			lista = fopen(lista_file_name,"r+");
			int uj_sorszam = sorszam + 1;
			fprintf(lista, "%d\n", uj_sorszam);
			fclose(lista);
			
			time_t rawtime;
			struct tm * timeinfo;
			time ( &rawtime );
			timeinfo = localtime ( &rawtime );
			lista = fopen(lista_file_name, "a+");
			fprintf(lista, "%d %s %s %s %s", sorszam, vnev, knev, email, asctime(timeinfo));
			fclose(lista);
			printf("\nUj tag azonosito: %d\n\n", sorszam);
		}
		else if(command == 5){
			char c;
			char azon[SIZE];
			while(5){
				printf("Rendezveny azonositoszama: ");
				fgets(azon, sizeof(azon), stdin);
				char sent[] = "";
				int sent_file = open(RENDEZVENY, O_RDONLY);
				while (read(sent_file,&c,sizeof(c))){
					strcat(sent, (char[2]) { (char) c, '\0' });
				}
				close(sent_file);
				char word[] = "\n";
				int len = strlen(azon);
				if (len > 0 && azon[len-1] == '\n')
				azon[len-1] = '\0';
				strcat(word, azon);
				strcat(word, " ");
				if(strstr(sent, word) != NULL) break;
			}
			
			char lista_file_name[SIZE];
			stpcpy(lista_file_name, azon);
			strcat(lista_file_name, ".txt");
			
			int read_file;
			read_file = open(lista_file_name, O_RDONLY);
			printf("\n");
			while (read(read_file,&c,sizeof(c))){ 
				printf("%c",c);
			} 
			printf("\n");
			close(read_file);
		}
		else if(command == 6){
			int len;
			char c;
			char azon[SIZE];
			while(6){
				printf("Rendezveny azonositoszama: ");
				fgets(azon, sizeof(azon), stdin);
				char sent[] = "";
				int sent_file = open(RENDEZVENY, O_RDONLY);
				while (read(sent_file,&c,sizeof(c))){
					strcat(sent, (char[2]) { (char) c, '\0' });
				}
				close(sent_file);
				char word[] = "\n";
				len = strlen(azon);
				if (len > 0 && azon[len-1] == '\n')
				azon[len-1] = '\0';
				strcat(word, azon);
				strcat(word, " ");
				if(strstr(sent, word) != NULL) break;
			}
			
			char lista_file_name[SIZE];
			stpcpy(lista_file_name, azon);
			strcat(lista_file_name, ".txt");
			
			char id[SIZE];
			while(9){
				printf("Modositando jelentkezo sorszama: ");
				fgets(id, sizeof(id), stdin);
				char sent[] = "";
				int sent_file = open(lista_file_name, O_RDONLY);
				while (read(sent_file,&c,sizeof(c))){
					strcat(sent, (char[2]) { (char) c, '\0' });
				}
				close(sent_file);
				char word[] = "\n";
				len = strlen(id);
				if (len > 0 && id[len-1] == '\n')
				id[len-1] = '\0';
				strcat(word, id);
				strcat(word, " ");
				if(strstr(sent, word) != NULL) break;
			}
			
			char uvnev[SIZE]; char uknev[SIZE]; char uemail[SIZE];
			printf("Modositando vezetek neve: ");
			fgets(uvnev, sizeof(uvnev), stdin);
			len = strlen(uvnev);
			if (len > 0 && uvnev[len-1] == '\n')
			uvnev[len-1] = '\0';
			printf("Modositando kereszt neve: ");
			fgets(uknev, sizeof(uknev), stdin);
			len = strlen(uknev);
			if (len > 0 && uknev[len-1] == '\n')
			uknev[len-1] = '\0';
			printf("Modositando email cime: ");
			fgets(uemail, sizeof(uemail), stdin);
			len = strlen(uemail);
			if (len > 0 && uemail[len-1] == '\n')
			uemail[len-1] = '\0';
			
			FILE *lista;
			FILE *temp;
			lista = fopen(lista_file_name, "r");
			temp = fopen(TEMP, "w");
			char sor[SIZE];
			char sorszam[SIZE];
			char email[SIZE];
			char vnev[SIZE]; char knev[SIZE]; char het[SIZE]; char honap[SIZE]; char nap[SIZE]; char ido[SIZE]; char ev[SIZE];
			while(fgets(sor, sizeof(sor), lista) != NULL)
			{
				sscanf(sor, "%s %s %s %s %s %s %s %s %s", sorszam, vnev, knev, email, het, honap, nap, ido, ev);
				if (strcmp(id, sorszam) != 0)
				{
					fputs(sor, temp);
				}
				else if(strcmp(id, sorszam) == 0)
				{
					fprintf(temp, "%s %s %s %s %s %s %s %s %s\n", sorszam, uvnev, uknev, uemail, het, honap, nap, ido, ev);
				}
			}
			fclose(lista);
			fclose(temp);
			
			lista = fopen(lista_file_name, "w");
			temp = fopen(TEMP, "r");
			int elso = 1;
			while(fgets(sor, sizeof(sor), temp) != NULL)
			{
				sscanf(sor, "%s %s %s %s %s %s %s %s %s", sorszam, vnev, knev, email, het, honap, nap, ido, ev);				
				if(elso == 1){
					fprintf(lista, "%s\n", sorszam);
					elso = 0;
				} else {
					fprintf(lista, "%s %s %s %s %s %s %s %s %s\n", sorszam, vnev, knev, email, het, honap, nap, ido, ev);
				}
			}
			fclose(temp);
			fclose(lista);
			remove(TEMP);
			printf("\nSikeres modositas\n\n");
		}
		else if(command == 7){
			char c;
			char azon[SIZE];
			while(7){
				printf("Rendezveny azonositoszama: ");
				fgets(azon, sizeof(azon), stdin);
				char sent[] = "";
				int sent_file = open(RENDEZVENY, O_RDONLY);
				while (read(sent_file,&c,sizeof(c))){
					strcat(sent, (char[2]) { (char) c, '\0' });
				}
				close(sent_file);
				char word[] = "\n";
				int len = strlen(azon);
				if (len > 0 && azon[len-1] == '\n')
				azon[len-1] = '\0';
				strcat(word, azon);
				strcat(word, " ");
				if(strstr(sent, word) != NULL) break;
			}
			
			char lista_file_name[SIZE];
			stpcpy(lista_file_name, azon);
			strcat(lista_file_name, ".txt");
			
			char id[SIZE];
			while(8){
				printf("Torlendo jelentkezo sorszama: ");
				fgets(id, sizeof(id), stdin);
				char sent[] = "";
				int sent_file = open(lista_file_name, O_RDONLY);
				while (read(sent_file,&c,sizeof(c))){
					strcat(sent, (char[2]) { (char) c, '\0' });
				}
				close(sent_file);
				char word[] = "\n";
				int len = strlen(id);
				if (len > 0 && id[len-1] == '\n')
				id[len-1] = '\0';
				strcat(word, id);
				strcat(word, " ");
				if(strstr(sent, word) != NULL) break;
			}
			
			FILE *lista;
			FILE *temp;
			lista = fopen(lista_file_name, "r");
			temp = fopen(TEMP, "w");
			char sor[SIZE];
			char sorszam[SIZE];
			char email[SIZE];
			char vnev[SIZE]; char knev[SIZE]; char het[SIZE]; char honap[SIZE]; char nap[SIZE]; char ido[SIZE]; char ev[SIZE];
			while(fgets(sor, sizeof(sor), lista) != NULL)
			{
				sscanf(sor, "%s %s %s %s %s %s %s %s %s", sorszam, vnev, knev, email, het, honap, nap, ido, ev);
				if (strcmp(id, sorszam) != 0)
				{
					fputs(sor, temp);
				}
			}
			fclose(lista);
			fclose(temp);
			
			lista = fopen(lista_file_name, "w");
			temp = fopen(TEMP, "r");
			int elso = 1;
			while(fgets(sor, sizeof(sor), temp) != NULL)
			{
				sscanf(sor, "%s %s %s %s %s %s %s %s %s", sorszam, vnev, knev, email, het, honap, nap, ido, ev);				
				if(elso == 1){
					fprintf(lista, "%s\n", sorszam);
					elso = 0;
				} else {
					fprintf(lista, "%s %s %s %s %s %s %s %s %s\n", sorszam, vnev, knev, email, het, honap, nap, ido, ev);
				}
			}
			fclose(temp);
			fclose(lista);
			remove(TEMP);
			printf("\nSikeres torles\n\n");
		}
		else if(command == 8){
			char c;
			char azon[SIZE];
			int len;
			while(1){
				printf("Rendezveny azonositoszama: ");
				fgets(azon, sizeof(azon), stdin);
				char sent[] = "";
				int sent_file = open(RENDEZVENY, O_RDONLY);
				while (read(sent_file,&c,sizeof(c))){
					strcat(sent, (char[2]) { (char) c, '\0' });
				}
				close(sent_file);
				char word[] = "\n";
				len = strlen(azon);
				if (len > 0 && azon[len-1] == '\n')
				azon[len-1] = '\0';
				strcat(word, azon);
				strcat(word, " ");
				if(strstr(sent, word) != NULL) break;
			}
			
			char lista_file_name[SIZE];
			stpcpy(lista_file_name, azon);
			strcat(lista_file_name, ".txt");
			
			int f=mkfifo(PIPE,0600);			
			if (f<0){perror("error");exit(1);}
		 
			struct timeval tv;
			tv.tv_sec=5;
			tv.tv_usec=0;
			
			f=open(PIPE,O_RDWR);
			fd_set watchedpipes_read, copy_watchedpipes_read;
			FD_ZERO(&watchedpipes_read);
			FD_SET(f,&watchedpipes_read);
			copy_watchedpipes_read=watchedpipes_read;
			
			signal(SIGUSR1,handler);
			pid_t parent=getpid();
			
			pid_t  child=fork();
			if (child<0){perror("The fork calling was not succesful\n"); printf("Hiba!\n");exit(1);}
			if (child>0){
				//2. nevek kuldese partnernek 
				pause();
				FILE *lista;
				lista = fopen(lista_file_name, "r");
				fseek(lista,sizeof(int),SEEK_CUR);
				char sor[SIZE]; char sorszam[SIZE]; char email[SIZE]; char vnev[SIZE]; char knev[SIZE]; char het[SIZE]; char honap[SIZE]; char nap[SIZE]; char ido[SIZE]; char ev[SIZE];
				int elso = 0;
				while(fgets(sor, sizeof(sor), lista) != NULL)
				{
					if(elso == 0){
						sscanf(sor, "%s %s %s %s %s %s %s %s", vnev, knev, email, het, honap, nap, ido, ev);
						elso = 1;
					} else {
						sscanf(sor, "%s %s %s %s %s %s %s %s %s", sorszam, vnev, knev, email, het, honap, nap, ido, ev);
					}
					char teljes_nev[SIZE];
					stpcpy(teljes_nev, vnev);
					strcat(teljes_nev, " ");
					strcat(teljes_nev, knev);
					//printf("kuldi: %s \n", teljes_nev);
					write(f,teljes_nev,sizeof(teljes_nev));
				}				
				
				int status;
				waitpid(child,&status,0); 
				
				//6. jelentes
				select(f+1,&watchedpipes_read,NULL,NULL,NULL);
				int success;
				char nev[SIZE];
				if (FD_ISSET(f,&watchedpipes_read))
				{
					read(f,&success,sizeof(int));
					while(2){
						read(f,&nev,sizeof(nev));
						if(strcmp(nev, "VEGE") == 0) break;
						printf("Nem jott el: %s\n", nev);
					}
				}
				printf("Rendezveny sikere: %d szazalek lett\n\n",success);
				fflush(stdout);
				fclose(lista);
				
				unlink(PIPE);
			} else {
				char nevek[SIZE][SIZE];
				int index=0;
				//1. partner elindul
				printf("\nPartner elindult (5 sec)\n");
				sleep(5);
				printf("Partner megerkezett a helyszinre\n");
				kill(parent,SIGUSR1);
				
				//3. resztvevok atvetele
				for(;;)
				{
					tv.tv_sec = 5;
					tv.tv_usec = 0;
					watchedpipes_read=copy_watchedpipes_read;
					//int result=
					select(f+1,&watchedpipes_read,NULL,NULL,&tv);
					if (FD_ISSET(f,&watchedpipes_read)) 
				    { 
						read(f,&nevek[index],SIZE);
						printf("Resztevot varjuk: %s\n", nevek[index]);
						index++;				   
					} 
					else
					{
						break;
					}
				}
				
				//4. rendezveny
				printf("Rendezveny elindult (5 sec)\n");
				sleep(5);
				printf("Rendezveny veget ert\n");
				
				//5. tajekoztato
				srand(time(NULL));
				int r=rand()%100;
				write(f,&r,sizeof(r));	
				int i;
				for(i=0;i<index;i++)
				{
					r = rand()%10;
					if(r==0) write(f,&nevek[i],SIZE);		
				}
				char vege[SIZE];
				stpcpy(vege, "VEGE");
				write(f,&vege,sizeof(vege));

				kill(getpid(),SIGKILL);			
			}
		}
	}
	return 0;
}