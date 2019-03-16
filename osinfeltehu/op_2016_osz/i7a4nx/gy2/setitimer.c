// alarm() fv nel joval komplexebb, fejlettebb a setitimer()

//ha az alarm() utani sorokat kikommentezzuk, akkor van egy jelzeskezelonk oszvissz
//majd le fog futni a handlerunk
//a sleep(2) arrol gondoskodik hogy letezzen meg a programunk
//az alarm() nem ismetlodo idozito

//feladat: ennek a handler fv ebe bele kellene irni a datum es ido kiolvasasat
//szoval ora szeruseget kellene csinalni
//time() fv c_time() fv szoveges tipussa alakitashoz

//MJ
//a handlerunkben ki lehet olvasni a jelzest kuldo pid szamat
//4bajt adatot is kuldhetunk a jelzes mellett...
//union sigval s_value_int7(5);
//sigqueue(...);

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

int count=0;
struct itimerval timer;

time_t current_time;
char* c_time_string;

void handleralarm_time(int signalnumber){

 current_time = time(NULL);
 c_time_string = ctime(&current_time);
 
 //printf(...)
}


void handleralarm(int signalnumber){
 printf("The program was alarmed by alarm function\n");
}

//a masodjara hasznalt timer fv
// count=5 nel kinullazzuk az idozitot es sigtermet dobunk magunknak
void handlersetitimer(int signalnumber)
{ count++;
  printf("The program was alarmed by setitimer the %i. time\n",count);
  if (count==5) {
    timer.it_interval.tv_sec=0; /* it will be stopped */
    timer.it_interval.tv_usec=0;
    
    raise(SIGTERM); //it will stop process
    // raise a signal and send it to the process itself
  }
}

int main(){
 
 //1. trial of creating a timer by alarm function
 signal(SIGALRM,handleralarm);
 alarm(1); //it is implemented in POSIX systems - but it is adviced to use setitimer
 sleep(2); //waiting the end of alarm BUT usually do not use sleep and alarm or setitimer together - may interference
 printf("\n\n");
 
 
 //2. trial of creating a timer by setitimer function
 struct sigaction sigact;
 sigemptyset(&sigact.sa_mask); //no blocked signals only the one, which arrives
 
 //jelzeskezelo beallitasa
 sigact.sa_handler=handlersetitimer;
 sigact.sa_flags=0; //no special behaviour
 sigaction(SIGALRM,&sigact,NULL); //an alarm signal is set

 //idozito beallitasa (2.par ban kapja lejjebb a setititmer())
 timer.it_interval.tv_sec=3; /* it will be repeated after 3 seconds */
 timer.it_interval.tv_usec=0;
 timer.it_value.tv_sec=1;    /* remaining time till expiration */
 timer.it_value.tv_usec=0;       

 //valosideju idozitest kerunk azaz 	tenylegesen mennyi ido telik el a 2 idozites kozott:
 //nem a cpu ban levo idozitest szeretnenk figyelembe venni, az terheltsegtol fugghet stb
 setitimer(ITIMER_REAL,&timer,NULL ); //result = 0, if it is good  
  //when expires, a signal will be sent to the process, and it restarts
  //1. parameter ITIMER_REAL - real time, ITIMER_VIRTUAL (during process execution)
  //2. parameter timer 
		//idozito beallitasa
  //3. old timer value
  
 struct itimerval expires;
 //folyamtosan kuldozgeti a sigalarm jelzeseket
 while (1){
  ;
  sleep(1); //be careful with sleep
  getitimer(ITIMER_REAL,&expires);//reads the remaining time 
  printf("\tTimer will be expired after %i seconds\n", expires.it_value.tv_sec);
 }
 
 return 0;
}