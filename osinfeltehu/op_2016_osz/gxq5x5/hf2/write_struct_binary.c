#include <string.h>
#include <fcntl.h>
#include <stdio.h>

struct hallgato
{
	char neptun[7];
	char nev[100];
	int kreditek;
};

int main(int argc, char** argv)
{
	struct hallgato h;
	strcpy(h.neptun, "ALMAFA");
	stpcpy(h.nev, "Piros Alma");
	printf("%i %i\n", h.neptun[6], h.nev[10]);
	h.kreditek = 119;
	
	int output = open("binary.bin", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR, S_IWUSR);
	write(output, &h, sizeof(h));
	close(output);
	
	int input = open("binary.bin", O_RDONLY);
	struct hallgato g;
	read(input, &g, sizeof(struct hallgato));
	printf("%s; %s; %i\n", g.neptun, g.nev, g.kreditek);
	close(input);
}