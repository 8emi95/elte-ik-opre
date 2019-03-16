/*
    Tárgy: Elte IK Operációs rendszerek 1. Beadandó
    Készítette: Sándor Balázs - AZA6NL
	Git: https://github.com/SandorBalazsHU/elte-ik-os-1

    A látogatókat reprezentáló adatszerkezetethez tartozó fügvények.
*/

//mallochoz szügséges
#include <stdlib.h>
//strcpy-hez
#include <string.h>
#include <time.h>
#include "./Visitor.h"

//Új látogató létrehozása
struct Visitor* newVisitor(char* name, char*email, time_t date)
{
    struct Visitor* _this = malloc(sizeof(struct Visitor));
    strcpy(_this->name, name);
    strcpy(_this->email, email);
    _this->date=date;
    return _this;
}

struct Visitor* modifyVisitor(Visitor* visitor, char* name, char* email)
{
    strcpy(visitor->name, name);
    strcpy(visitor->email, email);
    return visitor;
}

//A Visitor típus destruktora
void freeVisitor(Visitor* visitor)
{
    free(visitor);
}