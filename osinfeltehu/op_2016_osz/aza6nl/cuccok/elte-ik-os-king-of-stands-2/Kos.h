/*
    Tárgy: Elte IK Operációs rendszerek 1. Beadandó
    Készítette: Sándor Balázs - AZA6NL
	Git: https://github.com/SandorBalazsHU/elte-ik-os-1

    Egy egységes API-t biztosít. Így csak ezt az egy fejlécet kell használni.
*/

#ifndef KOS_H
#define KOS_H

//A konzolkezelő függvények gyűjteménye
#include "./view/ConsoleIO.h"

//A menünézetek megvalósításai.
#include "./view/Views.h"

//Az eseménytároló adatszerkezet és a hozzá tartozó függvények gyűjteménye.
#include "./model/Events.h"

/*Az eseményt reprezentáló szerkezet és a hozzá tartozó függvények gyűjteménye.
Ez tárolja a látogatókat is.*/
#include "./model/Event.h"

//A látogatókat reprezentáló szerkezet és a hozzá tartozó függvények gyűjteménye.
#include "./model/Visitor.h"

//A fájlműveletekért felelős függvények gyűjteménye
#include "./controller/File.h"

//A tesztfüggvények gyűjteménye
#include "./tests/Tests.h"

#endif