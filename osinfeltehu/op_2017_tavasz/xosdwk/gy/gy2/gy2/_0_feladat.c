// 2. gyakorlat. Jelzések
// Feladatok:
// 1) Készítsen óra alkalmazást! (setitimer vagy alarm, time(),ctime(&time_t), „\r” sor elejére áll)
// 2) Készítsen alkalmazást két gyerekfolyamattal.  A gyerekfolyamatok véletlenszámokat generálnak (1-100) és írnak ki, 
//		amíg 10-nél kisebb számot nem kapnak, ekkor küldjenek jelzést a szülõnek (mindegyik másikat), 
//		amelyik mindkét esetben írja ki azok fogadását. 
// 3) Készítsen alkalmazást 2 gyerekfolyamattal. Az egyik gyerekfolyamat küldjön jelzést (SIGUSR1) a szülõnek, aki írja ki, 
//		hogy megkapta és azonnal küldjön egy másikat (SIGUSR2) a másik gyereknek, aki fogadja azt és kiírja. 





// 1) Végtelen ciklusban a Ctrl+C jelzésre írjon ki a program egy üzenetet.
// 2) Random számok generálása (1-100 intervallumon) 50 és 75 köztieket jelezni.
//			SIGUSR1 -> ebbõl csak az elsõ jelzés ér célba!
//			SIGRTMN



























#include <stdio.h>
#include <unistd.h> 	// alarm, pause, fork, getpid, exec, fcntl, sleep
#include <sys/types.h>	// wait, getpid
#include <sys/wait.h>	// wait
#include <stdlib.h> 	// system, rand(), srand()
#include <fcntl.h> 		// fcntl, open
#include <sys/stat.h> 	// open
#include <signal.h> 	// signal, kill, sigaction, sigemptyset, sigprocmask,
#include <string.h> 	// strsignal – jelzés leírása
#include <sys/time.h> 	// setitimer


void kezelo( int uzenet ) { printf("mi van?\n"); }
main ( ) { 
	signal( SIGINT, kezelo ) ; // bejegyzi a függvényt SIGINT jelzés kezelésére.
	while( 1 ) { sleep (10); } // végtelen ciklusban várakozik, néhány másodpercenként üzenetet küldve a felhasználónak. 
}



