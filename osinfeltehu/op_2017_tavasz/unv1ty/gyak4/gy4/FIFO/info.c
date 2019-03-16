Létrehozás (mknod, mkfifo):  //második függvény tulajdonképpen az elsőt hívja

#include <sys/types.h>
#include <sys/stat.h>

int mknod(char *pathname, int mode, 0);
int mkfifo(const char *pathname, mode_t mode);

Siker esetén mindkét függvény 0-t térít vissza, különben -1
pathname argumentum a FIFO állomány elérési útvonala. A mode egész típusú változó az S_FIFO állomány típusát és a
hozzáférési jogokat (r, w, x) jelöli. Megadása: S_IFIFO  |  jogok
pl 'read' és 'write' jogokkal hozzunk létre FIFO-t mindenkinek:

int makepipe(char *path)
{
  return (mknod(path, S_IFIFO | 0666, 0));
}
-------------------------------------
!!! MEGNYITÁS(open):

hagyományos fájlokéhoz hasonlóan történik az open függvény használatával

int open(const char *pathname, int flags);

Visszatérített értéke az új fájlleíró, vagy -1 hiba esetén. flags változóban hozzáférési jogokat kell megadni
- O_RDONLY, csak olvasható,
- O_WRONLY, csak írható,
- O_RDWR, olvasható és írható.

pl: int fd;
	fd = open("FIFO_1", O_WRONLY);
-----------------------------------------
!!! READ/WRITE:

úgy történik, mint a hagyományos fájloké:

#include <unistd.h>
ssize_t read(int pfd, void *buf, size_t count);
ssize_t write(int pfd, const void *buf, size_t count);

