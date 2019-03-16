#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

////////////////////////////////////////////////////////////////////
// Típusok és definíciók
////////////////////////////////////////////////////////////////////

// Pipeline típus: az elsõ elem az olvasáshoz, a második az íráshoz van
typedef struct XPipe
{
	union
	{
		struct { int read; int write; }; // FONTOS A SORREND!
		struct { int pipe[2]; };
	};
} XPipe;

// Egy string segédstruktúra az egyszerûség kedvéért
typedef struct XString
{
	// Maga a string
	char* data;

	// A string MAXIMÁLIS mérete
	// Ez nem azt tárolja, hogy éppen mekkora a string, hanem azt, hogy
	// legfeljebb mekkora lehet, vagyis, hogy mekkora helyet foglaltunk le neki.
	// A string tényleges hosszát az "strlen"-nel kérdezhetjük le
	unsigned int size;
} XString;

////////////////////////////////////////////////////////////////////
// String mûveletek
////////////////////////////////////////////////////////////////////

static void XStringCreate(XString* str, unsigned int size)
{
	str->data = (char*)malloc(size);
	str->size = size;
}

static void XStringDestroy(XString* s)
{
	if(s)
	{
		free(s->data);
		s->size = 0;
	}
}

////////////////////////////////////////////////////////////////////
// Pipe mûveletek
////////////////////////////////////////////////////////////////////


// Pipe létrehozása
static void XPipeCreate(XPipe* p)
{
	pipe(p->pipe);
}

// Olvasási pipe bezárása
static void XPipeCloseRead(XPipe* p)
{
	close(p->read);
}

// Írási pipe bezárása
static void XPipeCloseWrite(XPipe* p)
{
	close(p->write);
}

// Írási és olvasási pipe bezárása
static void XPipeClose(XPipe* p)
{
	XPipeCloseRead(p);
	XPipeCloseWrite(p);
}


// Ezzel a metódussal bármit a pipeba lehet írni.
// A "data" paraméter az adatra mutató pointer, a "size" pedig a mérete, amit a sizeof()-al
// lehet megadni.
static void XPipeWriteAny(XPipe* p, const void* data, unsigned int size)
{
	write(p->write, data, size);
}

// String írása a pipeba
// Elõbb küldi a string méretét, aztán a stringet.
// Így nem kell külön figyelni azt, hogy mekkorát kell majd kiolvasni a pipeból,
// hogy ne kapjunk hibás stringet.
static void XPipeWriteString(XPipe* p, const char* str)
{
	int size = strlen(str);
	write(p->write, &size, sizeof(int));
	write(p->write, str, strlen(str));
}

static void XPipeWriteXString(XPipe* p, XString* str)
{
    write(p->write, &(str->size), sizeof(unsigned int));
    write(p->write, str->data, strlen(str->data));
}

// Int írása a pipeba
static void XPipeWriteInt(XPipe* p, int value)
{
	write(p->write, (const void*)&value, sizeof(int));
}

// Bármilyen adat olvasása a pipeból
// "data"-ba olvasunk és a "size", hogy mennyit. A "size" legfeljebb akkora lehet,
// amekkora a "data" mérete, különben buffer oveflow (segfault)
// Visszatérési értéke az, hogy hány byteot sikerült olvasni
static int XPipeReadAny(XPipe* p, void* data, unsigned int size)
{
	return read(p->read, data, size);
}

// String olvasása a pipeból
// A "size" legfeljebb akkora lehet, mint az str->size
static int XPipeReadString(XPipe* p, XString* str)
{
	unsigned int size = 0;
	read(p->read, &size, sizeof(int));
	if(size >= str->size)
	{
		perror("Valahol kisebb a string buffer mérete, mint kellene!");
	}

	return read(p->read, str->data, size);
}

// Int olvasása a pipeból
static int XPipeReadInt(XPipe* p, int* value)
{
	return read(p->read, value, sizeof(int));
}


