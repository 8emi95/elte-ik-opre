#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handler(int signum);

void handler(int signum) {
}

int MAX;

int szulo_to_gyerek[2];
int gyerek_to_szulo[2];
  
void szulo_process(void);
void gyerek_process(void);


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
  pipestatus = pipe(szulo_to_gyerek);
  if (pipestatus == -1) {
    perror("Pipe error");
  }

  pipestatus = pipe(gyerek_to_szulo);
  if (pipestatus == -1) {
    perror("Pipe error");
  }

  if (pid > 0) {
    szulo_process();
  } else {
    gyerek_process();
  }

  return 0;
}

void szulo_process() {
  signal(SIGUSR1, handler);

  close(szulo_to_gyerek[0]);
  close(gyerek_to_szulo[1]);
  srand(getpid());
  int hanyadik = (rand() % MAX) + 1;

  write(szulo_to_gyerek[1], &hanyadik, sizeof(int));

  printf("Szulo: Varok a gyerekre...\n");
  pause();

  char uzenet[100];
  read(gyerek_to_szulo[0], uzenet, sizeof(uzenet));

  printf("Szulo: Gyerek uzenete: %s\n", uzenet);

  close(szulo_to_gyerek[1]);
  close(gyerek_to_szulo[0]);
  wait(NULL);
  printf("Szulo: Megvartam az osszes vonalat es kileptem!\n");
}

void gyerek_process() {
  close(szulo_to_gyerek[1]);
  close(gyerek_to_szulo[0]);
  
  int hanyadik;
  read(szulo_to_gyerek[0], &hanyadik, sizeof(int));

  printf("hanyadik: %d\n", hanyadik);

  int siker;
  //srand(getpid());
  if (0 == rand() % 10) {
    siker = 1;
    printf("Vonal %d: Baj van!\n", hanyadik);
  } else {
    siker = 0;
    printf("Vonal %d: Sikeres menet!\n", hanyadik);
  }

  kill(SIGUSR1, getppid());

  char uzenet[100];
  strcpy(uzenet, siker ? "Sikeres menet!" : "Bananhej volt a sinek kozott!");
 
  write(gyerek_to_szulo[1], uzenet, sizeof(uzenet));

  close(szulo_to_gyerek[0]);
  close(gyerek_to_szulo[1]);
  exit(0);
}
