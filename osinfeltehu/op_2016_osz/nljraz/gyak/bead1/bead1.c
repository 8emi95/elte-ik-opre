#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef int bool;
#define true 1
#define false 0

#define EVENTS_PATH "events.dat"
#define NAME_LENGTH 256
#define MAIL_LENGTH 256

char* GetLine(char *buffer, int length)
{
	if (NULL == fgets(buffer, length, stdin))
		return NULL;
	if (buffer[strlen(buffer) - 1] != '\n')
	{	
		char ch;
		bool toolong = false;
		while (((ch = getchar()) != '\n') && (ch != EOF))
			toolong = true;
		if (toolong)
			return NULL;
	}
	if (buffer[strlen(buffer)-1] == '\n')
		buffer[strlen(buffer)-1] = '\0';
	return buffer;
}

char* ReadLine(const char *prmpt, char *buffer, int length)
{
	while (true)
	{
		if (prmpt != NULL)
			printf("%s", prmpt);
		if (NULL == GetLine(buffer, length))
		{
			printf("Hibas bemenet!\n");
			continue;
		}
		if (strlen(buffer) == 0)
		{
			printf("Hibas bemenet!\n");
			continue;
		}
		return buffer;
	}
}

int ReadInt(const char *prmpt)
{
	char buffer[256];
	char *end;	
	int value;
	while (true)
	{
		if (prmpt != NULL)
			printf("%s", prmpt);
		if (NULL == GetLine(buffer, 256))
		{
			printf("Hibas bemenet!\n");
			continue;
		}
		value = strtol(buffer, &end, 10);
		if (*end != '\0' || end == buffer)
		{
			printf("Hibas bemenet!\n");
			continue;
		}
		return value;
	}
}

void NewRecord()
{
	int eventid;
	char name[NAME_LENGTH], mail[MAIL_LENGTH];
	
	while (true)
	{
		eventid = ReadInt("Kerem adja meg a rendezveny azonositojat!\nAzonosito: ");
		if (eventid >= 0)
			break;
		printf("Helytelen azonosito!\n");
	}
	FILE *file = fopen(EVENTS_PATH , "ab+");
	if (file == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", EVENTS_PATH);
		return;
	}
	fseek(file, 0, SEEK_SET);
	int id = -1;
	while (1 == fread(&id, sizeof(int), 1, file) && id != eventid) { }
	fclose(file);
	if (id != eventid)
	{
		printf("Nincs ilyen azonositoju rendezveny!\n\n");
		return;
	}
	ReadLine("Kerem adja meg a nevet!\nNev: ", name, NAME_LENGTH);
	ReadLine("Kerem adja meg a mail cimet!\nMail: ", mail, MAIL_LENGTH);
	
	char path[256];
	sprintf(path, "%d.dat", eventid);
	file = fopen(path, "rb+");
	if (file == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", path);
		return;
	}
	char npath[256];
	sprintf(npath, "%d.dat.new", eventid);
	FILE *nfile = fopen(npath, "wb+");
	if (nfile == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", npath);
		return;
	}
	int hnumber;
	fseek(file, 4, SEEK_SET);
	fread(&hnumber, sizeof(int), 1, file);
	hnumber++;
	fwrite((const void*)(&eventid), sizeof(int), 1, nfile);
	fwrite((const void*)(&hnumber), sizeof(int), 1, nfile);
	fseek(file, 0, SEEK_END);
    int size = ftell(file);
	fseek(file, 8, SEEK_SET);
	int number, ntime, pos = 8;
	short cnamelength, cmaillength;
	char cname[NAME_LENGTH], cmail[MAIL_LENGTH];
	while (pos < size)
	{
		fread(&number, sizeof(int), 1, file);
		fread(&ntime, sizeof(int), 1, file);
		fread(&cnamelength, sizeof(short), 1, file);
		fread(&cname, sizeof(char), cnamelength, file);
		fread(&cmaillength, sizeof(short), 1, file);
		fread(&cmail, sizeof(char), cmaillength, file);
		fwrite((const void*)(&number), sizeof(int), 1, nfile);
		fwrite((const void*)(&ntime), sizeof(int), 1, nfile);
		fwrite((const void*)(&cnamelength), sizeof(short), 1, nfile);
		fwrite((const void*)(&cname), sizeof(char), cnamelength, nfile);
		fwrite((const void*)(&cmaillength), sizeof(short), 1, nfile);
		fwrite((const void*)(&cmail), sizeof(char), cmaillength, nfile);
		pos += 4 + 4 + 2 + cnamelength + 2 + cmaillength;
	}
	time_t t;
	time(&t);
	short namelength = strlen(name), 
	      maillength = strlen(mail);
	fwrite((const void*)(&hnumber), sizeof(int), 1, nfile);
	fwrite((const void*)(&t), sizeof(int), 1, nfile);
	fwrite((const void*)(&namelength), sizeof(short), 1, nfile);
	fwrite((const void*)(&name), sizeof(char), namelength, nfile);
	fwrite((const void*)(&maillength), sizeof(short), 1, nfile);
	fwrite((const void*)(&mail), sizeof(char), maillength, nfile);
	fclose(nfile);
	fclose(file);
	remove(path);
	rename(npath, path);
	
	printf("Jelentkezes megtortent.\n\n");
}

