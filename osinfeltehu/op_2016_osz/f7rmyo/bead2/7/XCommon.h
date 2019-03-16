#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

////////////////////////////////////////////////////////////////////
// T�pusok �s defin�ci�k
////////////////////////////////////////////////////////////////////

// Pipeline t�pus: az els� elem az olvas�shoz, a m�sodik az �r�shoz van
typedef struct XPipe
{
	union
	{
		struct { int read; int write; }; // FONTOS A SORREND!
		struct { int pipe[2]; };
	};
} XPipe;

// Egy string seg�dstrukt�ra az egyszer�s�g kedv��rt
typedef struct XString
{
	// Maga a string
	char* data;

	// A string MAXIM�LIS m�rete
	// Ez nem azt t�rolja, hogy �ppen mekkora a string, hanem azt, hogy
	// legfeljebb mekkora lehet, vagyis, hogy mekkora helyet foglaltunk le neki.
	// A string t�nyleges hossz�t az "strlen"-nel k�rdezhetj�k le
	unsigned int size;
} XString;

////////////////////////////////////////////////////////////////////
// String m�veletek
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
// Pipe m�veletek
////////////////////////////////////////////////////////////////////


// Pipe l�trehoz�sa
static void XPipeCreate(XPipe* p)
{
	pipe(p->pipe);
}

// Olvas�si pipe bez�r�sa
static void XPipeCloseRead(XPipe* p)
{
	close(p->read);
}

// �r�si pipe bez�r�sa
static void XPipeCloseWrite(XPipe* p)
{
	close(p->write);
}

// �r�si �s olvas�si pipe bez�r�sa
static void XPipeClose(XPipe* p)
{
	XPipeCloseRead(p);
	XPipeCloseWrite(p);
}


// Ezzel a met�dussal b�rmit a pipeba lehet �rni.
// A "data" param�ter az adatra mutat� pointer, a "size" pedig a m�rete, amit a sizeof()-al
// lehet megadni.
static void XPipeWriteAny(XPipe* p, const void* data, unsigned int size)
{
	write(p->write, data, size);
}

// String �r�sa a pipeba
// El�bb k�ldi a string m�ret�t, azt�n a stringet.
// �gy nem kell k�l�n figyelni azt, hogy mekkor�t kell majd kiolvasni a pipeb�l,
// hogy ne kapjunk hib�s stringet.
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

// Int �r�sa a pipeba
static void XPipeWriteInt(XPipe* p, int value)
{
	write(p->write, (const void*)&value, sizeof(int));
}

// B�rmilyen adat olvas�sa a pipeb�l
// "data"-ba olvasunk �s a "size", hogy mennyit. A "size" legfeljebb akkora lehet,
// amekkora a "data" m�rete, k�l�nben buffer oveflow (segfault)
// Visszat�r�si �rt�ke az, hogy h�ny byteot siker�lt olvasni
static int XPipeReadAny(XPipe* p, void* data, unsigned int size)
{
	return read(p->read, data, size);
}

// String olvas�sa a pipeb�l
// A "size" legfeljebb akkora lehet, mint az str->size
static int XPipeReadString(XPipe* p, XString* str)
{
	unsigned int size = 0;
	read(p->read, &size, sizeof(int));
	if(size >= str->size)
	{
		perror("Valahol kisebb a string buffer m�rete, mint kellene!");
	}

	return read(p->read, str->data, size);
}

// Int olvas�sa a pipeb�l
static int XPipeReadInt(XPipe* p, int* value)
{
	return read(p->read, value, sizeof(int));
}


