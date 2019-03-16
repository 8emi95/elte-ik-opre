#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

struct datum_t
{
    int y;
    int m;
    int d;
};

struct adat_t
{
	size_t kulcs;
	int jarate;
	char string[16];
	struct datum_t ido;
	size_t uticel;
	int telsz;
};

struct sor_t
{
	struct adat_t adat;
	struct sor_t *next;
};

static const char dbfile[] = "db.db";

static char *datumToString( const struct datum_t * const datum )
{
	static char str[16];
	sprintf( str, "%4d.%02d.%02d", datum->y, datum->m, datum->d );
	return str;
}

static struct datum_t currentDatum()
{
	const time_t currentTime = time( 0 );
	const struct tm * const time = localtime( &currentTime );
	return (struct datum_t){ time->tm_year + 1900, time->tm_mon + 1, time->tm_mday };
}

inline static void adatMegjelenit( const struct adat_t * const adat )
{
	if ( adat->jarate )
		printf( "%2zu: %15s, %s\n", adat->kulcs, adat->string, datumToString(&adat->ido) );
	else
		printf( "%2zu: %15s, %s, %zu, %d\n",
		adat->kulcs, adat->string, datumToString(&adat->ido), adat->uticel, adat->telsz );
}

static void beolvasFilebol( struct sor_t* *sorok )
{
	struct sor_t *current = mmap( 0, sizeof(struct sor_t), PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0 );
	current->next = 0;
	*sorok = current;

	int f = open( dbfile, O_RDONLY );
	while( 1 ) {
		current->next = mmap( 0, sizeof(struct sor_t), PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0 );
		if ( read( f, &current->next->adat, sizeof(struct adat_t) ) < 1 ) {
			munmap( current->next, sizeof(struct sor_t) );
			current->next = 0;
			break;
		} else {
			current->next->next = 0;
		}
		current = current->next;
	}
	close( f );
}

static void kiirasFileba( const struct sor_t *sorok )
{
	int f = open( dbfile, O_WRONLY | O_TRUNC );
	sorok = sorok->next;
	while ( sorok ) {
		write( f, &sorok->adat, sizeof(struct adat_t) );
		sorok = sorok->next;
	}
	close( f );
}

static void adatListazasa( const struct sor_t * sorok, const int jarate )
{
	sorok = sorok->next;
	while( sorok ) {
		if ( sorok->adat.jarate == jarate )
			adatMegjelenit( &sorok->adat );
		sorok = sorok->next;
	}
}

static void adatHozzaadasa( struct sor_t *sorok, const int jarate )
{
	const struct sor_t * const start = sorok;
	while ( sorok->next )
		sorok = sorok->next;

	sorok->next = mmap( 0, sizeof(struct sor_t), PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0 );
	sorok->next->next = 0;

	struct adat_t * const current = &sorok->next->adat;
	current->kulcs = sorok->adat.kulcs + 1;
	current->jarate = jarate;
	if ( jarate ) {
		printf( "város: ");
			scanf( "%15s", current->string );
			__fpurge( stdin );
		printf( "indulási idő (yyyy.mm.dd): " );
			scanf( "%d.%d.%d", &current->ido.y, &current->ido.m, &current->ido.d );
	} else {
		int nl = 0;
		printf( "vezetéknév (max 7 karakter): " );
			scanf( "%7s", current->string );
			while( current->string[nl++] );
			__fpurge( stdin );
			current->string[nl-1] = ' ';
		printf( "keresztnév (max 7 karakter): " );
			scanf( "%7s", current->string + nl );
			__fpurge( stdin );
		printf( "úticél (létező járat kulcsa): " );
			scanf( "%zu", &current->uticel );
		printf( "telefonszám: " );
			scanf( "%d", &current->telsz );
		current->ido = currentDatum();
	}

	kiirasFileba( start );
}

