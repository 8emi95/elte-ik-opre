#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

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


    do{
    start();
    char command;
    do
    {
        scanf("%c", &command);
        if(command == 'q'){
            return 0;
        }
    }
    while(!(command == '1'||command =='2'||command =='3'||command =='4'||command =='5'||command =='6'));

    while(getchar()!='\n');

    switch (command)
    {
    case '1' :
        new_visitor(vis_pointer,serial_pointer);
         vis_pointer = vis_pointer + 1;
        break;
    case '2' :
        vis_pointer_begin = &vis;
        int status;
        pid_t child = fork();
        if(child<0){
            perror("Hiba tortent!");
            exit(1);
        }
        if (child>0){
            waitpid(child,&status,0);
        }else{
            int i;
            for(i=2; i<=serial; i++){
                printf("%s %s %s %i %s\n",vis[serial].name,vis[serial].mail,vis[serial].identification,vis[serial].serial_number,vis[serial].time);
            }
        }
        break;
    case '3' :
        printf("asd");
        break;
    case '4' :
        printf("asd");
        break;
    case '5' :
        printf("asd");
        break;
    case '6' :
        printf("asd");
        break;
    default:
        printf("hiba");
    }
    }while(1);

    return 0;
}


void start()
{
    printf("1-Uj vendeg jelentkezese\n");
    printf("2-Jelentkezett vendegek listazasa\n");
    printf("3-Jelentkezett vendeg adatainak modositasa\n");
    printf("4-Vendég torlese\n");
    printf("5-Uj rendezveny inditasa\n");
    printf("6-Rendezveny torlese\n");
    printf("q-Kilepes\n");
}

void new_visitor(struct Visitor* vis_pointer, int* serial_pointer)
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
    //TODO lecheckolni hogy helyes-e az azonosító
    //TODO kilépési lehetőség
    char *pos;
    if ((pos=strchr(name, '\n')) != NULL)
    *pos = '\0';
    if ((pos=strchr(mail, '\n')) != NULL)
    *pos = '\0';
    if ((pos=strchr(identification, '\n')) != NULL)
    *pos = '\0';

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

    *serial_pointer = *serial_pointer + 1;

}
/*
 void list_visitors(struct Visitor* vis_pointer_begin, int* serial_number_pointer){
 int i;
 for(i=2; i<=*serial_number_pointer;i++){
    printf("%s %s %s %i %s\n",vis_pointer_begin->name, vis_pointer_begin->mail, vis_pointer_begin->identification,
          vis_pointer_begin->serial_number, vis_pointer_begin->time);
    vis_pointer_begin = vis_pointer_begin + 1;
 }
 printf("\n\n");
 }
 */
 void list_visitors(){

 }
