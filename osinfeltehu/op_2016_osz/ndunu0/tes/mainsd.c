#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <signal.h>

#define DATA_SIZE 512
#define MAX_GUESTS 50
#define MAX_EVENTS 100


struct guest {
    char* name;
    char* mail;
    int reg_time;
};

struct event {
	char* name;
	int event_id;
	struct guest guests[MAX_GUESTS];
	int num_guests;
};

void signal_handler(int sing_number) {
	//printf("\nSignal with number %i has arrived\n", sing_number);
}

void print_menu() {
    printf("\t\t Menü \t\t\n");
    printf("\t-----------------\n");
    printf("\t0. Kilépés\n");
    printf("\t1. Új Vendég\n");
    printf("\t2. Vendég módosítása\n");
    printf("\t3. Vendég eltávolítása\n");
    printf("\t4. Események listázása\n");
    printf("\t5. Új esemény\n");
    printf("\t6. Esemény eltávolítása\n");
    printf("\t7. Fájlba mentés\n");
    printf("\t8. Események lebonyolítása\n");
}

char* get_line(char* print) {
    char* tmp = malloc(sizeof(char) * 128);
    printf(print);
    fflush(stdin);
    scanf("%s", tmp);
    return tmp;
}

void list_events_guest(struct event* e, const int num_events) {
    if(num_events != 0) {
        int i;
        for(i = 0; i < num_events; i++) {
            printf("\t %d. %s -- %d\n", i, e[i].name, e[i].event_id);
            int j;
            for(j = 0; j < e[i].num_guests; j++) {
                printf("\t\t%d. %s, %s, %ld\n", j, e[i].guests[j].name, e[i].guests[j].mail, e[i].guests[j].reg_time);
            }
            printf("\t\tVendégek száma: %d\n", e[i].num_guests);
        }
        printf("\n");
    } else printf("Nincs esemény!!!\n");
}

void list_events(struct event* e, int num_events) {
    if(num_events != 0) {
        int i;
        for(i = 0; i < num_events; i++) {
            printf("\t %d. %s -- %d\n", i+1, e[i].name, e[i].event_id);
        }
        printf("\n");
     }
}

struct event* remove_event(struct event* e, int num_events) {
	struct event *t_event = (struct event*) malloc((num_events - 1) * sizeof(struct event));
	
	if(num_events > 0){
		fflush(stdin);
		printf("Event ID: ");
		int selected_event;
		scanf("%d", &selected_event);
		selected_event--;
		if(selected_event < num_events){
			int i;
			int newi = 0;
			for(i = 0; i < num_events; i++){
				if(selected_event != i){
					t_event[newi].name = e[i].name;
					t_event[newi].event_id = newi+1;
					int j;
					for(j = 0; j < e[i].num_guests; j++){
						t_event[newi].guests[j].name = e[i].guests[j].name;
						t_event[newi].guests[j].mail = e[i].guests[j].mail;
						t_event[newi].guests[j].reg_time = e[i].guests[j].reg_time;
					}
					newi++;
				}
			}
		}else {
			printf("Rossz esemény ID!(%d)\n",selected_event);
			return NULL;
		}
		
	} else printf("Nincs elérhető esemény!\n");
	return t_event;
}

