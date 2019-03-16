#include "hdr.h"

int main(void)
{
  int gyerek;
  ...
  mknod("PIPE_FIFO_1", 0666 | S_IFIFO, 0);          /* FIFO fájlok létrehozása */
  mknod("PIPE_FIFO_2", 0666 | S_IFIFO, 0);
  ...
  if ((gyerek = fork()) < 0)                        /* gyerekfolyamat létrehozása */
    err_sys("fork hiba");
  if (gyerek)
    apa();                                          /* P1 folyamat */
  else
    fiu();                                          /* P2 folyamat */
  exit(0);
}

void apa(void)                                      /* P1 folyamat */
{
  int fd1, fd2;
  ...
  if ((fd1 = open("PIPE_FIFO_1", O_WRONLY)) < 0)    /* FIFO fájl megnyitása írásra */
    err_sys("hiba a PIPE_FIFO_1 nyitasa soran");
  if ((fd2 = open("PIPE_FIFO_2", O_RONLY)) < 0)     /* másik FIFO nyitása olvasásra */
    err_sys("hiba a PIPE_FIFO_2 nyitasa soran");
  ...
  write(fd1, ...);                                  /* írás a PIPE_FIFO_1-be */
  ...
  read(fd2, ...);                                   /* olvasás a PIPE_FIFO_2-ből */
  ...
  close(fd1);                                       /* FIFO fájlok bezárása */
  close(fd2);
  exit(0);
}

void fiu(void)                                      /* P2 folyamat */
{
  int fd1, fd2;
  ...
  fd1 = open("PIPE_FIFO_1", O_RDONLY);              /* FIFO fájl megnyitása olvasásra */
  fd2 = open("PIPE_FIFO_2", O_WRONLY);              /* FIFO fájl megnyitása írásra */
  ...
  read(fd1, ...);                                   /* olvasás a PIPE_FIFO_1-ből */
  ...
  write(fd2, ...);                                  /* írás a PIPE_FIFO_2-be */
  ...
  close(fd1);                                       /* FIFO fájlok bezárása */
  close(fd2);
  exit(0);
}
