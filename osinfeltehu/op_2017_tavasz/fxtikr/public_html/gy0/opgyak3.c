// Paraméterek átadása
/*
	Az indítási paraméterek char**-ként adódnak át (tömbök tömbjei)
*/

int main(int argc, char** argv) //argc: argumetumok száma, 0. argumentum maga a program indítása (pl.: ./a.out)
//argc-edik elem: nullpointer

// Struktúrák

struct típusnév
{
	mezõtípus1 mezõazonosító1;
	...
	mezõtípusN mezõazonosítóN;
};

struct típusnév változónév // Nem elég a típusnév és az azonosító a változó létrehozásához, kell a struct elé!
változónév.mezõazonosító1 // Ponttal hivatkozhatunk a mezõkre

struct típusnév* ptr // Pointer
(*ptr).mezõnév VAGY ptr->mezõnév // Hivatkozás pointer esetén a mezõkre (A zárójel az elõbbinél fontos a precedencia miatt)

// Hibakezelés

/*
	<errno.h>: Tartalmaz egy errno rendszerváltozót, aminek kezdeti értéke 0. 
	Korábbi implementációkban egészként implementálják, késõbbiekben makróként.
	Mindig az utolsó hibának az értékét találjuk benne. (Az értékek jelentése is implementációfüggõ)
	
	<stdio.h>: Tartalmazza a perror(char* saját_hibaüzenet) függvényt.
	Kiírja a hibaüzenetet, kettõspontot rak és szóközt utána, majd a hibának megfelelõ hibaüzenetet írja ki.
*/

// Fájlkezelés

/*
	<unistd.h>: Tartalmazza a rendszerhívásokhoz szükséges dolgokat. (A szerveren amin dolgozunk nem kell külön includeolni)
	
	access(char* fájl, mód): A hozzáférési módot ellenõrzi
	mód paraméterek: R_OK: Olvasási engedély
					 W_OK: Írási engedély
					 X_OK: Végrehajtási engedély
					 F_OK: Létezés (Ha a fájl létezik, de nincs semmilyen hozzáférése, akkor gyakorlatilag "nem létezik")
	Eredmények: 0: Engedélyezett hozzáférés
				-1: Elutasítás (Oka kiíratható pl. a perror-al)
				
	<stdio.h>: Tartalmazza a FILE típust (FILE* f)
	fopen
	fclose
	
	Text hozzáférés:
	fprintf
	fputs
	fgets
	
	Bináris hozzáférés:
	fwrite
	fread
	
	fseek
	feof
*/

f = fopen(fájlnév, mód) // Fájl megnyitása általánosan, ha sikertelen akkor f == NULL
fclose(f) // Fájl bezárása

/* Módok: "r": olvasásra nyit meg egy fájlt
		  "w": létrehoz egy új fájlt, olvasásra és írásra
		  "a": ha nem létezik, létrehoz egy új fájlt olvasásra és írásra
		  "r+": olvasásra ÉS írásra nyit meg egy fájlt
		  "w+": mindenképpen létrehoz egy új fájlt olvasásra és írásra
		  "a+": ?
*/

fputs(string, f) // Beleírja a fájlba a megadott stringet, negatív az eredménye ha hiba volt
fprintf(f, formstr, ...) // Beleírja formázva a fájlba a stringet, eredménye a kiírt karakterek száma

fgets(string, maxchar /* beolvasható maximális karakter */, f) // Beolvassa a fájlt a string-be, NULL ha semmit sem tudott beolvasni.
// Akkor fejezi be ha eléri a maxchart, ha sorvég karaktert olvas, ha EOF (vagy ha megtelik a string)

feof(f) // Ha vége a fájlnak nem 0 értéket ad vissza (azaz túlolvasunk a fájlon -> visszatér 0-val)

/* Bináris fájlkezelés
   Minden megynitási mód után kerül egy "b" karakter, a mód nem változik ettõl, csak bináris lesz
 */
 
 fwrite(elsõ kiírandó elem címe, elemméret, elemszám, f) // Írás a fájlba
 fread(kezdõcím, ... ) // Ténylegesen beolvasott elemszám
 
 //t egy tömb
 fread(t, sizeof(*t), sizeof(t) / sizeof(*t))
 
 //v egy változó
 fread(&v, sizeof(v), 1)

/*
Házi feladat:
	Elérhetõ a holnapon
*/