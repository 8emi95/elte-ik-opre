#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Visitor
{
    char name[50];
    char mail[50];
    char identification[100];
    int serial_number;
    char time[100];
};

int main()
{
    int serial = 1;
    int* serial_pointer = &serial;
    struct Visitor vis[100];
    struct Visitor* vis_pointer;
    struct Visitor* vis_pointer_begin;
    vis_pointer = &vis;

    char* program [50];
    int i;
    for (i=0 ; i<50; i++)
    {
        if ((program[i] = malloc(sizeof(char) * 50)) == NULL)
        {
            printf("Hiba, nem tudtam lefoglalni memóriat! \n");
            return -1;
        }
    }

    char** program_pointer;
    program_pointer = &program;
    char** program_begin_pointer;
    program_begin_pointer = &program;
    do
    {
        start();
        char command;
        do
        {
            scanf("%c", &command);
            if(command == 'q')
            {
                int i;
                for (i=0 ; i<50; i++)
                    free(program[i]);

                return 0;
            }
        }
        while(!(command == '1'||command =='2'||command =='3'||command =='4'||command =='5'||command =='6'||command =='7'|| command =='8'));

        while(getchar()!='\n');

        switch (command)
        {
        case '1' :
            if(new_visitor(vis_pointer,serial_pointer,program_begin_pointer,program_pointer) == 0)
            {
                vis_pointer = vis_pointer + 1;
                *serial_pointer = *serial_pointer + 1;
            }
            program_begin_pointer =&program;
            break;
        case '2' :
            vis_pointer_begin = &vis;
            list_visitors(vis_pointer_begin,serial_pointer);
            break;
        case '3' :
            vis_pointer_begin = &vis;
            change_visitor(vis_pointer_begin,vis_pointer);
            vis_pointer_begin = &vis;
            break;
        case '4' :
            vis_pointer_begin = &vis;
            delete_visitor(vis_pointer_begin,vis_pointer);
            vis_pointer_begin = &vis;
            break;
        case '5' :
            new_program(program_pointer);
            program_pointer = program_pointer + 1;
            break;
        case '6' :
            program_begin_pointer = &program;
            vis_pointer_begin = &vis;
            delete_program(program_begin_pointer, program_pointer, vis_pointer_begin,vis_pointer);
            program_begin_pointer = &program;
            vis_pointer_begin = &vis;
            break;
        case '7':
            program_begin_pointer = &program;
            vis_pointer_begin = &vis;
            save(serial_pointer,vis_pointer_begin,vis_pointer,program_begin_pointer,program_pointer);
            program_begin_pointer = &program;
            vis_pointer_begin = &vis;
        case '8':
            vis_pointer_begin = &vis;
            program_begin_pointer = &program;
            load(serial_pointer,vis_pointer_begin,vis_pointer,program_begin_pointer,program_pointer);
            vis_pointer_begin = &vis;
            program_begin_pointer = &program;

            while (!(strcmp(*program_pointer,"") == 0))
            {
                program_pointer = program_pointer+1;
            }

            while (!(strcmp(vis_pointer->name,"") == 0))
            {
                vis_pointer = vis_pointer+1;
            }

            break;
        default:
            printf("hiba");
        }
    }
    while(1);

    return 0;
}


