#pragma once

#include <iostream>
#include "player.h"

enum Ftype {P, S, L};

class Player;

class Field{
public:
 virtual void play(Player *p) =0;
 virtual Ftype get_type() const =0;
 virtual ~Field();
};

class Property : public Field{
public:
 Property() : owner(NULL), hashouse(false){}
 bool has_owner() { return owner == NULL; }
 bool has_house() { return hashouse; }
 void play(Player *p);
 void buy(Player *p);
 void set_owner(Player *p) { owner = p; }
 void set_house(bool l) { hashouse = l; }
 Player* get_owner() { return owner; }
 Ftype get_type() const { return P; } 

private:
 Player *owner;
 bool hashouse;
};

class Services : public Field{
public:
 Services(int v) : value(v) {}
 void play(Player *p);
 Ftype get_type() const { return S; }
private:
 int value; 
};

class Luck : public Field{
public:
 Luck(int v) : value(v) {}
 void play(Player *p);
 Ftype get_type() const { return L; }
private:
 int value;
};
