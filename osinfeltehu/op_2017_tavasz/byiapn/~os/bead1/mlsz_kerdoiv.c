/* FELADAT
Kerdessor: 2-4 valasz/kerdes
Funkciok:
 - uj kerdes es datum+ido
 - kerdes modositas/torles: valasztas teljes kerdes/egyes valaszok kozott
 - listazas
 - veglegesites
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

struct question {
	int _id;
	char* _que;
	char* _ans;
	char* _time;
};

static const int LINE_MAX = 80;

static const char* input_err = "Nem megfelelo input.";
static const char* nfound_err = "A megadott elem nem talalhato.";
static const char* lsempty_err = "A lista ures.";

//struct fuggvenyek
void printque(struct question);
void copyque(struct question, struct question);

//struct lista fuggvenyek
int new(struct question*, int);
int edit(struct question*, int);
int delete(struct question*, int, int, int);
void list(struct question*, int);
void final(struct question*, int);
void writeqls(struct question*, int, char*);

//egyeb fuggvenyek
void help(const char**, const char**);

//onallo fuggvenyek
char* acttime(void);
int readint(void);
void readnempty(char*, char*);
int countocc(char, char*);
void copyinto(char*, char*, int, int);

void main(void) {
	const char* cmds[6] = {"new", "edit", "list", "final", "help", "quit"};
	const char* dscr[6] = {"- Uj kerdes felvetele", "- Kerdes szerkesztese/torlese", "- Kerdesek listazasa", "- Vegleges kerdoiv eloallitasa", "- Parancsok listazasa", "- Kilepes"};
	
	struct question quels[1000]; //beviteli lista
	int size = 0; //lista merete es az elso szabad rekord pozicioja

	printf("===== MLSZ kerdoiv =====\n");
	help(cmds, dscr);
	printf("=========================\n");

	char* line = (char*)malloc(LINE_MAX * sizeof(char));

	for(;;) {
		readnempty(line, "MLSZ>");
		
		line[strlen(line)-1] = '\0';

		int i = 0;
		while(i < 6 && strcmp(line,cmds[i]) != 0) {
			i++;
		}
		
		if(i == 5) {
			break;
		} else {
			if(i < 5) { printf("%s\n\n", dscr[i] + 2); }
			switch(i) {
				case 0:
					size = new(quels, size);
					break;
				case 1:
					size = edit(quels, size);
					break;
				case 2:
					list(quels, size);
					break;
				case 3:
					final(quels, size);
					break;
				case 4:
					help(cmds, dscr);
					printf("\n");
					break;
				default:
					printf("Ismeretlen parancs. Hasznalja a %s parancsot a parancsok listazasahoz.\n", cmds[4]);
					break;
			}
		}
	}

	free(line);
}

//struct fuggvenyek
void printque(struct question q) {
	printf("%d. %s%sHozzaadva: %s\n", q._id + 1, q._que, q._ans, q._time);
}

void copyque(struct question q, struct question other) {
	strcpy(q._que, other._que);
	strcpy(q._ans, other._ans);
	strcpy(q._time, other._time);
}

//struct lista fuggvenyek
int new(struct question* quels, int size) {
	char* line = (char*)malloc(LINE_MAX * sizeof(char));

	char* que = (char*)malloc((LINE_MAX + 1) * sizeof(char));
	char* ans = (char*)malloc((4 * LINE_MAX + 1) * sizeof(char));
	char* tme = (char*)malloc((24 + 1) * sizeof(char));

	printf("Adja meg a kerdest:\n");
	readnempty(que, "");

	int sv;
	int i = 0;
	while(i < 4) {
		if(line[0] != '\n') { printf("Adja meg a(z) %d. valaszlehetoseget:\n", i + 1); }
		if(i == 2) { printf("Ha nem szeretne tobb valaszlehetoseget megadni, nyomjon egy entert.\n"); }
		fgets(line, LINE_MAX, stdin);

		if(line[0] != '\n') {
			sv = i * LINE_MAX;
			copyinto(ans, line, sv, sv + LINE_MAX);
			ans[sv + LINE_MAX - 1] = '\n';
			ans[sv + LINE_MAX] = '\0';
			i++;
		} else if(i >= 2) {
			break;
		}
	}

	strcpy(tme, acttime());
	struct question q = {size, que, ans, tme};
	quels[size] = q; size++;

	free(line);

	return size;
}

int edit(struct question* quels, int size) {
	printf("Adja meg a kerdes sorszamat:\n");
	int _id = readint();

	if(_id != -1) {
		_id--;
		if(0 <= _id && _id < size) {
			printf("\nA modositasra kivalasztott kerdes:\n");
			printque(quels[_id]);
			
			char* line = (char*)malloc(LINE_MAX * sizeof(char));
			int sv; int anssize;

			printf("Valassza ki a modositani kivant elemet:\n[0 : a kerdes modositasa | 1..4 : az adott valasz modositasa]\n");
			readnempty(line, "");

			sv = line[0] - '0';
			if(line[1] == '\n' && 0 <= sv && sv <= 4) {
				anssize = countocc('\n', quels[_id]._ans);

				if(sv <= anssize) {
					printf("\nTorleshez hasznalja a delete parancsot.\n");
					readnempty(line, "");
					
					if(strcmp(line, "delete\n") != 0) {
						if(sv == 0) {
							strcpy(quels[_id]._que, line);
						} else {
							int sv2 = (sv - 1) * LINE_MAX;
							copyinto(quels[_id]._ans, line, sv2, sv2 + LINE_MAX);
							quels[_id]._ans[sv2 + LINE_MAX - 1] = '\n';
						}
					} else {
						size = delete(quels, size, _id, sv);
					}
				} else { 
					printf("%s\n", nfound_err);
				}
			} else {
				printf("%s\n", input_err);
			}

			free(line);

		} else { printf("%s\n", nfound_err); }
	} else { printf("%s\n", input_err); }

	return size;
}

int delete(struct question* quels, int size, int _id, int tag) {
	if(tag == 0) {
		size--;

		int i = _id;
		while(i < size) {
			copyque(quels[i], quels[i + 1]);
			i++;
		}

		free(quels[size]._que);
		free(quels[size]._ans);
		free(quels[size]._time);
	} else {
		int anssize = countocc('\n', quels[_id]._ans);
		if(2 < anssize) {
			int sv = (tag - 1) * LINE_MAX;
			if(tag != anssize) {
				strcpy(quels[_id]._ans + sv, quels[_id]._ans + sv + LINE_MAX);
			} else {
				quels[_id]._ans[sv] = '\0';
			}
		} else {
			printf("A valasz nem torolheto. Legalabb 2 valaszlehetosegnek szerepelnie kell.\n");
		}
	}

	return size;
}

void list(struct question* quels, int size) {
	if(0 < size) {
		int i = 0;
		while(i < size) {
			printque(quels[i]);
			i++;
		}
	} else {
		printf("%s\n", lsempty_err);
	}
}

void final(struct question* quels, int size) {
	if(0 < size) {
		struct question fnqls[1000];
		int fnsize = 0;

		char* line = (char*)malloc(LINE_MAX * sizeof(char));

		int i = 0;
		int j = -1;
		while(i < size) {
			if(0 < i) { printf("\n"); }
			if(j != i) { 
				printque(quels[i]);
				printf("Veglegesiti? y - igen, n - nem, c - megse\n");
			}

			readnempty(line, "");

			if(line[1] == '\n' && (line[0] == 'y' || line[0] == 'n' || line[0] == 'c')) {
				if(line[0] == 'y') {
					char* que = (char*)malloc((LINE_MAX + 1) * sizeof(char));
					char* ans = (char*)malloc((4 * LINE_MAX + 1) * sizeof(char));

					struct question q = {fnsize, que, ans, acttime()};
					copyque(q, quels[i]);

					fnqls[fnsize] = q;
					fnsize++;
				} else if(line[0] == 'c') {
					break;
				}
				i++;
			} else {
				printf("%s\n", input_err);
				j = i;
			}
		}

		if(line[0] != 'c' && 0 < fnsize) {
				printf("\nKerdoiv mentese...");
				writeqls(fnqls, fnsize, "output.bin");
				printf("kesz\n");
		}
	} else {
		printf("%s\n", lsempty_err);
	}
}

void writeqls(struct question* quels, int size, char* fname) {
    char* loc = (char*)malloc(1024 * sizeof(char));
    
    getcwd(loc, sizeof(loc));
    strcat(loc, fname);

	int f;
	if(access(loc, F_OK) == -1){
		f = open(loc, O_CREAT | O_WRONLY, S_IWUSR);
	} else {
    	f = open(loc, O_WRONLY);
	}
    
    int i = 0;
    while(i < size) {
        write(f, &quels[i]._id, sizeof(int));
        write(f, quels[i]._que, (LINE_MAX + 1) * sizeof(char));
        write(f, quels[i]._ans, (4 * LINE_MAX + 1) * sizeof(char));
        write(f, quels[i]._time, 25 * sizeof(char));
        i++;
    }

    close(f);
}

//egyeb fuggvenyek
void help(const char** cmds, const char** dscr) {
	int i = 0;
	while(i < 6) {
		printf("%s %s\n", cmds[i], dscr[i]);
		i++;
	}
}

//onallo fuggvenyek
char* acttime(void) {
	time_t rawtime;
	char* formtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	formtime = asctime(timeinfo);

	return formtime;
}

int readint(void) {
	char* line = (char*)malloc(LINE_MAX * sizeof(char));
	int num = 0; int err = 0;

	readnempty(line, "");

	if(line[0] == '0') {
		err++;
	} else {
		int sv; int sv2 = 1;
		int i = strlen(line) - 2;

		while(i >= 0 && !err) {
			sv = line[i] - '0';
			if(0 < sv && sv < 10) {
				num += sv * sv2;
			} else if(sv != 0) {err++;}
			i--; sv2 = sv2 * 10;
		}
	}

	if(err) {
		num = -1;
	}
	
	free(line);

	return num;
}

void readnempty(char* line, char* prefix) {
	line[0] = '\n';
	while(line[0] == '\n' || line[0] == '\0') {
		printf("%s", prefix);
		fgets(line, LINE_MAX, stdin);
	}
}

int countocc(char sample, char* str) {
	int c = 0;
	
	int i = 0;
	while(i < strlen(str)) {
		if(str[i] == sample) {
			c++;
		}
		i++;
	}

	return c;
}

void copyinto(char* into, char* from, int begin, int end) {
	int i = begin;
	int j = 0;
	while(i < end) {
		if(from[j] != '\0' && from[j] != '\n') {
			into[i] = from[j];
			j++;
		} else {
			into[i] = ' ';
		}
		i++;
	}
}