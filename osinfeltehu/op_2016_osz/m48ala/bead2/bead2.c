#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

void rendezvenyStart(const char* rendezveny);

void handler(int signumber) {
    //printf("Megerkezett a signal!\n");
}

struct Member {
	char nev[100];
	int resztvett; // 0 - nem, 1 -igen
};

struct MemberLista {
	struct Member member[1024];
	int db;
};


int main(int argc, char **argv)
{

    printf("|||---------------------\n");
    printf("Rendezveny kezdete\n");

    signal(SIGUSR1,handler);

    int q;
    char rendezveny[100];

    
    //printf("Melyik rendezveny kezdodik? : ");
    //scanf("%s", &rendezveny);

    //ha nem létezik az event legyen vége a programnak
    FILE *rend;
    if ((rend = fopen("rendezveny.txt", "r")) == NULL)
    {
        printf("Nem letezik ilyen rendezveny!\n");
        exit(1);         
    }
    else
    {
        fclose(rend);
        rendezvenyStart(rendezveny);
    }

    return 0;
}

void rendezvenyStart(const char* rendezveny){
    int myPipe1[2];
    int myPipe2[2];
    char buff[100];

    /*struct sigaction sigact;
    sigact.sa_handler=handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags=0;
    sigaction(SIGUSR1,&sigact,NULL);*/


    if ((pipe(myPipe1)) < 0)
    {
        perror("Hiba a pipe nyitasakor!\n");
        exit(EXIT_FAILURE);
    }
    
    if ((pipe(myPipe2)) < 0)
    {
        perror("Hiba a pipe nyitasakor!\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Fork hiba!\n");
        exit(EXIT_FAILURE);
    }

    /* parent process */
    if (pid > 0)
    {
        printf("Szulo indul!\n");

        close(myPipe1[0]);
        close(myPipe2[1]);


        /*sigset_t sigset;
        sigfillset(&sigset);
        sigdelset(&sigset,SIGUSR1);*/

        FILE *rend;
        if ((rend = fopen("rendezveny.txt", "r")) == NULL)
        {
            printf("Szulo /-/ Nem létezik ilyen fajl!\n");
            exit(1);         
        }


        char buffr[10];

        char rendNev[512];
        int  rendAzon;
        char line[512];
        while(fgets(line,100,rend) != NULL){
            if(strcmp(line,"") != 0){
                sscanf(line,"%d %s",&rendAzon, rendNev);
                
                char sor[512];
                char sorNev[512];
                char sorMain[512];
                int  rendAzon2;
                
                int dbMember = 0;
                struct MemberLista membersList;

                sprintf(buffr, "%d", rendAzon);
                
                FILE *members;
                members = fopen("members.txt","r");
                
                while(fgets(sor,100,members) != NULL)
                {
                    sscanf(sor,"%s %s %d", sorNev, sorMain, &rendAzon2);
                    if(rendAzon2 == rendAzon){
                        strcpy(membersList.member[dbMember].nev, sorNev);
                        dbMember++;
                    }
                }
                membersList.db = dbMember;
                fclose(members);	

                printf("-------------------------------\n");
                printf("Szulo /-/ rendezveny neve: %s\n", rendNev);

                write(myPipe1[1], buffr, sizeof(buffr));
                fflush(NULL);
                
                pause();
                printf("Szulo /-/ megkapta a signalt\n");
                
                // Elküldi a gyereknek, hogy ki a résztvevõk
                printf("Szulo /-/ elkuldi a gyerkenek a vendegeket\n");
                write(myPipe1[1], &membersList, sizeof(membersList));
                fflush(NULL);
                
                char buffer[10];
                read(myPipe2[0],buffer, sizeof(buffer));
                
                printf("Szulo /-/ megkapta a rendezveny sikeret\n");
                printf("Szulo /-/ a rendezveny sikere: %s\n", buffer);
                
                struct MemberLista vissza_membersList;
                read(myPipe2[0], &vissza_membersList, sizeof(vissza_membersList));
                printf("Szulo /-/ megkapta a rendezvenyen levok listajat\n");
                printf("Szulo /-/ akik nem vettek reszt a rendezvenyen:\n");
                int i;
                for(i = 0; i < vissza_membersList.db; i++) {
                    //printf("%s\n",vissza_membersList.member[i].nev);
                    if(vissza_membersList.member[i].resztvett == 0) {
                        printf("Szulo /-----> %s\n", vissza_membersList.member[i].nev);
                    }
                }
                printf("Szulo /-/ mindent kiirt, johet a kovetkezo rendezveny, ha van\n");					
            }
        }
        

        strcpy(buffr, "end");
		//buffr = "vege\0\0\0\0\0\0";
		write(myPipe1[1], buffr, sizeof(buffr));
		fflush(NULL);
		
		fclose(rend);
		
		close(myPipe1[1]);
		close(myPipe2[0]);
		
		int status;
		waitpid(pid, &status, 0);
		
		printf("Szulo vege!\n");
    }
    else{  //child process
        printf("Gyerek indul!\n");

        int szuloPid = getppid();
        
        close(myPipe1[1]);
        close(myPipe2[0]);
        
        char buffr[10];
        struct MemberLista membersList;
        
        while(1 == 1){
            read(myPipe1[0], buffr, sizeof(buffr));
            if(strcmp(buffr,"end") == 0){
                printf("GYEREK /-/ megkapta a veget jelzo stringet -- vege\n");
                break;
            }
            fflush(NULL);
            //printf("-------------------------------\n");
            printf("GYEREK /-/ fut\n");
            printf("GYEREK /-/ megkapott rendezveny azonositoja: %s\n", buffr);
            
            printf("GYEREK /-/ utazik a rendezvenyre\n");
            sleep(2);
            printf("GYEREK /-/ megerkezett a rendezvenyre, errol ertesiti a szulot\n");
            kill(szuloPid, SIGUSR1);
            printf("GYEREK /-/ var a vengeg listra\n");
            read(myPipe1[0], &membersList, sizeof(membersList));
            fflush(NULL);
            printf("GYEREK /-/ megkapta a vendegek listajat\n");
            printf("GYEREK /-/ vendegek:\n");
            
            
            int i;
            for(i = 0; i < membersList.db; i++) {
                printf("\t%s\n", membersList.member[i].nev);
                int random = rand() % 10;
                if(random == 1 || random == 5 || random == 9) {
                    membersList.member[i].resztvett = 0;
                }
                else{
                    membersList.member[i].resztvett = 1;
                }
            }

            
            printf("GYEREK /-/ levezeti a rendezvenyt\n");
            sleep(2);
            printf("GYEREK /-/ levezette a rendezvenyt\n");
            
            printf("GYEREK /-/ jelentest kuld a sikerrol\n");
            int siker =  rand() % 100 + 1;
            char buffr1[10];
            sprintf(buffr1, "%d", siker);

            write(myPipe2[1], buffr1, sizeof(buffr1));
            fflush(NULL);

            write(myPipe2[1], &membersList, sizeof(membersList));
            fflush(NULL); 
        }
        close(myPipe1[0]);
        close(myPipe2[1]);
        
        printf("Gyerek vege!\n");
        sleep(2);
    }
}