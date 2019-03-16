// 1. Inditsunk egy programbol pontosan 3 folyamatot (szulot beleertve).
// 2. Inditsunk egy programbol pontosan 10 folyamatot (szulot beleertve).
// 3. Inditsunk egy programbol pontosan argv[1] folyamatot (szulot belertve).
// 4. Inditsunk egy programbol pontosan 3 folyamatot, es adjunk nekik kulonbozo feladatokat! Az egyik hozzon letre egy fajlt, masik torolje (akar letezik akar nem), harmadik pedig listazza ki az aktualis konyvtar tartalmat. (Segitsegnek linux parancsok: "touch file", "rm file" "ls -la .") 
// 5. Inditsunk egy programbol pontosan ket folyamatot, egyik irja bele egy fajlba argv[1]-et, masik olvassa ki es irja ki a kepernyore (masiknak celszeru megvarnia, amig az egyik vegez).

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 
#include <string.h>
#include <fcntl.h>

void elso()
{
    pid_t child1 = fork();
    if (child1 < 0)
    {
        printf("Error.\n");
    }
    else if (child1 > 0) // szülő
    {
        pid_t child2 = fork();
        if (child2 < 0)
        {
            printf("Error\n");
        }
        else if (child2 > 0)
        {
            printf("Parent. Pid:%i pPid:%i\n", getpid(), getppid());
        }
        else
        {
            printf("Child 2  Pid:%i pPid:%i\n", getpid(), getppid());
        }
    }
    else
    {
        printf("Child 1  Pid:%i pPid:%i\n", getpid(), getppid());
    }
}

void masodik()
{
    int i = 0;
    int is_parent = 0;
    while (i++ < 9 && is_parent == 0)
    {
        pid_t child = fork();
        if (child < 0)
        {
            printf("Error\n");
            return;
        }
        else if (child > 0)
        {
            // still in the parent;
        }
        else
        {
            printf("child %i\n", i);
            is_parent = 1;
        }
    }
}

void harmadik(int szam)
{
    int i = 0;
    int is_parent = 0;
    while (i++ < szam - 1 && is_parent == 0)
    {
        pid_t child = fork();
        if (child < 0)
        {
            printf("Error\n");
            return;
        }
        else if (child > 0)
        {
            // still in the parent;
        }
        else
        {
            printf("child %i\n", i);
            is_parent = 1;
        }
    }

}

// Erdekesseg: a konyvtar tartalamanak listazasanak eredmenye 2-fele lehet,
// attol fuggoen, hogy a 6-fele sorrendbol melyik kovetkezik be (3 folyamat,
// barmilyen sorrendben futhatnak, 6-fele sorrend).
void negyedik()
{
    pid_t elso_child = fork();
    if (elso_child < 0)
    {
        printf("Error\n");
        return;
    }
    else if (elso_child > 0)
    {
        pid_t masodik_child = fork();
        if (masodik_child < 0)
        {
            printf("Error\n");
        }
        else if (masodik_child > 0)
        {
            printf("Parent\n");
            system("touch file");
            int status;
            waitpid(masodik_child, &status, 0);
            waitpid(elso_child, &status, 0);
        }
        else
        {
            printf("Masodik child\n");
            system("rm file");
        }
    }
    else
    {
        printf("Elso child\n");
        system("ls -la .");
    }
}

void otodik(char* data)
{
    FILE* handle;
    pid_t child = fork();
    if (child < 0)
    {
        // error
        return;
    }
    else if (child == 0) // child process ir fajlba
    {
        FILE* handle_write = fopen("output.txt", "w");
        if (handle_write == 0)
        {
            printf("File error.\n");
            return;
        }
        fputs(data, handle_write);
        fclose(handle_write);
    }
    else // parent olvas, es kiir kepernyore
    {
        int status = 0;
        waitpid(child, &status, 0);
        FILE* handle_read = fopen("output.txt", "r");
        if (handle_read == 0)
        {
            printf("File error.\n");
            return;
        }
        char* buffer = malloc(sizeof(char)*(strlen(data) + 1));
        fgets(buffer, strlen(data) + 1, handle_read);
        printf("%s\n", buffer);
        fclose(handle_read);
    }
}

int main(int argc, char** argv)
{
    // elso();
    // masodik();
    /*if (argc > 1)
    {
       int szam = atoi(argv[1]);
       harmadik(szam);
    }*/
    //negyedik();
    if (argc > 1)
    {
        otodik(argv[1]);
    }
    return 0;
}
