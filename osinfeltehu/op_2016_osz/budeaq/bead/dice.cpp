#include "Dice.h"
#include <cstdlib>
#include <fstream>

using namespace std;

Dice::Dice(){
    dice.open ("dicenumbers.txt");
}

Dice::~Dice(){
    dice.close();
}

void Dice::roll(){
 if(!dice.fail()){
  string str;
  getline(dice,str);
  random = atoi(str.c_str());
 }
 if(dice.fail()){
  random = rand()%6 + 1;
 }
}