void ListRecords()
{
	int eventid;

	while (true)
	{
		eventid = ReadInt("Kerem adja meg a rendezveny azonositojat!\nAzonosito: ");
		if (eventid >= 0)
			break;
		printf("Helytelen azonosito!\n");
	}
	FILE *file = fopen(EVENTS_PATH , "ab+");
	if (file == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", EVENTS_PATH);
		return;
	}
	fseek(file, 0, SEEK_SET);
	int id = -1;
	while (1 == fread(&id, sizeof(int), 1, file) && id != eventid) { }
	fclose(file);
	if (id != eventid)
	{
		printf("Nincs ilyen azonositoju rendezveny!\n\n");
		return;
	}
	
	char path[256];
	sprintf(path, "%d.dat", eventid);
	file = fopen(path, "rb+");
	if (file == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", path);
		return;
	}
	fseek(file, 0, SEEK_END);
    int size = ftell(file);
	fseek(file, 8, SEEK_SET);
	int number, ntime, pos = 8;
	short cnamelength, cmaillength;
	char cname[NAME_LENGTH], cmail[MAIL_LENGTH];
	printf("Jelentkezettek:\n");
	while (pos < size)
	{
		fread(&number, sizeof(int), 1, file);
		fread(&ntime, sizeof(int), 1, file);
		fread(&cnamelength, sizeof(short), 1, file);
		fread(&cname, sizeof(char), cnamelength, file);
		fread(&cmaillength, sizeof(short), 1, file);
		fread(&cmail, sizeof(char), cmaillength, file);
		time_t t = ntime;
		char strtime[256];
		sprintf(strtime, "%s", ctime(&t));
		strtime[strlen(strtime) - 1] = '\0';
		cname[cnamelength] = '\0';
		cmail[cmaillength] = '\0';
		printf("%d | %s | %s | %s\n", number, strtime, cname, cmail);
		pos += 4 + 4 + 2 + cnamelength + 2 + cmaillength;
	}
	fclose(file);
	
	printf("\n");
}

