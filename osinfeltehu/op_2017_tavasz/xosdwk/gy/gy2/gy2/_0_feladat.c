// 2. gyakorlat. Jelz�sek
// Feladatok:
// 1) K�sz�tsen �ra alkalmaz�st! (setitimer vagy alarm, time(),ctime(&time_t), �\r� sor elej�re �ll)
// 2) K�sz�tsen alkalmaz�st k�t gyerekfolyamattal.  A gyerekfolyamatok v�letlensz�mokat gener�lnak (1-100) �s �rnak ki, 
//		am�g 10-n�l kisebb sz�mot nem kapnak, ekkor k�ldjenek jelz�st a sz�l�nek (mindegyik m�sikat), 
//		amelyik mindk�t esetben �rja ki azok fogad�s�t. 
// 3) K�sz�tsen alkalmaz�st 2 gyerekfolyamattal. Az egyik gyerekfolyamat k�ldj�n jelz�st (SIGUSR1) a sz�l�nek, aki �rja ki, 
//		hogy megkapta �s azonnal k�ldj�n egy m�sikat (SIGUSR2) a m�sik gyereknek, aki fogadja azt �s ki�rja. 





// 1) V�gtelen ciklusban a Ctrl+C jelz�sre �rjon ki a program egy �zenetet.
// 2) Random sz�mok gener�l�sa (1-100 intervallumon) 50 �s 75 k�ztieket jelezni.
//			SIGUSR1 -> ebb�l csak az els� jelz�s �r c�lba!
//			SIGRTMN



























#include <stdio.h>
#include <unistd.h> 	// alarm, pause, fork, getpid, exec, fcntl, sleep
#include <sys/types.h>	// wait, getpid
#include <sys/wait.h>	// wait
#include <stdlib.h> 	// system, rand(), srand()
#include <fcntl.h> 		// fcntl, open
#include <sys/stat.h> 	// open
#include <signal.h> 	// signal, kill, sigaction, sigemptyset, sigprocmask,
#include <string.h> 	// strsignal � jelz�s le�r�sa
#include <sys/time.h> 	// setitimer


void kezelo( int uzenet ) { printf("mi van?\n"); }
main ( ) { 
	signal( SIGINT, kezelo ) ; // bejegyzi a f�ggv�nyt SIGINT jelz�s kezel�s�re.
	while( 1 ) { sleep (10); } // v�gtelen ciklusban v�rakozik, n�h�ny m�sodpercenk�nt �zenetet k�ldve a felhaszn�l�nak. 
}



