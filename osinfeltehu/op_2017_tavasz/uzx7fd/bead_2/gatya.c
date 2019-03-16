#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

int kerdesSzamol();
void kerdestKivalaszt(int a, int b, int c);
void valasztOsszegyujt(char *s[]);
void eredmenyKiir(char *s[]);

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(int argc, char *argv[]){
  // SIGNAL
  signal(SIGTERM,handler); 
  // PIPE
  int pipefd[2];
  pipe(pipefd);
  char sz[100];


  pid_t child=fork();
  // MLSZ, PARENT PROCCESS
  if (child>0)
  { 
    // COUNT HOW MANY QUESTIONS ARE THERE IN VEGLEGES.TXT
    int c = kerdesSzamol("vegleges.txt") + 1;
    // GENERATE 3 RANDOM NUMBERS, THESE WILL BE THE QUESTIONS' INDICES
    srand ( time(NULL) );
    int num1 = 0;
    while(num1 == 0) {
        num1 = rand() % c;     
    }    
    int num2 = 0;
    while(num2 == 0 || num1 == num2) {
        num2 = rand() % c;           
    }
    int num3 = 0;
    while(num3 == 0 || num2 == num3 || num1 == num3) {
        num3 = rand() % c;        
    }
    // SELECT THE 3 QUESTIONS FROM VEGLEGES.TXT, WRITE THEM TO KIV.TXT
    kerdestKivalaszt(num1, num2, num3);
    printf("MLSZ: Várok a kérdezőbiztos signaljára\n");
    // SEND FILENAME THROUGH PIPE
    write(pipefd[1], "kiv.txt",7);
    close(pipefd[1]);
    // WAITING FOR CHILD SIGNAL
    pause();
    // SIGNAL ARRIVED
    printf("MLSZ: Megkaptam a signalt, kérdezőbiztos, indulj %s városba\n", argv[1]);
    int status;
    wait(&status);
    // READ FILENAME WITH ANSWERS FROM PIPE
    read(pipefd[0],sz,sizeof(sz));
    printf("MLSZ: Megkaptam a válaszokat a kérdezőbiztostól\n");
    // WRITE ANSWERS FROM ER.TXT
    eredmenyKiir(sz);
    printf("MLSZ: Befejezem a működést\n");
  }
  else 
  {
    // SEND SIGNAL TO PARENT, SO IT CAN START IT'S WORK
    sleep(1);
    printf("Kérdezőbiztos: küldöm a signalt\n",SIGTERM);    
    kill(getppid(),SIGTERM); 
    sleep(3);
    printf("Kérdezőbiztos: Készen állok a kiküldetésre\n");
    // READ FILENAME WITH 3 QUESTIONS
    read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
    // GET 10-20 RANDOM ANSWERS FOR THE 3 QUESTIONS
    valasztOsszegyujt(sz);
    // SEND ANSWER FILENAME BACK TO PARENT
    write(pipefd[1], "er.txt",6);
    printf("Kérdezőbiztos: Megvannak a válaszok\n");
    printf("Kérdezőbiztos: Befejezem a működést\n");  
  }
  return 0;
}

int kerdesSzamol() {
    FILE *input = fopen("vegleges.txt", "r");
    char sor[150];
    int counter = 0;
    while(fgets(sor, sizeof(sor), input)) {
        //printf("kerdes: %s", sor);
        counter++;
        fgets(sor, sizeof(sor), input);
        int szam = sor[0] - '0';
        for(int i = 0; i < szam; i++) {
            fgets(sor, sizeof(sor), input);
            //printf("valasz %i.:%s", i + 1, sor);
        }
        //printf("\n");
    }
    fclose(input);
    return counter;
}

void kerdestKivalaszt(int a, int b, int c) {
    FILE *input = fopen("vegleges.txt", "r");
    FILE *output = fopen("kiv.txt", "w+");
    char sor[150];
    int counter = 0;
    while(fgets(sor, sizeof(sor), input)) {
        counter++;
        if(counter == a || counter == b || counter == c) {
            fputs(&sor, output);
            fgets(sor, sizeof(sor), input);
            int szam = sor[0] - '0';
            fputs(&sor, output);
            for(int i = 0; i < szam; i++) {
                fgets(sor, sizeof(sor), input);
                //printf("valasz %i.:%s", i + 1, sor);
            }
            //printf("\n");
        }
        else {
            fgets(sor, sizeof(sor), input);
            int szam = sor[0] - '0';
            for(int i = 0; i < szam; i++) {
                fgets(sor, sizeof(sor), input);
                //printf("valasz %i.:%s", i + 1, sor);
            }
        }
    }
    fclose(input);
    fclose(output);
}

void valasztOsszegyujt(char *s[]) {
    srand ( time(NULL) );

    int num1 = 0;
    while(num1 == 0) {
        num1 = rand() % 11 + 10;  
    }
    FILE *input = fopen(s, "r");
    FILE *uj = fopen("er.txt", "w+");
    char sor[150];
    for(int i = 0; i < 3; i++) {
        fgets(sor, sizeof(sor), input);
        fgets(sor, sizeof(sor), input);
        int a = sor[0] - '0';
        a++;
        for(int j = 0; j < num1; j++) {
            int asd = 0;
            while(asd == 0) {
                asd = rand() % a;                
            }
            fprintf( uj,"%i ", asd);
        }
        fprintf(uj,"\n");
    }
    fclose(input);
    fclose(uj);
}

void eredmenyKiir(char *s[]) {
    FILE *input = fopen(s, "r");
    char sor[150];
    int i = 1;
    while(fgets(sor, sizeof(sor), input)) {
        printf("MLSZ: %i. kerdes valaszai: %s", i, sor);
        i++;
    }
}
