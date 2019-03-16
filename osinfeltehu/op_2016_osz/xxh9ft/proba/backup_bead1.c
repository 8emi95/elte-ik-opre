//Read and print out the file given in the parameter 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>

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



void show_file(char* fname);
char* get_console_line(void);

void read_file(int prog_id);
void write_file(int prog_id);

void list_programs();
void list_attendants();
void menu();
int get_program_id();



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
        if(c == EOF)
            break;

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

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}

void show_file(char* fname) {
 FILE * f;
 f=fopen(fname,"r");
 if (f==NULL){perror("File opening error\n"); exit(1);}
 char line[160];
 while (!feof(f)){
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

void menu() {
    int again=1;
    do {
        /**/
        printf("\n***\n1 Rendezvenyek listazasa\n2 Rendezveny resztvevoinek listazasa\n\n3 Jelentkezes rendezvenyre\n4 Vendeg adatinak modositasa \n5 Vendeg torlese\n\n6 Rendezveny letrehozasa\n7 Rendezveny torlese\n\n0 kilepes\n***\n\nKivalasztott: ");
        char input = getchar();
        /**/

        /** /
        char input='2';
        /**/
        switch(input) {
            case '1':
                list_programs(); break;
            case '2':
                list_attendants(); break;
            case '3':
                attend(); break;
            case '4':
                change_attendant(); break;
            case '5':
                delete_attendant(); break;
            case '6':
                create_program(); break;
            case '7':
                delete_program(); break;
            case '0':
                again = 0; break;
            default:
                puts("Nincs ilyen parancs!");
                break;
        }
    } while (1==again);
}

/**/
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
/**/
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

/**/
int main(int argc,char** argv) {
    menu();

    /** /
    strcpy(p_names[program_id], "csiga");
    strcpy(data.email, "csiga@erdo.hu");
    data.program_id = 1;
    /**/

    //write_file(9);
    

    return 0;
}
/**/




/** /
#include <ctypes.h>  // for isspace()
#include <stdio.h> // for scanf(), getchar(), and EOF

char c2d[MAX_LINES][MAX_LENGTH_STRING_PER_LINE];
char *pstr;
float f2d[MAX_LINES][6]; // 6 floats per line
float *p;
int c, current_line_number;
char ch;
FILE *input;

input = fopen(...);
if (!input)
    ... handle the error

for (current_line_number = 0; ; ++current_line_number)
{
    // handle each line of input

    // first read 6 float values
    p = f2d + current_line_number;
    c = fscanf(input, "%f %f %f %f %f %f", p + 0, p + 1, p + 2, p + 3, p + 4, p + 5);
    if (c != 6)
        ... handle the error here

    // next grab string; stop at '<' or end of line or EOF
    pstr = c2d + current_line_number;
    for (;;)
    {
        ch = fgetc(input);
        if (ch == EOF || ch == '<' || ch == '\n')
        {
            *pstr = '\0';
            break;
        }
        *pstr++ = ch;
    }
    if (ch == '<')
    {
        // char was '<' so throw away rest of input line until end of line
        for (;;)
        {
            if (ch == EOF || ch == '\n')
                break;
            ch = fgetc(input);
        }
    }
    for (;;)
    {
        // eat up any white space, including blank lines in input file
        if (ch == EOF || !isspace(ch))
            break;
        ch = fgetc(input);
    }
    // once we have hit end of file we are done; break out of loop
    if (ch == EOF)
        break;
}

fclose(input);
/**/