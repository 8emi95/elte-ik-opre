// Feladatok:
//1) K�sz�ts�nk 2 gyerekfolyamatot, amelyek 50 db 1-100 
//		k�z�tti v�letlen sz�mot �rjanak ki a k�perny�re �s a saj�t PID-j�ket. 
//			getpid() / getppid()
//		A sz�l� v�rja meg a gyerekfolyamatok befejez�d�s�t! 
//		(V�letlensz�m rand(),srand() ) // rand,srand #include <stdlib.h>  
//2) Saj�t shell k�sz�t�se (execv vagy system)
//    - beolvas egy parancsot �s v�grehajtja
//3) A gyerekfolyamat �rja ki karakterenk�nt a 
//		�Gyerek vagyok Gyerek vagyok�� sz�veget, 
//		a sz�l� pedig olvassa ezt be, ha a gyerekfolyamat felengedte a z�rol�st.
//4) K�sz�ts�nk bin�ris f�jlt, amelyik a neveket �s a sz�let�si �veket tartalmazza 
//		� olvassuk is ezeket vissza.

#include <unistd.h> 	// fork, getpid, exec, fcntl, sleep
#include <sys/types.h>	// wait, getpid
#include <sys/wait.h>	// wait
#include <stdlib.h> 	// system, rand(), srand()
#include <fcntl.h> 		// fcntl, open
#include <sys/stat.h> 	// open

main() {

}