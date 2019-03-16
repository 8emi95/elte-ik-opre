#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>

int alma=5;

void sighandler(int type){
    switch(type){
	case SIGTERM: printf("SIGTERM signalt kaptam, kodja:%i\n",type);
		      break;
	default: printf("Nem SIGTERM signalt kaptam, kodja:%i\n",type);		      
    }
	printf("Alma erteke: %d\n",alma);	
}


int main(){    
    int child;

    signal(SIGTERM,sighandler);
    signal(SIGUSR1,sighandler);
//    signal(SIGINT,SIG_IGN);
    child=fork();
    
    if(child>0){
	sleep(3);
	kill(child,SIGTERM);
	kill(getpid(),SIGTERM);
	sleep(3);
	kill(child,SIGUSR1);
	signal(SIGINT,SIG_DFL);
	sleep(3);
	printf("Alma erteke a szulo vegen: %d\n",alma);	
    }else if(child==0){
	int i;
	for(i=0;i<14;++i){
	    printf("%d\n",i);
	    sleep(1);alma++;
	}
    }

    return 0;
}
