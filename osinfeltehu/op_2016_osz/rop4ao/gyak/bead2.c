/*Járvás Tamás Szilveszter ROP4AO*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <signal.h>


//uzenet struct a msgsnd, msgrcv muveletekhez
typedef struct Message 
{ 
     long mtype;
     char mtext [ 1024 ]; 
} Message;
typedef struct Event
{
	int id;
	char eventname[50];
} Event;

typedef struct Visitor
{
	char name[40];
	char email[60];
	int e_id;
	char time[50];
} Visitor;

//a kapott Event strukturat formazottan fajlba irja, a Visitorra ugyanez
void WriteEvent(FILE* file, const char* fname, struct Event* E, const size_t length)
{
	file = fopen(fname,"w+");
	int i;
	for (i = 0; i < length; i++)
	{
		fprintf(file, "%d %s\r\n", E[i].id, &(E[i].eventname));
	}
	rewind(file);
}
void WriteVisitor(FILE* file, const char* fname, struct Visitor* V, const size_t length)
{
	file = fopen(fname,"w+");
	int i;
	for (i = 0; i < length; i++)
	{
		fprintf(file, "%s %s %d %s\r\n", &(V[i].name), &(V[i].email), V[i].e_id, &(V[i].time));
	}
	rewind(file);
}

int main(int argc, char** argv)
{
	if (argc != 3) { perror("Adjon meg 2 fajlnevet argumentumnnak! (event.txt, visitor.txt)\n"); exit(1); }
	
	if (!access(argv[1], F_OK) == 0)
	{
		perror("Nem letezik ilyen fajl!\n");
		exit(1);
	}
	
	//fajlok megnyitasa olvasasra es hozzafuzesre
	FILE* eFile;
	eFile = fopen(argv[1], "a+");
	FILE* vFile;
	vFile = fopen(argv[2], "a+");
	
	if (eFile == NULL || vFile == NULL) { perror("Nem sikerult a fajlt megnyitni!\n"); exit(1); }
	
	//sorok szamolasa a fajlokban memoriafoglalas celjara
	size_t eLineNum = 0;
	char c = getc(eFile);
	while (c != EOF)
	{
		if (c == '\n') ++eLineNum;
		c = getc(eFile);
	}
	rewind(eFile);
	
	Event* E;
	E = (Event*)malloc(eLineNum * sizeof(Event));
	// fajlbol kiolvasas a sorok szamanak megfeleloen
	int i = 0;
	int j = 0;
	while(!feof(eFile) && i < eLineNum)
	{
		fscanf(eFile, "%d %s\r\n", &(E[i].id), E[i].eventname);
		++i;
	}
	rewind(eFile);

	//ugyanez a latogatokra is
	size_t vLineNum = 0;
	c = getc(vFile);
	while (c != EOF)
	{
		if (c == '\n') ++vLineNum;
		c = getc(vFile);
	}
	rewind(vFile);

	Visitor* V;
	V = (Visitor*)malloc(vLineNum * sizeof(Visitor));

	i = 0;
	j = 0;
	while(!feof(vFile) && i < vLineNum)
	{
		fscanf(vFile, "%s %s %d %s\r\n", V[i].name, V[i].email, &(V[i].e_id), V[i].time);
		++i;
	}
	rewind(vFile);
	
	int x = 1;
	char tmp[60];
	int temp;
	bool l = false;
	
	do
	{
		//menü
		printf("\r\nKing of Stands ---- Elektronikus latogatoi nyilvantartas\r\n");
		printf("\r\n 1. Uj Rendezveny\r\n 2. Rendezvenylista\r\n 3. Rendezveny torlese\r\n 4. Uj vendeg\r\n 5. Vendeglista\r\n 6. Vendeg modositasa\r\n 7. Vendeg torlese\r\n 8. Rendezveny lebonyolitasa\r\n 9. Kilepes\r\n");
		printf("\r\n");
		scanf("%d", &x);
		switch(x) {

			case 1: // Uj rendezveny
				++eLineNum;
				//memória ujrafoglalas
				E = (Event*)realloc(E, eLineNum * sizeof(Event));
				
				
				//structba behelyezes
				printf("Rendezveny azonositoja: ");
				scanf("%d", &(E[eLineNum - 1].id));
				printf("Rendezveny neve: ");
				scanf("%s", E[eLineNum - 1].eventname);

				WriteEvent(eFile, argv[1], E, eLineNum);
				break;

			case 2: // Rendezvenylista
				
				i = 0;
				j = 0;
				while (i < eLineNum)
				{
					printf("%d %s\r\n", E[i].id, E[i].eventname);
					i++;
				}
				
				break;

			case 3: // Rendezveny torlese
				printf("Rendezveny azonositoja: ");
				scanf("%d", &temp);
				i = 0;
				j = 0;
				//azonosito kereses
				while (i < eLineNum && !l)
				{
					if (E[i].id == temp)
					{
						l = true;
					}
					++i;
				}
				//ha talalt elcsusztatja a structot, ezzel torolve a talaltat
				if (l)
				{
					for (j = i; j < eLineNum; j++)
					{
						E[j - 1].id = E[j].id;
						strcpy(E[j - 1].eventname, E[j].eventname);
					}
					--eLineNum;
					E = (Event*)realloc(E, eLineNum * sizeof(Event));

					WriteEvent(eFile, argv[1], E, eLineNum);
				}
				else printf("Nincs ilyen rendezveny a listan!\r\n");
				break;

			case 4: // Uj vendeg
				++vLineNum;
				V = (Visitor*)realloc(V, vLineNum * sizeof(Visitor));

				printf("Latogato neve: ");
				scanf("%s%*c", V[vLineNum - 1].name);
				printf("Email cime: ");
				scanf("%s%*c", V[vLineNum - 1].email);

				l = false;
				while (!l)
				{
					//a rendezveny letezesenek ellenorzese
					printf("\r\nA rendezveny azonositoja, ahova jelentkezik:\r\n");
					scanf("%d", &temp);
					i = 0;
					while (!l && i < eLineNum)
					{
						if (E[i].id == temp)
						{
							l = true;
						}
						i++;
					}
					if (!l) printf("Nincs ilyen azonositoju rendezveny, adjon meg masik azonositot!\r\n");
				}
				V[vLineNum - 1].e_id = temp;
				
				//ido
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

				strcpy(V[vLineNum - 1].time, buffer);
				WriteVisitor(vFile, argv[2], V, vLineNum);
				printf("Jelentkezeset elkonyveltuk!\n");
				break;

			case 5: // Vendeglista
				i = 0;
				j = 0;
				while (i < vLineNum)
				{
					printf("Nev: %s\r\nEmail: %s\r\nRendezveny azonositoja: %d\r\nJelentkezes ideje: %s\r\n", V[i].name, V[i].email, V[i].e_id, V[i].time);
					printf("\n");
					++i;
				}
				break;

			case 6: // Vendeg modositasa
				printf("Jelentkezo neve: ");
				scanf("%s", tmp);
				i = 0;
				j = 0;
				l = false;
				while (i < vLineNum && !l)
				{
					l = strcmp(V[i].name, tmp) == 0;
					++i;
				}

				if (l)
				{
					printf("Modositom:\r\n 1. Nev\r\n 2. Email\r\n 3. Rendezveny azonosito\r\n");
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
								printf("\r\nUj rendezveny azonosito: \r\n");
								scanf("%d", &temp);
								j = 0;
								while (!l && j < eLineNum)
								{
									if (E[j].id == temp)
									{
										l = true;
										V[i - 1].e_id = temp;
									}
									j++;
								}
								if (!l) printf("Nincs ilyen azonositoju rendezveny, adjon meg masik azonositot!\r\n");
							}
							break;
						default: break;
					}

					WriteVisitor(vFile, argv[2], V, vLineNum);
				}
				else printf("Nincs ilyen nev a listan!\r\n");

				break;

			case 7: // Vendeg torlese
				printf("Latogato neve: ");
				scanf("%s", tmp);
				i = 0;
				j = 0;
				l = false;
				while (i < vLineNum && !l)
				{
					l = strcmp(V[i].name, tmp) == 0;
					++i;
				}
				//talalat eseten az egész Visitor tomb elcsusztatasa
				if (l)
				{
					for (j = i; j < vLineNum; j++)
					{
						strcpy(V[j - 1].name, V[j].name);
						strcpy(V[j - 1].email, V[j].email);
						V[j - 1].e_id = V[j].e_id;
						strcpy(V[j - 1].time, V[j].time);
					}
					--vLineNum;
					V = (Visitor*)realloc(V, vLineNum * sizeof(Visitor));

					WriteVisitor(vFile, argv[2], V, vLineNum);
				}
				else printf("Nincs ilyen nev a listan!\r\n");
				
				break;
				
			case 8: //Rendezvény lebonyolitasa
					printf("\nMelyik azonositoju rendezveny keruljon lebonyolitasra?\n");
					i = 0;
					j = 0;
					
					int pipefd[2]; 
				    pid_t pid;		//pipeline letrehozasa
				    char* sentVisitors[10];
				    char visitorNameFromParent[40];
					bool eventFound = false; 
					int messageQueue;
					int readCheck;
					int numOfReadVisitors;
					int status;
					
					while (i < eLineNum)
					{
						printf("%d %s\r\n", E[i].id, E[i].eventname);
						i++;
					}
					
					printf("\r\n");
					scanf("%d", &temp);
					
					key_t generatedKey;
					generatedKey = ftok("/tmp",1);    //kulcs generalasa
					messageQueue = msgget( generatedKey, 0600 | IPC_CREAT );   //letrehozza az uzenetsort 

					if ( messageQueue < 0 ) 
					{ 
				          perror("msgget hiba");
				          exit(1); 
				    }

					if (pipe(pipefd) == -1)
					{
				        perror("Hiba a pipe nyitaskor!");
				        exit(EXIT_FAILURE);
				    }

				    pid = fork();

					if (pid == -1)
					{
				        perror("Fork hiba");
				        exit(EXIT_FAILURE);
				    }

				    if (pid == 0)     // gyerekfolyamat
				    {   	
					    sleep(1);
				        close(pipefd[1]); //bezarja az output oldalat a pipenak
						numOfReadVisitors = 0;  //a megkapott latogatok szama

						while ((readCheck = read(pipefd[0], visitorNameFromParent, sizeof(visitorNameFromParent))) > 0)    //amíg olvas valamit a csobol addig megy
						{
							printf("Gyerek olvasta a csobol a %i. vendeg nevet: %s", numOfReadVisitors+1 , visitorNameFromParent);
							printf("\n");
							numOfReadVisitors++;
						}

				        close(pipefd[0]);
						int i;
						srand(time(NULL));

						sleep(1);
						int eventRating=(rand()%5)+1;
						char charEventRating[80];
						sprintf(charEventRating, "%d", eventRating);  //a generalt random szamot elkuldi uzenetben a szulonek
						const struct Message ratingMessage = { 1, *charEventRating }; 
						//int status;
						status = msgsnd( messageQueue, &ratingMessage, sizeof(ratingMessage.mtext), 0 );

						if ( status < 0 )
						{
							perror("msgsnd hiba"); 
						}

						for(i = 0; i < numOfReadVisitors; i++)   //minden kapott vendegre general egy random szamot, ami ha kisebb, mint 10, akkor nem jott meg
						{
							sleep(1);
							int arrivedOrNot = (rand()%100)+1;

							if (arrivedOrNot <= 10)
							{
								char charNotArrivedID[10];
								sprintf(charNotArrivedID, "%d", i);
								const struct Message notArrivedMessage = { 1, *charNotArrivedID }; 
								int status;
								status = msgsnd( messageQueue, &notArrivedMessage, sizeof(notArrivedMessage.mtext), 0 ); //ha 10-nél kevesebb, tehat nem erkezett meg, akkor kiirja a sorszamat a vendegek kozott 0-tol indexelve

								if ( status < 0 )
								{
									perror("msgsnd hiba"); 
								}
							}
						}

						const struct Message endMessage = { 5, "end" };   //end uzenet kuldese az messageQueuera, ha vegzett
						int status;
						status = msgsnd( messageQueue, &endMessage, sizeof(endMessage.mtext), 0 );

						if ( status < 0 )
						{
							perror("msgsnd hiba"); 
						}

						sleep(1);
						exit(0);
				    }
				    else   // szulofolyamat
				    {    
				        close(pipefd[0]);
						i = 0;
						while (!eventFound && i < eLineNum)    //rendezveny keresese, a valasztott ID alapjan
						{
							if (E[i].id == temp)
							{
								eventFound = true;
							}
							i++;
						}
						
						if (!eventFound)
						{
							printf("Nincs ilyen azonositoju rendezveny, adjon meg masik azonositot!\r\n");
						}
						else
						{
							i = 0;
							j = 0;
							while (i < vLineNum)
							{
								if (V[i].e_id == temp)
								{
									sentVisitors[j] = V[i].name;  //beirja a tombbe az elkuldott vendegek nevet, hogy kiirjuk majd ha nem jon meg
									sleep(1);
									write(pipefd[1], V[i].name, sizeof(V[i].name));      //vendeglista kuldese
									printf("A szulo beirta a %i. nevet a csobe!\n",j+1);
									fflush(NULL);
									j++;
								}
								i++;
							}
						}

						fflush(NULL); 
						close(pipefd[1]);
						struct Message messageFromChild; 
						int status;
						
						status = msgrcv(messageQueue, &messageFromChild, 1024, 0, 0 );   //uzenetek fogadasa a gyerektol
						if ( status < 0 )
						{
							perror("msgrcv hiba"); 
						}
						printf( "A Rendezveny sikeressege (1-5 skala) :  %s\n", messageFromChild.mtext ); //az messageQueuebol kapja meg az esemeny ertekeleset

						bool isRunning = true;
						while(isRunning)
						{
							status = msgrcv(messageQueue, &messageFromChild, 1024, 0, 0 );

							if ( status < 0 )
							{
								perror("msgrcv hiba"); 
							}
							else if(isRunning = (strcmp(messageFromChild.mtext,"end") != 0))         //amíg igaz az hogy a kapott uzenet nem egyenlo az enddel, addig csinalja
							{
								int index = atoi( messageFromChild.mtext );						//ha nem "end" jon, hanem egy sorszam, akkor az a vendeg nem jott meg
								printf( "Nem érkezett meg:  %s\n", sentVisitors[index] );
							}
						}
						wait();
					}
					break;

			case 9: break;
			default: break;
		}
	} while (x != 9);

	fclose(eFile);
	fclose(vFile);
	return 0;
}