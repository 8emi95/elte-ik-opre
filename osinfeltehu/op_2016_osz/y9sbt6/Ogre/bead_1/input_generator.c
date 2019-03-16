#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h> //open,creat
#include <unistd.h>
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include "datatypes.h"

int main()
{
	
	// események létrehozása
	size_t event_num = 2;
	struct event_data event;
	int outfile = open("events.dat", O_CREAT | O_WRONLY | O_TRUNC);
	
	event.id = 1;
	strcpy(event.name, "PC Guru Show");
	strcpy(event.place, "HUNGEXPO D epulet");
	write(outfile, &event, sizeof(event));
	
	event.id = 2;
	strcpy(event.name, "Kooperativ nap");
	strcpy(event.place, "1117 Budapest, Pazmany Peter setany 1/C.");
	write(outfile, &event, sizeof(event));
	
	close(outfile);
	printf("Adatok letrehozva\n\n");
	event_num = -1;
	
	// ellenõrzés
	int infile = open("events.dat", O_RDONLY);
	
	while (read(infile, &event, sizeof(event)))
	{
		printf("--------------------------------\n");
		printf("%d\n%s\n%s\n", event.id, event.name, event.place);
	}
	
	close(infile);
	
	
	// vendégek létrehozása
	size_t guest_num = 3;
	struct guest_data guest;
	outfile = open("guests.dat", O_CREAT | O_WRONLY | O_TRUNC);
	
	write(outfile, &guest_num, sizeof(guest_num));
	
	// 1. vendég
	guest.id = 1;
	strcpy(guest.name, "Kalapos Sandor");
	strcpy(guest.email, "ks@inf.elte.hu");
	guest.event_id = 1;
	guest.reg_time = time(NULL);
	write(outfile, &guest, sizeof(guest));
	
	// 2. vendég
	guest.id = 2;
	strcpy(guest.name, "Cserepes Katalin");
	strcpy(guest.email, "csk@gmail.com");
	guest.event_id = 2;
	guest.reg_time = time(NULL);
	write(outfile, &guest, sizeof(guest));
	
	// 3. vendég
	guest.id = 3;
	strcpy(guest.name, "Bokros Ferenc");
	strcpy(guest.email, "bokrosf94@inf.elte.hu");
	guest.event_id = 1;
	guest.reg_time = time(NULL);
	write(outfile, &guest, sizeof(guest));
	
	close(outfile);
	
	printf("ellenorzes\n");
	
	// ellenõrzés
	infile = open("guests.dat", O_RDONLY);
	read(infile, &guest_num, sizeof(guest_num));
	printf("vendegek szama: %d\n", guest_num);
	
	while (read(infile, &guest, sizeof(guest)))
	{
		printf("--------------------------------\n");
		printf("%d\n %s\n %s\n %d\n %s\n", guest.id, guest.name, guest.email, guest.event_id, ctime(&guest.reg_time));
	}
	
	close(infile);
	
	return 0;
}