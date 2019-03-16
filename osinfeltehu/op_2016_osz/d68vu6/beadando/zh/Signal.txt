      
Jelzések (signals)
Azaz a folyamatok informálása előre definiált jelek segítségével...
 

  Definíció

  Jelzések kezelése

  signal

  kill, raise

  alarm, pause

  setjmp, longjmp

  sigaction

 

 

  Folyamatokkal végzett műveletek

  Definíció

A jelzés olyan szoftveres megszakítás, amelyet egy folyamat kap valamilyen esemény bekövetkeztekor. A jelzéseket főként kivételek fellépésekor, valamilyen riasztáskor, váratlan befejezéskor, vagy ritkábban folyamatok közti kommunikációkor használják. Ezek jelentik a legegyszerűbb, legrégibb, de legmerevebb kommunikációt a folyamatok között. A jelzéseket felfoghatjuk akár figyelmeztetésként is egy folyamat felé.

Minden jelzésnek van egy neve, amely kötelezően a SIG előtaggal kezdődik. Ezek a nevek szimbolikus konstansok formájában vannak definiálva, és szigorúan pozitív egész számokat jelölnek.

Egy jelzés által hordozott információ a folyamat számára minimális, hiszen csupán a típust (magát a számot) foglalja magába. Minden jelzésnek van egy forrása és valamilyen okból keletkezik.

A jelzések okait a következőképpen csoportosíthatjuk:

a. futtatás során fellépő hibák:

- a folyamat megengedett határain kívül eső címzés (ekkor egy SIGSEGV jel keletkezik),
- írási próbálkozás egy csak olvasható memóriazónára (például a kódszegmensbe),
- magasabb privilégium szinttel rendelkező utasítások végrehajtása,
- hardverhiba detektálása esetén,

b. szoftverhiba egy rendszerfüggvény hívásakor:

- nem létező rendszerfüggvény,
- egy pipe állományba való írás, anélkül, hogy egy másik folyamat olvasná azt (SIGPIPE),
- nem megfelelő paraméter használata egy függvényhívásban,
- valamilyen szükséges erőforrás hiánya egy függvény végrehajtása során (például nincs elegendő külső memória egy állomány betöltésére),

c. kommunikáció két folyamat vagy folyamatcsoportok között úgy, hogy a folyamat egy jelet kap a kill függvényen keresztül,

d. háttérben futó folyamat befejezése a kill parancs használatával,

e. egy folyamat erőteljes befejezése a rendszer által egy SIGKILL jel segítségével (például egy shutdown parancs esetén),

f. egy folyamat az idő függvényében egy SIGALRM jelet küld magának,

g. a felhasználó a billentyűzetről megszakít egy terminálon futó folyamatot,

- egy folyamat feladása (Ctrl-\),
- megszakítás generálása (Ctrl-C/Break vagy DELETE),
- a terminálhoz való újrakapcsolódás,

h. egy folyamat befejezése, amint éppen az exit függvényt hajtja végre, vagy amint a folyamat meghívja a SIGCHLD jelet a signal rendszerfüggvény segítségével,

i. egy folyamat megjelölése.

A jelzés típusa értelemszerűen tükrözi a jelzés okát is. A Függelékben bemutatjuk a UNIX SVR2 által definiált 19 jelzéstípust. A Unix SVR4 és 4.3+BSD verzióiban már 31 típusú jelzés szerepel. Ezek részletes bemutatását lásd a bibliográfiában.

Egy folyamat, amely valamilyen jelet kapott a következőképpen viselkedhet:

a. Figyelmen kívül hagyja a jelet és folyatatja az aktivitását. A jelzések közül csak a SIGKILL nem halasztható el (a rendszernek jogában áll befejezni bármelyik folyamatot).

b. A jel kezelését a rendszer magjára bízza. Ebben az esetben, kivétel a SIGCLD és SIGPWR (és újabban a SIGINFO, SIGURG és SIGWINCH), amelyeket figyelmen kívül hagy, az összes többi jelzés a folyamat befejezéséhez megy – ez az implicit működés.