void write_file(const char* fname, struct event* e, int num_events) {
	int file = open(fname, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    if(file < 0) {
        printf("Hiba a fájl megnyitása közben(%s)\n", fname);
        exit(1);
    }
    int i;
    for(i = 0; i < num_events; i++) {
        write(file, &e[i].name, strlen(e[i].name));
        write(file, ", ", sizeof(char));
        write(file, &e[i].event_id, sizeof(e[i].event_id));
        write(file, "\n", sizeof(char));
        fflush(stdout);
        int j;
        for(j = 0; j < e[i].num_guests; j++) {
            write(file, "\t", sizeof(char));
            write(file, &e[i].guests[j].name, strlen(e[i].guests[j].name));
            write(file, ", ", sizeof(char));
            write(file, &e[i].guests[j].mail, strlen(e[i].guests[j].mail));
            write(file, ", ", sizeof(char));
            write(file, &e[i].guests[j].reg_time, sizeof(e[i].guests[j].reg_time));
            write(file, "\n", sizeof(char));
        }
    }
    printf("Sikeres Fájlba írás!(%s)\n", fname);
    close(file);
}

void new_guest(struct event* e, int num_events) {
    int selected_event;
    fflush(stdin);
    printf("Esemény ID: ");
    scanf("%d", &selected_event);
	selected_event--;
	
	if(selected_event < num_events) {
		int db = e[selected_event].num_guests;
		e[selected_event].guests[db].name = get_line("Vendég neve: ");
		e[selected_event].guests[db].mail = get_line("Mail: ");
		e[selected_event].guests[db].reg_time = rand()%100;
		e[selected_event].num_guests++;
	} else printf("Nincs ilyen esemény!\n");
}

void list_guest(struct event* e, int selected) {
    int i;
    for(i = 0; i < e[selected].num_guests; i++) {
        printf("\t%d. %s, %s, %ld\n", i, e[selected].guests[i].name, e[selected].guests[i].mail, e[selected].guests[i].reg_time);
    }
}

void modify_guest(struct event* e, int num_events) {
    int selected_event;
	fflush(stdin);
	printf("Esemény ID: ");
	scanf("%d", &selected_event);
	selected_event--;
	
    if(e[selected_event].num_guests != 0) {
        printf("Kiválasztott esemény: %s\n", e[selected_event].name);
        list_events(e, selected_event);
        
		list_guest(e,selected_event);
		printf("Vendég száma: ");
        int guest_number;
		fflush(stdin);
		scanf("%d", &guest_number);
		
        printf("Kiválasztott vendég: %s\n", e[selected_event].guests[guest_number].name);
        e[selected_event].guests[guest_number].name = get_line("Vendég neve: ");
        e[selected_event].guests[guest_number].mail = get_line("Mail: ");
        printf("Sikeres módosítás!\n");
    } else printf("Üres vendéglista!\n");
}

void remove_guest(struct event* e) {
	int p;
	int selected_event;
	fflush(stdin);
	printf("Esemény ID: ");
	scanf("%d", &selected_event);
	selected_event--;
	
    if(e[selected_event].num_guests != 0) {
        printf("Kiválasztott esemény: %s\n", e[selected_event].name);
        list_guest(e, selected_event);
        printf("Vendég száma: ");
        int guest_number;
		fflush(stdin);
		scanf("%d", &guest_number);
		
        int i = guest_number + 1;
        while(i <= e[selected_event].num_guests) {
            e[selected_event].guests[i - 1] = e[selected_event].guests[i];
            i++;
        }
        e[selected_event].num_guests--;
    } else printf("Üres vendég lista!\n");
}

int main() {
    char select_op = ' ';
	int num_events = 0;
	struct event *events = (struct event*) malloc(MAX_EVENTS * sizeof(struct event));
	struct event *tmp;
	
	int r_h;
	int pipefd[2];
	int pipefd2[2];
	key_t key;
	pid_t pid_id;
	char helyszin[500] = "";
	
	if(pipe(pipefd) == -1) {
		perror("Pipe open error!");
		return 1;
	}
	
	if(pipe(pipefd2) == -1) {
		perror("Pipe open error!");
		return 1;
	}
	
	signal(SIGTERM, signal_handler);

    print_menu();
	
    while(select_op != '0') {
        scanf("%c", &select_op);
		switch(select_op) {
            case '1':
				list_events(events, num_events);
                new_guest(events, num_events);
				print_menu();
            break;
			case '2':
				list_events(events, num_events);
				modify_guest(events, num_events);
				print_menu();
			break;
			case '3':
				list_events(events, num_events);
				remove_guest(events);
				print_menu();
			break;
			case '4':
				list_events_guest(events,num_events);
				print_menu();
			break;
			case '5':
				events[num_events].name = get_line("Esemény neve: ");
				events[num_events].event_id = num_events + 1;
				num_events++;
				print_menu();
			break;
			case '6':
				list_events(events, num_events);
				tmp = remove_event(events, num_events);
                if(tmp != NULL) {
                    events = tmp;
                    num_events--;
                }
				print_menu();
			break;
			case '7':
				write_file("out.txt",events,num_events);
				print_menu();
			break;
			case '8':
				if(num_events > 0){
					pid_id = fork();
					if(pid_id == -1) {
						perror("Fork error!");
						return 1;
					}
					
					if(pid_id > 0) { //parent
						printf("Jelenlegi rendezvény: %s\n",events[0].name);
						close(pipefd[0]); //read off
						close(pipefd2[1]); //write2 off
						
						write(pipefd[1], events[0].name, strlen(events[0].name));
						pause();
						
						r_h = read(pipefd2[0], helyszin, sizeof(helyszin));
						kill(pid_id, SIGTERM);
						if(r_h < 0) {
							perror("parent fail");
							exit(1);
						} else if(r_h == 0) {
							printf("parent eof\n");
						} else {
							printf("%s\n",helyszin);
						}
						sleep(1);
						kill(pid_id, SIGTERM);
						printf("Mennek a vendegek\n");
						
						int i;
						for(i=0; i < events[0].num_guests; i++){
							write(pipefd[1], events[0].guests[i].name, strlen(events[0].guests[i].name));
							pause();
						}
						
						sleep(2);
						
						wait();
						print_menu();
						
					} else if(pid_id == 0){ //child
						close(pipefd[1]); //write off
						close(pipefd2[0]); //read2 off
						r_h = read(pipefd[0], helyszin, sizeof(helyszin));
						kill(getppid(), SIGTERM);
						if(r_h < 0){
							perror("child fail");
						} else if(r_h == 0){
							printf("child eof\n");
						} else {
							printf("Ide jottem(partner):%s\n",helyszin);
							strcpy(helyszin,"Partner a helyszinen");
							//printf("kuld(child):%s\n",helyszin);
							write(pipefd2[1], helyszin, strlen(helyszin));
							pause();
						}
						sleep(2);
						kill(getppid(), SIGTERM);
						
						
						int j;
						for(j=0; j < events[0].num_guests; j++){
							r_h = read(pipefd[0], helyszin, sizeof(helyszin));
							kill(getppid(), SIGTERM);
							if(r_h < 0){
								perror("child fail");
							} else if(r_h == 0){
								printf("child eof\n");
							} else {
								printf("%s\n",helyszin);
							}
							
						}
						
						exit(0);
					}
				}else
					printf("Nincs elkövetkező esemény!");
				
				
            break;			
        }
    }

    return 0;
}
