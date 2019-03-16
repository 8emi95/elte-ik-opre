#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#include <signal.h>
#include <unistd.h> // for pipe()

void clear(void)
{
    while (getchar() != '\n')
        ;
}

#define MAXSTRINGSIZE 100
#define MAXALERTS 100
#define EMPTYALERTNAME "#&# Empty Ł@#"

struct Alert
{
    char name[MAXSTRINGSIZE];
    char address[MAXSTRINGSIZE];
    char email[MAXSTRINGSIZE];
    char time[MAXSTRINGSIZE];
    char phone[MAXSTRINGSIZE];
    int urgent;
};

const struct Alert createAlert(const char *name)
{
    struct Alert a;
    strcpy(a.name, name);
    return a;
}

struct Alert readStuff()
{
    struct Alert new;

    // TIME
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strcpy(new.time, asctime(timeinfo));

    // NAME
    printf("Bejelento neve: ");
    char name[MAXSTRINGSIZE];
    if (fgets(name, MAXSTRINGSIZE, stdin) != NULL)
    {
        name[strlen(name) - 1] = 0;
        strcpy(new.name, name);
    }

    // ADDRESS
    printf("Bejelento címe: ");
    char address[MAXSTRINGSIZE];
    if (fgets(address, MAXSTRINGSIZE, stdin) != NULL)
    {
        address[strlen(address) - 1] = 0;
        strcpy(new.address, address);
    }

    // PHONE
    printf("Bejelento telefonszáma: ");
    char phone[MAXSTRINGSIZE];
    if (fgets(phone, MAXSTRINGSIZE, stdin) != NULL)
    {
        phone[strlen(phone) - 1] = 0;
        strcpy(new.phone, phone);
    }

    // EMAIL
    printf("Bejelento emailje: ");
    char email[MAXSTRINGSIZE];
    if (fgets(email, MAXSTRINGSIZE, stdin) != NULL)
    {
        email[strlen(email) - 1] = 0;
        strcpy(new.email, email);
    }

    // URGENT
    printf("Sürgős-e (1 - igen, 0 - nem): ");
    int urgent;

    if (scanf("%d", &urgent) == 1)
    {
        if (urgent == 1)
        {
            new.urgent = 1;
        }
        else
        {
            new.urgent = 0;
        }
    }
    clear();
    printf("Bejelentés sikeresen létrehozva!\n\n");
    return new;
}

void print(struct Alert a)
{
    printf("név: %s\n", a.name);
    printf("cím: %s\n", a.address);
    printf("telefon: %s\n", a.phone);
    printf("email: %s\n", a.email);
    printf("sürgősség: %d\n", a.urgent);
}

int changeStuff(struct Alert *alerts, int size)
{
    printf("Melyik bejelentést kívánja módosítani?\n");
    printf("Adja meg a bejelentés teljes listázáskor kapott sorszámát:\n ");
    printf("> ");
    int input;
    if (scanf("%d", &input) == 1)
    {
        if (input < 1 || input > size)
        {
            printf("\nNem létezik bejelentés a megadott sorszámmal.\n");
            return 1;
        }
    }
    --input;
    printf("A kiválasztott bejelentés: \n");
    print(alerts[input]);
    printf("\n");

    printf("\nMelyik mezőt kívánja módosítani?\n");
    printf("1 - név\n");
    printf("2 - cím\n");
    printf("3 - telefonszám\n");
    printf("4 - email\n");
    printf("5 - sürgősség\n");
    printf("> ");
    int mezo;
    if (scanf("%d", &mezo) == 1)
    {
        if (mezo < 1 || mezo > 5)
        {
            printf("Nincs ilyen mező!\n");
            return 2;
        }
    }
    clear();
    printf("\nAdja meg az új adatot:\n");
    printf("> ");
    if (mezo != 5)
    {
        char uj[MAXSTRINGSIZE];
        if (fgets(uj, MAXSTRINGSIZE, stdin) != NULL)
        {
            uj[strlen(uj) - 1] = 0;
            switch (mezo)
            {
            case 1:
                strcpy(alerts[input].name, uj);
                break;
            case 2:
                strcpy(alerts[input].address, uj);
                break;
            case 3:
                strcpy(alerts[input].phone, uj);
                break;
            case 4:
                strcpy(alerts[input].email, uj);
                break;
            }
        }
    }
    else
    {
        printf("1- sürgős, 0 - nem sürgős: ");
        int urgent;
        if (scanf("%d", &urgent) == 1)
        {
            if (urgent == 1)
            {
                alerts[input].urgent = 1;
            }
            else
            {
                alerts[input].urgent = 0;
            }
        }
    }
    printf("Bejelentés sikeresen módosítva!\n\n");
    return 0;
}

