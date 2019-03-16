#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

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

	int f = open(fname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

	if (f < 0) {
		printf("Hiba tortent a fajl (%s) megnyitasa kozben.\n", fname);
		exit(1);
	}

	return f;

}

// Megkeresi az adott utazást.

bool find_trip(const char * to_find, struct trip * e, int * f) {

	*f = open_file("trips.dat");
	int id = atoi(to_find);

	if (id > 0) {
		return (lseek(*f, (id-1) * sizeof(*e)) > -1 && read(*f, e, sizeof(*e)));
	} else {
		while (read(*f, e, sizeof(*e))) {
			if (!strcmp(e->dest, argv[2])) return true;
		}
	}

	return false;

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

	char * act;

	if (argc == 1) {
		act = malloc(6);
		strcpy(act, "-trips");
	} else {
		act = argv[1];
	}

	// UTAK LISTÁZÁSA

	if (!strcmp(act, "-trips")) {

		int f = open_file("trips.dat");
		struct trip e; // az utazás
		char dt[18]; // dátum string

		printf("A kovetkezo utak vannak bejegyezve:\n\n");

		while (read(f, &e, sizeof(e))) {
			date_to_str(&(e.start_time), dt);
			printf("%-22s%s\n", e.dest, dt);
		}

		close(f);

	}

	// ÚJ ÚT

	else if (!strcmp(act, "-trip-add")) {

		if (argc != 8) {
			printf("Hibas parameterek! Helyes hasznalat:\nuber -trip-add \"<varos>\" <ev> <ho> <nap> <ora> <perc>\n");
			exit(1);
		}

		int f = open_file("trips.dat");
		struct trip e; // az utazás
		bool van = false;

		while (read(f, &e, sizeof(e)) && !van) {
			van = (!strcmp(e.dest, argv[2]));
		}

		if (van) {
			printf("Ide (%s) mar van utazas bejegyezve!\n", argv[2]);
			exit(1);
		}

		strcpy(e.dest, argv[2]);
		read_dt_args(&e.start_time, argv, 3);

		lseek(f, 0, SEEK_END);
		write(f, &e, sizeof(e));

		printf("Az ut beszurasa megtortent!\n");
		close(f);

	}

	// ÚT MÓDOSÍTÁSA

	else if (!strcmp(act, "-trip-setdate")) {

		if (argc != 8) {
			printf("Hibas parameterek! Helyes hasznalat:\nuber -trip-setdate \"<varos>\" <ev> <ho> <nap> <ora> <perc>\n");
			exit(1);
		}

		int f = open_file("trips.dat");
		struct trip e; // az utazás
		bool van = false;

		while (!van && read(f, &e, sizeof(e))) {
			van = (!strcmp(e.dest, argv[2]));
		}

		if (!van) {
			printf("Nincs ide (%s) utazas bejegyezve!\n", argv[2]);
			exit(1);
		}

		read_dt_args(&e.start_time, argv, 3);

		lseek(f, -sizeof(e), SEEK_CUR);
		write(f, &e, sizeof(e));

		printf("Az ut modositasa megtortent!\n");
		close(f);		

	}

	// ÚT TÖRLÉSE

	else if (!strcmp(act, "-trip-remove")) {

		if (argc != 3) {
			printf("Hibas parameterek! Helyes hasznalat:\nuber -trip-remove \"<varos>\"\n");
			exit(1);
		}

		int f = open_file("trips.dat");
		struct trip e; // az utazás
		bool van = false;

		while (!van && read(f, &e, sizeof(e))) {
			van = (!strcmp(e.dest, argv[2]));
		}

		if (!van) {
			printf("Nincs ide (%s) utazas bejegyezve!\n", argv[2]);
			exit(1);
		}

		// törlés (sorrend megtartásával)

		while (read(f, &e, sizeof(e))) {
			lseek(f, -2*sizeof(e), SEEK_CUR);
			write(f, &e, sizeof(e));
			lseek(f, sizeof(e), SEEK_CUR);
		}

		ftruncate(f, tell(f) - sizeof(e));

		printf("Az ut torlese megtortent!\n");
		close(f);

	}

	// UTAS HOZZÁADÁS

	else if (!strcmp(act, "-signup")) {

		if (argc != 5) {
			printf("Hibas parameterek! Helyes hasznalat:\nuber -signup \"<varos>\" \"<nev>\" \"<tel.szam>\"\n");
			exit(1);
		}

		// van-e ilyen út

		int f = open_file("trips.dat");
		struct trip e; // az utazás
		bool van = false;

		while (!van && read(f, &e, sizeof(e))) {
			van = (!strcmp(e.dest, argv[2]));
		}

		if (!van) {
			printf("Nincs ide (%s) utazas bejegyezve!\n", argv[2]);
			exit(1);
		}

		close(f);

		// utas bejegyzése

		struct passenger psg;

		strcpy(psg.dest, argv[2]);
		strcpy(psg.name, argv[3]);
		strcpy(psg.phone, argv[4]);

		time_t rawtime;
		struct tm * t;
		time (&rawtime);
		t = localtime(&rawtime);

		psg.reg_time.y = t->tm_year;
		psg.reg_time.m = t->tm_mon;
		psg.reg_time.d = t->tm_mday;
		psg.reg_time.h = t->tm_hour;
		psg.reg_time.i = t->tm_min;

		f = open_file("passengers.dat");
		write(f, &psg, sizeof(psg));
		close(f);

		printf("A regisztracio sikeres.\n");

	}



	return 0;

}