#ifndef __STRUKTURA___
#define __STRUKTURA___

#include <string.h>
#include <stdlib.h>

struct Resztvevo {
	char nev[100];
	int megjelent; // 0 - nem, 1 -igen
};

struct ResztvevokListaja {
	struct Resztvevo resztvevo[1024];
	int db;
};

struct Jelentes {
	int sikeres; // 0-100 közötti egész szám
	struct ResztvevokListaja resztvevok;
};

struct Jelentkezo {
    int id;
    char nev[100];
    char mail[100];
    int rendezveny_azon;
    time_t time;
};

struct Jelentkezo_struct {
    int jelentkezettek_szama;
    int kov_jelentkezett_id;
    struct Jelentkezo jelentkezok[1000];
};

struct Rendezveny {
    int id;
    char nev[100];
	char hely[100];
};

struct Rendezveny_struct {
    int rendezvenyek_szama;
    int kov_rendezveny_id;
    struct Rendezveny rendezvenyek[1000];
};

#endif