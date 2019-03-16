//Read and print out the file given in the parameter 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <assert.h>

void handler(int signumber){
  //printf("Signal with number %i has arrived\n",signumber);
}



struct registration {
   char name[150];
   char email[150];
   char date[18];
};

struct program {
    int attendant_num;
    struct registration attendants[1000];
};

struct pr_id_name {
    int id;
    char name[100];
};

struct program data;
int p_num;
char p_names[100][100];



void menu();
int get_program_id();
char* get_console_line(void);
void show_file(char* fname);
void ask_for_enter();

void do_program();

void read_programs();
//void write_programs();
void list_programs();

void list_attendants();
//void attend();
//void change_attendant();
//void delete_attendant();

//void create_program();
//void delete_program();

void read_file(int prog_id);
void write_file(int prog_id);


/** /
int main(void) {
        int     fd[2], nbytes;
        pid_t   childpid;
        char    string[] = "Hello, world!\n";
        char    readbuffer[80];

        pipe(fd);
        
        if((childpid = fork()) == -1) {
                perror("fork");
                exit(1);
        }

        if(childpid == 0) {
                // Child process closes up input side of pipe 
                close(fd[0]);

                // Send "string" through the output side of pipe 
                write(fd[1], string, (strlen(string)+1));
                exit(0);
        } else {
                // Parent process closes up output side of pipe
                close(fd[1]);

                // Read in a string from the pipe 
                nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
                printf("Received string: %s", readbuffer);
        }
        
        return(0);
}
/**/

int main(int argc,char** argv) {
    menu();

    return 0;
}

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

void do_program() {
    signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 

    int pToC[2], cToP[2], n, status;
    char bufP[200];
    char bufC[2000];

    pid_t pid;
    char str1[]="erdo";
    char str3[]="Hello!\n";
    char str4[]="Juh\n";
    char str5[]="csacsi\n";

    pipe(pToC);
    pipe(cToP);

    // Főfolyamat gyermekfolyamatot indít az esemény lebonyolítására.
    if((pid=fork()) < 0) {
        abort();
    } else if(pid > 0) {// parent
        close(pToC[0]);

        // Elkuldi a rendezveny helyszinet.
        write(pToC[1], str1, strlen(str1));

        // Varja, hogy a gyermek a helyszinre erkezzen.
        pause();
        printf("szulo: Jelzes erkezett.\n",SIGTERM);
        
        // Elkuldi a resztvevoket
        get_program_id();
        char str2[200];
        int i;
        for(i=0; i<data.attendant_num-1; ++i) {
            strcpy(str2, data.attendants[i].name);
            strcat(str2, ",");
            write(pToC[1], str2, strlen(str2));
        }
        strcpy(str2, data.attendants[i].name);
        strcat(str2, ",");
        write(pToC[1], str2, strlen(str2+1));

        close(pToC[1]);



        close(cToP[1]); // close write channel of pipe in child

        // varjuk, es kiirjuk a gyerek valaszait
        int n;
        read(cToP[0], &n, sizeof(n));
        printf("A rendezveny ennyire volt sikeres: %d\n", n);

        while (n = read(cToP[0], bufP, sizeof(bufP)) > 0) { // Read until it returns 0 (EOF) or -1 (error)
            printf("Akik nem jelentek meg a rendezvenyen:\n");
            printf(bufP);
        }
        if (n < 0) { // -1 = error
            perror("read from pipe");
        }

        //close(cToP[0]);

        wait(NULL);

        ask_for_enter();
    }
    else
    { // child
        // gyermek megkapja, hol kerul a rendezveny lebonyolitasra
        close(pToC[1]);

        read(pToC[0], bufC, sizeof(bufC));

        // Helyszínre utazik, majd jelzést küld vissza a főfolyamatnak, hogy várja a résztvevőket.
        printf("gyermek: Utazasom celja %s, utana jelzest fogok kuldeni.\n", bufC);
        sleep(2); // ez az utazas
        kill(getppid(),SIGTERM);

        // A gyermek képernyőre írja a résztvevőket.
        read(pToC[0], bufC, sizeof(bufC));
        printf("A rendezveny resztvevoi:\n");
        char** tokens;
        tokens = str_split(bufC, ',');
        if(tokens) {
            int i;
            for (i = 0; *(tokens + i); i++) {
                printf("%s\n", *(tokens + i));
            }
            printf("\n");
        }

        close(pToC[0]);



        close(cToP[0]);

        // A gyermek folyamat véletlenszámot küld vissza. (mennyire sikeres a rendezvény)
        char snum[3];
        srand(time(NULL));
        int rndnum = rand()%100+1;
        write(cToP[1], &rndnum, sizeof(rndnum));

        // A gyermek folyamat véletlenszerűen visszaküldi a nevek 10%-át (ki nem érkezett meg)
        char tenPercentOfNames[2000] = "";
        int i;
        for (i = 0; *(tokens + i); i++) {
            if(0==rand()%10) {
                strcat(tenPercentOfNames, *(tokens + i));
                strcat(tenPercentOfNames, "\n");
            }
        }
        write(cToP[1], tenPercentOfNames, strlen(tenPercentOfNames)+1);

        close(cToP[1]);

        // feltakarit maga utan
        for (i = 0; *(tokens + i); i++) {
            free(*(tokens + i));
        }
        free(tokens);
    }
}

