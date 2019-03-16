#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

//feladat: a szulo kuldjon egy jelzest a gyerekfolyamatnak, a gyerek pedig kuldjon vissza egy jelzest a szulonek es a szulo jelenitsen emg egy uzenetet hogy megkapta azt a jelzest

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  //a default handler: amikor megerkezik a jelzes, akkor alljon le
  //signal(SIGTERM,SIG_DFL);
  //a folyamat mar sajat maganak dobhasson egy jelzest:
  //raise(SIGTERM);
  //ennek a hatasara elkuldte a signalt, es nem fut le a pause() utani resz
}

// ./a.out & 		háttérben futtatás
// kill -SIGTERM 29231
// pause() jelzésre várakozás, de sigtermnél nem áll meg hanem lefut a jelzéskezelő

int main(){

//leallas helyett a handler fut le:    
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  
  //SIG_IGN a masik default jelzeskezelo
  //elkuldi a gyerekfolyamat a jelzes, a pausenal a szulo varja a jelzest, de a  sig ignore hatasara ignoralodik
  
  pid_t child=fork();
  if (child>0)
  { 
	//ha itt lenne az else helyett a sleep3 akkora szülő hiába várna ajelzésre mert az már korábban megérkezett
	//sleep(3);
    pause(); //waits till a signal arrive 
    printf("Signal arrived\n",SIGTERM);
    wait(&status);
    int status;
    printf("Parent process ended\n");
  }
  //vár egy jelzésre és vár a gyerekfolyamat végére
  else 
  {
    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
    sleep(3);
	//getppid() adott folyamat pid számát lekérdezi, a szülőfolyamatnak akar küldeni egy jelzést
	//a jelzésig vár a szülőfolymaat a pasue()nál
    kill(getppid(),SIGTERM); 
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    printf("Child process ended\n");  
  }
  return 0;
}