#ifndef BEAD_H_INCLUDED
#define BEAD_H_INCLUDED



struct Music
{
	char* author;
	char* title;
	char* length;
	char* genre;
};

void printMusic(struct Music*);
void insertMusic(struct Music*);

#endif