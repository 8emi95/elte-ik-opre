#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "field.h"
#include "player.h"

struct data{
 std::string name;
 int value;
 char type;
};

class Game{
public:
 Game();
 ~Game();
 void run();
private:
 std::ifstream file;
 Field **board;
 Player **Players;
 
 int count_fields; 
 int count_player;
 
 void load();
 void rounds();
};