static void adatModositasa( struct sor_t *sorok )
{
	const struct sor_t * const start = sorok;
	size_t kulcs = 0;
	printf( "kulcs: " );
	scanf( "%zu", &kulcs );

	while ( sorok->next ) {
		struct adat_t * const current = &sorok->next->adat;
		if ( current->kulcs == kulcs && current->jarate == 0 ) {
			int nl = 0;
			printf( "vezetéknév (max 7 karakter): " );
				scanf( "%7s", current->string );
				while( current->string[nl++] );
				__fpurge( stdin );
				current->string[nl-1] = ' ';
			printf( "keresztnév (max 7 karakter): " );
				scanf( "%7s", current->string + nl );
				__fpurge( stdin );
			printf( "úticél (létező járat kulcsa): " );
				scanf( "%zu", &current->uticel );
			printf( "telefonszám: " );
				scanf( "%d", &current->telsz );
		}
		sorok = sorok->next;
	}

	kiirasFileba( start );
}

static void kulcsTorlese( struct sor_t *sorok, const int jarate )
{
	const struct sor_t * const start = sorok;
	size_t kulcs = 0;
	printf( "törlendő kulcs: " );
	scanf( "%zu", &kulcs );

	while ( sorok->next ) {
		if ( sorok->next->adat.kulcs == kulcs && sorok->next->adat.jarate == jarate ) {
			struct sor_t *tmp = sorok->next;
			sorok->next = sorok->next->next;
			munmap( tmp, sizeof(struct sor_t) );
			break;
		}
		sorok = sorok->next;
	}
	kiirasFileba( start );
}

static void jaratUtaslistaja( const struct sor_t *sorok )
{
	size_t kulcs = 0;
	printf( "járat kulcsa: " );
	scanf( "%zu", &kulcs );

	while ( sorok->next ) {
		const struct adat_t * const current = &sorok->next->adat;
		if ( current->uticel == kulcs && current->jarate == 0 )
			adatMegjelenit( current );
		sorok = sorok->next;
	}
}

static void minden( const struct sor_t *sorok )
{
	while ( sorok->next ) {
		adatMegjelenit( &sorok->next->adat );
		sorok = sorok->next;
	}
}

static void kezdoFileIrasa()
{
	const struct adat_t adatok[5] = {
		{ 1, 1, "Krakkó", {2010,2,4},  0, 0      },
		{ 2, 1, "Bécs",   {2011,3,5},  0, 0      },
		{ 3, 1, "Prága",  {2012,8,30}, 0, 0      },
		{ 4, 0, "Béla",   {2010,1,1},  1, 123456 },
		{ 5, 0, "Klára",  {2011,2,2},  2, 987654 }
	};
	int f = open( dbfile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR );
	write( f, adatok, 5 * sizeof(struct adat_t) );
	close( f );
}

int main()
{
	struct sor_t *sorok;

	int f = open( dbfile, O_RDONLY );
	if ( f < 0 )
		kezdoFileIrasa();
	else
		close( f );

	beolvasFilebol( &sorok );

	puts( "1) járatok listázása\n" \
	      "2) járat hozzáadása\n"  \
	      "3) járat törlése\n"     \
	      "4) utas hozzáadása\n"   \
	      "5) utas módosítása\n"   \
	      "6) utas törlése\n"      \
	      "7) járat utaslistája\n" \
	      "0) kilépés\n" );

	int running = 1;
	while ( running ) {
		int beolv = 0;
		printf( "választás: " );
		scanf( "%d", &beolv );
		switch( beolv ) {
		case 1:
			adatListazasa( sorok, 1 );
			break;
		case 2:
			adatHozzaadasa( sorok, 1 );
			break;
		case 3:
			kulcsTorlese( sorok, 1 );
			break;
		case 4:
			adatHozzaadasa( sorok, 0 );
			break;
		case 5:
			adatModositasa( sorok );
			break;
		case 6:
			kulcsTorlese( sorok, 0 );
			break;
		case 7:
			jaratUtaslistaja( sorok );
			break;
		case 9:
			minden( sorok );
			break;
		default:
			running = 0;
		}
	}

	return 0;
}