void start()
{
    printf("1-Uj vendeg jelentkezese\n");
    printf("2-Jelentkezett vendegek listazasa\n");
    printf("3-Jelentkezett vendeg adatainak modositasa\n");
    printf("4-Vendeg torlese\n");
    printf("5-Uj rendezveny inditasa\n");
    printf("6-Rendezveny torlese\n");
    printf("7-Mentes\n");
    printf("8-Beolvas\n");
    printf("q-Kilepes\n");
}
void save(int* serial_pointer,struct Visitor* vis_pointer_begin,struct Visitor* vis_pointer,char** program_begin_pointer,
          char** program_pointer)
{
    FILE *f = fopen("program.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    while(program_begin_pointer != program_pointer)
    {
        if(*program_begin_pointer != "")
        {
            fprintf(f,"%s\n",*program_begin_pointer);
        }
        program_begin_pointer = program_begin_pointer +1;
    }
    fclose(f);

    FILE *g = fopen("visitors.txt", "w");
    if (g == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    while(vis_pointer_begin != vis_pointer)
    {
        if(vis_pointer_begin->serial_number != -1 && vis_pointer_begin->serial_number !=0)
        {
            fprintf(g,"%s %s %s %i %s\n",vis_pointer_begin->name, vis_pointer_begin->mail, vis_pointer_begin->identification,
                    vis_pointer_begin->serial_number, vis_pointer_begin->time);
        }
        vis_pointer_begin = vis_pointer_begin +1;
    }
    fclose(g);

}

void load(int* serial_pointer,struct Visitor* vis_pointer_begin,struct Visitor* vis_pointer,char** program_begin_pointer,
          char** program_pointer)
{


    FILE* file;
    file = fopen("program.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    do
    {
        int row = fscanf(file, "%s", *program_begin_pointer) != EOF;
        if(row)
        {
            program_begin_pointer = program_begin_pointer + 1;
        }
        else
        {
            break;
        }
    }
    while(1);

    program_pointer = program_begin_pointer;
    strcpy(*program_pointer,"");

    FILE* f;
    f = fopen("visitors.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    do
    {
        char time2[5];
        char time3[20];
        char time1[30];
        int row = fscanf(f, "%s %s %s %i %s %s %s", &vis_pointer_begin->name, &vis_pointer_begin->mail
                         ,&vis_pointer_begin->identification,&vis_pointer_begin->serial_number,&time1
                         ,&time2,&time3) != EOF;
        char str[80];
        strcpy(str, time1);
        strcat(str, " ");
        strcat(str, time2);
        strcat(str, " ");
        strcat(str, time3);
        strcpy(vis_pointer_begin->time,str);
        if(row)
        {
            *serial_pointer = (vis_pointer_begin->serial_number)+1;
            vis_pointer_begin = vis_pointer_begin + 1;

        }
        else
        {
            break;
        }
    }
    while(1);
    vis_pointer = vis_pointer_begin;
    strcpy(vis_pointer->name,"");

}


void delete_program(char** program_begin_pointer,char** prog_pointer, struct Visitor* vis_pointer_begin,struct Visitor* vis_pointer)
{
    char prog_name[100];
    printf("Add meg a rendezveny nevet: ");
    fgets(prog_name, 100, stdin);
    char *pos;
    if ((pos=strchr(prog_name, '\n')) != NULL)
        *pos = '\0';

    int ok = 0;
    while(program_begin_pointer != prog_pointer)
    {
        if(strcmp(*program_begin_pointer,prog_name) == 0)
        {
            ok = 1;
            break;
        }
        else
        {
            program_begin_pointer = program_begin_pointer + 1;
        }
    }
    if(ok == 0)
    {
        printf("Nincs ilyen program az adatbazisban!\n");
    }
    else
    {
        *program_begin_pointer = "";

        while(vis_pointer_begin != vis_pointer)
        {
            if(strcmp(vis_pointer_begin->identification,prog_name) == 0)
            {
                vis_pointer_begin->serial_number=-1;
                strcpy(vis_pointer_begin->identification,"");
                strcpy(vis_pointer_begin->mail,"");
                strcpy(vis_pointer_begin->name,"");
                strcpy(vis_pointer_begin->time,"");
            }
            vis_pointer_begin = vis_pointer_begin + 1;
        }
    }
}

void change_visitor(struct Visitor* vis_pointer_begin,struct Visitor* vis_pointer)
{
    char name[100];
    printf("Add meg a nevet: ");
    fgets(name, 100, stdin);
    char *pos;
    if ((pos=strchr(name, '\n')) != NULL)
        *pos = '\0';

    int ok = 0;
    while(vis_pointer_begin != vis_pointer)
    {
        if(strcmp(vis_pointer_begin->name,name) == 0)
        {
            ok = 1;
            break;
        }
        else
        {
            vis_pointer_begin = vis_pointer_begin + 1;
        }
    }
    if(ok == 0)
    {
        printf("Nincs ilyen nev az adatbazisban!\n");
    }
    else
    {
        char name[100];
        printf("Add meg a neved: ");
        fgets(name, 100, stdin);
        printf("A neved: %s", name);

        char mail[100];
        printf("Add meg az e-mailed: ");
        fgets(mail, 100, stdin);
        printf("Az e-mailed: %s", mail);

        char *pos;
        if ((pos=strchr(name, '\n')) != NULL)
            *pos = '\0';
        if ((pos=strchr(mail, '\n')) != NULL)
            *pos = '\0';

        strcpy( vis_pointer_begin->name,name);
        strcpy( vis_pointer_begin->mail,mail);
    }
}

void delete_visitor(struct Visitor* vis_pointer_begin,struct Visitor* vis_pointer)
{
    char name[100];
    printf("Add meg a nevet: ");
    fgets(name, 100, stdin);
    char *pos;
    if ((pos=strchr(name, '\n')) != NULL)
        *pos = '\0';

    int ok = 0;
    while(vis_pointer_begin != vis_pointer)
    {
        if(strcmp(vis_pointer_begin->name,name) == 0)
        {
            ok = 1;
            break;
        }
        else
        {
            vis_pointer_begin = vis_pointer_begin + 1;
        }
    }
    if(ok == 0)
    {
        printf("Nincs ilyen nev az adatbazisban!\n");
    }
    else
    {
        vis_pointer_begin->serial_number=-1;
        strcpy(vis_pointer_begin->identification,"");
        strcpy(vis_pointer_begin->mail,"");
        strcpy(vis_pointer_begin->name,"");
        strcpy(vis_pointer_begin->time,"");

    }
}



int new_visitor(struct Visitor* vis_pointer, int* serial_pointer, char** program_begin_pointer,char** program_pointer )
{
    char name[100];
    printf("Add meg a neved: ");
    fgets(name, 100, stdin);
    printf("A neved: %s", name);

    char mail[100];
    printf("Add meg az e-mailed: ");
    fgets(mail, 100, stdin);
    printf("Az e-mailed: %s", mail);

    char identification[100];
    printf("A rendezveny azonositoja: ");
    fgets(identification, 100, stdin);
    printf("A rendezveny azonositoja: %s", identification);

    char *pos;
    if ((pos=strchr(name, '\n')) != NULL)
        *pos = '\0';
    if ((pos=strchr(mail, '\n')) != NULL)
        *pos = '\0';
    if ((pos=strchr(identification, '\n')) != NULL)
        *pos = '\0';

    int ok = 0;
    while(program_begin_pointer != program_pointer)
    {
        if(strcmp(*program_begin_pointer,identification) == 0)
        {
            ok = 1;
            break;
        }
        else
        {
            program_begin_pointer = program_begin_pointer + 1;
        }
    }

    if(ok == 0)
    {
        printf("Nincs ilyen rendezveny, a program visszalep a fomenube\n");
        return 1;
    }
    else
    {
        time_t rawtime;
        struct tm *info;
        char date[80];
        time( &rawtime );
        info = localtime( &rawtime );
        strftime(date,80,"%x - %I:%M%p", info);
        printf("%s\n", date );

        strcpy( vis_pointer->name,name);
        strcpy( vis_pointer->mail,mail);
        strcpy( vis_pointer->identification,identification);
        vis_pointer->serial_number = *serial_pointer;
        strcpy( vis_pointer->time,date);
        return 0;
    }
}


void list_visitors(struct Visitor* vis_pointer_begin, int* serial_number_pointer)
{
    int i;
    for(i=2; i<=*serial_number_pointer; i++)
    {
        if(vis_pointer_begin->serial_number != -1 && vis_pointer_begin->serial_number !=0)
        {
            printf("%s %s %s %i %s\n",vis_pointer_begin->name, vis_pointer_begin->mail, vis_pointer_begin->identification,
                   vis_pointer_begin->serial_number, vis_pointer_begin->time);
        }
        vis_pointer_begin = vis_pointer_begin + 1;
    }
    printf("\n\n");
}

void new_program(char** program_pointer)
{
    char program_name [50];
    printf("Add meg a program nevet: ");
    fgets(program_name, 50, stdin);
    printf("A program neve: %s", program_name);

    char *pos;
    if ((pos=strchr(program_name, '\n')) != NULL)
        *pos = '\0';

    strcpy(*program_pointer,program_name);
}

