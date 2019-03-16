#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
//#include <sys/wait.h> 
#include <time.h>
#include <sys/types.h>
#include <signal.h>


//Regisztráció
void newUser(void)
{
    while(getchar()!='\n');
	//printf("Adja meg a nevet, emailcimet és a rendezveny azonositojat\n|-jellel elvalasztva:\n");
	printf("Kozpont::Adja meg a nevet: ");
	char name[50];
	fgets(name, 50, stdin);
	int new_line = strlen(name) -1;
	if (name[new_line] == '\n')
	name[new_line] = '\0';
				
	printf("Kozpont::Adja meg az emailcimet: ");
	char email[50];
	fgets(email, 50, stdin);
	new_line = strlen(email) -1;
	if (email[new_line] == '\n')
		email[new_line] = '\0';
				
	printf("Kozpont::Adja meg a rendezveny azonosítojat: ");
	char id[10];
	fgets(id, 10, stdin);
	new_line = strlen(id) -1;
	if (id[new_line] == '\n')
		id[new_line] = '\0';
		
	//vendegek = vendegek + 1;
	FILE * p = fopen("data.txt", "a");
				
	char a[100];
	time_t now = time (0);
	strftime (a, 100, "%Y-%m-%d %H:%M:%S", localtime (&now));
				
					
	fprintf(p, "[%s | %s | <%s> | %s]", name, email, id, a);
	fclose(p);
	//printf("\nKozpont::Gratulalunk! On a(z) %d szamu vendeg\n", vendegek);
}
//Felhasználók kilistázása
void listUsers(void)
{
    printf("\nKozpont::A regisztralt felhasznalok:***********************************\n\n");
	printf("Nev             Email            Rendezveny azonosito    Jelentkezes ideje\n");
	FILE * p = fopen("data.txt", "r");
	if(p == NULL)
	{
		printf("Meg nincsenek felhasznalok!\n");
	}
	else
	{
		char c;
		char line[100];
		c = fgetc(p);
		while (!feof(p))
		{
			while (c != '[' && c != EOF)
			{
				c = fgetc(p);
							}
				int i = 0;
				while( c != ']' && c != EOF)
				{
					c = fgetc(p);
					if(c != ']')
					{
						line[i] = c;
						i = i + 1;
					}
				}
				line[i] = '\0';
				printf("%s\n", line);
				}
				fclose(p);
		}
				
}
//új rendezvény hozzáaádsa
void newEvent(void)
{
	while(getchar()!='\n');
	printf("Kozpont::Add meg a rendezveny nevet: ");
	char name[50];
	fgets(name, 50, stdin);
	int new_line = strlen(name) -1;
	if (name[new_line] == '\n')
			name[new_line] = '\0';
					
	printf("Kozpont::Adja meg a rendezveny azonosítojat (csak egyjegyu szam lehet): ");
	char id[10];
	fgets(id, 10, stdin);
	new_line = strlen(id) -1;
	if (id[new_line] == '\n')
		id[new_line] = '\0';	
		
	FILE * p = fopen("dt.txt", "a");
				
	fprintf(p, "[%s | <%s>]", name, id);
	fclose(p);
}
//Rendezvények kilistázása
void listEvents(void)
{
	printf("Kozpont::\n");
	printf("Nev             Rendezveny azonosito\n");
	FILE * p = fopen("dt.txt", "r");
	if(p == NULL)
	{
		printf("\nMeg nincsenek rendezvenyek!\n");
	}
	else
	{
		char c;
		char line[50];
		c = fgetc(p);
		while (!feof(p))
		{
			while (c != '[' && c != EOF)
			{
				c = fgetc(p);
			}
			int i = 0;
			while( c != ']' && c != EOF)
			{
				c = fgetc(p);
				if(c != ']')
				{
					line[i] = c;
					i = i + 1;
				}
			}
			line[i] = '\0';
			printf("%s\n", line);
		}
				
		fclose(p);
	}
}

