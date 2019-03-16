#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <mqueue.h>
#include <errno.h>

void msg_queing(const char* address, int size);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    perror("Bad args...");
    exit(1);
  }

  const char* user_pipe = "./user.pipe";
  if (mkfifo(user_pipe, S_IRUSR | S_IWUSR)) {
    perror("pipe");
  }


  int pid = fork();
  if (pid < 0) {
    perror("no forks");
  }

  if (pid == 0) {
    int fd = open(user_pipe, O_WRONLY);
    write(fd, argv[1], strlen(argv[1]));
    close(fd);
  } else {
    int fd = open(user_pipe, O_RDONLY);
    char buffer[256];
    read(fd, buffer, sizeof(buffer));
    close(fd);
    printf("A cím: '%s'\n", buffer, buffer[0]);
    unlink(user_pipe);

    msg_queing(buffer, strlen(buffer));

    int stat;
    waitpid(pid, &stat);
    printf("Child exited with: %d\n", WEXITSTATUS(stat));
  }

  return 0;
}

void msg_queing(const char* address, int size) {
  const char* queue_name = "/taxiqueue";
  struct mq_attr attributes;
  attributes.mq_maxmsg = 5;
  attributes.mq_msgsize = 256;

  mq_unlink(queue_name);
  int qd = mq_open(queue_name, O_CREAT|O_RDWR, 0600, &attributes);
  if (qd < 0) {
    printf("mq_open (%d)\n", errno);
    exit(1);
  }

  int pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(1);
  }

  if (pid == 0) {
    char buffer[256];
    mq_receive(qd, buffer, sizeof(buffer), NULL);
    printf("(Taxi) Destination: %s\n", buffer);
    char msg[] = "Elindultam";
    mq_send(qd, msg, sizeof(msg), 5);

    mq_close(qd);
  } else {
    mq_send(qd, address, size, 5);
    sleep(1);
    char buffer[256];
    mq_receive(qd, buffer, sizeof(buffer), NULL);
    printf("(Parent) msg_recv: %s", buffer);

    mq_close(qd);
    mq_unlink(queue_name);
    int stat;
    waitpid(pid, &stat);
    printf("Child exited with: %d\n", WEXITSTATUS(stat));
  }
}
