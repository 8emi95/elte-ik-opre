#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <unistd.h>
#include <mqueue.h>

void utasJelentkezes(int signum){
    printf("Beerkezo jelentkezes erkezett a kozpontba (%d)\n",signum);
}

/*void getCim(char *buf){
    //65-122 48-57
    char buf[50];
    int i,l = rand()%40+8;
    for(i=0;i<l;i++){
        buf[i] = (rand()%58)+65;
    }
    
    strcpy(buf," ");
    strcpy(buf,rand()%9+1);
    
    return buf;
}*/

int main(int argc, char** args){
    srand(time(NULL));
    
    struct sigaction sigact;
    sigact.sa_handler = utasJelentkezes;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGUSR1,&sigact,NULL);

    int cso[2][2];
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
        
        pause();
        
        char* mqname = "/radio";
        struct mq_attr attr;
        mqd_t mqdes1, mqdes2;

        attr.mq_maxmsg = 5;
        attr.mq_msgsize = 100;
        mq_unlink(mqname);

        mqdes1 = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr);
        
        pid_t taxi = fork();
        if( taxi == -1 ){
                perror("Error creating child process");
                exit(3);
        }
        if( taxi != 0 ){
            // Parent
            mqdes2 = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr);
            
            read(cso[1][0],buff,sizeof(buff));
            printf("Az utas lakcime: %s\n",buff);
            long tel;
            read(cso[1][0],&tel,sizeof(tel));
            char tel_str[50];
            sprintf(tel_str,"%ld",tel);
            //printf("Teleelele: %ld\n",tel);
            
            mq_send(mqdes2,buff,sizeof(buff),10);
            mq_send(mqdes2,tel_str,sizeof(tel_str),10);
            mq_close(mqdes2);
            
            char mq_buff[100];
            mq_receive(mqdes1,mq_buff,sizeof(mq_buff),0);
            
        }else{
            // Child
            char mq_buff[100];
            
            mq_receive(mqdes1,mq_buff,sizeof(mq_buff),0);
            printf("Cel: %s\n",mq_buff);
            sleep(1);
            mq_receive(mqdes1,mq_buff,sizeof(mq_buff),0);
            printf("Telefonszam: %s\n",mq_buff);
            
            mqdes2 = mq_open(mqname, O_CREAT|O_RDWR, 0600, &attr);
            mq_send(mqdes2,"Elindultam!\n",12,10);
            
            mq_close(mqdes2);
        }
        
        mq_close(mqdes1);
        mq_unlink(mqname);
        
        //write(cso[0][1],,sizeof());

        close(cso[0][1]);
        close(cso[1][0]);
        fflush(NULL);
        wait(NULL);
    }else{
        // Child
        close(cso[0][1]);
        close(cso[1][0]);

        sleep((rand()%3)+2);
        kill(getppid(),SIGUSR1);
        
        //getCim(buff);
        write(cso[1][1],args[1],sizeof(args[1]));
        write(cso[1][1],(long*)&pid,sizeof(long));
        

        //read(cso[0][0],,sizeof());

        close(cso[0][0]);
        close(cso[1][1]);
    }

    return 0;
}