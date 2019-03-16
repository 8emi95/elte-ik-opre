#pragma once

#include <fstream>

class Dice{
public:
 Dice();
 virtual ~Dice();
 int get_random_number() {roll(); return random;}
 void roll();
private:
 std::ifstream dice;
 int random;
};
