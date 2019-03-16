/*
    Tárgy: Elte IK Operációs rendszerek 1. Beadandó
    Készítette: Sándor Balázs - AZA6NL
    Leadás: 2016.11.07
	Git: https://github.com/SandorBalazsHU/elte-ik-os-1

    A menünézetek megvalósításai.
    A nézetek részletezését lásd a Documentation.txt-ben.
*/
#ifndef VIEWS_H
#define VIEWS_H

#include "../model/Events.h"

    #define sleepTime 2
    int  view1(Events*);
    void view2(Events*);
    void view3(Events*);
    void view4(Events*);
    void view5(Events*);
    void view6(Events*);
    void view7(Events*, int);
    void view8(Events*, int);
    void view9(Events*, int, int);
    void view10(Events*, int);
    void view11(Events*);
#endif