c. Egy saját eljárással automatikusan lekezeli a jelet, ahogy az megjelenik. Az eljárás befejeztével a program ott folytatódik, ahol abbahagyta.

A fork rendszerfüggvény hívására a létrejövő gyerekfolyamat a szülőtől örökli a jelzésekhez kapcsolódó eseményeket is.

Az exec függvény a rendszer magjára hagyja a jelzések kezelését, még akkor is, ha ezeket végül a folyamat hajtja végre. Csak az előrelátott, eredeti tevékenységeket tartja meg, figyelmen kívül hagyva egyes jelzéseket a folyamat felé. Ez azért van, mert az exec hívására az aktuális folyamat kódszegmense elvesztődik, s ezáltal a jelzések kezelését végző eljárások is megsemmisülnek.

Függetlenül attól, hogy miként reagál egy program egy bizonyos jelre, a tevékenység befejeztével – ha a jelet nem hagyjuk figyelmen kívül – a rendszer újrainicializálja az értékeket, felkészülve ezáltal egy későbbi jel fogadására. Kivételt képeznek ez alól a SIGILL és a SIGTRAP jelek, amelyeket nagyon gyakran előfordulnak, ezért ezek többszöri aktualizálása igencsak lassítaná a kezelésüket.

Ha egy függvény végrehajtása során a rendszer egy jelet érzékel, a hívás hibával fog befejeződni. Sajátos esetben, ha a rendszer egy lassú periférián próbál ki/bemeneti műveletet végezni és egy jelzést kap, a függvény -1 értéket (azaz hibát) fog visszatéríteni. Ebben az esetben az errno változó az EINTR értéket fogja tartalmazni. Ilyenkor az errno változó tesztelése után, a program újra próbálkozhat a művelet elvégzésével.

A folyamathoz érkezett jelzések a rendszer nem tárolja egy várakozási sorban. Éppen ezért, ha a folyamat egy jelet figyelmen kívül hagyott, az mindörökre elveszett. Egyetlen kivétel a SIGCLD jel, amelyet a gyerek küld a szülőnek, tudatva, hogy pályafutását befejezte. Ezt a jelet a szülő egy wait függvényhívás során érzékelheti. Ez azért fontos, mert egy gyerek már azelőtt befejeződhet, hogy a szülő kiadná a wait parancsot. Ha viszont a jelet nem őriznénk meg, a szülő leblokálna miközben hiába várná a gyerek befejeződését. Mivel a bejövő jelzéseket a folyamatok nem tudják megőrizni ez a mechanizmus nem túl hatékony, és sok hibával járhat.

Összefoglalás: Egy jelet elküldhetünk bármely pillanatban, időszakosan egy másik folyamattól, de általában a kerneltől indulnak valamilyen különleges esemény hatására. A jelek nem tartalmaznak információt csak amit a típusuk által hordoznak. A jelzések másik hátránya, hogy a folyamatok nem tudják azonosítani a beérkezett jelek forrását.

  Jelzések kezelése

  signal

A signal függvény szerepe biztosítani a jelzések programból történő kezelését.

Alakja:

#include <signal.h>

int (*signal (jelzes, fuggveny))();
int jelzes;
int (*fuggveny)();

vagy

#include <signal.h>

void (*signal (int jelzes, void (*fuggveny)(int)))(int);

Az első változat a régebben használatos stílus (amikor még nem volt értelmezve a void típus). A második változatban az argumentumok típusa is fel van tüntetve: egy egész szám és egy mutató egy függvényhez. 

A jelzes paraméter a jel száma vagy az ennek megfelelő szimbólum, amelynek a kezelését szeretnénk testreszabni.

A fuggveny argumentum leírja, hogyan kezelje a folyamat az adott jelet. A következő értékeket veheti fel:

- SIG_DFL – A jelzések kezelését a rendszer magja végzi. Ez a jelzések implicit kezelése.

- SIG_IGN – Figyelmen kívül hagyja az adott  jelet (kivétel a SIGKILL és a SIGSTOP).

