#include"field.h"

void Luck::play(Player *p){
 p->change_money(value); 
}

void Services::play(Player *p){
 p->change_money((-1)*value);
}

void Property::play(Player *p){
 if(owner != NULL){
  if(hashouse) p->pay(owner, 2000);
  else p->pay(owner, 500);
 }
}

void Property::buy(Player *p){
 if(!hashouse){
  owner = p;
  p->change_money(-1000);
 }
 else{
  hashouse = true;
  p->change_money(-4000);
 }
}
