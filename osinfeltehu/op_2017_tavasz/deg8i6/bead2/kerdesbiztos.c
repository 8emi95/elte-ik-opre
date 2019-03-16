#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#include "kerdesbiztos.h"
#include "common.h"

struct kerdes{
	char kerdes[MAX_KERDES_LENGTH];
        int responses;
        char valasz[MAX_VALASZ_COUNT][MAX_VALASZ_LENGTH];
};

void kiutazas(){
    printf("A kerdezobiztos uton van a centrumba.\n");
    sleep(rand()%4+1);
}

void parentSaysChildHasArrived(int signum){
    printf("A kerdezobiztos odaert a varosba (%d)\n",signum);
}

int kerdes(struct kerdes *q){
    char filename[30], buf[100];
    
    printf("Add meg a kerdoiv nevet: ");
    scanf("%s",buf);
    sprintf(filename,"veglegesitett/%s",buf);
    
    if( checkFileExistence(filename) == false ){
        printf("Nem letezik ilyen kerdoiv!\n");
        return;
    }
    
    FILE *file = fopen(filename,"r");
    
    if( file == NULL ){ perror("ERROR: Could not open file for read"); return; }
    
    int questions = 0;
    fgets(buf,sizeof(buf),file);
    
    while( fgets(buf,sizeof(buf),file) ){
        if( buf[0] == '\n' ) continue;
        
        strcpy(q[questions].kerdes,buf);
        
        q[questions].responses = 0;
        while( fgets(buf,sizeof(buf),file) ){
            if( buf[0] == '\n' ) break;
            strcpy(q[questions].valasz[q[questions].responses++],buf);
        }
        questions++;
    }
    
    fclose(file);
    return questions;
}

void getKerdesek(int c, int *ids){
    if( c == 1 ){ ids[0] = 0; ids[1] = 0; ids[2] = 0; }
    else if( c == 2 ){ ids[0] = 0; ids[1] = 1; ids[2] = rand()%2; }
    else{
        int i;
        for(i=0;i<3;i++){
            if( i != 0 ) ids[i] = rand()%(c-(2-i)-(ids[i-1]+1))+ids[i-1]+1;
	    else ids[i] = rand()%(c-(2-i));
        }
    }
}

int getValaszok(int v[20][3],struct kerdes *q){
    int i,j,c = rand()%11+10;
    
    for(i=0;i<c;i++) for(j=0;j<3;j++){
        v[i][j] = rand()%q[j].responses;
    }
    
    return c;
}

void valaszokKiir(struct kerdes *q,int vk[20][3],int vc){
    int valok[3][MAX_VALASZ_COUNT];
    
    int i,j;
    for(i=0;i<3;i++) for(j=0;j<MAX_VALASZ_COUNT;j++) valok[i][j] = 0;
    for(i=0;i<vc;i++) for(j=0;j<3;j++) valok[j][vk[i][j]]++;
    
    for(i=0;i<3;i++){
        printf("%s",q[i].kerdes);
        for(j=0;j<q[i].responses;j++){
            printf("%d: %s (%d)\n",j+1,q[i].valasz[j],valok[i][j]);
        }
        printf("\n");
    }
    
}

int biztosKuld(){
    srand(time(NULL));
    
    struct sigaction sigact;
    sigact.sa_handler = parentSaysChildHasArrived;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGUSR1,&sigact,NULL);

    int cso[2][2],y;
    pid_t pid;
    char buff[100];

    if( pipe(cso[0]) == -1 ){
            perror("Error opening pipe");
            exit(1);
    }

    if( pipe(cso[1]) == -1 ){
            perror("Error opening pipe2");
            exit(1);
    }

    pid = fork();
    if( pid == -1 ){
            perror("Error creating child process");
            exit(2);
    }

    if (pid != 0){
        // Parent
        close(cso[0][0]);
        close(cso[1][1]);

        struct kerdes q[MAX_KERDES_COUNT];
        int qc = kerdes(q);

        int nums[3]; getKerdesek(qc,nums);

        int i;
        for(i=0;i<3;i++){
            write(cso[0][1],&q[nums[i]],sizeof(q[0]));
        }

        int valaszolt,valaszok[20][3];
        read(cso[1][0],&valaszolt,sizeof(valaszolt));
        read(cso[1][0],valaszok,sizeof(valaszok));
        
        valaszokKiir(q,valaszok,valaszolt);

        close(cso[0][1]);
        close(cso[1][0]);
        fflush(NULL);
        //wait(NULL);
    }else{
        struct kerdes q[3];
        int valaszok[20][3];
        // Child
        sleep(3);
        close(cso[0][1]);
        close(cso[1][0]);

        kiutazas();
        kill(getppid(),SIGUSR1);

        read(cso[0][0],q,sizeof(q[0])*3);
        int vc = getValaszok(valaszok,q);
        
        write(cso[1][1],&vc,sizeof(vc));
        write(cso[1][1],valaszok,sizeof(valaszok));

        close(cso[0][0]);
        close(cso[1][1]);
    }

    return 0;
}