#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

struct Latogato{
    char nev[100];
    char email[100];
    char azon[50];
    time_t time;
};

void handler(int x)
{
    x = x;
}

void betoltes(struct Latogato latogatok[], char rendezvenyek[100][100], int* l_length, int* r_length);
void menu(struct Latogato latogatok[], char rendezvenyek[100][100], int* l_length, int* r_length);
void frissit(struct Latogato latogatok[], int l_length);

void Uj_Latogato(struct Latogato latogatok[], char rendezvenyek[100][100], int* l_length, int* r_length);
void Listaz(struct Latogato latogatok[], int l_length);
void Uj_Rendezveny(char rendezvenyek[100][100], int *r_length);
void Rendezveny_Torlese(struct Latogato latogatok[], char rendezvenyek[100][100], int r_length, int l_length);
void Latogato_Torlese(struct Latogato latogatok[], int l_length);
void Modosit(struct Latogato latogatok[], int l_length);
void Levezenyel(struct Latogato latogatok[], int l_length);

int main()
{
    struct Latogato latogatok[200];
    char rendezvenyek[100][100];
    int l_length = 0;
    int r_length = 0;
	
	struct sigaction sigact;
    sigact.sa_handler = handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGUSR1, &sigact, NULL);

    betoltes(latogatok, rendezvenyek, &l_length, &r_length);
    menu(latogatok, rendezvenyek, &l_length, &r_length);
    frissit(latogatok, l_length);

    return 0;
}

void betoltes(struct Latogato latogatok[], char rendezvenyek[100][100], int* l_length, int* r_length){
    FILE * fPointer;
    fPointer = fopen("output.txt", "r");

    int i = 0;
    int j = 0;
    bool find = false;
    char x[1024];
    while (fscanf(fPointer, " %1023s", x) == 1) {
        switch(j%4){
            case 0:
                memcpy(latogatok[i].nev, x, strlen(x)+1);
                j++;
                break;
            case 1:
                memcpy(latogatok[i].email, x, strlen(x)+1);
                j++;
                break;
            case 2:
                memcpy(latogatok[i].azon, x, strlen(x)+1);
                j++;
                break;
            case 3:
                latogatok[i].time = atoi(x);
                j++;
                i++;
                break;
        }
    }
    *l_length = i;

    fclose(fPointer);

    for(i = 0; i < *l_length; i++){
        memcpy(x, latogatok[i].azon, strlen(latogatok[i].azon)+1);

        for(j = 0; j < *r_length; j++){
            if (!strcmp(rendezvenyek[j], x)){
                find = true;
            }
        }

        if (!find){
            memcpy(rendezvenyek[*r_length], x, strlen(x)+1);
            *r_length += 1;
        }
    }
}

void menu(struct Latogato latogatok[], char rendezvenyek[100][100], int* l_length, int* r_length){
    char command = ' ';
    printf("1. beadando - Zelena Viktor (IS3Z5U)\n\n");

    printf("1. Uj latogato\n");
    printf("2. Latogatok listazasa\n");
    printf("3. Latogato tolese\n");
    printf("4. Latogato modositasa\n");
    printf("5. Uj rendezveny\n");
    printf("6. Rendezveny torlese\n");
	printf("7. Rendezveny levezenylese\n");
    printf("0. Kilepes\n");

    while(command != '0'){
        printf("\nKerem adja meg mit kivan tenni: ");
        scanf("%c", &command);
        if(command == '\n'){ scanf("%c", &command); }

        if(command == '1'){
            Uj_Latogato(latogatok, rendezvenyek, l_length, r_length);
        }
        if(command == '2'){
            Listaz(latogatok, *l_length);
        }
        if(command == '3'){
            Latogato_Torlese(latogatok, *l_length);
        }
        if(command == '4'){
            Modosit(latogatok, *l_length);
        }
        if(command == '5'){
            Uj_Rendezveny(rendezvenyek, r_length);
        }
        if(command == '6'){
            Rendezveny_Torlese(latogatok, rendezvenyek, *r_length, *l_length);
        }
		if(command == '7'){
			Levezenyel(latogatok, *l_length);
        }
    }
}

void frissit(struct Latogato latogatok[], int l_length){

    FILE *f = fopen("output.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
    } else {
        int i;

        for(i=0;i<l_length;i++){
            if(strcmp(latogatok[i].nev, "")){
                fprintf(f, "%s ", latogatok[i].nev);
                fprintf(f, "%s ", latogatok[i].email);
                fprintf(f, "%s ", latogatok[i].azon);
                fprintf(f, "%d\n", latogatok[i].time);
            }
        }
        fclose(f);
    }
}
void Uj_Latogato(struct Latogato latogatok[], char rendezvenyek[100][100], int* l_length, int* r_length)
{
    char name[200];
    char mail[200];
    char razon[200];
    bool letezik = false;
    int i =0;

    printf("Adja meg a nevet: ");
    scanf("%s", name);

    printf("Adja meg az email cimet: ");
    scanf("%s", mail);

    printf("Adja meg a rendezveny azonositojat: ");
    scanf("%s", razon);

    for(i=0;i<*r_length;i++){
        if(!strcmp(rendezvenyek[i], razon)){
            letezik = true;
        }
    }

    if(letezik){
        memcpy(latogatok[*l_length].nev, name, strlen(name)+1);
        memcpy(latogatok[*l_length].email, mail, strlen(mail)+1);
        memcpy(latogatok[*l_length].azon, razon, strlen(razon)+1);

        latogatok[*l_length].time = time(NULL);
        *l_length += 1;
    } else {
        printf("\nNem letezik ilyen rendezveny!\n");
    }

}
void Listaz(struct Latogato latogatok[], int l_length)
{
    int i;
    for(i=0; i<l_length; ++i)
    {
        if(strcmp(latogatok[i].nev, ""))
        {
            printf("  %s, ", latogatok[i].nev);
            printf("%s, ", latogatok[i].email);
            printf("%s, ", latogatok[i].azon);
            printf("%s", ctime(&latogatok[i].time));
        }


    }

}
void Uj_Rendezveny(char rendezvenyek[100][100], int *r_length)
{
    char x[200];
    printf("Add meg az uj rendezveny azonositot!\n");
    scanf("%s", x);

    memcpy(rendezvenyek[*r_length], x, strlen(x)+1);
    *r_length += 1;

}

