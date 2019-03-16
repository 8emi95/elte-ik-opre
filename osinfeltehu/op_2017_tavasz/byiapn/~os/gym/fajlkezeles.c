//Fájlkezelés
//
//#include <stdio.h>:input/output
//type: FILE  -> FILE ptr pl: FILE *f
//func: alap: fopen(),fclose(),feof(),
//	text: fprintf(),fputs(),fgets(),
//	bin : fwrite(),fread(),fseek(),
//f=fopen(fájlnév,mód);
//módok:
//text:	"r":megnyit olvasásra,		"r+":megnyit olvasásra és írásra,
//	"w":megnyit/létrehoz írásra, 	"w+":létrehoz olvasásra és írásra,
//	"a",                    	"a+";
//bin:	"rb", "wb", "ab", "rb+", "wb+", "ab+";
//TEXT:
//írás: 	fputs(string,f) -> negatív, ha hiba; fprintf(f,formstr,...) -> karakterszám; 
//olvasás: 	fgets(string,max_char,f) -> NULL, ha semmit nem olvasott;
//BIN:
//írás: 	fwrite(elsõ_kiírandó_elem_kezdõcíme,elemméret,elemszám,f) -> elemszám amit kiírt;
//olvasás: 	fread(kezdõcím,elemméret,elemszám,f) -> ténylegesen beolvasott elemszám;
//	   	fseek(f, elmozdulás(byte), viszonyítási pont) -> nem 0: hiba
//			viszonyítási pontok: SEEK.SET, SEEK.CUR, SEEK.END
//			pl: fseek(f, -2*sizeof(char), SEEK.END) : utolsó elõtti karakter a fájlban
//
//Rendszerhívások
//
//#include <fcntl.h>:paraméter konstansok
//#include <unistd.h>:rendszerhívások (nem szükséges az elte os rendszerén)
//func: access(fájl, mód):hozzáférést vizsgál(0:hozzáfér,-1:nem)
//                 módok: R_OK:olvasható,W_OK:írható,X_OK:írható+olvasható,F_OK:létezõ
//	open(fnév, mód, [engedélyek]) -> -1: hiba, intben tároljuk
//		   módok: O_RDONLY: olvasás, O_WRONLY: írás, O_RDWR: írás-olvasás,
//			  O_APPEND: hozzáír, O_TRUNC: töröl mindent, O_CREATE: létrehozza ha nem létezik
//			  					 	O_EXCL: create-tel együtt használva:
//										nem létezhet a fájl, mindenképp létre kell hozni
//		   engedélyek(O_CREATE-hez szükségesek): S_I + [R/W/X] + [USR/GRP/OTH] pl: S_IXUSR -> user végrehajthatja
//	close(f)
//	read(f, kezdõcím, olvasandó byte-ok száma) -> ténylegesen beolvasott byte-ok száma; negatív ha hiba
//	write(f, kezdõcím, kiírandó byte-ok száma) -> kiírt byte-ok száma; negatív ha hiba
//	lseek(f, elmozdulás(byte), viszonyítási pont) -> mutató pozíció a fájl elejétõl nézve; -1: hiba
//
//Folyamatok
//
//Fogalmak:
//PID: Process ID
//PPID: Parent Process ID
//#include <sys/types.h>:pid_t típus
//szétválasztás:
//	fork() -> pid_t; -1: hiba
//		     - szülõ: gyerek PID-je
//		     - gyerek: 0
//				pl: pid_t pid; pid = fork();
//	getpid()
//	getppid()
//	sleep(sec)
//	usleep(microsec)
//	wait(int &status) : felfüggeszti a folyamat végrehajtását, amíg bármelyik gyerekfolyamat nem terminál -> gyerek pid; -1: nincs gyerek
//	waitpid(pid_t PID, int &status, int options) : addig függeszti fel, míg a megadott gyerekfolyamat nem vált állapotot.
//		PID: -1 esetén bármelyik gyerekre vár.
//		options: állapotváltozás amire vár. WNOCHANG: terminált, WUNTRACED: leállt(folytatódhat), WCONTINUED: folytatódott(leállás után)
//