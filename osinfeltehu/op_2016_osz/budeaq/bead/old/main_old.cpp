#include <iostream>
#include <stdlib.h>
#include "game.h"
#include "field.h"
#include "player.h"

using namespace std;

class Menu{
    public:
        void Run();
    private:
      	void Menu_text();
        void list();
        void read();
};

int main(){
 system("clear");
 cout<<"Capitaly\n"<<endl;
 cout<<">:"<<endl;
 Menu menu;
 menu.Run();
 return 0;
}

void Menu::Run(){
    int c = 0;
    do{
    Menu_text();
    cin >> c;
     switch(c){
    	case 1: list(); break;
    	case 2: read(); break;
     }
    }while(c!=0);
   system("clear");
}

void Menu::Menu_text(){
    cout<<endl;
	cout<<"Menü:"<<endl;
	cout<<"1) Beolvasható fájlok listázása"<<endl;
        cout<<"2) Fájl beolvasása"<<endl;
        cout<<"0) EXIT"<<endl<<endl;
        cout<<"Kérem válasszon: ";
}

void Menu::list(){
	system("clear");
	cout<<">:"; system("ls | grep .txt");
}

void Menu::read(){
	system("clear"); 
}

