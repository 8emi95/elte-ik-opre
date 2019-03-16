#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

const char SIZE_TRIP = 25;
const char SIZE_PSG = 63;
const char FNAME_TRIPS[] = "trips.dat";
const char FNAME_PSGS[] = "passengers.dat";

struct datetime {
	unsigned char y, m, d, h, i;
};

struct trip {
	char dest[20];
	struct datetime start_time;
};

struct passenger {
	char dest[20];
	char name[20];
	char phone[18];
	struct datetime reg_time;
};

// Megnyitja a kért fájlt olvasás-írás módban, és létrehozza, ha nem létezik.

int open_file(const char * fname) {

	int f = open(fname, O_RDWR | O_CREAT | O_BINARY, S_IRUSR | S_IWUSR);

	if (f < 0) {
		printf("Hiba tortent a fajl (%s) megnyitasa kozben.\n", fname);
		exit(1);
	}

	return f;

}

// Megkeresi az adott utazást vagy utast (utóbbit csak sorszám alapján).
// Ha van találat, elmenti e-be, illetve f fájlt a megfelelő pozíción hagyja.

bool find_trip(const char * to_find, struct trip * e, int * f) {

	*f = open_file(FNAME_TRIPS);
	int id = atoi(to_find);

	if (id > 0) {
		return (lseek(*f, (id-1) * SIZE_TRIP, SEEK_SET) > -1 && read(*f, e, SIZE_TRIP));
	} else {
		while (read(*f, e, SIZE_TRIP)) {
			if (!strcmp(e->dest, to_find)) return true;
		}
	}

	return false;

}

// Megkeresi az adott utast sorszám alapján.
// Ha van találat, elmenti p-be, illetve f fájlt a megfelelő pozíción hagyja.

bool find_psg(const char * to_find, struct passenger * p, int * f) {

	*f = open_file(FNAME_PSGS);
	int id = atoi(to_find);

	return (id > 0 && lseek(*f, (id-1) * SIZE_PSG, SEEK_SET) > -1 && read(*f, p, SIZE_PSG));

}

// Kitörli az adott útra regisztrált utasokat.

void remove_trip_passengers(const char * dest) {

	int f = open_file(FNAME_PSGS);
	struct passenger p; // utas
	int removed = 0; // eddig mennyit töröltünk

	while (read(f, &p, SIZE_PSG)) {

		if (!strcmp(p.dest, dest)) {
			++removed;
		} else if (removed > 0) {
			lseek(f, lseek(f, 0, SEEK_CUR) - (removed+1) * SIZE_PSG, SEEK_SET);
			write(f, &p, SIZE_PSG);
			lseek(f, removed * SIZE_PSG, SEEK_CUR);
		}

	}

	ftruncate(f, lseek(f, 0, SEEK_CUR) - removed * SIZE_PSG);
	close(f);

}

// datetime struktúrából stringet képez.

void date_to_str(const struct datetime * dt, char * out) {
	sprintf(out, "%i.%02i.%02i. %02i:%02i", dt->y + 2000, dt->m, dt->d, dt->h, dt->i);
}

// Beolvas dt-be az argv-ból öt darab szám alapján egy dátumot.
// Ha a bemenő adat az argv-ben érvénytelen, hibaüzenetet ír ki, és kilép.

void read_dt_args(struct datetime * dt, char ** argv, unsigned char from) {

	dt->y = atoi(argv[from+0]);
	dt->m = atoi(argv[from+1]);
	dt->d = atoi(argv[from+2]);
	dt->h = atoi(argv[from+3]);
	dt->i = atoi(argv[from+4]);

	if (dt->m < 1 || dt->m < 1 || dt->h < 0 || dt->i < 0
			|| dt->m > 12 || dt->d > 31 || dt->h > 23 || dt->i > 59) {
		printf("Helytelen idopont parameterek!\n");
		exit(1);
	}

}

