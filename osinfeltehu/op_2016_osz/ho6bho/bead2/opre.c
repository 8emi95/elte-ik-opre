#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
//#include <sys/wait.h> 
#include <time.h>

int main()
{
	
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
	pid_t child = fork();
	//Hiba******************************************************************************************************************************************************
	if(child < 0) { perror("Kirendelt kollega nem elerheto!"); exit(-1);}
	//Szülőfolyamat, a KoS menedzser program**********************************************************************************************************************************************************************************
	if(child > 0)
	{
		//dup2(pipeToChild[0], STDIN_FILENO);
		//close(pipeToChild[1]);
		//close(pipeToChild[0]);
		//dup2(pipeFromChild[1], STDIN_FILENO);
	
		int vendegek = 0;
		int rendezveny = 0;
		
		printf("\nKozpont::Koszontjuk a King of Sands elektronikus nyilvantarto kornyezeteben!\n");
		while(1)
		{
			printf("Kerjuk, valasszon menupontot:\n");
			printf("\n1. Regisztracio\n");
			printf("\n2. Foglalasok listazasa\n");
			printf("\n3. Uj rendezveny\n");
			printf("\n4. Rendezvenyek listazasa\n");
			printf("\n5. Rendezveny lebonyolitasa\n");

			
			int n;
			scanf("%d", &n);
			if( (n < 1) || (n > 5) )
			{
				printf("Hiba, a megadott menupont nem letezik!\n\n");
			}
			//Regisztráció*************************************************************************************************************************************
			else if(n == 1)
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
		
				vendegek = vendegek + 1;
				FILE * p = fopen("data.txt", "a");
				
				char a[100];
				time_t now = time (0);
				strftime (a, 100, "%Y-%m-%d %H:%M:%S", localtime (&now));
				
					
				fprintf(p, "[%s | %s | <%s> | %s]", name, email, id, a);
				fclose(p);
				printf("\nKozpont::Gratulalunk! On a(z) %d szamu vendeg\n", vendegek);
			}
			//Regisztrált felhasználók kilistázása**************************************************************************************************************
			else if(n == 2)
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
			//Új rendezvény hozzáadása****************************************************************************************************************************
			else if(n == 3)
			{
				while(getchar()!='\n');
				printf("Kozpont::Add meg a rendezveny nevet: ");
				char name[50];
				fgets(name, 50, stdin);
				int new_line = strlen(name) -1;
				if (name[new_line] == '\n')
					name[new_line] = '\0';
					
				printf("Kozpont::Adja meg a rendezveny azonosítojat: ");
				char id[10];
				fgets(id, 10, stdin);
				new_line = strlen(id) -1;
				if (id[new_line] == '\n')
					id[new_line] = '\0';	
		
				FILE * p = fopen("dt.txt", "a");
				
				fprintf(p, "[%s | %s]", name, id);
				fclose(p);
			}
			//Rendezvények listázása******************************************************************************************************************************
			else if(n == 4)
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
			
			//Rendezvény lebnonyolítása***********************************************************************************************************************************
			else if(n == 5)
			{
				FILE * p = fopen("dt.txt", "r");
				if(p == NULL)
				{
					printf("\nMeg nincsenek rendezvenyek!\n");
				}
				else
				{
					
					char c;
					char line[50];
					int i = 0;
					c = fgetc(p);
					while (!feof(p))
					{
						while (c != '[' && c != EOF)
						{
							c = fgetc(p);
						}
						
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
						//printf("%s\n", line);
					}
					//line[i] = '\0';
					fclose(p);
					
					
					printf("Kozpont:: Kovetkezo rendezveny: %s \n", line);
					write(pipeToChild[1], line, strlen(line));
					fflush(NULL);

					printf("Kozpont::Adatok tovabbiva!\n");
					//wait();
					//Válasz várása
					char reply[50];
					read(pipeFromChild[0], reply, sizeof(reply));
					printf("Kozpont*********************************************\n");
					printf("Kozpont:: Uzenet a szervezotol: %s \n", reply);
					
					//Résztvevők kiküldése
					FILE * p = fopen("data.txt", "r");
					if(p == NULL)
					{
						printf("\Meg nincsenek resztvevok!\n");
					}
					else
					{
						
						char c;
						char line[50];
						int i = 0;
						c = fgetc(p);
						while (!feof(p))
						{
							while (c != '[' && c != EOF)
							{
								c = fgetc(p);
							}
							
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
							//printf("%s\n", line);
						}
						//line[i] = '\0';
						fclose(p);
						
					}
			}
	}
	}
	
	//Gyerekfolyamat, a rendezvény helyi képviselője********************************************************************************************************************************
	else
	{

		
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
		id[y-3] = '\0';
		 
		
		printf("Szervezo*********************************************\n");
		printf("Szervezo:: Megkaptam az adatokat:\n");
		printf("%s\n", name);
		printf("%s\n", id);
		
		
		//Válasz írása
		write(pipeFromChild[1], "Helyben vagyok!",16); 

	}
	
	return 0;
}
		