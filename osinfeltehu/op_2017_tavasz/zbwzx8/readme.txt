Alföldi Mátyás Mihály
Neptun kód:ZBWZX8

Kipróbált tesztesetek:
1.
-0 kérdésnél akarunk törölni/módosítani kérdést
-Eredmény:kiírja, hogy nincsenek kérdések
2.
-Annyi kérdést adunk meg, hogy realloc-al új helyet kell foglaljunk
-Eredmény:realloc után is minden jól fog mûködni
3.
-Megadunk kérdéseket, válaszokkal, ezután törlünk/módosítunk kérdéseket, és véglegesítünk
 pár darab/összes kérdést.
-Eredmény: kerdoiv fajlban rendesen megjelenik a dátum,a kérdések, a válaszok száma, és
 a válaszok
4.
-Hibás adatot adunk meg valamikor a program futása alatt
-Eredmény: Kiírja, hogy hibas adatot adtunk meg, és újra bekéri ezután.
(A program a számok bekérdezésekor elég helyet tárol, hogy a legutolsó pozicíón lévõ
számra is elég helyet tároljon a \0 karakterrel együtt, és ha az ember egy ennél
hosszabb számot ad meg levágja a megfelelõ ponton, hogy buffer overflow-ot elkerüljük.)


Példa teszt adat:
1 //az elsõ menûpontot választjuk
elso? //a kérdést beírjuk
2 //kiválasztjuk 2 és 4 között a válaszok számát 
e1 //megadjuk az elsõ választ
e2 //..
1 //az elsõ menûpontot választjuk
masodik? //a kérdést beírjuk
3 //kiválasztjuk 2 és 4 között a válaszok számát 
m1 //megadjuk az elsõ választ
m2 //..
m3
2 //a második menûpontot választjuk
//kiírja az eddigi kérdések és válaszokat
0 // a 0 számmal jelölt kérdést módosítjuk
elso //megadjuk az új nevet
3 //kiválasztjuk 2 és 4 között a válaszok számát
e1 //megadjuk az elsõ választ
e2 //..
e3
3 // a harmadik menûpontot válaszjuk
1 // az 1-es számmal jelölt kérdést töröljük
4 //a 4-es menûpontot választjuk
//kiírja az összes kérdést és választ
5 //az 5-ös menûpontot választjuk
0 // a 0-val jelölt kérdést a végleges kérdõívbe rakjuk
//létrejön a kerdoiv fájl a megfelelõ adatokkal,
//,és a program hiba nélkül terminál, mivel érzékeli, hogy nincs több
//választható kérdés.