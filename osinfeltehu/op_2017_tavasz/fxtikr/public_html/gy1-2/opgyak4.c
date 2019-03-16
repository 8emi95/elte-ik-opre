// Külsõ programok indítása

void execv(char* parancs, char** argumentumok);

// Egyszerre dolgoznak egy fájlban akkor olvashatatlan lesz a fájl, ha ilyen van
// akkor érdemes zárolni a fájlt, míg dolgozik benne az egyik szál

#include <fcntl.h> // Tartalmazza a flock függvényt

struct flock { short l_type, short l_whence, off_t l_start, off_t l_len, pid_t l_pid };
l_type: F_RDLCK, F_WRLCK, F_UNLCK // A Lockolás módja
l_whence: SEEK_SET, SEEK_CUR, SEEK_END // Honnan (Eleje, jelenlegi poz, vége)
l_start // Tényleges kezdõpozíció (bájtban)
l_len // Bájtok száma (0 esetén teljes fájl)
l_pid // Lockoló folyamat PID-je


int fcntl(FILE* f, mód, struct flock& fl)
módok: F_SETLKW, F_SETLK // Egyik ellenõrzi, hogy lockolható-e a fájl, F_SETLKW megvárja míg lockolható, F_SETLK ha nem tud lockolni, úgy hagyja

// Visszatérési értékek: -1: Hiba

// Véletlen szám-generálás

#include <stdlib.h> // rand függvény
#include <time.h>

int rand() // [0, RAND_MAX] közé esõ számot ad vissza
srand(unsigned int seed)

time_t time(NULL) // 1970. január 1. óta eltelt idõ ms-ben
tm localtime(&time_t time) // Átkonvertálja a time_t-t tm-re, ami egy struktúra a helyi idõ szerint

struct tm {
	tm_sec // 0-61
	tm_min // 0-59
	tm_hour // 0-23
	tm_day // 1-31
	tm_mon // 0-11
	tm_year // 1900-tól felfele
	tm_wday // 0-6 Vasárnaptól szombatig eltelt napok száma
	tm_yday // Január 1. óta eltelt napok száma (0-365)
    tm_isdst // 1 - nyári, 0 - téli, -1 - nincs átállás helyi idõ szerint
};

/*

	SZIGNÁLOK

*/

#include <signal.h>

void pause() // Megáll a folyamat addig, amíg nem jön szignál (Ha nem jön signal zombi lesz belõle)
        // Csak az utána érkezõ szignálokra érvényes!!! (Ha elõbb érkezik a szignál, beragad a folyamat)
		
int kill(PID, signal) // Szignál küldése
                      // 0: Jó, -1: Hiba
					  // Ha a PID-re 0-t írok, akkor a jelenlegi folyamattal azonos csoportban lévõ folyamatnak küld szignált
					  // PID -1: Küldési engedély


void handler(int signnm) { // Egy eljárás, ami egy adott szignál esetén lefut
	
 	// ...
	
}

signal(signal, handler /* A megírt eljárásunk */);