int deleteAlert(struct Alert *alerts, int size, int input)
{
    int i;
    for (i = input; i < size - 1; ++i)
    {
        alerts[i] = alerts[i + 1];
    }
    return size - 1;
}

int deleteStuff(struct Alert *alerts, int size)
{
    printf("Melyik bejelentést kívánja törölni?\n");
    printf("Adja meg a bejelentés teljes listázáskor kapott sorszámát:\n");
    printf("> ");
    int input;
    if (scanf("%d", &input) == 1)
    {
        if (input < 1 || input > size)
        {
            printf("\nNem létezik bejelentés a megadott sorszámmal.\n");
            return size;
        }
    }
    --input;

    printf("A kiválasztott bejelentés: \n");
    print(alerts[input]);
    printf("\n");

    int sure;

    printf("Biztosan törli? (1 - igen, 0 - mégsem):\n");
    printf("> ");
    if (scanf("%d", &sure) == 1)
    {
        if (sure == 1)
        {
            size = deleteAlert(alerts, size, input);
        }
    }
    printf("Bejelentés sikeresen törölve!\n\n");
    return size;
}

int cmpName(struct Alert a, char *n)
{
    return (strcmp(a.name, n) == 0) ? 1 : 0;
}
int cmpAddress(struct Alert a, char *n)
{
    return (strcmp(a.address, n) == 0) ? 1 : 0;
}
int cmpPhone(struct Alert a, char *n)
{
    return (strcmp(a.phone, n) == 0) ? 1 : 0;
}
int cmpEmail(struct Alert a, char *n)
{
    return (strcmp(a.email, n) == 0) ? 1 : 0;
}
int cmpUrgent(struct Alert a, int n)
{
    return (a.urgent == n) ? 1 : 0;
}

int listStuff(struct Alert *alerts, int size)
{
    printf("\n");
    printf("Válassza ki a listázás módját:\n");
    printf("1 - Teljes listázás\n");
    printf("2 - Szűrés ügyfélre\n");
    printf("3 - Szűrés sürgősségre\n");
    printf("> ");
    int input;
    if (scanf("%d", &input) == 1)
    {
        if (input < 1 || input > 3)
        {
            printf("\nNem létezik ilyen opció.\n");
            return 1;
        }
    }
    clear();
    int i;
    switch (input)
    {
    case 1:
        if (size == 0)
        {
            printf("Nincsen bejelentés a rendszerben!\n");
        }
        for (i = 0; i < size; ++i)
        {
            printf("%d. bejelentés:\n", i + 1);
            print(alerts[i]);
            printf("\n");
        }
        break;
    case 2:
        printf("\nAdja meg a keresett ügyfél nevét:\n");
        printf("> ");
        char name[MAXSTRINGSIZE];
        if (fgets(name, MAXSTRINGSIZE, stdin) != NULL)
        {
            name[strlen(name) - 1] = 0;
        }
        int c = 0;
        for (i = 0; i < size; ++i)
        {
            if (cmpName(alerts[i], name) == 1)
            {
                print(alerts[i]);
                printf("\n");
                ++c;
            }
        }
        if (c == 0)
        {
            printf("Nincsen a szűrésnek megfelelő bejelentés a rendszerben!\n");
        }
        break;
    case 3:
        printf("\nAdja meg a keresett sürgősséget");
        printf("(1- sürgős, 0 - nem sürgős):\n");
        printf("> ");
        int urgent;
        scanf("%d", &urgent);
        clear();
        int d = 0;
        for (i = 0; i < size; ++i)
        {
            if (cmpUrgent(alerts[i], urgent) == 1)
            {
                print(alerts[i]);
                printf("\n");
                ++d;
            }
        }
        if (d == 0)
        {
            printf("Nincsen a szűrésnek megfelelő bejelentés a rendszerben!\n");
        }
        break;
    }
    printf("Listázás sikeresen véget ért\n\n");
    return 0;
}

