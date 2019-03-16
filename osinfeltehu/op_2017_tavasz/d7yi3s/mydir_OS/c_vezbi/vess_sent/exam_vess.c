#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

struct message{
	long mtype;
	int results[10];
};

int send(int mqueue, int res[10]) 
{ 
	int r[10];
	int i;
	for(i=0; i<10; i++){
		r[i]=res[i];
	}

    const struct message m = { 5, *r }; 
    int status; 
     
    status = msgsnd( mqueue, &m, sizeof(m.results), 0); 
	
    if(status < 0){ 
        perror("msgsnd error");
	}		  
     return 0; 
} 

int receive( int mqueue) 
{ 
    struct message m; 
    int status; 
   
    status = msgrcv(mqueue, &m, 10*sizeof(int), 5, 0 ); 
     
	printf("Results of the voting!\n");
	 
    if(status < 0 ){ 
        perror("msgrcv error");
	}		  
    else{
		int i;
		for(i=0; i<10; i++){
			printf( "%d\n", m.results[i] ); 
		}
	}
    return 0; 
} 

void handler1(int sig){
	printf("Child one ready to work!\n");
	signal(sig, handler1);
}

void handler2(int sig){
	printf("Child two ready to work!\n");
	signal(sig, handler2);
}

int main(int argc, char *argv[]){
	
	signal(SIGUSR1, handler1);
	signal(SIGUSR2, handler2);
	
	srand(time(NULL));
	
	pid_t child1, child2;
	int number_of_voters=atoi(argv[1]);
	
	int messg, status; 
    key_t key; 
	
	key = ftok(argv[0],1); 

    messg = msgget(key, 0600 | IPC_CREAT); 
    if ( messg ==-1 ) { 
        perror("msgget error"); 
        return 1; 
    } 
	
	int pipe1[2];
	int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR );

	if(pipe(pipe1)==-1 || fid==-1){
		perror("Error in creating pipe!\n");
		exit(EXIT_FAILURE);	
	}

	child1=fork();
	
	if (child1<0){perror("The fork calling was not succesful\n"); exit(1);} 
	
	else if (child1>0) {
		child2=fork();
	}
	if (child2<0){perror("The fork calling was not succesful\n"); exit(1);}
	
	if(child1==0){
		sleep(1);
		kill(getppid(), SIGUSR1);	
		
		int fd=open("fifo.ftc",O_WRONLY);
		close(pipe1[1]); //close the write end
		int ID[number_of_voters];
		int can_vote[number_of_voters][2];
		
		read(pipe1[0], &ID, sizeof(ID));
		int i,j;
		for(i=0; i<number_of_voters; i++){
			printf("%d\n", ID[i]);
		}
			
		for(i=0; i<number_of_voters; i++){
			int chance=rand()% 5+1;
			if(chance==3){
				can_vote[i][0]=ID[i];
				can_vote[i][1]=0;
			}else{
				can_vote[i][0]=ID[i];
				can_vote[i][1]=1;
			}	
		}
		write(fd, &can_vote, sizeof(can_vote));
		close(pipe1[0]); //close the read end
		close(fd);
		exit(0);
	}else if(child2==0){
		sleep(2);
		kill(getppid(), SIGUSR2);
		
		close(pipe1[1]);
		close(pipe1[0]);
		int fd=open("fifo.ftc",O_RDONLY);
		int can_vote[number_of_voters][2];
		int i;
		int r[10];
		
		read(fd, &can_vote, sizeof(can_vote));
		
		for(i=0; i<number_of_voters; i++){
			if(can_vote[i][1]==1){
				int vote=rand()%6+1;
				r[i]=vote;
				printf("****%d****\n",r[i]);
			}
		}
		
		for(i=0; i<number_of_voters; i++){
			printf("Voter %d %s.\n", can_vote[i][0], (can_vote[i][1]==0) ? "cannot vote" : "can vote");
		}
		send(messg, r); 
		status = msgctl(messg, IPC_RMID, NULL); 
		
        if(status < 0){ 
			perror("msgctl error"); 
		}
		
		close(fd);
		unlink("fifo.ftc");
		exit(0);
	}else{
		pause();
		pause();
		
		close(pipe1[0]); //close the read end
		
		int ID[number_of_voters];
		int i;
		
		for(i=0; i<number_of_voters; i++){
			ID[i]=rand() % 1000+1001;
		}
		write(pipe1[1], &ID, sizeof(ID));
		close(pipe1[1]); //close the write end
		
		return receive(messg);
	}

	return 0;
}