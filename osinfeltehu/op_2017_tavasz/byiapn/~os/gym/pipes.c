0/*
Évfolyam zh-n vszeg üzenetsor lesz!
*/

/*
Csõvel kommunikálva is fájlokon át kommunikálunk (FIFO fájlok - first input first output).

Névtelen csõ: pipe
#include <unistd.h>
pipe(int pipefd[2]) -> 2 elemû tömbben kapjuk vissza a csõvezeték adatait: 0. elem: olvasó vég, 1. elem: író vég
Egyértelmûen mindig az elõtt a fork elõtt kell megnyitni, amiben használni szeretnénk.
Mindig le kell zárni az író véget, hogy az olvasó vég ne várjon tovább.
Probléma: több, mint 2 szálú program esetén figyelni kell a szálakban a csövek lezárására.

Nevesített csõ: fifo
#include <unistd.h>
mkfifo(fájlnév, engedélyek) -> (pozitív! int) fájlazonosító, ha siker: létrehozza a megadott fájlnévvel
és hozzáférési engedélyekkel a FIFO fájlt.
unlink(fájlnév) -> 0, ha siker: törli a fájlt.
További utasítások: open, read, write. !Az open esetén mindkét véget meg kell nyitni, különben várakozik!
Csak abban a szálban nyitjuk meg amiben használni akarjuk.

pollozás
#include <poll.h>
struct pollfd: 	fd: fileazonosító
		events: figyelt esemény (short)		\ POLLIN: van kiolvasható adat
		tevents: bekövetkezett esemény (short)	/ POLLOUT
poll(figyelendõk tömbje(pollfd tömb), figyelendõk száma, idõtúllépés(int)(ms)) - 0ms esetén nem vár, ellenõrzésre használható
 -> 0: idõtúllépés, 0+: az az index+1 amiben esemény történt, 0-: hiba
ppoll(figyelendõk tömbje, figyelendõk száma, idõtúllépés(timespec), szignálmaszk)

select
#include <sys/select.h>
Típus:
fd_set
Mûveletei:
FD_ZERO(készlet) - üríti a készletet
FD_SET(fájlazon., készlet) - hozzáadja a fájlazonosítót
FD_CLEAR(fájlazon., készlet) - kiveszi a fájlazonosítót
FD_ISSET(fájlazon., készlet) -> 0, ha nem tartalmazza
Használata:
select(nfds, readfds, writefds, exceptfds, idõtúllépés(timeval)) - 0 esetén meghatározatlan ideig vár

ipc
<sys/ipc.h>
ftok(path, projectid): ipc kulcs generálása -> key_t vagy -1, ha hiba
	path lehet: argv[0] (maga a programfájl) vagy "/tmp"
msgget(key_t, jogosultságok): üzenetsor létrehozása -> üzenetsor azonosító vagy -1, ha hiba
	jogosultságok: IPC_PRIVATE (csak a programon belüli folyamatok látják!) (| IPC_CREAT) 
üzenet struct:
	long: kategória
	mutató: üzenet
msgsnd(üzenetsor azon., &üzenet struct, méret, flag)
	flag: 0 vagy IPC_NOWAIT
msgrcv(üzenetsor azon., &üzenet struct, puffer méret, üzenet kategória, flag)
	üz. kat.: 0 esetén a soron következõ üzenetet olvassa, egyébként csak az adott kategóriás üzeneteket olvassa
msgctl(üzenetsor azon., IPC_RMID, NULL): üzenetsor törlése -> 0, ha siker vagy -1, ha nem
*/