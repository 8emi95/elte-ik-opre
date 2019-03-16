#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

//fork akt futo folyamatot tukrozi (gyerekfolyamat), processz tablaba is bekerul

//HF hogy csinalunk egy helyett 2gyereket?
//HF mi lenne hogy ha 3-4 gyereket v tobbet is tudnank csinalni
//tanacs ha a gyerekszam tekinteteben nagy szammal szeretnenk probalkozni eloszor
//ne olyan szerevreren tegyuk meg amit leterhelhet tulzottan
//szoval ne probalkozzunk 2-3-4 nel tobbel os.inf.en


// if(child>0) {printf("szulo\n");}


int main()
{
 //hogy tenyleg csak 4 folyamat jojjon letre:
 //meg kell vizsgalni hogy ha epp aktualisoan a szuloben vagyok, csak akkor 
 //mukodjon a fork() hivas
 
 //pl butabb megoldas: jegyezzuk meg a szulo folyamat pid szamat
 //es akkor nem kezd el 2 hatvanyaval szaporodni a folyamatsazm
 
 //fontos a szuloben varni a gyerekfolyamatok befejezesere 
 //		waitpid(,,,) fuggvennyel
 
 
 
 
 return 0;
}