void EditRecord()
{
	int eventid, number;
	
	while (true)
	{
		eventid = ReadInt("Kerem adja meg a rendezveny azonositojat!\nAzonosito: ");
		if (eventid >= 0)
			break;
		printf("Helytelen azonosito!\n");
	}
	FILE *file = fopen(EVENTS_PATH , "ab+");
	if (file == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", EVENTS_PATH);
		return;
	}
	fseek(file, 0, SEEK_SET);
	int id = -1;
	while (1 == fread(&id, sizeof(int), 1, file) && id != eventid) { }
	fclose(file);
	if (id != eventid)
	{
		printf("Nincs ilyen azonositoju rendezveny!\n\n");
		return;
	}
	
	while (true)
	{
		number = ReadInt("Kerem adja meg a jelentkezes sorszamat!\nSorszam: ");
		if (number >= 0)
			break;
		printf("Helytelen sorszam!\n");
	}
	char path[256];
	sprintf(path, "%d.dat", eventid);
	file = fopen(path, "rb+");
	if (file == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", path);
		return;
	}
	char npath[256];
	sprintf(npath, "%d.dat.new", eventid);
	FILE *nfile = fopen(npath, "wb+");
	if (nfile == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", npath);
		return;
	}
	int hnumber;
	fseek(file, 4, SEEK_SET);
	fread(&hnumber, sizeof(int), 1, file);
	fwrite((const void*)(&eventid), sizeof(int), 1, nfile);
	fwrite((const void*)(&hnumber), sizeof(int), 1, nfile);
	fseek(file, 0, SEEK_END);
    int size = ftell(file);
	fseek(file, 8, SEEK_SET);
	int cnumber = -1, ntime, pos = 8;
	short cnamelength, cmaillength;
	char cname[NAME_LENGTH], cmail[MAIL_LENGTH];
	while (pos < size)
	{
		fread(&cnumber, sizeof(int), 1, file);
		fread(&ntime, sizeof(int), 1, file);
		fread(&cnamelength, sizeof(short), 1, file);
		fread(&cname, sizeof(char), cnamelength, file);
		fread(&cmaillength, sizeof(short), 1, file);
		fread(&cmail, sizeof(char), cmaillength, file);
		pos += 4 + 4 + 2 + cnamelength + 2 + cmaillength;		
		if (number == cnumber)
			break;
		fwrite((const void*)(&cnumber), sizeof(int), 1, nfile);
		fwrite((const void*)(&ntime), sizeof(int), 1, nfile);
		fwrite((const void*)(&cnamelength), sizeof(short), 1, nfile);
		fwrite((const void*)(&cname), sizeof(char), cnamelength, nfile);
		fwrite((const void*)(&cmaillength), sizeof(short), 1, nfile);
		fwrite((const void*)(&cmail), sizeof(char), cmaillength, nfile);
	}
	if (number != cnumber)
	{
		printf("Nincs ilyen sorszamu jelentkezes!\n\n");
		fclose(nfile);
		remove(npath);
		fclose(file);
		return;
	}
	time_t t = ntime;
	char strtime[256];
	sprintf(strtime, "%s", ctime(&t));
	strtime[strlen(strtime) - 1] = '\0';
	cname[cnamelength] = '\0';
	cmail[cmaillength] = '\0';
	printf("Jelenlegi adatok:\n%d | %s | %s | %s\n", number, strtime, cname, cmail);
	
	char name[NAME_LENGTH], mail[MAIL_LENGTH];
	ReadLine("Kerem adja meg a nevet!\nNev: ", name, NAME_LENGTH);
	ReadLine("Kerem adja meg a mail cimet!\nMail: ", mail, MAIL_LENGTH);
	time(&t);
	short namelength = strlen(name), 
	      maillength = strlen(mail);
	fwrite((const void*)(&number), sizeof(int), 1, nfile);
	fwrite((const void*)(&t), sizeof(int), 1, nfile);
	fwrite((const void*)(&namelength), sizeof(short), 1, nfile);
	fwrite((const void*)(&name), sizeof(char), namelength, nfile);
	fwrite((const void*)(&maillength), sizeof(short), 1, nfile);
	fwrite((const void*)(&mail), sizeof(char), maillength, nfile);
	
	while (pos < size)
	{
		fread(&cnumber, sizeof(int), 1, file);
		fread(&ntime, sizeof(int), 1, file);
		fread(&cnamelength, sizeof(short), 1, file);
		fread(&cname, sizeof(char), cnamelength, file);
		fread(&cmaillength, sizeof(short), 1, file);
		fread(&cmail, sizeof(char), cmaillength, file);
		fwrite((const void*)(&cnumber), sizeof(int), 1, nfile);
		fwrite((const void*)(&ntime), sizeof(int), 1, nfile);
		fwrite((const void*)(&cnamelength), sizeof(short), 1, nfile);
		fwrite((const void*)(&cname), sizeof(char), cnamelength, nfile);
		fwrite((const void*)(&cmaillength), sizeof(short), 1, nfile);
		fwrite((const void*)(&cmail), sizeof(char), cmaillength, nfile);
		pos += 4 + 4 + 2 + cnamelength + 2 + cmaillength;
	}
	fclose(nfile);
	fclose(file);
	remove(path);
	rename(npath, path);

	printf("Jelentkezes modositva.\n\n");
}