struct Alert alerts[MAXALERTS];
int alertsSize = 0;
int pipe_alert[2];
int pipe_time_taken[2];
int sentAlertIndex = MAXALERTS + 10;

void madarFogado(int signumber)
{
    printf("\nA madár munkára kész\n");
}

struct Alert pullNextAlert()
{
    if (alertsSize == 0)
    {
        return createAlert(EMPTYALERTNAME);
    }
    int i;
    int ind = 0;
    struct Alert ret_val;
    int found = -1;
    for (i = 0; i < alertsSize && found == -1; ++i)
    {
        if (cmpUrgent(alerts[i], 1) == 1)
        {
            ind = i;
            found = 0;
        }
    }
    sentAlertIndex = ind;
    ret_val = alerts[ind];
    return ret_val;
}

void sendAddress(int signumber)
{
    struct Alert al = pullNextAlert();
    if (strcmp(al.name, EMPTYALERTNAME) != 0)
    {
        write(pipe_alert[1], &al, sizeof(struct Alert));
    }
}

void checkout(int signumber)
{
    struct Alert a = alerts[sentAlertIndex];

    char filename[MAXSTRINGSIZE + 10];
    strcpy(filename, alerts[sentAlertIndex].name);
    int r = rand() % 8000 + 2000;
    char rString[32];
    sprintf(rString, "%d", r);
    char space[] = "_";
    char ext[] = ".txt";
    strcat(filename, space);
    strcat(filename, rString);
    strcat(filename, ext);

    FILE *ptr_receipt = fopen(filename, "ab");

    int receiptFile = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (ptr_receipt != NULL)
    {
        fputs("Számla\n\n", ptr_receipt);

        fputs("Bejelentő neve: ", ptr_receipt);
        fputs(a.name, ptr_receipt);
        fputs("\n", ptr_receipt);

        fputs("Bejelentő címe: ", ptr_receipt);
        fputs(a.address, ptr_receipt);
        fputs("\n", ptr_receipt);

        fputs("Sürgősség: ", ptr_receipt);
        if (a.urgent == 1)
        {
            fputs("sürgős", ptr_receipt);
        }
        else
        {
            fputs("nem sürgős", ptr_receipt);
        }
        fputs("\n", ptr_receipt);

        int time = -10;
        char tString[32];

        read(pipe_time_taken[0], &time, sizeof(time));
        sprintf(tString, "%d", time);
        fputs("Befogás időtartama: ", ptr_receipt);
        fputs(tString, ptr_receipt);
        fputs("\n", ptr_receipt);

        fputs("Fizetendő összeg: ", ptr_receipt);
        fputs(rString, ptr_receipt);
        fputs("\n\n\tKiállította: Ne-Sas-Old Kft.\n", ptr_receipt);

        close(receiptFile);
    }
    else
    {
        printf("Hiba a számla írásban.\n");
    }

    alertsSize = deleteAlert(alerts, alertsSize, sentAlertIndex);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("\nEgy  paraméterrel hívja a programot, ami az adatokat tartalmazó fájl neve legyen.\n");
        printf("Nem létező fájlnevet is meg lehet adni, \n\tekkor a program jelzi, hogy nem találta a fájlt,\n");
        printf("\tde létrehozza, és abba fog menteni.\n\n");
        return 0;
    }

    srand(time(NULL));
    printf("\n");

    int idomar = -20;
    int madar = -30;
    int kozpont = getpid();

    if (pipe(pipe_alert) == -1)
    {
        perror("Hiba a pipe_alert nyitaskor!");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe_time_taken) == -1)
    {
        perror("Hiba a pipe_time_taken nyitaskor!");
        exit(EXIT_FAILURE);
    }

    struct Alert msgAlert;
    int time_taken = 0;

    int i;
    for (i = 0; i < 2; ++i)
    {
        if (getpid() == pid)
        {
            if (i == 0)
            {
                idomar = fork();
                ;
            }
            else if (i == 1)
            {
                madar = fork();
                ;
            }
        }
    }

    if (kozpont == getpid())
    {

        signal(SIGUSR1, madarFogado);
        signal(SIGTERM, sendAddress);
        signal(SIGUSR2, checkout);

        int menu = 1;

        int i;
        int f;
        FILE *ptr_myfile;
        ptr_myfile = fopen(argv[1], "rb");
        if (ptr_myfile)
        {
            while (fread(&(alerts[alertsSize]), sizeof(struct Alert), 1, ptr_myfile) == 1)
            {
                alertsSize++;
            }
            fclose(ptr_myfile);
            printf("sikeresen beolvasva: %d db bejelentés \n", alertsSize);
        }
        else
        {
            printf("Nem található bemeneti fájl, nem történt beolvasás!\n");
        }

        while (menu > 0 && menu < 5)
        {
            struct Alert temp;

            printf("Válassza ki a használni kívánt funkciót!\n");
            printf("1 - Új bejelentés\n");
            printf("2 - Bejelentés módosítása\n");
            printf("3 - Bejelentés törlése\n");
            printf("4 - Bejelenzések listázása\n");
            printf("5 - Kilépés\n");
            printf("> ");
            scanf("%i", &menu);
            clear();
            switch (menu)
            {
            case 1:
                alerts[alertsSize] = readStuff();
                ++alertsSize;
                break;
            case 2:
                changeStuff(alerts, alertsSize);
                break;
            case 3:
                alertsSize = deleteStuff(alerts, alertsSize);
                break;
            case 4:
                listStuff(alerts, alertsSize);
                break;
            }
        }

        ptr_myfile = fopen(argv[1], "wb");
        if (ptr_myfile)
        {
            i = 0;
            while (i < alertsSize)
            {
                fwrite(&(alerts[i]), sizeof(struct Alert), 1, ptr_myfile);
                ++i;
            }
            fclose(ptr_myfile);
        }
        else
        {
            printf("Hiba az írásban.\n");
        }

        kill(madar, SIGKILL);
        kill(idomar, SIGKILL);
    }
    else if (0 == idomar)
    {
        close(pipe_time_taken[0]);

        struct Alert receivedAlert;
        sleep(1);
        kill(getppid(), SIGUSR1);
        sleep(1);
        kill(getppid(), SIGTERM);

        sleep(1);
        //int q = 0;
        while (1)
        { //q < 5
            read(pipe_alert[0], &receivedAlert, sizeof(struct Alert));

            /* DEBUG
            if(strcmp(receivedAlert.name,EMPTYALERTNAME) == 0){
                printf("pipe_alert is empty\n");
                write(pipe_alert[1],&EmptyAlert,sizeof(struct Alert));
            }else{
                printf("pipe_alert is not empty, contents:\n");
                print(receivedAlert);
                write(pipe_alert[1],&EmptyAlert,sizeof(struct Alert));
            }*/

            printf("\n Idomár: Cím megérkezett : %s\n", receivedAlert.address);

            time_taken = rand() % 5 + 1;
            sleep(time_taken);
            write(pipe_time_taken[1], &time_taken, sizeof(time_taken));
            kill(getppid(), SIGUSR2);
            sleep(1);
            kill(getppid(), SIGTERM);
        }
    }
    else if (0 == madar)
    {
    }
    return 0;
}