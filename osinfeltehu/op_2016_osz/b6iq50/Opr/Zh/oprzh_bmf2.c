#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

#define CSONEV "egyedicso_b6iq50.ftc"

#define FAJL "bmf.data"
#define TMP "bmf_temp.data"

/*
szülő - gyerek
gyerek megkapja, hogy ő hanyadik szerelvény (1-random)
	és megkapja hogy hány szerelvény van (paranscsorról)
gyerek 10%-kal elcsúszik vagy lefagy
gyerek kiírja hogy ő hanyadik szerelvény
		még hány lesz utána
		illetve, hogy célba ért-e
gyerek (ha végzett) üzen a szülőnek, hogy sikeresen célba ért-e vagy bevontatták (utóbbi esetben ok)
*/

const char * ido_formatum(time_t t);

int main(int argc,char** argv){
	
	return 0; 
} 

//http://www.tankonyvtar.hu/hu/tartalom/tamop412A/2010-0011_szamalap2/lecke11_lap4.html

const char * ido_formatum(time_t t){
	static char buff[20];
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));
	return buff;
}