void menu() {
    int again=1;
    do {
        /**/
        printf("\n***\n1 Rendezvenyek listazasa\n2 Rendezveny resztvevoinek listazasa\n\n3 Rendezveny lebonyolitasa\n0 kilepes\n***\n\nKivalasztott: ");
        char input = getchar();
        /**/

        /** /
        char input='3';
        /**/
        switch(input) {
            case '1':
                list_programs(); break;
            case '2':
                list_attendants(); break;
            case '3':
                do_program();
            case '0':
                again = 0; break;
            default:
                puts("Nincs ilyen parancs!");
                break;
        }
    } while (1==again);
}

int get_program_id() {
    // betu azonosito esetén 0 lesz :/
    char* line;
    printf("Adja meg a rendezveny azonositojat: ");
    getchar();
    line = get_console_line();
    int program_id = atoi(line);
    read_file(program_id);
    return program_id;
}

char* get_console_line(void) {
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF) { break; }

        if(0 == --len) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n'){ break; }
    }
    *line = '\0';
    return linep;
}

void show_file(char* fname) {
    FILE * f;
    f=fopen(fname,"r");
    if(f==NULL) { perror("File opening error\n"); exit(1); }
    char line[160];
    while (!feof(f)) {
        fgets(line,sizeof(line),f);
        printf("%s",line);
    } 
    printf("\n");
    fclose(f);
}

void ask_for_enter(){
    printf("Tovabblepes enterrel");
    //getchar();
    get_console_line();
}


void read_programs() {
    char bin[100];

    FILE * f;
    f=fopen("programs.txt","rb");
    if (f==NULL){perror("File opening error\n"); exit(1);}
    int i, c;

    char p_num_str[3];
    c = fscanf(f, "%s", p_num_str);
    fgets(bin, sizeof(bin), f);
    p_num = atoi(p_num_str);

    // handle each line of input
    for(i=0; i<p_num; ++i) {
        fgets(p_names[i], sizeof(p_names[i]), f);
        strtok(p_names[i], "\n");
    }

    fclose(f);
}