void DeleteRecord()
{
	int eventid, number;
	
	while (true)
	{
		eventid = ReadInt("Kerem adja meg a rendezveny azonositojat!\nAzonosito: ");
		if (eventid >= 0)
			break;
		printf("Helytelen azonosito!\n");
	}
	FILE *file = fopen(EVENTS_PATH , "ab+");
	if (file == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", EVENTS_PATH);
		return;
	}
	fseek(file, 0, SEEK_SET);
	int id = -1;
	while (1 == fread(&id, sizeof(int), 1, file) && id != eventid) { }
	fclose(file);
	if (id != eventid)
	{
		printf("Nincs ilyen azonositoju rendezveny!\n\n");
		return;
	}
	
	while (true)
	{
		number = ReadInt("Kerem adja meg a jelentkezes sorszamat!\nSorszam: ");
		if (number >= 0)
			break;
		printf("Helytelen sorszam!\n");
	}
	char path[256];
	sprintf(path, "%d.dat", eventid);
	file = fopen(path, "rb+");
	if (file == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", path);
		return;
	}
	char npath[256];
	sprintf(npath, "%d.dat.new", eventid);
	FILE *nfile = fopen(npath, "wb+");
	if (nfile == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", npath);
		return;
	}
	int hnumber;
	fseek(file, 4, SEEK_SET);
	fread(&hnumber, sizeof(int), 1, file);
	fwrite((const void*)(&eventid), sizeof(int), 1, nfile);
	fwrite((const void*)(&hnumber), sizeof(int), 1, nfile);
	fseek(file, 0, SEEK_END);
    int size = ftell(file);
	fseek(file, 8, SEEK_SET);
	int cnumber = -1, ntime, pos = 8;
	short cnamelength, cmaillength;
	char cname[NAME_LENGTH], cmail[MAIL_LENGTH];
	bool deleted = false;
	while (pos < size)
	{
		fread(&cnumber, sizeof(int), 1, file);
		fread(&ntime, sizeof(int), 1, file);
		fread(&cnamelength, sizeof(short), 1, file);
		fread(&cname, sizeof(char), cnamelength, file);
		fread(&cmaillength, sizeof(short), 1, file);
		fread(&cmail, sizeof(char), cmaillength, file);
		pos += 4 + 4 + 2 + cnamelength + 2 + cmaillength;		
		if (number != cnumber)
		{
			fwrite((const void*)(&cnumber), sizeof(int), 1, nfile);
			fwrite((const void*)(&ntime), sizeof(int), 1, nfile);
			fwrite((const void*)(&cnamelength), sizeof(short), 1, nfile);
			fwrite((const void*)(&cname), sizeof(char), cnamelength, nfile);
			fwrite((const void*)(&cmaillength), sizeof(short), 1, nfile);
			fwrite((const void*)(&cmail), sizeof(char), cmaillength, nfile);
		}
		else
			deleted = true;
	}
	if (!deleted)
	{
		printf("Nincs ilyen sorszamu jelentkezes!\n\n");
		fclose(nfile);
		remove(npath);
		fclose(file);
		return;
	}
	fclose(nfile);
	fclose(file);
	remove(path);
	rename(npath, path);
	
	printf("Jelentkezes torolve.\n\n");
}

void NewEvent()
{
	int eventid;
	
	bool ok = false;
	while (!ok)
	{
		eventid = ReadInt("Kerem adja meg a rendezveny azonositojat!\nAzonosito: ");
		if (eventid < 0)
		{
			printf("Helytelen azonosito!\n");
			continue;
		}
		FILE *file = fopen(EVENTS_PATH , "ab+");
		if (file == NULL)
		{
			printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", EVENTS_PATH);
			return;
		}
		fseek(file, 0, SEEK_SET);
		int id = -1;
		while (1 == fread(&id, sizeof(int), 1, file) && id != eventid) { }
		fclose(file);
		if (id == eventid)
			printf("Ilyen azonositoju rendezveny mar van!\n");
		else
			ok = true;
	}
	
	FILE *file = fopen(EVENTS_PATH, "rb+");
	if (file == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", EVENTS_PATH);
		return;
	}
	fseek(file, 0, SEEK_END);
	fwrite((const void*)(&eventid), sizeof(int), 1, file);
	fclose(file);
	
	char path[256];
	sprintf(path, "%d.dat", eventid);
	file = fopen(path, "wb+");
	if (file == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", path);
		return;
	}
	int hnumber = 0;
	fwrite((const void*)(&eventid), sizeof(int), 1, file);
	fwrite((const void*)(&hnumber), sizeof(int), 1, file);
	fclose(file);
	
	printf("Rendezveny letrehozva.\n\n");
}

