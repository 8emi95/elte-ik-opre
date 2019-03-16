//#ifndef KOS_H
//#define KOS_H

#include <time.h>

struct rendezveny_t {
  int id;
  char nev[256];
  int vendegek;
};

struct vendeg_t {
  int rendezveny_id;
  int hanyadik_vendeg;
  char nev[256];
  char mail[256];
  time_t jelentkezes_ideje;
};

//#endif
