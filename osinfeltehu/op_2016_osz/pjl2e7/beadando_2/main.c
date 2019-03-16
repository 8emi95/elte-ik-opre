#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <poll.h>
#include <mqueue.h>

#define MAXMAILLENGTH 50
#define MAXNAMELENGTH 50
#define MAXVISITORNUMBER 100
#define MAXEXHIBITIONNAMELENGTH 50
#define MAXEXHIBITIONNUMBER 50
#define MENUSIZE 9
#define MAXEXHIBITIONIDLENGTH 10
#define MAXVENUELENGTH 30

struct Visitor{
	char name[MAXNAMELENGTH];
	char mail[MAXMAILLENGTH];
	int exhib_id;
	time_t signup_time;
};

struct Exhibition{
	char exhibition_name[MAXEXHIBITIONNAMELENGTH];
	int exhib_id;
};

void handler(int signumber){
    printf("A jelzes megerkezett.\n");
}

void clear_input_stream(FILE *fp){
	char ch;
	while ((ch = fgetc(fp)) != EOF && ch != '\n') {
    }
}

int find_exhibition(struct Exhibition *exhibitions, int actual_exhibition_num, int exhib_id){
	int exhib_not_found = 1;
	int i;
	for(i = 0; (i < actual_exhibition_num) && (exhibitions[i].exhib_id != exhib_id); i++){
	}
	return i;
}

int find_visitor(struct Visitor *visitors, int visitor_number, char* name, char* mail, int exhib_id){
	int visitor_not_found = 1;
	int i;
	for(i = 0; i < visitor_number && (strcmp(visitors[i].name, name) || strcmp(visitors[i].mail, mail) || visitors[i].exhib_id != exhib_id); i++){
	}
	return i;
}

void remove_trailing_newline(char *word){
    char* newline_occurrence = strchr(word, '\n');
    if(newline_occurrence != NULL){
	   *newline_occurrence = '\0';
    }
}

void read_from_stdin(char** chrsequence, int maxlength){
    *chrsequence = (char*)malloc(maxlength * sizeof(char));
    fgets(*chrsequence, maxlength, stdin);
    int len = strlen(*chrsequence);
    if(len == maxlength - 1){
        clear_input_stream(stdin);
    }
    remove_trailing_newline(*chrsequence);
    
}

void read_visitor_data(int* exhib_id, char** name, char** mail){
	printf("Adja meg a rendezveny azonositojat:");
	char *exhib_id_chr;
    read_from_stdin(&exhib_id_chr, MAXEXHIBITIONIDLENGTH);
    *exhib_id = atoi(exhib_id_chr);
	printf("Adja meg a nevet:");
	read_from_stdin(name, MAXNAMELENGTH);
    printf("Adja meg az e-mail cimet:");
    read_from_stdin(mail, MAXMAILLENGTH);
}

int remove_visitor(struct Visitor* visitors, int* actual_visitor_num, int exhib_id, char* name, char* mail){
	int j = find_visitor(visitors, *actual_visitor_num, name, mail, exhib_id);
	if(j < *actual_visitor_num){
		for(;(j < *actual_visitor_num - 1) && (j < MAXVISITORNUMBER-1); j++){
			visitors[j] = visitors[j+1];
		}
		(*actual_visitor_num)--;
		return 1;
	}else{
		return 0;
	}
}

int visitor_number(struct Visitor* visitors, int exhib_id, int actual_visitor_num){
	int i;
	int count = 0;
	for(i = 0; i < actual_visitor_num; i++){
		if(visitors[i].exhib_id == exhib_id){
			count++;
		}
	}
	return count;
}

int prev_max_exhib_id(struct Exhibition*  exhibitions, int actual_exhibition_num){
	int max_exhib_id = 0;
	int i;
	for(i = 0; i < actual_exhibition_num; i++){
		if(exhibitions[i].exhib_id > max_exhib_id){
			max_exhib_id = exhibitions[i].exhib_id;
		}
	}
	return max_exhib_id;
}



