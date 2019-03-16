#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>

typedef struct Event
{
	int id;
	char eventname[30];
} Event;

typedef struct Visitor
{
	char name[30];
	char email[30];
	int e_id;
	char time[50];
} Visitor;

void ReadingEvent(FILE* g, const char* fname, struct Event* E, const size_t length)
{
	g = fopen(fname,"w+");
	int i;
	for (i = 0; i < length; i++)
	{
		fprintf(g, "%d %s\r\n", E[i].id, &(E[i].eventname));
	}
	rewind(g);
}

void ReadingVisitor(FILE* g, const char* fname, struct Visitor* V, const size_t length)
{
	g = fopen(fname,"w+");
	int i;
	for (i = 0; i < length; i++)
	{
		fprintf(g, "%s %s %d %s\r\n", &(V[i].name), &(V[i].email), V[i].e_id, &(V[i].time));
	}
	rewind(g);
}

int main(int argc, char** argv)
{
	if (argc != 3) { perror("Adjon meg 2 fajlnevet argumentumkent! (rendezveny, latogatok)\n"); exit(1); }
	
	if (!access(argv[1], F_OK) == 0)
	{
		perror("Nem letezik ilyen fajl!\n");
		exit(1);
	}
	
	FILE* df;
	df = fopen(argv[1], "a+");
	FILE *pf;
	pf = fopen(argv[2], "a+");
	
	if (df == NULL || pf == NULL) { perror("Nem sikerult a fajlt megnyitni!\n"); exit(1); }
	

	size_t ds = 0;
	char p = getc(df); //counting lines in file to allocate memory for struct
	while (p != EOF)
	{
		if (p == '\n') ++ds;
		p = getc(df);
	}
	rewind(df);
	
	Event* E;
	E = (Event*)malloc(ds * sizeof(Event));
	
	int i = 0;
	int j = 0;
	while(!feof(df) && i < ds)
	{
		fscanf(df, "%d %s\r\n", &(E[i].id), E[i].eventname);
		++i;
	}
	rewind(df);


	size_t ps = 0;
	p = getc(pf);
	while (p != EOF)
	{
		if (p == '\n') ++ps;
		p = getc(pf);
	}
	rewind(pf);

	Visitor* V;
	V = (Visitor*)malloc(ps * sizeof(Visitor));

	i = 0;
	j = 0;
	while(!feof(pf) && i < ps)
	{
		fscanf(pf, "%s %s %d %s\r\n", V[i].name, V[i].email, &(V[i].e_id), V[i].time);
		++i;
	}
	rewind(pf);
	
	int x = 1;
	int y;
	char tmp[20];
	int seged;
	bool l = false;
	
	do
	{
		printf("\r\n 1. Rendezvenyek listazasa\r\n 2. Rendezveny hozzaadasa\r\n 3. Rendezveny torlese\r\n 4. Jelentkezesek listaja\r\n 5. Uj vendeg jelentkezese\r\n 6. Vendeg modositasa\r\n 7. Vendeg torlese\r\n 8. Kilepes\r\n");
		printf("\r\n");
		scanf("%d", &x);
		switch(x) {

			case 1: // Rendezvenyek listazasa
				i = 0;
				j = 0;
				while (i < ds)
				{
					printf("%d %s\r\n", E[i].id, E[i].eventname);
					i++;
				}
				break;

			case 2: // Rendezveny hozzaadasa
				++ds;
				E = (Event*)realloc(E, ds * sizeof(Event));
				
				printf("Rendezveny azonositoja: ");
				scanf("%d", &(E[ds - 1].id));
				printf("Rendezveny neve: ");
				scanf("%s", E[ds - 1].eventname);

				ReadingEvent(df, argv[1], E, ds);
				break;

			case 3: // Rendezveny torlese
				printf("Rendezveny azonositoja: ");
				scanf("%d", &seged);
				i = 0;
				j = 0;
				while (i < ds && !l)
				{
					if (E[i].id == seged)
					{
						l = true;
					}
					++i;
				}

				if (l)
				{
					for (j = i; j < ds; j++)
					{
						E[j - 1].id = E[j].id;
						strcpy(E[j - 1].eventname, E[j].eventname);
					}
					--ds;
					E = (Event*)realloc(E, ds * sizeof(Event));

					ReadingEvent(df, argv[1], E, ds);
				}
				else printf("Nincs ilyen rendezveny a listan!\r\n");
				break;

			case 4: // Jelentkezesek listaja
				i = 0;
				j = 0;
				while (i < ps)
				{
					printf("Nev: %s\r\nEmail: %s\r\nRendezveny azonositoja: %d\r\nJelentkezes ideje: %s\r\n", V[i].name, V[i].email, V[i].e_id, V[i].time);
					++i;
				}
				break;

			case 5: // Jelentkezes felvetele
				++ps;
				V = (Visitor*)realloc(V, ps * sizeof(Visitor));

				printf("Latogato neve: ");
				scanf("%s", V[ps - 1].name);
				printf("Email cime: ");
				p = getchar();
				gets(tmp);
				strcpy(V[ps - 1].email, tmp);

				l = false;
				while (!l)
				{
					printf("\r\nValasszon rendezveny azonositot:\r\n");
					for (i = 0; i < ds; i++)
					{
						printf("%d %s\r\n", E[i].id, E[i].eventname);
					}
					printf("\r\n");
					scanf("%d", &seged);
					i = 0;
					//j = 0;
					while (!l && i < ds)
					{
						if (E[i].id == seged)
						{
							l = true;
						}
						i++;
					}
					if (!l) printf("Nincs ilyen rendezveny!\r\n");
				}
				V[ps - 1].e_id = seged;

				time_t now;
				struct tm *now_tm;
				char buffer[30];
				now = time(NULL);
				now_tm = localtime(&now);
				sprintf(tmp, "%d", now_tm->tm_hour);
				strcpy(buffer, tmp);
				strcat(buffer, ":");
				strcat(tmp, ":");
				sprintf(tmp, "%d", now_tm->tm_min);
				strcat(buffer, tmp);

				strcpy(V[ps - 1].time, buffer);
				ReadingVisitor(pf, argv[2], V, ps);
				char msg[80];
				sprintf(msg, "Sikeres jelentkezes! On a: %d", ps);
				strcat(msg, ". vendegunk.");
				printf("%s\r\n",msg);
				break;

			case 6: // Jelentkezes modositasa
				printf("Jelentkezo neve: ");
				scanf("%s", tmp);
				i = 0;
				j = 0;
				l = false;
				while (i < ps && !l)
				{
					l = strcmp(V[i].name, tmp) == 0;
					++i;
				}

				if (l)
				{
					printf("Modositom:\r\n 1. Nev\r\n 2. email\r\n 3. rendezveny azonosito\r\n");
					scanf("%d", &x);
					switch(x)
					{
						case 1:
							printf("Uj nev: ");
							scanf("%s", tmp);
							strcpy(V[i - 1].name, tmp);
							break;
						case 2:
							printf("Uj email: ");
							scanf("%s", tmp);
							strcpy(V[i - 1].email, tmp);
							break;
						case 3:
							l = false;
							while (!l)
							{
								printf("\r\nValasszon rendezveny azonositot:\r\n");
								for (j = 0; j < ds; j++)
								{
									printf("%d %s\r\n", E[j].id, E[j].eventname);
								}
								printf("\r\n");
								scanf("%d", &seged);
								j = 0;
								while (!l && j < ds)
								{
									if (E[j].id == seged)
									{
										l = true;
										V[i - 1].e_id = seged;
									}
									j++;
								}
								if (!l) printf("Nincs ilyen rendezveny!\r\n");
							}
							break;
						default: break;
					}

					ReadingVisitor(pf, argv[2], V, ps);
				}
				else printf("Nincs ilyen nev a listan!\r\n");

				break;

			case 7: // Jelentkezes torlese
				printf("Latogato neve: ");
				scanf("%s", tmp);
				i = 0;
				j = 0;
				l = false;
				while (i < ps && !l)
				{
					l = strcmp(V[i].name, tmp) == 0;
					++i;
				}

				if (l)
				{
					for (j = i; j < ps; j++)
					{
						strcpy(V[j - 1].name, V[j].name);
						strcpy(V[j - 1].email, V[j].email);
						V[j - 1].e_id = V[j].e_id;
						strcpy(V[j - 1].time, V[j].time);
					}
					--ps;
					V = (Visitor*)realloc(V, ps * sizeof(Visitor));

					ReadingVisitor(pf, argv[2], V, ps);
				}
				else printf("Nincs ilyen nev a listan!\r\n");

				break;
			case 8: break;
			default: break;
		}
	} while (x != 8);

	fclose(df);
	fclose(pf);
	return 0;
}
