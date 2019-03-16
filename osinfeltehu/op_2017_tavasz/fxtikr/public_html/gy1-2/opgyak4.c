// K�ls� programok ind�t�sa

void execv(char* parancs, char** argumentumok);

// Egyszerre dolgoznak egy f�jlban akkor olvashatatlan lesz a f�jl, ha ilyen van
// akkor �rdemes z�rolni a f�jlt, m�g dolgozik benne az egyik sz�l

#include <fcntl.h> // Tartalmazza a flock f�ggv�nyt

struct flock { short l_type, short l_whence, off_t l_start, off_t l_len, pid_t l_pid };
l_type: F_RDLCK, F_WRLCK, F_UNLCK // A Lockol�s m�dja
l_whence: SEEK_SET, SEEK_CUR, SEEK_END // Honnan (Eleje, jelenlegi poz, v�ge)
l_start // T�nyleges kezd�poz�ci� (b�jtban)
l_len // B�jtok sz�ma (0 eset�n teljes f�jl)
l_pid // Lockol� folyamat PID-je


int fcntl(FILE* f, m�d, struct flock& fl)
m�dok: F_SETLKW, F_SETLK // Egyik ellen�rzi, hogy lockolhat�-e a f�jl, F_SETLKW megv�rja m�g lockolhat�, F_SETLK ha nem tud lockolni, �gy hagyja

// Visszat�r�si �rt�kek: -1: Hiba

// V�letlen sz�m-gener�l�s

#include <stdlib.h> // rand f�ggv�ny
#include <time.h>

int rand() // [0, RAND_MAX] k�z� es� sz�mot ad vissza
srand(unsigned int seed)

time_t time(NULL) // 1970. janu�r 1. �ta eltelt id� ms-ben
tm localtime(&time_t time) // �tkonvert�lja a time_t-t tm-re, ami egy strukt�ra a helyi id� szerint

struct tm {
	tm_sec // 0-61
	tm_min // 0-59
	tm_hour // 0-23
	tm_day // 1-31
	tm_mon // 0-11
	tm_year // 1900-t�l felfele
	tm_wday // 0-6 Vas�rnapt�l szombatig eltelt napok sz�ma
	tm_yday // Janu�r 1. �ta eltelt napok sz�ma (0-365)
    tm_isdst // 1 - ny�ri, 0 - t�li, -1 - nincs �t�ll�s helyi id� szerint
};

/*

	SZIGN�LOK

*/

#include <signal.h>

void pause() // Meg�ll a folyamat addig, am�g nem j�n szign�l (Ha nem j�n signal zombi lesz bel�le)
        // Csak az ut�na �rkez� szign�lokra �rv�nyes!!! (Ha el�bb �rkezik a szign�l, beragad a folyamat)
		
int kill(PID, signal) // Szign�l k�ld�se
                      // 0: J�, -1: Hiba
					  // Ha a PID-re 0-t �rok, akkor a jelenlegi folyamattal azonos csoportban l�v� folyamatnak k�ld szign�lt
					  // PID -1: K�ld�si enged�ly


void handler(int signnm) { // Egy elj�r�s, ami egy adott szign�l eset�n lefut
	
 	// ...
	
}

signal(signal, handler /* A meg�rt elj�r�sunk */);