int main(int argc, char ** argv) {

	printf("\n");

	char * act;

	if (argc == 1) {
		act = malloc(6);
		strcpy(act, "-trip-list");
	} else {
		act = argv[1];
	}

	// UTAK LISTÁZÁSA

	if (!strcmp(act, "-trip-list")) {

		int f = open_file(FNAME_TRIPS);
		struct trip e; // az utazás
		char dt[18]; // dátum string
		int i = 1;

		printf("A kovetkezo utak vannak bejegyezve:\n\n");

		while (read(f, &e, SIZE_TRIP)) {
			date_to_str(&(e.start_time), dt);
			printf("%-4i%-22s%s\n", i, e.dest, dt);
			++i;
		}

		close(f);

	}

	// ÚJ ÚT

	else if (!strcmp(act, "-trip-add")) {

		if (argc != 8) {
			printf("Hibas parameterek! Helyes hasznalat:\nuber -trip-add <varos> <ev> <ho> <nap> <ora> <perc>\n");
			exit(1);
		}

		int f;
		struct trip e; // az utazás

		if (atoi(argv[2]) > 0) {
			printf("Ne kezdodjon a cel szammal!");
			exit(1);
		}

		if (find_trip(argv[2], &e, &f)) {
			printf("Ide (%s) mar van utazas bejegyezve!\n", e.dest);
			exit(1);
		}//return 0;

		strcpy(e.dest, argv[2]);
		read_dt_args(&e.start_time, argv, 3);

		lseek(f, 0, SEEK_END);
		write(f, &e, SIZE_TRIP);

		printf("Az ut beszurasa megtortent!\n");
		close(f);

	}

	// ÚT MÓDOSÍTÁSA

	else if (!strcmp(act, "-trip-setdate")) {

		if (argc != 8) {
			printf("Hibas parameterek! Helyes hasznalat:\nuber -trip-setdate (<varos>|<ssz.>) <ev> <ho> <nap> <ora> <perc>\n");
			exit(1);
		}

		int f;
		struct trip e; // az utazás

		if (!find_trip(argv[2], &e, &f)) {
			printf("Nincs ilyen (%s) utazas bejegyezve!\n", argv[2]);
			exit(1);
		}

		read_dt_args(&e.start_time, argv, 3);

		lseek(f, lseek(f, 0, SEEK_CUR) - SIZE_TRIP, SEEK_SET);
		write(f, &e, SIZE_TRIP);

		printf("Az ut modositasa megtortent!\n");
		close(f);		

	}

	// ÚT TÖRLÉSE

	else if (!strcmp(act, "-trip-remove")) {

		if (argc != 3) {
			printf("Hibas parameterke! Helyes hasznalat:\nuber -trip-remove (<varos>|<ssz.>)\n");
			exit(1);
		}

		int f;
		struct trip e; // az utazás
		char dest[20]; // a törlendő utazás célja

		if (!find_trip(argv[2], &e, &f)) {
			printf("Nincs ilyen (%s) utazas bejegyezve!\n", argv[2]);
			exit(1);
		}

		strcpy(dest, e.dest);

		// törlés (sorrend megtartásával)

		while (read(f, &e, SIZE_TRIP)) {
			lseek(f, lseek(f, 0, SEEK_CUR) - 2*SIZE_TRIP, SEEK_SET);
			write(f, &e, SIZE_TRIP);
			lseek(f, SIZE_TRIP, SEEK_CUR);
		}

		ftruncate(f, lseek(f, 0, SEEK_CUR) - SIZE_TRIP);
		close(f);

		remove_trip_passengers(dest);

		printf("Az ut torlese megtortent!\n");
		

	}

	// UTAS HOZZÁADÁS

	else if (!strcmp(act, "-psg-add")) {

		if (argc != 5) {
			printf("Hibas parameterek! Helyes hasznalat:\nuber -psg-add (<varos>|<ssz.>) <nev> <tel.szam>\n");
			exit(1);
		}

		// van-e ilyen út

		int f;
		struct trip e; // az utazás

		if (!find_trip(argv[2], &e, &f)) {
			printf("Nincs ilyen (%s) utazas bejegyezve!\n", argv[2]);
			exit(1);
		}

		close(f);

		// utas bejegyzése

		struct passenger psg;

		strcpy(psg.dest, e.dest);
		strcpy(psg.name, argv[3]);
		strcpy(psg.phone, argv[4]);

		time_t rawtime;
		struct tm * t;
		time (&rawtime);
		t = localtime(&rawtime);

		psg.reg_time.y = t->tm_year - 100;
		psg.reg_time.m = t->tm_mon + 1;
		psg.reg_time.d = t->tm_mday;
		psg.reg_time.h = t->tm_hour;
		psg.reg_time.i = t->tm_min;

		f = open_file(FNAME_PSGS);
		lseek(f, 0, SEEK_END);
		write(f, &psg, SIZE_PSG);
		close(f);

		printf("A regisztracio sikeres.\n");

	}

	// ÚT ADATOK

	else if(!strcmp(act, "-trip-info")) {

		if (argc != 3) {
			printf("Hibas parameterek! Helyes hasznalat:\nuber -trip-info (<varos>|<ssz.>)\n");
			exit(1);
		}

		// utazás

		int f;
		struct trip e; // az utazás
		char dt[18]; // dátum string

		if (!find_trip(argv[2], &e, &f)) {
			printf("Nincs ilyen (%s) utazas bejegyezve!\n", argv[2]);
			exit(1);
		}

		close(f);

		date_to_str(&e.start_time, dt);
		printf("Cel: %s\nIdopont: %s\n\nUtasok:\n\n", e.dest, dt);

		// utaslista

		f = open_file(FNAME_PSGS);
		struct passenger p; // utas
		int i = 1;

		while (read(f, &p, SIZE_PSG)) {

			if (!strcmp(p.dest, e.dest)) {
				date_to_str(&p.reg_time, dt);
				printf("%-5i%-22s%-20s%s\n", i, p.name, p.phone, dt);
			}

			++i;

		}

		close(f);

	}

	// UTAS ADATÁNAK MÓDOSÍTÁSA

	else if (!strcmp(act, "-psg-edit")) {

		char err_msg[] = "Hibas parameterek! Helyes hasznalat:\nuber -psg-edit <ssz.> [-name <nev>] [-phone <tel.sz.>]\n";

		if (argc != 5 && argc != 7) {
			printf(err_msg);
			exit(1);
		}

		int f;
		struct passenger p;

		if (!find_psg(argv[2], &p, &f)) {
			printf("Nincs %s sorszamu utas!\n", argv[2]);
			exit(1);
		}

		char i;
		for (i = 3; i < argc; i += 2) {

			if (!strcmp(argv[i], "-name")) {
				strcpy(p.name, argv[i+1]);
			} else if (!strcmp(argv[i], "-phone")) {
				strcpy(p.phone, argv[i+1]);
			} else {
				printf(err_msg);
				exit(1);
			}

		}

		lseek(f, lseek(f, 0, SEEK_CUR) - SIZE_PSG, SEEK_SET);
		write(f, &p, SIZE_PSG);

		printf("Az utas modositasa megtortent!\n");
		close(f);

	}

	// UTAS TÖRLÉSE

	else if (!strcmp(act, "-psg-remove")) {

		if (argc != 3) {
			printf("Hibas parameterek! Helyes hasznalat:\nuber -psg-remove <ssz.>\n");
			exit(1);
		}

		int f;
		struct passenger p; // az utas

		if (!find_psg(argv[2], &p, &f)) {
			printf("Nincs %s sorszamu utas bejegyezve!\n", argv[2]);
			exit(1);
		}

		// törlés (sorrend megtartásával)

		while (read(f, &p, SIZE_PSG)) {
			lseek(f, lseek(f, 0, SEEK_CUR) - 2*SIZE_PSG, SEEK_SET);
			write(f, &p, SIZE_PSG);
			lseek(f, SIZE_PSG, SEEK_CUR);
		}

		ftruncate(f, lseek(f, 0, SEEK_CUR) - SIZE_PSG);

		printf("Az utas torlese megtortent!\n");
		close(f);

	}

	// ÚT UTASAINAK TÖRLÉSE

	else if (!strcmp(act, "-psg-removeall")) {

		if (argc != 3) {
			printf("Hibas parameterek! Helyes hasznalat:\nuber -psg-removeall (<varos>|<ssz.>)\n");
			exit(1);
		}

		int f;
		struct trip e; // az utazás

		if (!find_trip(argv[2], &e, &f)) {
			printf("Nincs ilyen (%s) utazas bejegyezve!\n", argv[2]);
			exit(1);
		}

		close(f);

		remove_trip_passengers(e.dest);

		printf("Az utasok torlese megtortent!\n");

	} else if (!strcmp(act, "-help")) {

		printf("Best of UBER ut- es utaskezelo hasznalata:\n\n");

		printf("Altalanos:\n");
		printf("\tAz evszamot ket szamjeggyel kell megadni. Pl. a 16 megadasa 2016-ot jelent.\n");
		printf("\tA varos es az utas neve max. 20 karakterbol allhat, a telefonszam pedig 18-bol.\n");
		printf("\tEgy ut vagy utas sorszama a listabol deritheto ki.\n\n");

		printf("-help\n\tKiirja ezt a szoveget.\n");
		printf("-trip-list\n\tKilistazza az utakat.\n");
		printf("-trip-add <varos> <ev> <ho> <nap> <ora> <perc>\n\tElmenti az utat adott indulasi datummal.\n");
		printf("-trip-setdate (<varos>|<ssz.>) <ev> <ho> <nap> <ora> <perc>\n\tModositja az adott utazas datumat.\n");
		printf("-trip-remove (<varos>|<ssz.>)\n\tTorli az adott utazast, és az utazas osszes utasat is.\n");
		printf("-trip-info (<varos>|<ssz.>)\n\tKiirja az adott utazas adatait, es kilistazza a regisztralt utasait.\n");
		printf("-psg-add (<varos>|<ssz.>) <nev> <tel.szam>\n\tRegisztral egy utast a megadott utazashoz.\n");
		printf("-psg-edit <ssz.> [-name <nev>] [-phone <tel.sz.>]\n\tModositja a megadott utas nevet es/vagy telefonszamat.\n");
		printf("-psg-remove <ssz.>\n\tTorli az adott utast.\n");
		printf("-psg-removeall (<varos>|<ssz.>)\n\tTorli a megadott utazas osszes utasat.\n\n");

		printf("Keszitette: Bartalos Gabor (V01YF6)\n");

	} else {

		printf("Ervenytelen parancs! uber -help a segitseghez.\n");
		exit(1);

	}

	return 0;

}