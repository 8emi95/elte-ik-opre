fork.c

#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
wait, waitpid: gyerek folyamat végére vár

signal.c, signal1.c

pause(): várakozik egy jelzésre (ha egy jelzés korábban megérkezik, mint ahogy várja a szülő, akkor nem fut tovább a program)
raise(): a folyamat saját magának dobhat jelzést lsd: signal1.c


Háttérben indítás eéindítása:
./a.out &
amit kiír, job szám, pid szám

folyamat megnézés:
ps aux | grep wgknro


getppid(): szülő folyamat azonosítójának lekérése

pl:
#include <signal.h>

pid_t child_pid = -1 ; //Global

void kill_child(int sig)
{
    kill(child_pid,SIGKILL);
}

int main(int argc, char *argv[])
{
    signal(SIGALRM,(void (*)(int))kill_child);
    child_pid = fork();
    if (child_pid > 0) {
     /*PARENT*/
        alarm(30);
        /*
         * Do parent's tasks here.
         */
        wait(NULL);
    }
    else if (child_pid == 0){
     /*CHILD*/
        /*
         * Do child's tasks here.
         */
    }
}

Beragadt folyamat kilövése:
kill PIDSZÁM


sigprocmask.c

Blokkolásos üzi
blokkolás feloldása után megmaradnak az addig beérkezett üzenetek
