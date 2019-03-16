//F�jlkezel�s
//
//#include <stdio.h>:input/output
//type: FILE  -> FILE ptr pl: FILE *f
//func: alap: fopen(),fclose(),feof(),
//	text: fprintf(),fputs(),fgets(),
//	bin : fwrite(),fread(),fseek(),
//f=fopen(f�jln�v,m�d);
//m�dok:
//text:	"r":megnyit olvas�sra,		"r+":megnyit olvas�sra �s �r�sra,
//	"w":megnyit/l�trehoz �r�sra, 	"w+":l�trehoz olvas�sra �s �r�sra,
//	"a",                    	"a+";
//bin:	"rb", "wb", "ab", "rb+", "wb+", "ab+";
//TEXT:
//�r�s: 	fputs(string,f) -> negat�v, ha hiba; fprintf(f,formstr,...) -> karaktersz�m; 
//olvas�s: 	fgets(string,max_char,f) -> NULL, ha semmit nem olvasott;
//BIN:
//�r�s: 	fwrite(els�_ki�rand�_elem_kezd�c�me,elemm�ret,elemsz�m,f) -> elemsz�m amit ki�rt;
//olvas�s: 	fread(kezd�c�m,elemm�ret,elemsz�m,f) -> t�nylegesen beolvasott elemsz�m;
//	   	fseek(f, elmozdul�s(byte), viszony�t�si pont) -> nem 0: hiba
//			viszony�t�si pontok: SEEK.SET, SEEK.CUR, SEEK.END
//			pl: fseek(f, -2*sizeof(char), SEEK.END) : utols� el�tti karakter a f�jlban
//
//Rendszerh�v�sok
//
//#include <fcntl.h>:param�ter konstansok
//#include <unistd.h>:rendszerh�v�sok (nem sz�ks�ges az elte os rendszer�n)
//func: access(f�jl, m�d):hozz�f�r�st vizsg�l(0:hozz�f�r,-1:nem)
//                 m�dok: R_OK:olvashat�,W_OK:�rhat�,X_OK:�rhat�+olvashat�,F_OK:l�tez�
//	open(fn�v, m�d, [enged�lyek]) -> -1: hiba, intben t�roljuk
//		   m�dok: O_RDONLY: olvas�s, O_WRONLY: �r�s, O_RDWR: �r�s-olvas�s,
//			  O_APPEND: hozz��r, O_TRUNC: t�r�l mindent, O_CREATE: l�trehozza ha nem l�tezik
//			  					 	O_EXCL: create-tel egy�tt haszn�lva:
//										nem l�tezhet a f�jl, mindenk�pp l�tre kell hozni
//		   enged�lyek(O_CREATE-hez sz�ks�gesek): S_I + [R/W/X] + [USR/GRP/OTH] pl: S_IXUSR -> user v�grehajthatja
//	close(f)
//	read(f, kezd�c�m, olvasand� byte-ok sz�ma) -> t�nylegesen beolvasott byte-ok sz�ma; negat�v ha hiba
//	write(f, kezd�c�m, ki�rand� byte-ok sz�ma) -> ki�rt byte-ok sz�ma; negat�v ha hiba
//	lseek(f, elmozdul�s(byte), viszony�t�si pont) -> mutat� poz�ci� a f�jl elej�t�l n�zve; -1: hiba
//
//Folyamatok
//
//Fogalmak:
//PID: Process ID
//PPID: Parent Process ID
//#include <sys/types.h>:pid_t t�pus
//sz�tv�laszt�s:
//	fork() -> pid_t; -1: hiba
//		     - sz�l�: gyerek PID-je
//		     - gyerek: 0
//				pl: pid_t pid; pid = fork();
//	getpid()
//	getppid()
//	sleep(sec)
//	usleep(microsec)
//	wait(int &status) : felf�ggeszti a folyamat v�grehajt�s�t, am�g b�rmelyik gyerekfolyamat nem termin�l -> gyerek pid; -1: nincs gyerek
//	waitpid(pid_t PID, int &status, int options) : addig f�ggeszti fel, m�g a megadott gyerekfolyamat nem v�lt �llapotot.
//		PID: -1 eset�n b�rmelyik gyerekre v�r.
//		options: �llapotv�ltoz�s amire v�r. WNOCHANG: termin�lt, WUNTRACED: le�llt(folytat�dhat), WCONTINUED: folytat�dott(le�ll�s ut�n)
//