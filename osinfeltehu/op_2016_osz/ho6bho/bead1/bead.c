#include <stdio.h>
#include <stdlib.h>

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
		
		int n;
		scanf("%d", &n);
		if( (n < 1) || (n > 4) )
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
			
			int new_line = strlen(line) -1;
			if (line[new_line] == '\n')
				line[new_line] = '\0';
				
			fprintf(p, "%-50.50s\n", line);
			fclose(p);
			printf("\nGratulalunk! On a(z) %d szamu vendeg\n", vendegek);
		}
		
		else if(n == 2)
		{
			printf("\nA regisztralt felhasznalok:***********************************\n\n");
			FILE * p = fopen("data.txt", "r");
			char line[50];
			while(!feof(p))
			{
				fgets(line, 50, p);
				printf("%s \n", line);
			}
			fclose(p);
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
			
			char line[50];
			while(!feof(p))
			{
				fgets(line, 50, p);
				
				
				if(strstr(line,nev) != NULL)
				{
					printf("Adja meg a nevet, emailcimet és a rendezveny azonositojat\nszokozzel elvalasztva:\n");
					char sor[50];
					fgets(sor, 50, stdin);
					
					int new_line = strlen(sor) -1;
					if (sor[new_line] == '\n')
						sor[new_line] = '\0';
						
					fprintf(f, "%-50.50s", sor);
				}
				else
				{
					fprintf(f,"%-s", line);
				}
			}
			fclose(p);
			fclose(f);
			
			remove("data.txt");
			rename("data2.txt", "data.txt");	
		}
		
		else if(n == 4)
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
			
			char line[50];
			while(!feof(p))
			{
				fgets(line, 50, p);
				
				if(strstr(line,nev) == NULL)
				{
					int new_line = strlen(line) -1;
					if (line[new_line] == '\n')
						line[new_line] = '\0';
					fprintf(f,"%s\n", line);
					//fputs(line, f);
				}
			}
			fclose(p);
			fclose(f);
			
			remove("data.txt");
			rename("data2.txt", "data.txt");			
		}
		
}	
}