void write_programs() {
    FILE *f = fopen("programs.txt", "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "%d\n", p_num);

    int i;
    for(i=0; i<p_num; ++i) {
        fprintf(f, "%s\n", p_names[i]);
    }

    fclose(f);
}

void list_programs() {
    read_programs();

    printf("\njelenlegi rendezvenyek:\n");

    int i;
    for(i=0; i<p_num; ++i) {
        printf("%d - %s\n", i, p_names[i]);
    }

    puts("");
    getchar();
    ask_for_enter();
}



void list_attendants() {
    int program_id = get_program_id();

    int i;
    printf("\n%s:\n", p_names[program_id]);
    for(i=0; i<data.attendant_num; ++i) {
        printf("%d. %s, %s, %s %s\n", i+1, data.attendants[i].name, data.attendants[i].email, p_names[program_id], data.attendants[i].date);
    }
    puts("");
    ask_for_enter();
}

void attend() {
    char* line;
    int program_id=get_program_id();

    printf("e-mail cim: ");
    line=get_console_line();
    strtok(line, "\n");
    strcpy(data.attendants[data.attendant_num].email, line);
    
    int i=0;
    while(strcmp(data.attendants[i].email, line) != 0 && i<data.attendant_num) {
        ++i;
    }
    if(data.attendant_num==i){
        strcpy(data.attendants[data.attendant_num].email, line);

        printf("nev: ");
        line=get_console_line();
        strtok(line, "\n");
        strcpy(data.attendants[data.attendant_num].name, line);

        time_t now = time(NULL);
        strftime(data.attendants[data.attendant_num].date, sizeof(data.attendants[data.attendant_num].date), "%Y.%m.%d-%H:%M", gmtime(&now));

        //printf("%s, %s, %s %s\n", data.attendants[data.attendant_num].name, data.attendants[data.attendant_num].email, p_names[program_id], data.attendants[data.attendant_num].date);

        ++data.attendant_num;
        printf("\nSikeres jelentkezes.\nJelentkezes sorszama: %d\n\n", data.attendant_num);

        write_file(program_id);
    } else {
        puts("\nEzzel az e-mail cimmel erre a rendezvenyre mar van jelentkezo!\n\n");
    }
    ask_for_enter();
}

void change_attendant() {
    char* line;
    int program_id = get_program_id();

    printf("Adja meg a modositando jelentkezeshez tartozo sorszamot: ");
    line=get_console_line();
    strtok(line, "\n");

    int i = atoi(line) - 1;
    if(data.attendant_num<=i){
        puts("\nEzen a rendezvenyen nincs resztvevo ezzel a sorszammal.\n");
    } else {
        printf("uj nev: ");
        line=get_console_line();
        strtok(line, "\n");
        strcpy(data.attendants[i].name, line);
        
        int i2=0;
        do {
            printf("uj e-mail: ");
            line=get_console_line();
            strtok(line, "\n");

            i2 = 0;
            while(strcmp(data.attendants[i].email, line) != 0 && i2<data.attendant_num) {
                ++i2;
            }
        } while (data.attendant_num!=i2);

        strcpy(data.attendants[i].email, line);

        write_file(program_id);
        puts("\nModositas megtortent.\n");
    }
    ask_for_enter();
}

void delete_attendant() {
    char* line;
    int p_id = get_program_id();

    printf("Adja meg a torlendo jelentkezeshez tartozo sorszamot: ");
    line=get_console_line();
    strtok(line, "\n");

    int i = atoi(line) - 1;
    if(data.attendant_num<=i){
        puts("\nEzen a rendezvenyen nincs resztvevo ezzel a sorszammal.\n");
    } else {
        while(i<data.attendant_num-1){
            strcpy(data.attendants[i].name, data.attendants[i+1].name);
            strcpy(data.attendants[i].email, data.attendants[i+1].email);
            strcpy(data.attendants[i].date, data.attendants[i+1].date);
            ++i;
        }
        --data.attendant_num;

        write_file(p_id);
        puts("\nTorles megtortent.\n");
    }
    ask_for_enter();
}



void create_program() {
    read_programs();

    char *line;

    printf("Redezveny neve: ");
    getchar();
    line=get_console_line();
    strtok(line, "\n");
    strcpy(p_names[p_num], line);

    data.attendant_num = 0;

    write_file(p_num);
    ++p_num;
    //printf("%d", p_num);
    write_programs();
    puts("\nRendezveny Letrehozva\n");
    ask_for_enter();
}

void delete_program() {
    int p_id = get_program_id();
    //getchar();

    read_programs();
    int i = p_id;
    if(p_id>=p_num){
        puts("\nIlyen azonositoval nincs rendezveny.\n\n");
    } else {
        char filename[7];
        snprintf(filename, sizeof(filename),"%d", p_id);
        strcat(filename, ".txt");
        remove(filename);

        while(i<p_num-1){
            strcpy(p_names[i], p_names[i+1]);

            char new_filename[7];
            snprintf(new_filename, sizeof(filename),"%d",i);
            strcat(new_filename, ".txt");

            char old_filename[7];
            snprintf(old_filename, sizeof(filename),"%d",i+1);
            strcat(old_filename, ".txt");

            rename(old_filename, new_filename);

            ++i;
        }
        --p_num;

        int j=0;
        for(j=0; i<p_num; ++i){
            puts(p_names[i]);
        }

        write_programs();
        puts("\nRendezveny torlese megtortent.\n");

        ask_for_enter();
    }
}



void read_file(int prog_id) {
    char bin[150];
    
    char filename[7];
    snprintf(filename, sizeof(filename),"%d",prog_id);
    strcat(filename, ".txt");
    
    FILE * f;
    f=fopen(filename,"rb");
    if (f==NULL){perror("File opening error\n"); exit(1);}
    int i, c;

    // read name, max attendants, attandant number
    fgets(p_names[prog_id], sizeof(p_names[prog_id]), f);
    strtok(p_names[prog_id], "\n");

    char numtmpstr[3];
    c = fscanf(f, "%s", numtmpstr);
    fgets(bin, sizeof(bin), f);

    data.attendant_num = atoi(numtmpstr);

    // handle each line of input
    for(i=0; i<data.attendant_num; ++i) {
        //read the name
        fgets(data.attendants[i].name, sizeof(data.attendants[i].name), f);
        strtok(data.attendants[i].name, "\n");

        // read email and date
        c = fscanf(f, "%s %s", data.attendants[i].email, data.attendants[i].date);
        fgets(bin, sizeof(bin), f);
        if (2 != c) {
            puts("File reading error.\n");
            exit(1);
        }
    }

    fclose(f);
}

void write_file(int prog_id){
    char filename[7];
    sprintf(filename, "%d", prog_id);
    strcat(filename, ".txt");

    //strcpy(filename, "out.txt");

    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "%s\n%d\n", p_names[prog_id] , data.attendant_num);

    int i;
    for(i=0; i<data.attendant_num; ++i) {
        fprintf(f, "%s\n %s %s\n", data.attendants[i].name, data.attendants[i].email, data.attendants[i].date);
    }

    fclose(f);
}
