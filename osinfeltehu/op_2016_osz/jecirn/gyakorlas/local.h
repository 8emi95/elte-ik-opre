#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <stdlib.h>

using namespace std;

const char *PUBLIC = "valami";
const int B_SIZ = (PIPE_BUF / 2);

struct message {
  char fifo_name[B_SIZ];
  char cmd_line[B_SIZ];
};
