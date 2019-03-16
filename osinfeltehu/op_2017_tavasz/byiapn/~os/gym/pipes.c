0/*
�vfolyam zh-n vszeg �zenetsor lesz!
*/

/*
Cs�vel kommunik�lva is f�jlokon �t kommunik�lunk (FIFO f�jlok - first input first output).

N�vtelen cs�: pipe
#include <unistd.h>
pipe(int pipefd[2]) -> 2 elem� t�mbben kapjuk vissza a cs�vezet�k adatait: 0. elem: olvas� v�g, 1. elem: �r� v�g
Egy�rtelm�en mindig az el�tt a fork el�tt kell megnyitni, amiben haszn�lni szeretn�nk.
Mindig le kell z�rni az �r� v�get, hogy az olvas� v�g ne v�rjon tov�bb.
Probl�ma: t�bb, mint 2 sz�l� program eset�n figyelni kell a sz�lakban a cs�vek lez�r�s�ra.

Neves�tett cs�: fifo
#include <unistd.h>
mkfifo(f�jln�v, enged�lyek) -> (pozit�v! int) f�jlazonos�t�, ha siker: l�trehozza a megadott f�jln�vvel
�s hozz�f�r�si enged�lyekkel a FIFO f�jlt.
unlink(f�jln�v) -> 0, ha siker: t�rli a f�jlt.
Tov�bbi utas�t�sok: open, read, write. !Az open eset�n mindk�t v�get meg kell nyitni, k�l�nben v�rakozik!
Csak abban a sz�lban nyitjuk meg amiben haszn�lni akarjuk.

polloz�s
#include <poll.h>
struct pollfd: 	fd: fileazonos�t�
		events: figyelt esem�ny (short)		\ POLLIN: van kiolvashat� adat
		tevents: bek�vetkezett esem�ny (short)	/ POLLOUT
poll(figyelend�k t�mbje(pollfd t�mb), figyelend�k sz�ma, id�t�ll�p�s(int)(ms)) - 0ms eset�n nem v�r, ellen�rz�sre haszn�lhat�
 -> 0: id�t�ll�p�s, 0+: az az index+1 amiben esem�ny t�rt�nt, 0-: hiba
ppoll(figyelend�k t�mbje, figyelend�k sz�ma, id�t�ll�p�s(timespec), szign�lmaszk)

select
#include <sys/select.h>
T�pus:
fd_set
M�veletei:
FD_ZERO(k�szlet) - �r�ti a k�szletet
FD_SET(f�jlazon., k�szlet) - hozz�adja a f�jlazonos�t�t
FD_CLEAR(f�jlazon., k�szlet) - kiveszi a f�jlazonos�t�t
FD_ISSET(f�jlazon., k�szlet) -> 0, ha nem tartalmazza
Haszn�lata:
select(nfds, readfds, writefds, exceptfds, id�t�ll�p�s(timeval)) - 0 eset�n meghat�rozatlan ideig v�r

ipc
<sys/ipc.h>
ftok(path, projectid): ipc kulcs gener�l�sa -> key_t vagy -1, ha hiba
	path lehet: argv[0] (maga a programf�jl) vagy "/tmp"
msgget(key_t, jogosults�gok): �zenetsor l�trehoz�sa -> �zenetsor azonos�t� vagy -1, ha hiba
	jogosults�gok: IPC_PRIVATE (csak a programon bel�li folyamatok l�tj�k!) (| IPC_CREAT) 
�zenet struct:
	long: kateg�ria
	mutat�: �zenet
msgsnd(�zenetsor azon., &�zenet struct, m�ret, flag)
	flag: 0 vagy IPC_NOWAIT
msgrcv(�zenetsor azon., &�zenet struct, puffer m�ret, �zenet kateg�ria, flag)
	�z. kat.: 0 eset�n a soron k�vetkez� �zenetet olvassa, egy�bk�nt csak az adott kateg�ri�s �zeneteket olvassa
msgctl(�zenetsor azon., IPC_RMID, NULL): �zenetsor t�rl�se -> 0, ha siker vagy -1, ha nem
*/