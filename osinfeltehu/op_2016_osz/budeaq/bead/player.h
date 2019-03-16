#pragma once

#include<string>
#include"field.h"

enum Ptype {G, C, T};

class Field;

class Player{
public:
 std::string get_name() const { return name; }
 bool is_in_game() const { return money > 0; }
 int get_money() const { return money; }
 int get_current() const { return current; } 
 int change_money(const int &value);
 void pay(Player *p, const int &value); //nincs az oszt diagramban
 void move(const int &num);  //NEM JÓ, NEM CIKLIKUS!
 void play(Field *f);

 virtual void buy(Field *f) =0;  
 virtual Ptype get_type() const =0;
 virtual ~Player(){} 
protected:
 std::string name;
 int money;
 int current; //aktuális mező
};

class Greedy : public Player{
public: 
 Greedy(std::string pname){ name = pname; money = 10000; current = 0; }
 Ptype get_type() const {return G;}
 void buy(Field *f);    
};

class Careful : public Player{
public: 
 Careful(std::string pname) { name = pname; money = 10000; current = 0; }
 void set_curr_money() { curr_money = money / 2; }
 int get_curr_money() const { return curr_money; }
 Ptype get_type() const {return C;}
 void buy(Field *f);
private:
 int curr_money;
};

class Tactican : public Player{
public: 
 Tactican(std::string pname){ name = pname; money = 10000; current = 0; count = 1; }
 void set_count() { ++count; }
 int get_count() const { return count; }
 Ptype get_type() const {return T;}
 void buy(Field *f);
private:
 int count;
};

