#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int MAX;

int pipe1[2];
int pipe2[2];
  
void szulo_process(void);
void gyerek_process(void);


void handler(int signum) {
  printf("handler called\n");
  char uzenet[100];
  read(pipe2[0], uzenet, sizeof(uzenet));

  printf("Szulo: Gyerek uzenete: %s\n", uzenet);
}


int main(int argc, char** argv) {

  if (argc < 2) {
    printf("Hasznalat: %s <max_vonalszam>\n", argv[0]);
    exit(-1);
  }

  MAX = atoi(argv[1]);

  printf("Max %d vonal indul\n", MAX);

  pid_t pid;

  pid = fork();
  if (pid == -1) {
    perror("Fork hiba!");
    exit(-1);
  }

  int pipestatus;
  pipestatus = pipe(pipe1);
  if (pipestatus == -1) {
    perror("Pipe error");
  }

  pipestatus = pipe(pipe2);
  if (pipestatus == -1) {
    perror("Pipe error");
  }

  if (pid > 0) {
    signal(SIGUSR1, handler);

    close(pipe1[0]);
    close(pipe2[1]);
    //srand(getpid());
    szulo_process();

    close(pipe1[1]);
    close(pipe2[0]);
    wait(NULL);
    printf("Szulo: Megvartam az osszes vonalat es kileptem!\n");

  } else {

    close(pipe1[1]);
    close(pipe2[0]);
    //srand(getpid());
    gyerek_process();

    close(pipe1[0]);
    close(pipe2[1]);
    exit(0);
  }

  return 0;
}

void szulo_process() {
  int hanyadik = (rand() % MAX) + 1;

  printf("szulo: hanyadik: %d\n", hanyadik);

  write(pipe1[1], &hanyadik, sizeof(int));

  printf("Szulo: Varok a gyerekre...\n");
  pause();
}

void gyerek_process() {
  int hanyadik;

  read(pipe1[0], &hanyadik, sizeof(int));

  printf("gyerek: hanyadik: %d\n", hanyadik);

  int siker;
  if (0 == (rand() % 10)) {
    siker = 1;
    printf("Vonal %d: Baj van!\n", hanyadik);
  } else {
    siker = 0;
    printf("Vonal %d: Sikeres menet!\n", hanyadik);
  }

  kill(SIGUSR1, getppid());

  char uzenet[100];
  strcpy(uzenet, siker == 1 ? "Sikeres menet!" : "Bananhej volt a sinek kozott!");
 
  write(pipe2[1], uzenet, sizeof(uzenet));
}
