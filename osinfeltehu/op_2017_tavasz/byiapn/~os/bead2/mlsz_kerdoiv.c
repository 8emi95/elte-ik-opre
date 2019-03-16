#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

struct question {
    int _id;
    char* _que;
    char* _ans;
    char* _time;
};

static const int LINE_MAX = 80;

static const int LEN_QUE = 81; //(LINE_MAX + 1)
static const int LEN_ANS = 321; //(4 * LINE_MAX + 1)
static const int LEN_TME = 81;

static const char* FIFO = "mlsz.pipe";

static const char* mkfifo_err = "Hiba a cso letrehozasakor!";
static const char* noresp_err = "A kerdezobiztos nem valaszol!";
static const char* input_err = "Nem megfelelo input.";
static const char* nfound_err = "A megadott elem nem talalhato.";
static const char* lsempty_err = "A lista ures.";
static const char* ansdel_err = "A valasz nem torolheto. Valaszok minimalis szama: 2";
static const char* fread_err = "Nem sikerult a fajlt beolvasni.";
static const char* nocity_err = "Nincs megadva varos.";
static const char* lessqss_err = "Nincs eleg kerdes.";
static const char* unexpected_err = "Varatlan hiba keletkezett.";

//parent process fofuggvenye
void menu();

//struct question fuggvenyek
struct question createque(char*, int);
void writeque(struct question, int);
void printque(struct question);
void copyque(struct question, struct question);
int getanssize(struct question);

//struct question lista fuggvenyek
int new(struct question*, int);
int edit_menu(struct question*, int);
void edit(struct question*, int, int, int, const char*);
int delete(struct question*, int, int, int);
void list(struct question*, int);
int final(struct question*, int);
int readqls(struct question*, const char*);
void writeqls(struct question*, int, const char*);

//egyeb parent process fuggvenyek
void communicate(int);
void help(const char**, const char**);

//child process fofuggvenye
void askppl(const char*);

//onallo fuggvenyek (kesobb felhasznalhatoak lehetnek)
char* acttime();
int getrandom(int);
int readint();
void readnempty(char*, const char*);
int countocc(const char, const char*);
void copyintoline(char*, const char*, const int, const int);
void copyintotext(char*, const char*, const int, const int);

void main(int argc, char* argv[]) {
    char* city = (char*)malloc(LINE_MAX * sizeof(char));
    
    if(argc > 1) {
        strcpy(city, argv[1]);
    } else {
        printf("%s\n", nocity_err);
        printf("%s\n", "Adja meg a varost!");
        readnempty(city, "");
        printf("\n");
    }
    
    if(access(FIFO, F_OK) != -1) {
        unlink(FIFO);
    }
    
    int fd = mkfifo(FIFO, S_IRUSR | S_IWUSR);
    if(fd >= 0) {
        pid_t child = fork();
        if(child > 0) {
            int status;
            menu();
//            communicate(0);
            waitpid(child, &status, 0);
            unlink(FIFO);
            printf("szulo vege.\n");
        } else {
            askppl(city);
            printf("gyerek vege.\n");
        }
    } else {
        printf("%s\n", mkfifo_err);
    }
    free(city);
}