- mutató egy függvényhez, amely lekezeli a jelet – Ebben az esetben egy jel érkezésekor az adott függvény meghívódik. Ez a függvény a kezelő rutin nevét viseli. Egyetlen argumentuma van, ami éppen a kezelni kívánt jel számát jelenti. Miután egy jelzést ily módon lekezeltünk, ugyanannak a jelnek a következő érzékelése már implicit módon történik. Tehát, ha ugyanezt a függvény még egyszer szeretnénk használni megfelelő intézkedéseket kell tennünk.

A függvény visszatérési értéke az előzőleg definiált függvény erre a jelre. Hiba esetén a SIG_ERR konstanst kapjuk, amelynek definíciója:

#define SIG_ERR (void (*)())-1;

A signal függvény bemutatására tekintsük a következő példát:

#include <signal.h>
#include "hdr.h"

static void sig_usr1(int);         /* generat cu kill -USR1 <pid> */
static void sig_intr(int);         /* generat la Ctrl-C si rearmat */
static void sig_quit(int);         /* generat cu Ctrl-\ si resetat */
static void sig_alarm(int);        /* generat dupa scurgerea timpului t din alarm(t) */

int main(void)
{
  if (signal(SIGALRM, sig_alarm) == SIG_ERR)
    err_sys("hiba: signal(SIGALRM, ...)");
  if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
    err_sys("hiba: signal(SIGUSR1, ...)");
  if (signal(SIGINT, sig_intr) == SIG_ERR)
    err_sys("hiba: signal(SIGINT, ...)");
  if (signal(SIGQUIT, sig_quit) == SIG_ERR)
    err_sys("hiba: signal(SIGQUIT, ...)");
  for ever pause();
}

static void sig_alarm(int sig)
{
  printf("SIGALRM jelet vettem...\n");
  return;
}

static void sig_quit(int sig)
{
  printf("SIGQUIT jelet vettem...\n");
  if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
    err_sys("nem lehet visszaallitani ezt a jelet...");
  return;
}

static void sig_intr(int sig)
{
  printf("SIGINT jelet vettem...\n");
  if (signal(SIGINT, sig_intr) == SIG_ERR)
    err_sys("nem lehet ujra betolteni...");
  return;
}

static void sig_usr1(int sig)
{
  printf("SIGUSR1 jelet vettem...\n");
  alarm(1);
  printf("a riasztas 1 masodperc mulva elindul!\n");
  return;
}

Az alarm függvény a hívásától számított 1 másodperc leteltével egy SIGALRM jelet generál.

Ha a programot háttérben futtatjuk, a következőképpen viselkedik:

$ a.out&
[1] 324                                      a shell kiírja a folyamat azonosítóját

$ kill -USR1 324                             a folyamatnak egy SIGUSR1 jelet küld
SIGUSR1 jelet vettem...                      generálja a jelet 1 másodperc múlva
a riasztas 1 masodperc mulva elindul!
SIGALRM jelet vettem...                      a SIGALRM jelet generálta

$ kill 324                                   egy SIGTERM jelet küld

Ha azonban a programot nem a háttérben futtatjuk:

$ a.out
                                             leütünk egy Ctrl-C billentyűt
SIGINT jelet vettem...
SIGINT jelet vettem...
                                             leütünk egy Ctrl-\ billentyűt
SIGQUIT jelet vettem...
                                             még egyszer leütünk egy Ctrl-\ billentyűt
$

A SIGINT-et kezelő rutin újra és újra betölti saját magát minden alkalommal amikor végrehajtódik. Erre azért van szükség, mert alapértelmezésben ez az eljárás csak az első jel érkezéséig él. Ezzel szemben a SIGQUIT-hoz tartozó eljárást nem töltjük be csak egyszer, ezért másodikszor már az eredeti rutin szerint jár el, és bezárja a programot.

  kill, raise

A kill függvény egy jelet küld egy folyamatnak vagy egy folyamatcsoportnak. A raise megengedi a folyamatnak, hogy saját magának is küldjön jelet. Az első függvény a POSIX.1 verzióban van definiálva, míg a második az ANSI C-ben.

Szintaxisuk:

#include <sys/types.h>
#include <signal.h>

