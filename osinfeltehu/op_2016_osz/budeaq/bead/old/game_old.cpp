#include "game.h"
/*#include "filed.h"
#include "player.h"*/
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

Game::Game(){
 std::string name;
 system("clear");
 std::cout<<"Add meg a filenevet: "<<std::endl;
 std::cin>>name;
 file.open(name.c_str());
 if(x.fail()){
  system("clear");
  std::cout<<"Nincsen ilyen file!"<<std::endl;
//  exit(1);
 }
 file>>count_fields;
 if(file.fail() || fail.eof()){
  system("clear");
  std::cout<<"Üres a file!"<<std::endl;
//  exit(1);
 }
 if(count_fields == 0){
  system("clear");
  std::cout<<"Nincsen tábla!"<<std::endl;
//  exit(1);
 }
 
 board = new Field* [count_fields];
 
// players = new Player* [count_player];
}

Game::~Game(){
    file.close();
}

void Game::load(){
 data rekord;
 
 for(int i=0; i<count_fields && !file.fail(); i++){
  file >> rekord.name;
  file >> rekord.value;

  switch(rekord.name){
   case "Property" : board[i] = new Property(); break;
   case "Services" : board[i] = new Services(rekord.value); break;
   case "Luck" :     board[i] = new Luck(rekord.value); break;
  }
 }
 
 file >> count_player;

 if(file.fail() || fail.eof()){
  system("clear");
  std::cout<<"Üres a file!"<<std::endl;
  exit(1);
 }
 if(count_player == 0){
  system("clear");
  std::cout<<"Nincsenek játékosok!"<<std::endl;
  exit(1);
 } 

 players = new Player* [count_player];

 for(int i=0; i<count_fields && !file.fail(); i++){
  file >> rekord.name;
  file >> rekord.type;

  switch(rekord.type){
   case "G" : players[i] = new Greedy(rekord.name); break;
   case "C" : players[i] = new Careful(rekord.name); break;
   case "T" : players[i] = new Tactican(rekord.name); break;
  }
 }
}

void Game::rounds(){
 Dice dice;
 int lose = 0;
 int i = 0;
 int c = 0;
 while(lose != 2){
  if(players[i]->is_in_game()){
   players[i]->move(dice.get_random_number());
   c = players[i]->get_current();
   player[i]->play(board[c]);  
 
   if(players[i]->is_in_game()) ++lose; 
  }
  ++i;
 }
 std::cout<<"A másodszorra kiesett játékos: "<<players[i]->get_name()<<std::endl; 
}

void Game::run(){
 load();
 rounds();
}
