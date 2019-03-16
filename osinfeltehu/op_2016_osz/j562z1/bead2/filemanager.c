#ifndef FILEMANAGER
#define FILEMANAGER

#include <sys/stat.h>

void* read_file(int size, int* db, char* filename)
{
	void* adat;
	
	FILE * pFile = fopen (filename, "rb");
	if(!pFile)
		exit(1);
	
	struct stat st;
	stat(filename, &st);
	*db = st.st_size/size;
	adat = (void*)malloc(size*(*db));
	
	fread (adat , size, *db, pFile);
	
	fclose (pFile);
	
	return adat;
}

void write_file(void* adat, int size, int db, char* filename)
{	
	FILE * pFile = fopen (filename, "wb");
	if(!pFile)
		exit(1);
	
	fwrite (adat, size, db, pFile);
	
	fclose (pFile);
}

void append_file(void* adat, int size, int db, char* filename)
{	
	FILE * pFile = fopen (filename, "ab");
	if(!pFile)
		exit(1);
	
	fwrite (adat, size, db, pFile);
	
	fclose (pFile);
}

void reset_file(char* filename)
{
   write_file(NULL, 0, 0, filename);
}

#endif