int main(int argc, char** argv){
	char exhibitions_filename[] = "exhibitions.data";
	char visitors_filename[] = "visitors.data";
	struct Exhibition exhibitions[MAXEXHIBITIONNUMBER];
	struct Visitor visitors[MAXVISITORNUMBER];
	int actual_exhibition_num;
	int actual_visitor_num;
	int max_exhib_id;
	char* menunumber_char;
	int menunumber;
	char *endptr;

	actual_visitor_num = 0;
	actual_exhibition_num = 0;
	max_exhib_id = 1;
	
	int exhibitions_file;
	int visitors_file;
	
	FILE *fexhibitons_file;
	FILE *fvisitors_file;
  
	if(access(exhibitions_filename, R_OK) == 0){
		fexhibitons_file = fopen(exhibitions_filename ,"r");
		if(fexhibitons_file == NULL){
			perror("Nem sikerult megnyitni a kiallitasok adatait tartalmazo fajlt! A program folytatja a mukodest beolvasott adatok nelkul.\n");
		}else{
			int read_bytes = fread(&actual_exhibition_num, sizeof(int), 1, fexhibitons_file);
			if(read_bytes != 1 || fread(exhibitions, sizeof(struct Exhibition), actual_exhibition_num, fexhibitons_file) != actual_exhibition_num){
				perror("Hiba tortent a kiallitasok adatait tartalmazo fajl olvasasa kozben. A program leall.\n");
				exit(1);
			}
			fclose(fexhibitons_file);
			printf("A rendezvenyek adatainak beolvasasa sikeres volt!\n");

			if(access(visitors_filename, R_OK) == 0){
				fvisitors_file = fopen(visitors_filename, "r");
				if(fvisitors_file == NULL){
					perror("Nem sikerult megnyitni a latogatok adatait tartalmazo fajlt! A program folytatja a mukodest a latogatokra vonatkozo beolvasott adatok nelkul.\n");
				}else{
					int read_bytes = fread(&actual_visitor_num, sizeof(int), 1, fvisitors_file);
					if(read_bytes != 1 || fread(visitors, sizeof(struct Visitor), actual_visitor_num, fvisitors_file) != actual_visitor_num){
						perror("Hiba tortent a latogatok adatait tartalmazo fajl olvasasa kozben. A program leall.\n");
						exit(1);
					}
					fclose(fvisitors_file);
					printf("A latogatok adatainak beolvasasa sikeres volt!\n");
				}
			}else{
				perror("Nem sikerult elerni a latogatok adatait tartalmazo fajlt! A program folytatja a mukodest a latogatokra vonatkozo adatok nelkul.\n");
			}
		}
	}else{
		perror("Nem sikerult elerni a kiallitasok adatait tartalmazo fajlt! A program folytatja a mukodest beolvasott adatok nelkul.\n");
	}
	
	max_exhib_id = 1 + prev_max_exhib_id(exhibitions, actual_exhibition_num);
	  
	do{
		printf("\n0. Kilepes \n1. Jelentkezes \n2. Jelentkezettek listazasa \n3. Vendeg adatainak modositasa \n4. Vendeg torlese \n5. Uj rendezveny inditasa \n6. Rendezvenyek listazasa \n7. Rendezveny torlese \n8. Rendezveny elinditasa\n");

        read_from_stdin(&menunumber_char, 80);
		menunumber = strtol(menunumber_char, &endptr, 0);
		if(endptr != menunumber_char && menunumber >= 0 && menunumber < MENUSIZE){
			switch(menunumber){
				case 1:{
					int exhib_id;
					char* name;
					char* mail;
					read_visitor_data(&exhib_id, &name, &mail);
					if(find_exhibition(exhibitions, actual_exhibition_num, exhib_id) >= actual_exhibition_num){
						printf("A megadott rendezveny nem talalhato!\n");
					}else if(find_visitor(visitors, actual_visitor_num, name, mail, exhib_id) < actual_visitor_num){
						printf("Ezekkel az adatokkal mar jelentkezett valaki erre a rendezvenyre!\n");
					}else{
						strcpy(visitors[actual_visitor_num].name, name);
						strcpy(visitors[actual_visitor_num].mail, mail);
						visitors[actual_visitor_num].signup_time = time(0);
						visitors[actual_visitor_num].exhib_id = exhib_id;
						actual_visitor_num++;
						printf("Sikeres jelentkezes! Jelentkezes sorszama: %i\n", visitor_number(visitors, exhib_id, actual_visitor_num));
					}
					free(name);
					free(mail);
				}
				break;
				case 2:
				{
					printf("Jelentkezett vendegek:\n");
					int i;
                    printf("Kiallitas azonosito | Kiallitas neve | Latogato neve | Latogato e-mail cime | Jelentkezes ideje\n");
					for(i = 0; i < actual_visitor_num; i++){
						int j = find_exhibition(exhibitions, actual_exhibition_num, visitors[i].exhib_id);
						char buff[100];
						strftime(buff, 100, "%Y-%m-%d %H:%M:%S.000", localtime(&visitors[i].signup_time));
						printf("%i | %s | %s | %s | %s\n", visitors[i].exhib_id, exhibitions[j].exhibition_name, visitors[i].name, visitors[i].mail, buff);
					}
				}
				break;
				case 3:
				{
					int old_exhib_id;
					char* old_name;
					char* old_mail;
					read_visitor_data(&old_exhib_id, &old_name, &old_mail);
					int j = find_visitor(visitors, actual_visitor_num, old_name, old_mail, old_exhib_id);
					if(j < actual_visitor_num){
						int new_exhib_id;
						char* new_name;
						char* new_mail;
						read_visitor_data(&new_exhib_id, &new_name, &new_mail);
						int k = find_visitor(visitors, actual_visitor_num, new_name, new_mail, new_exhib_id);
						if(k < actual_visitor_num){
							printf("A megadott adatokkal valaki mar jelentkezett. Nem lett vegrehatjva a modositas.\n");
						}else if(find_exhibition(exhibitions, actual_exhibition_num, new_exhib_id) >= actual_exhibition_num){
							printf("Nincs a megadott azonositoval rendelkezo rendezveny. Nem lett vegrehajtva a modositas.\n");
						}
						else{
							strcpy(visitors[j].name, new_name);
							strcpy(visitors[j].mail, new_mail);
							visitors[j].exhib_id = new_exhib_id;
							printf("A valtoztatasok megtortentek.\n");
						}
						free(old_name);
						free(old_mail);
						free(new_name);
						free(new_mail);
					}else{
						printf("Nem jelentkezett vendeg a megadott adatokkal.");
					}
				}
				break;
				case 4:
				{
					int exhib_id;
					char* name;
					char* mail;
					read_visitor_data(&exhib_id, &name, &mail);
					if(remove_visitor(visitors, &actual_visitor_num, exhib_id, name, mail)){
						printf("A megadott felhasznalo torolve lett.");
					}else{
						printf("Nem jelentkezett vendeg a megadott adatokkal.");
					}
					free(name);
					free(mail);
				}
				break;
				case 5:
				{
					if(actual_exhibition_num < MAXEXHIBITIONNUMBER){
						char *temp_exhibition_name;
						printf("Adja meg az uj rendezveny nevet:");
                        read_from_stdin(&temp_exhibition_name, MAXEXHIBITIONNAMELENGTH);
						strcpy(exhibitions[actual_exhibition_num].exhibition_name, temp_exhibition_name);
						exhibitions[actual_exhibition_num].exhib_id = max_exhib_id;
						actual_exhibition_num++;
						max_exhib_id++;
						free(temp_exhibition_name);
                        printf("A rendezveny fel lett veve, azonositoja: %i\n", max_exhib_id - 1);
					}else{
						printf("Nem lehet tobb rendezvenyt felvenni!\n");
					}
				}
				break;
				case 6:
				{
					printf("Eddigi rendezvenyek:\n");
                    printf("Rendezveny azonositoja | Rendezveny neve\n");
					int i;
					for(i = 0; i < actual_exhibition_num; i++){
						printf("%i | %s\n", exhibitions[i].exhib_id, exhibitions[i].exhibition_name);
					}
				}
				break;
				case 7:
				{
					printf("Adja meg a torlendo rendezveny azonositojat:");
					char *exhib_id_chr;
                    read_from_stdin(&exhib_id_chr, MAXEXHIBITIONIDLENGTH);
					int exhib_id = atoi(exhib_id_chr);
					int i = find_exhibition(exhibitions, actual_exhibition_num, exhib_id);
					if(i < actual_exhibition_num){
						int j;
						for(j = 0; j < actual_visitor_num; j++){
							if(visitors[j].exhib_id == exhibitions[i].exhib_id){
								if(remove_visitor(visitors, &actual_visitor_num, visitors[j].exhib_id, visitors[j].name, visitors[j].mail)){
									j--;
								};
							}
						}
						for(;(i < actual_exhibition_num - 1) && (i < MAXEXHIBITIONNUMBER-1); i++){
							exhibitions[i] = exhibitions[i+1];
						}
						actual_exhibition_num--;
                        printf("A megadott rendezveny es a hozza tartozo latogatok torolve lettek.\n");
					}else{
						printf("\nNem talalhato a megadott rendezveny!\n");
					}
					free(exhib_id_chr);
				}	
				break;
                case 8:
                {
                    printf("Adja meg a lebonyolitando esemeny azonositojat:");
                    char *exhib_id_chr;
                    read_from_stdin(&exhib_id_chr, MAXEXHIBITIONIDLENGTH);
					int exhib_id = atoi(exhib_id_chr);
					int i = find_exhibition(exhibitions, actual_exhibition_num, exhib_id);

					if(i < actual_exhibition_num){
                        int pipefd[2];
                        if(pipe(pipefd) == -1){
                            printf("Nem sikerult megnyitni a csovet.\n");
                            continue;
                        }
                        char* mqname = "/kingofstandspjl2e7";
                        struct mq_attr attr;
                        mqd_t mq;
                        attr.mq_maxmsg = 5;
                        attr.mq_msgsize = sizeof(struct Visitor);
                        
                        mq_unlink(mqname);
                        mq = mq_open(mqname, O_CREAT|O_RDWR, S_IRUSR | S_IWUSR, &attr);
                        
                        if(mq < 0){
                            printf("Nem sikerult letrehozni az uzenetsort.\n");
                            continue;
                        }
                        //char data[100];
						pid_t child = fork();
                        if(child < 0){
                            printf("Nem sikerult letrehozni a gyerekfolyamatot.\n");
                            continue;
                        }else
                        if(child > 0){//parent process
                            
                        
                            struct sigaction sigact;
                            sigact.sa_handler = handler;
                            sigemptyset(&sigact.sa_mask);
                            sigact.sa_flags = 0;
                            sigaction(SIGUSR1, &sigact, NULL);
                            
                            sigset_t sigset;
                            sigfillset(&sigset);
                            sigdelset(&sigset, SIGUSR1);
                        
                            close(pipefd[0]);
                            char *exhib_venue;
                            printf("\nAdja meg a lebonyolitando esemeny helyszinet:");
                            read_from_stdin(&exhib_venue, MAXVENUELENGTH);
                            write(pipefd[1], exhib_venue, strlen(exhib_venue));
                            
                            sigsuspend(&sigset);
                            
                            int i;
                            for(i = 0; i < actual_visitor_num; i++){
                                if(visitors[i].exhib_id == exhib_id){
                                    write(pipefd[1], &visitors[i], sizeof(struct Visitor));
                                }
                            }
                            
                            close(pipefd[1]);
                            
                            int success_rate[sizeof(struct Visitor)];
                            
                            struct timespec timeout;
                            
                            clock_gettime(CLOCK_REALTIME, &timeout);
                            
                            timeout.tv_sec += 10;
                            
                            if(mq_timedreceive(mq, (char*)&success_rate, sizeof(int)*sizeof(struct Visitor), NULL, &timeout) >= 0){
                                printf("A rendezveny sikeressege %i\%\n", success_rate[0]);
                            }else{
                                perror("Az uzenet nem erkezett meg.");
                                kill(child, SIGTERM);
                                continue;
                            };
                            
                            clock_gettime(CLOCK_REALTIME, &timeout);
                            timeout.tv_sec += 10;
                            struct Visitor didnt_show_up;
                            printf("Akik nem mentek el a rendezvenyre:\n");
                            int receivedbytes;
                            int stopwaiting = 0;
                            while(!stopwaiting && (receivedbytes = mq_timedreceive(mq, (char*)&didnt_show_up, sizeof(struct Visitor), NULL, &timeout))){
                                if(receivedbytes < sizeof(struct Visitor)){
                                    stopwaiting = 1;                                    
                                }else{
                                    printf("%s %s\n", didnt_show_up.name, didnt_show_up.mail);
                                    clock_gettime(CLOCK_REALTIME, &timeout);
                                    timeout.tv_sec += 10;
                                }
                            }
                            
                            int status;
                            wait(&status);
                            
                            mq_close(mq);
                            mq_unlink(mqname);
                            
                            printf("A partner befejezte a munkat.\n");
                        }else{//child process
                            char exhib_venue[MAXVENUELENGTH];
                            close(pipefd[1]);
                            read(pipefd[0], exhib_venue, MAXVENUELENGTH);
                            printf("A partner megkapta a helyszint: %s\n", exhib_venue);
                            kill(getppid(), SIGUSR1);
                            
                            struct Visitor exhib_visitors[MAXVISITORNUMBER];
                            int visitors_of_exhibition = 0;
                            
                            //struct pollfd poll_fds[1];
                            //poll_fds[0].fd = pipefd[0];
                            //poll_fds[0].events=POLLIN;
                            
                            //while(read(pipefd[0], &exhib_visitors[i], sizeof(struct Visitor))){
                              //  printf("Latogato: %s\n",exhib_visitors[i].name);
                            //}
                            int readbytes;
                            do{
                                visitors_of_exhibition++;
                                readbytes = read(pipefd[0], &exhib_visitors[visitors_of_exhibition-1], sizeof(struct Visitor));
                            }while(readbytes == sizeof(struct Visitor));
                            if(readbytes < 0){
                                perror("Hiba a csobol olvasasnal.");
                                continue;
                            }
                            visitors_of_exhibition--;
                            int i = 0;
                            printf("Latogatok:\n");
                            printf("Latogato neve | Latogato e-mail cime | Jelentkezes ideje\n");
                            for(;i < visitors_of_exhibition; i++){
                                char buff[100];
                                strftime(buff, 100, "%Y-%m-%d %H:%M:%S.000", localtime(&exhib_visitors[i].signup_time));
                                printf("%s | %s | %s\n", exhib_visitors[i].name, exhib_visitors[i].mail, buff);
                            }
                            
                            printf("A partner lebonyolitja az esemenyt.\n");
                            
                            
                            srand(getpid());
                            
                            int r = rand() % 100;
                            //printf("Random szam a gyerekben: %i\n", r);
                            
                            mq_send(mq, (char *)&r, sizeof(int), 1);
                            
                            for(i = 0; i < visitors_of_exhibition; i++){
                                if(rand() % 100 < 10){
                                    mq_send(mq, (char *)&exhib_visitors[i], sizeof(struct Visitor), 1);
                                }
                            }
                            
                            mq_send(mq, (char *)"", 0, 1);
                            
                            mq_close(mq);
                            close(pipefd[0]);
                            _exit(0);
                        }
					}else{
						printf("\nNem talalhato a megadott rendezveny!\n");
					}
					free(exhib_id_chr);
                    
                }
                break;
			}
		}else{
			printf("Hibas bemenet! \n");
			menunumber = -1;
		}
        free(menunumber_char);
	}while(menunumber != 0);
	
	fexhibitons_file = fopen(exhibitions_filename, "w+");
	if(fexhibitons_file == NULL){
		perror("Nem sikerult megnyitni a kiallitasok kimeneti fajljat.\n");
	}else{
		int written_bytes = fwrite(&actual_exhibition_num, sizeof(int), 1, fexhibitons_file);
		if(written_bytes != 1 || fwrite(exhibitions, sizeof(struct Exhibition), actual_exhibition_num, fexhibitons_file) != actual_exhibition_num){
			perror("Nem sikerult kiirni a kiallitasok adatait.\n");
		}else{
			fvisitors_file = fopen(visitors_filename, "w+");
			if(fvisitors_file == NULL){
				perror("Nem sikerult megnyitni a latogatok kimeneti fajljat.\n");
			}else{
				int written_bytes = fwrite(&actual_visitor_num, sizeof(int), 1, fvisitors_file);
				if(written_bytes != 1 || fwrite(visitors, sizeof(struct Visitor), actual_visitor_num, fvisitors_file) != actual_visitor_num){
					perror("Nem sikerult kiirni a kiallitasok adatait.\n");
				}else{
					printf("Sikeres kiiras.\n");
				}
			}
		}
		fclose(fexhibitons_file);
	}
	
	return 0;
}