int main()
{
	//Csővezetékek létrehozása
	int pipeToChild[2];
	int pipeFromChild[2];
	
	if(pipe(pipeToChild) == -1)
	{
		perror("Nem mukodnek a telefonok!"); 
		exit(-1);
	}
	
	if(pipe(pipeFromChild) == -1)
	{
		perror("Nem mukodnek a telefonok!"); 
		exit(-1);
	}
    
    //Gyermekfolyamat létrehozása
	pid_t child = fork();
    
	//Hiba******************************************************************************************************************************************************
	if(child < 0) { perror("Kirendelt kollega nem elerheto!"); exit(-1);}
	//Szülőfolyamat, a KoS menedzser program**********************************************************************************************************************************************************************************
	if(child > 0)
	{
	
		//int vendegek = 0;
		int rendezveny = 0;
		printf("\nKoszontjuk a King of Sands elektronikus nyilvantarto kornyezeteben!\n");
		while(1)
		{
			printf("Kerjuk, valasszon menupontot:\n");
			printf("\n1. Regisztracio\n");
			printf("\n2. Foglalasok listazasa\n");
			printf("\n3. Uj rendezveny\n");
			printf("\n4. Rendezvenyek listazasa\n");
			printf("\n5. Rendezvenyek lebonyolitasa\n");

			
			int n;
			scanf("%d", &n);
			if( (n < 1) || (n > 5) )
			{
				printf("Hiba, a megadott menupont nem letezik!\n\n");
			}
			//Regisztráció
			else if(n == 1)
			{
				newUser();
			}
			//Regisztrált felhasználók kilistázása
			else if(n == 2)
			{
				listUsers();
			}
			//Új rendezvény hozzáadása
			else if(n == 3)
			{
				newEvent();
			}
			//Rendezvények listázása
			else if(n == 4)
			{
				listEvents();
			}
			
			//Rendezvény lebnonyolítása
			else if(n == 5)
			{
				close(pipeToChild[0]);
				close(pipeFromChild[1]);
				
				//Rendezvény azonosítója, később beolvasásra kerül
				char eventId[100];
				
				//Következő rendezvény kiolvasása
				FILE * q = fopen("dt.txt", "r");
				if(q == NULL)
				{
					printf("\nMeg nincsenek rendezvenyek!\n");
				}
				else
				{
					
					char h;
					
					h = fgetc(q);
					while (!feof(q))
					{
						char line[100];
						int i = 0;
						int j = 0;
						while (h != '[' && h != EOF)
						{
							h = fgetc(q);
						}
						
						while( h != ']' && h != EOF)
						{
							h = fgetc(q);
							if(h != ']')
							{
								if(h == '<')
								{
									h = fgetc(q);
									while(h != '>')
									{
										eventId[j] = h;
										line[i] = h;
										++j;
										++i;
										h = fgetc(q);
									}
								}
								line[i] = h;
								i = i + 1;
							}
						}
						line[i-1] = '\0';
						eventId[1] = '\0';
						
							//Rendezvény megküldése a gyerekfolyamatnak
						write(pipeToChild[1], line, strlen(line));
						fflush(NULL);
						//printf("Kozpont::Adatok tovabbitva! %s\n", eventId);
						
						
						//Válasz várása
						
						char reply[50];
						read(pipeFromChild[0], reply, sizeof(reply));
						//printf("Kozpont***************************************************************\n");
						//printf("Kozpont:: Uzenet a szervezotol:\n" );
						
						//Résztvevők kiküldése
						FILE * p = fopen("data.txt", "r");
						if(p == NULL)
						{
							printf("\Meg nincsenek resztvevok!\n");
							write(pipeToChild[1], ".", 2);
							fflush(NULL);
						}
						else
						{
							char c;
							
							char name2[100];
							char id2[100];
							
							int bool = 0;
							//int i = 0;
							//int j = 0;
							
							c = fgetc(p);
							while (!feof(p))
							{
								int i = 0;
								int j = 0;
								//memset(name2, 0, sizeof(name2));
								while (c != '[' && c != EOF)
								{
									c = fgetc(p);
								}
								c = fgetc(p);
								while( c != '|' && c != EOF)
								{
									
									name2[i] = c;
									++i;
									c = fgetc(p);
								}
								
								while(c != '<' && c != EOF)
								{
									c = fgetc(p);
								}
								c = fgetc(p);
								while(c != '>' && c != EOF)
								{
									
									id2[j] = c;
									++j;
									c = fgetc(p);
								}
								
								name2[i] = '\0';
								id2[j] = '\0';
								//printf("Resztvevo: %s\n", name2);
								//printf("Resztvevo id: %s\n", id2);
								
								
								if(strcmp(eventId, id2) == 0)
								{
									write(pipeToChild[1], name2, strlen(name2));
									sleep(2);
									//close(pipeToChild[1]);
									//printf("Kuld\n");
									fflush(NULL);
								}

							}
							write(pipeToChild[1], ".", 2);
							fflush(NULL);
							
							
							fclose(p);
							
						}
						
						//Meg nem érkezettek:
						//printf("Kozpont***************************************************************\n");
						int b = 0;
						while(b == 0)
						{
							char lost[50];
							memset(lost, 0, sizeof(lost));
							read(pipeFromChild[0], lost, sizeof(lost));

							if(strcmp(lost, ".") == 0)
							{
								b = 1;
							}
							else
							{
								printf("%s vegul nem vett reszt a rendezvenyen\n",  lost);		
							}
						}
						
						while (h != ']' && h != EOF)
						{
							h = fgetc(q);
						}
						
					}
					
					fclose(q);
					printf("********Minden rendezveny lezajlott, a program most kilep!********\n");
					kill(child, SIGTERM);
					return 1;	
					
					
				}
			}
		}
	}
	//Gyerekfolyamat, a rendezvény helyi képviselője********************************************************************************************************************************
	else
	{
		while(1)
		{
			srand(time(NULL));
			
			close(pipeFromChild[0]);
			close(pipeToChild[1]);
			
			char line[100];
			read(pipeToChild[0], line, sizeof(line));
			
			
			char name[50];
			char id[10];
			
			int i = 0;
			int bool = 0;
			int x = 0;
			int y = 0;
			while (i < strlen(line))
			{
				if(line[i] == '|')
				{
					bool = 1;
					++i;
				}
				else if(bool == 0)
				{
					name[x] = line[i];
					++x;
				}
				else
				{
					id[y] = line[i];
					++y;
				}

				++i;
			}
		
			
			name[x] = '\0';
			id[1] = '\0';
			
			
			//printf("Szervezo**********************************************************************\n");
			printf("Szervezo:: A rendezveny adatai:\n");
			printf("Rendezveny: %s\n", name);
			printf("Azonosito: %s\n", id);
			
			
			//Válasz írása
			write(pipeFromChild[1], "Helyben vagyok!",16); 
			printf("Kezdodhet a %s rendezveny!\n", name);
			
			//Vendégek fogadása
			//printf("Szervezo*********************************************************************\n");
			char lost[100][100];
			int l = 0;
			
			int b = 0;
			while(b == 0)
			{
				char name2[50];
				memset(name2, 0, sizeof(name2));
				read(pipeToChild[0], name2, sizeof(name2));

				
				if(strcmp(name2, ".") == 0)
				{
					b = 1;
				}
				else
				{
					int r = rand() % 10;
					if(r != 0)//10% esély, hogy a vendég nem érkezik meg.
					{
						printf(" Uj vendeg erkezett: %s\n",  name2);
					}
					else
					{
						strcpy(lost[l], name2);
						++l;
					}
					
				}
			}
			
			i = 0;
			while(i < l)
			{
				write(pipeFromChild[1], lost[i], strlen(lost[i]));
				fflush(NULL);
				sleep(2);
				++i;
			}
			write(pipeFromChild[1], ".", 2);
			fflush(NULL);
		}
		
	}
	
	return 0;
}



