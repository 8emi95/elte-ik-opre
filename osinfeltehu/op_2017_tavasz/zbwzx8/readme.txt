Alf�ldi M�ty�s Mih�ly
Neptun k�d:ZBWZX8

Kipr�b�lt tesztesetek:
1.
-0 k�rd�sn�l akarunk t�r�lni/m�dos�tani k�rd�st
-Eredm�ny:ki�rja, hogy nincsenek k�rd�sek
2.
-Annyi k�rd�st adunk meg, hogy realloc-al �j helyet kell foglaljunk
-Eredm�ny:realloc ut�n is minden j�l fog m�k�dni
3.
-Megadunk k�rd�seket, v�laszokkal, ezut�n t�rl�nk/m�dos�tunk k�rd�seket, �s v�gleges�t�nk
 p�r darab/�sszes k�rd�st.
-Eredm�ny: kerdoiv fajlban rendesen megjelenik a d�tum,a k�rd�sek, a v�laszok sz�ma, �s
 a v�laszok
4.
-Hib�s adatot adunk meg valamikor a program fut�sa alatt
-Eredm�ny: Ki�rja, hogy hibas adatot adtunk meg, �s �jra bek�ri ezut�n.
(A program a sz�mok bek�rdez�sekor el�g helyet t�rol, hogy a legutols� pozic��n l�v�
sz�mra is el�g helyet t�roljon a \0 karakterrel egy�tt, �s ha az ember egy enn�l
hosszabb sz�mot ad meg lev�gja a megfelel� ponton, hogy buffer overflow-ot elker�lj�k.)


P�lda teszt adat:
1 //az els� men�pontot v�lasztjuk
elso? //a k�rd�st be�rjuk
2 //kiv�lasztjuk 2 �s 4 k�z�tt a v�laszok sz�m�t 
e1 //megadjuk az els� v�laszt
e2 //..
1 //az els� men�pontot v�lasztjuk
masodik? //a k�rd�st be�rjuk
3 //kiv�lasztjuk 2 �s 4 k�z�tt a v�laszok sz�m�t 
m1 //megadjuk az els� v�laszt
m2 //..
m3
2 //a m�sodik men�pontot v�lasztjuk
//ki�rja az eddigi k�rd�sek �s v�laszokat
0 // a 0 sz�mmal jel�lt k�rd�st m�dos�tjuk
elso //megadjuk az �j nevet
3 //kiv�lasztjuk 2 �s 4 k�z�tt a v�laszok sz�m�t
e1 //megadjuk az els� v�laszt
e2 //..
e3
3 // a harmadik men�pontot v�laszjuk
1 // az 1-es sz�mmal jel�lt k�rd�st t�r�lj�k
4 //a 4-es men�pontot v�lasztjuk
//ki�rja az �sszes k�rd�st �s v�laszt
5 //az 5-�s men�pontot v�lasztjuk
0 // a 0-val jel�lt k�rd�st a v�gleges k�rd��vbe rakjuk
//l�trej�n a kerdoiv f�jl a megfelel� adatokkal,
//,�s a program hiba n�lk�l termin�l, mivel �rz�keli, hogy nincs t�bb
//v�laszthat� k�rd�s.