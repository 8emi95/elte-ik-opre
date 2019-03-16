// Param�terek �tad�sa
/*
	Az ind�t�si param�terek char**-k�nt ad�dnak �t (t�mb�k t�mbjei)
*/

int main(int argc, char** argv) //argc: argumetumok sz�ma, 0. argumentum maga a program ind�t�sa (pl.: ./a.out)
//argc-edik elem: nullpointer

// Strukt�r�k

struct t�pusn�v
{
	mez�t�pus1 mez�azonos�t�1;
	...
	mez�t�pusN mez�azonos�t�N;
};

struct t�pusn�v v�ltoz�n�v // Nem el�g a t�pusn�v �s az azonos�t� a v�ltoz� l�trehoz�s�hoz, kell a struct el�!
v�ltoz�n�v.mez�azonos�t�1 // Ponttal hivatkozhatunk a mez�kre

struct t�pusn�v* ptr // Pointer
(*ptr).mez�n�v VAGY ptr->mez�n�v // Hivatkoz�s pointer eset�n a mez�kre (A z�r�jel az el�bbin�l fontos a precedencia miatt)

// Hibakezel�s

/*
	<errno.h>: Tartalmaz egy errno rendszerv�ltoz�t, aminek kezdeti �rt�ke 0. 
	Kor�bbi implement�ci�kban eg�szk�nt implement�lj�k, k�s�bbiekben makr�k�nt.
	Mindig az utols� hib�nak az �rt�k�t tal�ljuk benne. (Az �rt�kek jelent�se is implement�ci�f�gg�)
	
	<stdio.h>: Tartalmazza a perror(char* saj�t_hiba�zenet) f�ggv�nyt.
	Ki�rja a hiba�zenetet, kett�spontot rak �s sz�k�zt ut�na, majd a hib�nak megfelel� hiba�zenetet �rja ki.
*/

// F�jlkezel�s

/*
	<unistd.h>: Tartalmazza a rendszerh�v�sokhoz sz�ks�ges dolgokat. (A szerveren amin dolgozunk nem kell k�l�n includeolni)
	
	access(char* f�jl, m�d): A hozz�f�r�si m�dot ellen�rzi
	m�d param�terek: R_OK: Olvas�si enged�ly
					 W_OK: �r�si enged�ly
					 X_OK: V�grehajt�si enged�ly
					 F_OK: L�tez�s (Ha a f�jl l�tezik, de nincs semmilyen hozz�f�r�se, akkor gyakorlatilag "nem l�tezik")
	Eredm�nyek: 0: Enged�lyezett hozz�f�r�s
				-1: Elutas�t�s (Oka ki�rathat� pl. a perror-al)
				
	<stdio.h>: Tartalmazza a FILE t�pust (FILE* f)
	fopen
	fclose
	
	Text hozz�f�r�s:
	fprintf
	fputs
	fgets
	
	Bin�ris hozz�f�r�s:
	fwrite
	fread
	
	fseek
	feof
*/

f = fopen(f�jln�v, m�d) // F�jl megnyit�sa �ltal�nosan, ha sikertelen akkor f == NULL
fclose(f) // F�jl bez�r�sa

/* M�dok: "r": olvas�sra nyit meg egy f�jlt
		  "w": l�trehoz egy �j f�jlt, olvas�sra �s �r�sra
		  "a": ha nem l�tezik, l�trehoz egy �j f�jlt olvas�sra �s �r�sra
		  "r+": olvas�sra �S �r�sra nyit meg egy f�jlt
		  "w+": mindenk�ppen l�trehoz egy �j f�jlt olvas�sra �s �r�sra
		  "a+": ?
*/

fputs(string, f) // Bele�rja a f�jlba a megadott stringet, negat�v az eredm�nye ha hiba volt
fprintf(f, formstr, ...) // Bele�rja form�zva a f�jlba a stringet, eredm�nye a ki�rt karakterek sz�ma

fgets(string, maxchar /* beolvashat� maxim�lis karakter */, f) // Beolvassa a f�jlt a string-be, NULL ha semmit sem tudott beolvasni.
// Akkor fejezi be ha el�ri a maxchart, ha sorv�g karaktert olvas, ha EOF (vagy ha megtelik a string)

feof(f) // Ha v�ge a f�jlnak nem 0 �rt�ket ad vissza (azaz t�lolvasunk a f�jlon -> visszat�r 0-val)

/* Bin�ris f�jlkezel�s
   Minden megynit�si m�d ut�n ker�l egy "b" karakter, a m�d nem v�ltozik ett�l, csak bin�ris lesz
 */
 
 fwrite(els� ki�rand� elem c�me, elemm�ret, elemsz�m, f) // �r�s a f�jlba
 fread(kezd�c�m, ... ) // T�nylegesen beolvasott elemsz�m
 
 //t egy t�mb
 fread(t, sizeof(*t), sizeof(t) / sizeof(*t))
 
 //v egy v�ltoz�
 fread(&v, sizeof(v), 1)

/*
H�zi feladat:
	El�rhet� a holnapon
*/