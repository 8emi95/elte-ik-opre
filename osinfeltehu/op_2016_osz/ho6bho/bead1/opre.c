#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	int vendegek = 0;
	
	printf("\nKoszontjuk a King of Sands elektronikus nyilvantarto kornyezeteben!\n");
	while(1)
	{
		printf("Kerjuk, valasszon menupontot:\n");
		printf("\n1. Regisztracio\n");
		printf("\n2. Foglalasok listazasa\n");
		printf("\n3. Regisztrácio modositasa\n");
		printf("\n4. Felhasználó törlése\n");
		printf("\n5. Rendezveny hozzaadasa\n");
		printf("\n6. Rendezveny torlese\n");
		printf("\n7. Rendezvenyek kilistazasa\n");
		
		int n;
		scanf("%d", &n);
		if( (n < 1) || (n > 7) )
		{
			printf("Hiba, a megadott menupont nem letezik!\n\n");
		}
		//Regisztráció
		else if(n == 1)
		{
			while(getchar()!='\n');
			printf("Adja meg a nevet, emailcimet és a rendezveny azonositojat\nszokozzel elvalasztva:\n");
			char line[50];
			fgets(line, 50, stdin);
	
			vendegek = vendegek + 1;
			FILE * p = fopen("data.txt", "a");
			
			char a[100];
			time_t now = time (0);
			strftime (a, 100, "%Y-%m-%d %H:%M:%S", localtime (&now));
			
			int new_line = strlen(line) -1;
			if (line[new_line] == '\n')
				line[new_line] = '\0';
				
			fprintf(p, "[%s\t%s]", line, a);
			fclose(p);
			printf("\nGratulalunk! On a(z) %d szamu vendeg\n", vendegek);
		}
		
		else if(n == 2)
		{
			printf("\nA regisztralt felhasznalok:***********************************\n\n");
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
		
		else if(n == 3)
		{
			while(getchar()!='\n');
			printf("\nMelyik felhasznalot szeretned modositani?\n");
			char nev[30];
			fgets(nev, 30, stdin);
			int new = strlen(nev) -1;
			if (nev[new] == '\n')
				nev[new] = '\0';
			
			FILE * p = fopen("data.txt", "r");
			FILE * f = fopen("data2.txt","w");
			
			if(p == NULL)
			{
				printf("Meg nincsenek felhasznalok!\n");
			}
			else
			{
				char c;
				char line[50];
				c = fgetc(p);
				while(!feof(p))
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
					
					if(strstr(line,nev) != NULL)
					{
						printf("Adja meg a nevet, emailcimet és a rendezveny azonositojat\nszokozzel elvalasztva:\n");
						char sor[50];
						fgets(sor, 50, stdin);
						
						char a[100];
						time_t now = time (0);
						strftime (a, 100, "%Y-%m-%d %H:%M:%S", localtime (&now));
						
						int new_line = strlen(sor) -1;
						if (sor[new_line] == '\n')
							sor[new_line] = '\0';
							
						fprintf(f, "[%s\t%s]", sor, a);
					}
					else
					{
						fprintf(f,"[%s]", line);
					}
				}
				fclose(p);
				fclose(f);
				
				remove("data.txt");
				rename("data2.txt", "data.txt");
			}
			
				
		}
		
		else if(n == 4)
		{
			while(getchar()!='\n');
			printf("\nMelyik felhasznalot szeretned torolni?\n");
			char nev[30];
			fgets(nev, 30, stdin);
			int new = strlen(nev) -1;
			if (nev[new] == '\n')
				nev[new] = '\0';
			
			FILE * p = fopen("data.txt", "r");
			FILE * f = fopen("data2.txt","w");
			
			if(p == NULL)
			{
				printf("Meg nincsenek felhasznalok!\n");
			}
			else
			{
				char c;
				char line[50];
				c = fgetc(p);
				while(!feof(p))
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
					
					if(strstr(line,nev) == NULL)
					{
						int new_line = strlen(line) -1;
						if (line[new_line] == '\n')
							line[new_line] = '\0';
						fprintf(f,"[%s]", line);
						//fputs(line, f);
					}
				}
				fclose(p);
				fclose(f);
				
				remove("data.txt");
				rename("data2.txt", "data.txt");	
			}
			
					
		}
		
		else if(n == 5)
		{
			while(getchar()!='\n');
			printf("Add meg a rendezveny adatait egy sorban!\n");
			char line[50];
			fgets(line, 50, stdin);
	
			FILE * p = fopen("dt.txt", "a");
			
			int new_line = strlen(line) -1;
			if (line[new_line] == '\n')
				line[new_line] = '\0';
				
			fprintf(p, "[%s]", line);
			fclose(p);
		}
		
		else if(n == 6)
		{
			while(getchar()!='\n');
			printf("\nMelyik rendezvenyt szeretned torolni?\n");
			char nev[30];
			fgets(nev, 30, stdin);
			int new = strlen(nev) -1;
			if (nev[new] == '\n')
				nev[new] = '\0';
			
			FILE * p = fopen("dt.txt", "r");
			FILE * f = fopen("dt2.txt","w");
			if(p == NULL)
			{
				printf("Meg nincsenek rendezvenyek!\n");
			}
			else
			{
				char c;
				char line[50];
				c = fgetc(p);
				while(!feof(p))
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
					
					if(strstr(line,nev) == NULL)
					{
						int new_line = strlen(line) -1;
						if (line[new_line] == '\n')
							line[new_line] = '\0';
						fprintf(f,"[%s]", line);
						//fputs(line, f);
					}
				}
				fclose(p);
				fclose(f);
				
				remove("dt.txt");
				rename("dt2.txt", "dt.txt");
			}
		}
		
		else if(n == 7)
		{
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
		
}	
}