int kill(pid_t pid, int jel);
int raise(int jel);

Mindkét folyamat 0 értéket térít vissza, ha a művelet sikeres volt, és -1-et hiba esetén. A pid változó azt a folyamatot, vagy folyamatcsoportot jelöli, aminek a jel jelet szeretnénk küldeni.

A pid változó értékei a következők lehetnek:

- pid > 0  –  a jelet a pid azonosítójú folyamatnak küldi,

- pid = 0  –  a jelet minden olyan folyamatnak elküldi az aktuális csoporton belül, amelyekhez van joga (kivétel a swapper (0), az init (1) és a pagedaemon (2) folyamatok); ez a dolog nagyon gyakori, amikor a kill 0 utasítással töröljük a háttérben futó folyamatokat, anélkül, hogy az azonosítóikat megadnánk,

- pid < 0,

- pid ≠ -1  –  a jelet minden olyan folyamatnak elküldi, amelyeknek az ID-je megegyezik a pid változó abszolút értékével (és természetesen van ehhez joga az adott folyamatnak),

- pid = -1  –  a POSIX.1 ezt a lehetőséget nem specifikálta; az SVR4 és 4.3+BSD ezt az értéket a broadcast jeleknél használja; ezek nem küldődnek el a fent már említett folyamatokhoz; ha a küldő a superuser, akkor a jelet minden folyamat megkapja; ezt a SIGTERM jel küldésekor szokták használni, azért, hogy a rendszert felfüggesszék.

A folyamatok közti jelküldésnél a szabály az, hogy a küldőnek legyen joga a jelet elküldeni (a valódi vagy effektív felhasználó ID-ja legyen egyenlő a valódi vagy effektív vevő uid-jével). A superuser bármelyik folyamathoz küldhet jeleket.

Kivételt képez a fenti szabály alól a SIGCONT jel, amelyet bármely folyamatnak el lehet küldeni, amely ugyanahhoz a géphez tartozik.

A POSIX.1 szabvány definiálja a 0 jelet is, amely a nul jelzésnek felel meg. A kill függvény 0 paraméterrel nem küldi el a megadott jelet, hanem csak ellenőrzi, hogy létezik-e a pid azonosítójú folyamat. Ha a folyamat nem létezik, a függvény -1-et térít vissza és az errno értéke ESRCH lesz.

A raise függvény implementációja a kill függvény segítségével:

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

int raise(int jel)
{
  return (kill(getpid(), jel));
}

  alarm, pause

Az alarm függvény lehetővé teszi egy időzítő beállítását. A megadott idő (mp másodperc) elteltével egy SIGALRM jelet ad ki. Ha a folyamat a jelet figyelmen kívül hagyja, vagy nem érzékeli, a függvény alapbeállítás szerint befejezi a folyamatot. Alakja:

#include <unistd.h>
unsigned int alarm(unsigned int mp);

A visszatérített érték vagy 0, vagy az előző SIGALRM jelzés kiadása óta eltelt másodpercek száma.

Minden folyamathoz egyetlen időmérő (óra) tartozik, ezért a függvénynek egy újabb meghívása esetén az előző (mp) érték felülíródik. Ha az mp értéke 0, az előzőleg kiadott SIGALRM kérések törlődnek.

Mivel a SIGALRM implicit a folyamat befejezéséhez vezet, több folyamat, amely az alarm függvényt használja érzékeli ezt a jelet. Mielőtt a folyamat befejeződne lehetőségünk nyílik különféle utómunkák (például törlések) végrehajtására.

A pause függvény felfüggeszti (várakozási állapotba helyezi) a hívó folyamatot a legelső jel érkezéséig.

#include <unistd.h>
int pause(void);

Ha a bejövő jelet a folyamat nem kezeli le vagy figyelmen kívül hagyja, akkor ez a művelet a folyamat befejezéséhez vezet. A program pause függvényből csak a kapott jel lekezelése után jön ki. Ezért a függvény a minden esetben a -1 értéket téríti vissza, míg az errno változó értéke EINTR lesz.

A pause függvényt legtöbbször az alarm-mal együttesen szoktuk használni.

