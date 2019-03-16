Létrehozás (pipe): pipe függvény segítségével 

#include <unistd.h>
int pipe(int pfd[2]); //0-t térít vissza, ha a létrehozás sikerült, és -1-et, ha nem.

pfd[0]-ból olvasunk, és a pfd[1]-be írunk.
---------------------------------------
!!! Bezárás (close): int close(int pfd); //dettó visszatérés

pfd egy egész szám, tehát csak az állomány egyik végét zárja be.
--------------------------------------
!!! read/write:

//A függvények visszatérített értéke a pipe-ból sikeresen kiolvasott (beírt) bájtok száma.
ssize_t read(int pfd, void *buf, size_t count);
ssize_t write(int pfd, const void *buf, size_t count);