void Rendezveny_Torlese(struct Latogato latogatok[], char rendezvenyek[100][100], int r_length, int l_length)
{
    int i;
    char x[1024];
    printf("Melyik rendezvenyt szeretned torolni?\n");
    scanf("%s", x);

    for(i=0;i<l_length;i++){
        if(!strcmp(latogatok[i].azon, x)){
            memcpy(latogatok[i].nev, "", 1);
            memcpy(latogatok[i].email, "", 1);
            memcpy(latogatok[i].azon, "", 1);
            latogatok[i].time = 0;
        }
    }

    for(i=0;i<r_length;i++){
        if(!strcmp(rendezvenyek[i], x)){
            memcpy(rendezvenyek[i], "", 1);
        }
    }
}

void Latogato_Torlese(struct Latogato latogatok[], int l_length)
{
    int i;
    char x[1024];
    printf("Melyik felhasznalot szeretned torolni?\n");
    scanf("%s", x);

    for(i=0;i<l_length;i++){
        if(!strcmp(latogatok[i].nev, x)){
            memcpy(latogatok[i].nev, "", 1);
            memcpy(latogatok[i].email, "", 1);
            memcpy(latogatok[i].azon, "", 1);
            latogatok[i].time = 0;
        }
    }
}

void Modosit(struct Latogato latogatok[], int l_length){
    int i;
    char x[1024];
    char name[1024];
    char mail[1024];
    printf("Melyik felhasznalot szeretned modositani?\n");
    scanf("%s", x);

    printf("Uj felhasznalo nev: ");
    scanf("%s", name);
    printf("Uj mail: ");
    scanf("%s", mail);

    for(i=0;i<l_length;i++){
        if(!strcmp(latogatok[i].nev, x)){
            memcpy(latogatok[i].nev, name, strlen(name)+1);
            memcpy(latogatok[i].email, mail, strlen(mail)+1);
        }
    }
}

void Levezenyel(struct Latogato latogatok[], int l_length)
{
		char azn[2];
		printf("Melyik rendezvenyt vezenyeljuk le: ");
		scanf("%s", &azn);  
		char megkapottnevek[50][100];
		char vendegek[50][100];
		int size = 0;		
        
        char nemjottekel[50][100];

        char buff[300];
		int i;

		for(i=0;i<l_length;i++){
            if(!strcmp(latogatok[i].azon, azn)){
				strcpy(vendegek[size],latogatok[i].nev);
				++size;
           }
        }
		
		int fd[2];
        int Pipe = pipe(fd);
        if (Pipe < 0)
        {
            perror("Pipe error!");
            exit(-1);
        }

        pid_t child = fork();
        if (child < 0)
        {
            perror("fork() error!");
            exit(-1);
        }

        /* parent process */
        if (0 != child)
        {
            sigset_t sigset;
            sigfillset(&sigset);
            sigdelset(&sigset, SIGUSR1);

            write(fd[1], azn, strlen(azn));

            if (sigsuspend(&sigset) == -1)
            {
                printf("A partner megerkezett a(z) %s-s rendezvenyre\n", azn);
            };

            write(fd[1], vendegek, sizeof vendegek);
            sigsuspend(&sigset);

            wait(NULL);

            read(fd[0], nemjottekel, sizeof nemjottekel);
			printf("King of Stands: Megkaptam a mulasztok listajat!\n");
            int j;
			int otthonmaradok = 0;
            for (j = 0; j < size; ++j)
            {
                if (strcmp(nemjottekel[j],"O elment") != 0)
                {
                    printf("Nem volt itt: %s \n", nemjottekel[j]);
					++otthonmaradok;
                }
            }
			if(otthonmaradok==0)
			{
				printf("King of Sands: Mindenki elment a rendezvenyre!");
			}
        }
        /* child process */
        else
        {
            memset(buff, 0, strlen(buff));
            read(fd[0], buff, strlen(azn));
            kill(getppid(), SIGUSR1);
            printf("Partner: Megkaptam a rendezveny azonositojat: %s\n", buff);

            read(fd[0], megkapottnevek, sizeof megkapottnevek);
            kill(getppid(), SIGUSR1);
            printf("Partner: Megkaptam a neveket!\n");

            srand(time(NULL));
            int i;
			int sorsolt;
            for (i = 0; i < size; i++)
            {
                sorsolt = (rand() % 100 +1);
				printf("%i\n",sorsolt);
				if ( sorsolt <= 10)
                {
					strcpy(nemjottekel[i], megkapottnevek[i]);
                }
				else
				{
					strcpy(nemjottekel[i], "O elment");
				}
            }

            write(fd[1], nemjottekel, sizeof nemjottekel);
            exit(0);
        }


}

