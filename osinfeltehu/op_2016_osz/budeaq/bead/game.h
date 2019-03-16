#pragma once

#include "dice.h"
#include "field.h"
#include "player.h"
#include <iostream>
#include <fstream>

struct data{
 int c; 
};

class Game{
public:
 Game();
 ~Game();
 void print();
private:
 std::ifstream x;
 int count;
};
