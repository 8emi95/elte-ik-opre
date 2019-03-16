// Feladatok:
//1) Készítsünk 2 gyerekfolyamatot, amelyek 50 db 1-100 
//		közötti véletlen számot írjanak ki a képernyõre és a saját PID-jüket. 
//			getpid() / getppid()
//		A szülõ várja meg a gyerekfolyamatok befejezõdését! 
//		(Véletlenszám rand(),srand() ) // rand,srand #include <stdlib.h>  
//2) Saját shell készítése (execv vagy system)
//    - beolvas egy parancsot és végrehajtja
//3) A gyerekfolyamat írja ki karakterenként a 
//		„Gyerek vagyok Gyerek vagyok…” szöveget, 
//		a szülõ pedig olvassa ezt be, ha a gyerekfolyamat felengedte a zárolást.
//4) Készítsünk bináris fájlt, amelyik a neveket és a születési éveket tartalmazza 
//		– olvassuk is ezeket vissza.

#include <unistd.h> 	// fork, getpid, exec, fcntl, sleep
#include <sys/types.h>	// wait, getpid
#include <sys/wait.h>	// wait
#include <stdlib.h> 	// system, rand(), srand()
#include <fcntl.h> 		// fcntl, open
#include <sys/stat.h> 	// open

main() {

}