//parent process fofuggvenye
void menu() {
    const char* cmds[6] = {"new", "edit", "list", "final", "help", "quit"};
    const char* dscr[6] = {"- Uj kerdes felvetele", "- Kerdes szerkesztese/torlese", "- Kerdesek listazasa",
                                      "- Vegleges kerdoiv eloallitasa", "- Parancsok listazasa", "- Kilepes"};
    
    struct question quels[1000];
    int size = 0;

    printf("===== MLSZ kerdoiv =====\n");
    help(cmds, dscr);
    printf("=========================\n");

    char* line = (char*)malloc(LINE_MAX * sizeof(char));
    
    int exit = 0;
    while(exit == 0) {
        readnempty(line, "MLSZ>");

        line[strlen(line)-1] = '\0';

        int i = 0;
        while(i < 6 && strcmp(line,cmds[i]) != 0) {
            i++;
        }

        if(i == 5) {
            exit = 1;
        } else {
            if(i < 5) { printf("%s\n\n", dscr[i] + 2); }
            switch(i) {
                case 0:
                    size = new(quels, size);
                    break;
                case 1:
                    size = edit_menu(quels, size);
                    break;
                case 2:
                    list(quels, size);
                    break;
                case 3:
                    if(final(quels, size) == 0) { exit = 2; }
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
    
    while(size > 0) {
        size = delete(quels, size, 0, 0);
    }
    
    communicate(exit);
}

//struct fuggvenyek
struct question createque(char* str, int id) {
    int anssize = countocc('\n', str) - 4;
    
    char* que = (char*)malloc(LEN_QUE * sizeof(char));
    char* ans = (char*)malloc(LEN_ANS * sizeof(char));
    char* tme = (char*)malloc(LEN_TME * sizeof(char));
    
    copyintoline(que, str, 0, LEN_QUE);
    que[LINE_MAX - 1] = '\n';
    que[LINE_MAX] = '\0';
    while((unsigned char)*str != '\n') str++; str++;
    
    int sv;
    int i = 0;
    while(i < anssize) {
        sv = i * LINE_MAX;
        copyintoline(ans, str, sv, sv + LINE_MAX);
        ans[sv + LINE_MAX - 1] = '\n';
        ans[sv + LINE_MAX] = '\0';
        while((unsigned char)*str != '\n') str++; str++;
        i++;
    }
    while((unsigned char)*str != '\n') str++; str++;
    
    copyintoline(tme, str, 0, LEN_TME);
    tme[LINE_MAX - 1] = '\n';
    tme[LINE_MAX] = '\0';
    
    struct question q = {id, que, ans, tme};
    return q;
}

void writeque(struct question q, int f) {
    char tmp[LEN_ANS];
    copyintotext(tmp, q._ans, 0, LEN_ANS);
    write(f, q._que, (LEN_QUE - 1) * sizeof(char));
    write(f, tmp, (LEN_ANS - 1) * sizeof(char));
    write(f, "\n", sizeof(char));
    write(f, q._time, (LEN_TME - 1)* sizeof(char));
    write(f, "\n", sizeof(char));
}

void printque(struct question q) {
    printf("%d. kerdes:\n%sValaszok:\n%sHozzaadva:\n%s\n", q._id + 1, q._que, q._ans, q._time);
}

void copyque(struct question q, struct question other) {
    strcpy(q._que, other._que);
    strcpy(q._ans, other._ans);
    strcpy(q._time, other._time);
}

int getanssize(struct question q) {
    return countocc('\n', q._ans);
}

//struct lista fuggvenyek
int new(struct question* quels, int size) {
    char* line = (char*)malloc(LINE_MAX * sizeof(char));

    char* que = (char*)malloc(LEN_QUE * sizeof(char));
    char* ans = (char*)malloc(LEN_ANS * sizeof(char));
    char* tme = (char*)malloc(LEN_TME * sizeof(char));

    printf("Adja meg a kerdest:\n");
    readnempty(line, "");
    
    copyintoline(que, line, 0, LINE_MAX);
    que[LINE_MAX - 1] = '\n';
    que[LINE_MAX] = '\0';

    int sv;
    int i = 0;
    while(i < 4) {
        if(line[0] != '\n') { printf("Adja meg a(z) %d. valaszlehetoseget:\n", i + 1); }
        if(i == 2) { printf("Ha nem szeretne tobb valaszlehetoseget megadni, nyomjon egy entert.\n"); }
        fgets(line, LINE_MAX, stdin);

        if(line[0] != '\n') {
            sv = i * LINE_MAX;
            
            copyintoline(ans, line, sv, sv + LINE_MAX);
            ans[sv + LINE_MAX - 1] = '\n';
            ans[sv + LINE_MAX] = '\0';
            
            i++;
        } else if(i >= 2) {
            break;
        }
    }

    copyintoline(tme, acttime(), 0, LINE_MAX);
    tme[LINE_MAX - 1] = '\n';
    tme[LINE_MAX] = '\0';
    
    struct question q = {size, que, ans, tme};
    quels[size] = q; size++;

    free(line);

    return size;
}

int edit_menu(struct question* quels, int size) {
    printf("Adja meg a kerdes sorszamat:\n");
    int id = readint();

    if(id != -1) {
        id--;
        if(0 <= id && id < size) {
            printf("\nA modositasra kivalasztott kerdes:\n");
            printque(quels[id]);

            char* line = (char*)malloc(LINE_MAX * sizeof(char));
            int tag; int anssize;

            printf("Valassza ki a modositani kivant elemet:\n[0 : a kerdes modositasa | 1..4 : az adott valasz modositasa]\n");
            readnempty(line, "");

            tag = line[0] - '0';
            if(line[1] == '\n' && 0 <= tag && tag <= 4) {
                anssize = getanssize(quels[id]);

                if(tag <= anssize) {
                    printf("\nTorleshez hasznalja a delete parancsot.\n");
                    readnempty(line, "");

                    if(strcmp(line, "delete\n") != 0) {
                        edit(quels, size, id, tag, line);
                    } else {
                        size = delete(quels, size, id, tag);
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

void edit(struct question* quels, int size, int id, int tag, const char* line) {
    if(tag == 0) {
        copyintoline(quels[id]._que, line, 0, LINE_MAX);
        quels[id]._que[LINE_MAX - 1] = '\n';
    } else {
        int sv = (tag - 1) * LINE_MAX;
        copyintoline(quels[id]._ans, line, sv, sv + LINE_MAX);
        quels[id]._ans[sv + LINE_MAX - 1] = '\n';
    }
} 

int delete(struct question* quels, int size, int id, int tag) {
    if(tag == 0) {
        size--;

        int i = id;
        while(i < size) {
            copyque(quels[i], quels[i + 1]);
            i++;
        }

        free(quels[size]._que);
        free(quels[size]._ans);
        free(quels[size]._time);
    } else {
        int anssize = countocc('\n', quels[id]._ans);
        if(2 < anssize) {
            int sv = (tag - 1) * LINE_MAX;
            if(tag != anssize) {
                    strcpy(quels[id]._ans + sv, quels[id]._ans + sv + LINE_MAX);
            } else {
                    quels[id]._ans[sv] = '\0';
            }
        } else {
            printf("%s\n", ansdel_err);
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

int final(struct question* quels, int size) {
    int err = 0;
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
                    fnqls[fnsize] = quels[i];
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
            printf("\nKerdoiv mentese...\n");
            writeqls(fnqls, fnsize, "output.bin");
            printf("...kesz.\n");
        }
    } else {
        printf("%s\n", lsempty_err);
        err++;
    }
    return err;
}

void writeqls(struct question* quels, int size, const char* fname) {
    int f;
    if(access(fname, F_OK) == -1){
        f = open(fname, O_CREAT | O_WRONLY, S_IRWXU);
    } else {
        f = open(fname, O_WRONLY | O_TRUNC);
    }
    
    int i = 0;
    while(i < size) {
        writeque(quels[i], f);
        i++;
    }

    close(f);
}

int readqls(struct question* quels, const char* fname) {
    if(access(fname, F_OK) != -1){
        int f = open(fname, O_RDONLY);
        int size = 0;
        
        int question_size = (LEN_QUE + LEN_ANS + LEN_TME - 1) * sizeof(char);
        char buffer[question_size];
        
        int i = 0;
        while(1) {
            int sv = read(f, buffer + i, sizeof(char));
            if(sv == 0) {
                break;
            } else {
                i++;
            }
            
            if(i == question_size) {
                quels[size] = createque(buffer, size);
                size++;
                i = 0;
            }
        }
        
        close(f);
        return size;
    } else {
        printf("%s %s\n", fread_err, fname);
        return -1;
    }
}

//egyeb fuggvenyek
void communicate(int status) {
    int f;
    
    int sv;
    f = open(FIFO, O_RDONLY);
    int ok = read(f, &sv, sizeof(int));
    printf("adat olvasva %d %d\n", ok, sv);
    close(f);
    
    if(ok == 4 && sv == 1) {
        f = open(FIFO, O_WRONLY);
        write(f, &status, sizeof(int));
    printf("adat elkuldve %d\n", status);
        close(f);
        
        if(status != 1) {
            struct question quels[1000];
            int size = readqls(quels, "output.bin");
            if(size >= 3) {
                int i, j, r, contains;
                printf("size: %d\n", size);
                
                int selected[3] = { -1, -1, -1 };
                f = open(FIFO, O_WRONLY);
                i = 0;
                while(i < 3) {
                    r = getrandom(size);
                    contains = 0;
                    j = 0;
                    while(j < i && contains == 0) {
                        if(selected[j] == r)
                            contains++;
                        j++;
                    }
                    if(contains == 0) {
                        selected[i] = r;
                        writeque(quels[r], f);
                        printf("elkuld: %d\n", r);
                        i++;
                    }
                }
                close(f);

                int tmp;
                int results[3][4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
                f = open(FIFO, O_RDONLY);
                int rd = read(f, &tmp, sizeof(int));
                while(rd == 4) {
                    i = 0;
                    while(i < 3 && rd == 4) {
                        results[i][tmp]++;
                        printf("olvas: %d\n", tmp);
                        i++;
                        rd = read(f, &tmp, sizeof(int));
                    }
                }
                close(f);

                printf("Eredmenyek:\n");
                i = 0;
                while(i < 3) {
                    struct question q = quels[selected[i]];
                    printf("%d. kerdes:\n%sValaszok:\n%sHozzaadva:\n%s\n", i + 1, q._que, q._ans, q._time);
                    j = 0;
                    int n = getanssize(quels[selected[i]]);
                    while(j < n) {
                        printf("%d. valasz: %d ember\n", j + 1, results[i][j]);
                        j++;
                    }
                    printf("\n");
                    i++;
                }
            } else if(size != -1) {
                printf("%s %d\n", lessqss_err, size);
            }
        }
    } else {
        printf("%s %s\n", noresp_err, ok);
    }
}

void help(const char** cmds, const char** dscr) {
    int i = 0;
    while(i < 6) {
        printf("%s %s\n", cmds[i], dscr[i]);
        i++;
    }
}

//child process fofuggvenye
void askppl(const char* city) {
    int f;
    
    int sv = 1;
    f = open(FIFO, O_WRONLY);
    int ok = write(f, &sv, sizeof(int));
    printf("adat elkuldve %d\n", ok);
    close(f);
    
    if(ok == 4) {
        int status = 1;
        f = open(FIFO, O_RDONLY);
        ok = read(f, &status, sizeof(int));
    printf("adat olvasva %d\n", status);
        close(f);
        
        if(status != 1) {
            struct question quels[3];
            int size = readqls(quels, FIFO);
            if(size == 3) {
                int anssize[3] = { getanssize(quels[0]), getanssize(quels[1]), getanssize(quels[2]) };

                f = open(FIFO, O_WRONLY);
                int ppl = getrandom(9) + 11;
                int i, j, r;
                i = 0;
                while(i < ppl) {
                    j = 0;
                    while(j < 3) {
                        r = getrandom(anssize[j]);
                        write(f, &r, sizeof(int));
                        printf("gy elkuld: %d\n", r);
                        j++;
                    }
                    i++;
                }
                close(f);
            } else {
                printf("%s\n", unexpected_err);
            }
        }
    }
}

//onallo fuggvenyek
char* acttime() {
    time_t rawtime;
    char* formtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    formtime = asctime(timeinfo);

    return formtime;
}

int getrandom(int n) {
    struct timeval t1;
    gettimeofday(&t1, NULL);
    srand(t1.tv_usec * t1.tv_sec);
    
    if(n - 1 == RAND_MAX)
        return rand();
    
    return rand() % n;
}

int readint() {
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

void readnempty(char* line, const char* prefix) {
    line[0] = '\n';
    while(line[0] == '\n' || line[0] == '\0') {
        printf("%s", prefix);
        fgets(line, LINE_MAX, stdin);
    }
}

int countocc(const char sample, const char* str) {
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

void copyintoline(char* dest, const char* src, const int begin, const int end) {
    int i = begin;
    int j = 0;
    while(i < end) {
        if(src[j] != '\0' && src[j] != '\n') {
            dest[i] = src[j];
            j++;
        } else {
            dest[i] = ' ';
        }
        i++;
    }
}

void copyintotext(char* dest, const char* src, const int begin, const int end) {
    int i = begin;
    int j = 0;
    while(i < end) {
        if(src[j] != '\0') {
            dest[i] = src[j];
            j++;
        } else {
            dest[i] = ' ';
        }
        i++;
    }
}