Az alarm függvényt nagyon gyakran alkalmazzuk olyan esetekben, amikor egy bizonyos műveletet valamilyen időintervallumon belül szeretnénk elvégezni. Amennyiben a műveletet a megadott idő alatt nem sikerült elvégezni a végrehajtást felfüggesztjük.

Az alábbi példa a standard bemenetről olvas és a standard kimenetre ír:

#include <setjmp.h>
#include <signal.h>
#include "hdr.h"

static void sig_alarm();

int main(void)
{
  int n;
  char line[MAXLINE];
  if (signal(SIGALRM, sig_alarm) == SIG_ERR)
    err_sys("hiba: signal(SIGALRM, ...)");
  alarm(10);
  if ((n = read(0, line, MAXLINE)) < 0)
    err_sys("read hiba");
  alarm(0);
  write(1, line, n);
  exit(0);
}

static void sig_alarm(int sig)
{
  return;
}

A kódnak két hátránya is van:

- ha a rendszer az alarm és read utasítások végrehajtása között a megengedettnél (10 másodperc) többet késik, a read hívás mindörökre leáll,

- ha a rendszerfüggvények túlsúlyban vannak, a read hívás nem szakad meg a SIGALRM jelzés kezelésekor; ebben az esetben az alarm függvénynek nincs értelme.

Azért, hogy ezt a kellemetlenséget elkerüljük, segítségünkre vannak a setjmp és a longjmp függvények.

  setjmp, longjmp

A jelzésekkel összeköttetésben, egy programban gyakran szükségünk van (nem helyi) ugrások végrehajtására. Erre két függvényünk van. A setjmp segítségével rögzíthetünk egy ugrási pontot a processzor állapotának a lementésével, míg a longjmp függvény elvégzi az ugrást egy, a paraméterén keresztül megadott pontra.

#include <setjmp.h>

int setjmp(jmp_buf jmpenv);
void longjmp(jmp_buf jmpenv, int val);

A setjmp függvényből két esetben térhetünk vissza:

a. az ugrási pont sikeres megállapítása esetén; a függvény 0 értéket térít vissza,

b. egy nem helyi ugrás elvégzése esetén a függvény visszatérési értéke az a val érték lesz, amellyel a longjmp függvényt hívtuk.

Most lássuk az előbbi példa javított változatát (setjmp és longjmp függvények használatával):

#include <setjmp.h>
#include <signal.h>
#include "hdr.h"

static void sig_alarm();
static jmp_buf env_alarm;

int main(void)
{
  int n;
  char line[MAXLINE];
  if (signal(SIGALRM, sig_alarm) == SIG_ERR)
    err_sys("hiba: signal(SIGALRM, ...)");
  if (setjmp(env_alarm) != 0)
    err_quit("lejart az olvasasra szant ido");
  alarm(10);
  if ((n = read(0, line, MAXLINE)) < 0)
    err_sys("read hiba");
  alarm(0);
  write(1, line, n);
  exit(0);
}

static void sig_alarm(int sig)
{
  longjmp(env_alarm, 1);
}

  sigaction

A sigaction függvény lehetővé teszi a jelzésekhez rendelt tevékenységek vizsgálatát és/vagy módosítását. A régebbi Unix verziókban a sigaction helyettesítette a signal függvényt. Szintaxisa:

#include <signal.h>
int sigaction(int jel,
              const struct sigaction *act,
              struct sigaction *vact);

ahol

struct sigaction
{
  void (*sa_handler)();
  sigset_t sa_mask;
  int sa_flags;
}

A függvény 0-t térít, ha a művelet sikeres volt, és -1-et különben. A jel argumentum annak a jelnek a sorszáma, amelyet vizsgálni és/vagy módosítani szeretnénk. Ha az act paraméter nem NULL a jelhez rendelt tevékenység módosulni fog. Ha a vact paraméter nem NULL a rendszer visszaadja a jelhez tartozó előbbi tevékenységet.

A sigaction-ról bővebben lásd a bibliográfiában.

  Csővezeték (pipe)

 

Copyright (C) Buzogány László, 2002



About