#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <wait.h>
 #include <signal.h>

volatile int signal_arrived = 0;

void handler(int signumber)
{
  signal_arrived = 1;
}

struct message
{
  long msg_type;
  char msg_text[500];
};

int send_message(int msg_row, char *msg)
{
  struct message message_to_send;
  message_to_send.msg_type = 5;
  strcpy(message_to_send.msg_text, msg);

  int status;
  status = msgsnd(msg_row, &message_to_send, sizeof(message_to_send.msg_text), 0);
  if(status < 0) {perror("msgsnd()\n"); fflush(stderr); exit(1);}
  return 0;
}

char* receive_message(int msg_row)
{
  struct message message_to_receive;
  int status;
  status = msgrcv(msg_row, &message_to_receive, (size_t) 500, 5, 0);
  if(status < 0)
  {
    perror("msgrcv()\n"); fflush(stderr); exit(1);
  }

  char *msg = calloc(500, sizeof(char));
  if(msg == NULL) {perror("Nincs eleg szabad memoria.\n"); fflush(stderr); exit(1);}
  strcpy(msg, message_to_receive.msg_text);
  return msg;
}

int main(int argc, char const *argv[])
{
  if(argc < 2)
  {
    perror("Hianyzo argumentum!\n");
    fflush(stderr);
    exit(1);
  }

  char child_msg[500];
  char parent_msg[500];

  int pipefd[2]; //read:0, write:1
  if(pipe(pipefd) == -1)
  {
    fprintf(stdout, "Nem sikerult a pipe-ot megnyitni.\n");
    fflush(stdout);
    exit(1);
  }

  struct sigaction sig_action;
  sig_action.sa_flags = 0;
  sigemptyset(&sig_action.sa_mask);
  sig_action.sa_handler = handler;
  if(sigaction(SIGUSR1, &sig_action, NULL) < 0) {fprintf(stdout, "sigaction()\n"); fflush(stdout); exit(1);}

  pid_t pid = fork();
  if(pid < 0)
  {
    fprintf(stdout, "Nem sikerult a fork.\n");
    fflush(stdout);
    exit(1);
  }
  else if(pid > 0) //szulo
  {
    close(pipefd[1]); //a cso iro felet a szulo nem fogja hasznalni

    sigdelset(&sig_action.sa_mask, SIGUSR1);
    if(sigaction(SIGUSR2, &sig_action, NULL) < 0) { perror("sigaction()\n"); exit(1);}

    fprintf(stdout, "A taxitarsasag varja az utas bejelentkezeset.\n");
    fflush(stdout);
    while(signal_arrived != 1)
    {
      fprintf(stdout, "A tarsasag tovabbra is varja az utas bejelentkezeset.\n");
      fflush(stdout);
      sleep(2);
    }
    signal_arrived = 0;

    read(pipefd[0], &parent_msg, (size_t) 500);

    fprintf(stdout, "Az utas bejelentkezett! A kovetkezo cimet adta meg: \"%s\"\n", parent_msg);
    fflush(stdout);

    int status;
    waitpid(pid, &status, WEXITED);
    close(pipefd[0]); //mar nincs szukseg az olvaso vegere sem a csonek.
  }
  else if(pid == 0) //gyerek
  {
    sleep(1);
    close(pipefd[0]); //a cso olvaso felet a gyerek nem fogja hasznalni

    int i;
    strcpy(child_msg, argv[1]);
    for(i = 2; i < argc; ++i)
    {
      strcat(child_msg, " ");
      strcat(child_msg, argv[i]);
    }

    write(pipefd[1], &child_msg, (size_t) 500);
    fflush(NULL);
    kill(getppid(), SIGUSR2);

    close(pipefd[1]); //mar nincs szukseg az iro felere sem.
    exit(0);
  }

  key_t key = ftok(argv[0],1);
  int msg_row = msgget( key, 0600 | IPC_CREAT );
  if ( msg_row < 0 ) {perror("msgget()\n"); fflush(stderr); exit(1);}

  pid = fork();
  if(pid < 0)
  {
    fprintf(stdout, "Nem sikerult a fork.\n");
    fflush(stdout);
    exit(1);
  }
  else if(pid > 0) //szulo
  {
    send_message(msg_row, parent_msg);
    sleep(5);

    strcpy(parent_msg, receive_message(msg_row));

    fprintf(stdout, "A taxis elindult az utasert.\n");
    fflush(stdout);

    int msg_status = msgctl(msg_row, IPC_RMID, NULL);
    if(msg_status < 0) {perror("msgctl()\n"); fflush(stderr);}

    int status;
    waitpid(pid, &status, WEXITED);
  }
  else if(pid == 0) //gyerek
  {
    sleep(1);

    strcpy(child_msg, receive_message(msg_row));
    fprintf(stdout, "A taxis a kovetkezo uzenetet kapta: \"%s\"\n", child_msg);

    send_message(msg_row, "Elindultam.");
    exit(0);
  }

  return 0;
}
