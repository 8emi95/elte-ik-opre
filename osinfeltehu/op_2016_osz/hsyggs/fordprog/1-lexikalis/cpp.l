%option noyywrap c++ yylineno

%{
#include <iostream>
#include <cstdlib>
%}

SZAMJEGY    [0-9]
WS          [ \t\n]
AZON_START  [a-zA-Z_]

%%

int     	std::cout << "int" << std::endl;
string     	std::cout << "string" << std::endl;
main     	std::cout << "main" << std::endl;
unsigned   	std::cout << "egesz" << std::endl; 
bool     	std::cout << "logikai" << std::endl;
if          std::cout << "if" << std::endl;
else        std::cout << "else" << std::endl;
while       std::cout << "while" << std::endl;
cout        std::cout << "cout" << std::endl;
cin        	std::cout << "cin" << std::endl;
";"         std::cout << "utasitas vege" << std::endl;
">>"        std::cout << "olvas" << std::endl;
"<<"        std::cout << "ir" << std::endl;
"="         std::cout << "ertekadas" << std::endl;
"=="        std::cout << "egyenlo" << std::endl;
"<"         std::cout << "kisebb" << std::endl;
">"         std::cout << "nagyobb" << std::endl;
"+"         std::cout << "plusz" << std::endl;
"-"         std::cout << "minusz" << std::endl;
"*"         std::cout << "szorzas" << std::endl;
"/"         std::cout << "osztas" << std::endl;
"%"         std::cout << "maradek" << std::endl;
&&	        std::cout << "and" << std::endl;
"||"        std::cout << "or" << std::endl;
!           std::cout << "not" << std::endl;
"{"       	std::cout << "begin" << std::endl;
"}"        	std::cout << "end" << std::endl;
"("         std::cout << "balzarojel" << std::endl;
")"         std::cout << "jobbzarojel" << std::endl;

{SZAMJEGY}+ std::cout << "szamliteral: " << YYText() <<std::endl;
true        std::cout << "true" << std::endl;
false       std::cout << "false" << std::endl;

"//".*\n    // nem kell semmit csinalni

{AZON_START}({AZON_START}|{SZAMJEGY}|_)*    std::cout << "azonosito: " << YYText() <<std::endl;

{WS}+   // feher szokozok: semmi teendo

. {
    std::cerr << lineno() << ". sor: Lexikalis hiba!" << std::endl;
    exit(1);
}

%%