void DeleteEventDatas()
{
	int eventid;
	
	while (true)
	{
		eventid = ReadInt("Kerem adja meg a rendezveny azonositojat!\nAzonosito: ");
		if (eventid >= 0)
			break;
		printf("Helytelen azonosito!\n");
	}
	FILE *file = fopen(EVENTS_PATH , "ab+");
	if (file == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", EVENTS_PATH);
		return;
	}
	fseek(file, 0, SEEK_SET);
	int id = -1;
	while (1 == fread(&id, sizeof(int), 1, file) && id != eventid) { }
	fclose(file);
	if (id != eventid)
	{
		printf("Nincs ilyen azonositoju rendezveny!\n\n");
		return;
	}
	
	char path[256];
	sprintf(path, "%d.dat", eventid);
	file = fopen(path, "wb+");
	if (file == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", path);
		return;
	}
	int hnumber = 0;
	fwrite((const void*)(&eventid), sizeof(int), 1, file);
	fwrite((const void*)(&hnumber), sizeof(int), 1, file);
	fclose(file);
	
	printf("Rendezveny adatai torolve.\n\n");
}

void DeleteEvent()
{
	int eventid;
	
	while (true)
	{
		eventid = ReadInt("Kerem adja meg a rendezveny azonositojat!\nAzonosito: ");
		if (eventid >= 0)
			break;
		printf("Helytelen azonosito!\n");
	}
	FILE *file = fopen(EVENTS_PATH , "ab+");
	if (file == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", EVENTS_PATH);
		return;
	}
	fseek(file, 0, SEEK_SET);
	int id = -1;
	while (1 == fread(&id, sizeof(int), 1, file) && id != eventid) { }
	if (id != eventid)
	{
		printf("Nincs ilyen azonositoju rendezveny!\n\n");
		fclose(file);
		return;
	}

	char npath[256];
	sprintf(npath, "%s.new", EVENTS_PATH);
	FILE *nfile = fopen(npath , "wb+");
	if (nfile == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", npath);
		fclose(file);
		return;
	}
	fseek(file, 0, SEEK_SET);
	while (1 == fread(&id, sizeof(int), 1, file))
	{
		if (id != eventid)
			fwrite((const void*)(&id), sizeof(int), 1, nfile);
	}
	fclose(nfile);
	fclose(file);
	remove(EVENTS_PATH);
	rename(npath, EVENTS_PATH);
	
	char path[256];
	sprintf(path, "%d.dat", eventid);
	remove(path);
	
	printf("Rendezveny torolve.\n\n");
}

void ListEvents()
{
	FILE *file = fopen(EVENTS_PATH , "ab+");
	if (file == NULL)
	{
		printf("ERROR: Nem sikerult megnyitni a \"%s\" fajlt!\n\n", EVENTS_PATH);
		return;
	}
	fseek(file, 0, SEEK_SET);
	int id;
	printf("Rendezvenyek: ");
	while (1 == fread(&id, sizeof(int), 1, file))
	{
		printf("%d ", id);
	}
	printf("\n\n");
	fclose(file);
}

int main(int argc, char **argv) 
{
	printf("Beadando 1 by Tovari Tamas - NLJRAZ\n\n");
	
	while (true)
	{
		printf("Opciok:\n");
		printf("0. Kilepes\n");
		printf("1. Jelentkezes rendezvenyre\n");
		printf("2. Rendezvenyre jelentkezettek listazasa\n");
		printf("3. Jelentkezes adatainak modositasa\n");
		printf("4. Jelentkezes torlese\n");
		printf("5. Rendezveny letrehozasa\n");
		printf("6. Rendezveny adatainak torlese\n");
		printf("7. Rendezveny torlese\n");
		printf("8. Rendezvenyek listazasa\n\n");
		
		int answer = ReadInt("Kerem adja meg az opcio sorszamat!\nOpcio sorszama: ");
		switch (answer)
		{
			case 0: return 0;
			case 1: NewRecord(); break;
			case 2: ListRecords(); break;
			case 3: EditRecord(); break;
			case 4: DeleteRecord(); break;
			case 5: NewEvent(); break;
			case 6: DeleteEventDatas(); break;
			case 7: DeleteEvent(); break;
			case 8: ListEvents(); break;
			default: printf("Ilyen opcio nincs!\n\n"); break;
		}
	}
}