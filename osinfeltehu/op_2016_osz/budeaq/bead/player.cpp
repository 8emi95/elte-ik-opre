#include"player.h"
#include"field.h"

int Player::change_money(const int &value){
 money += value;
}
 
void Player::pay(Player *p, const int &value){
 p->money += money;
 money -= value;
}


//NEM JÓ, NEM CIKLIKUS!
void Player::move(const int &num){
  current = current + num;
}

void Player::play(Field *f){
 switch(f->get_type()){
  case P : if(!(f->has_owner()) || f->get_owner() == *this){
             buy(f);
           }else{
            /* if(f->has_house()){
                   pay(f->get_owner(), 4000);
             }else pay(f->get_owner(), 1000);
           */  
            f->play(); 
           }
   break;
  case S : f->play(*this);  break;
  case L : f->play(*this);  break;
 }
}

void Greedy::buy(Field *f){
 if(money > 2000 && !(f->has_house())){
  f->buy(*this);
  change_money(-2000);
 }else if(money > 500 && !(f->has_house())){
  f->buy(*this);
  change_money(-500);
 }
}

void Careful::buy(Field *f){
 set_curr_money();
 if(get_curr_money() > 2000 && !(f->has_house())){
  f->buy(*this);
  change_money(-2000);
 }else if(get_curr_money() > 500 && !(f->has_house())){
  f->buy(*this);
  change_money(-500);
 }
}

void Tactican::buy(Field *f){
 if(money > 2000 && !(f->has_house()) && get_count() % 2 == 1){
  f->buy(*this);
  change_money(-2000);
 }else if(money > 500 && !(f->has_house()) && get_count() % 2 == 1){
  f->buy(*this);
  change_money(-500);
 }
 set_count(); 
}

