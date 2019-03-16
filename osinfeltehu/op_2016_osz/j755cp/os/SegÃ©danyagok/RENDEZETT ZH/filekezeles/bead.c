#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h> 
 
#include "bead.h"

//a stringb�l a '\n' karaktert '\0'-ra cser�li
char* removeNewLine(char* str)
{
	if (str != NULL)
    {
		char *newline = strchr(str, '\n'); //visszaadja h hol van az str-ben a '\n', pointerk�nt
		if (newline != NULL)
		{
			*newline = '\0'; 
		}
    }
}

//beolvassa a f�jlt, lekezeli a hib�kat
void readFile(char* fileName, int mod, int* file)
{
	*file = open(fileName, mod, 0600);
	
	if (errno != 0)
	{
		perror("hiba");
		exit(1);
	}
}

//kit�rli a megadott f�jl megadott sor�t + a 3 ut�na l�v�t, tmp f�jl haszn�lat�val
void delete(char* fileName, int lineNumber)
{
	int dataFile; 
	readFile(fileName, O_RDONLY, &dataFile);
	int tmpFile; 
	readFile("tmpfile.txt", O_RDONLY | O_WRONLY | O_CREAT, &tmpFile);

	char c;
	int currentLine = 1; //els� sorr�l indulunk
	int lineToDelete = (lineNumber-1)*4 + 1;
	while (read(dataFile, &c, 1))
	{
		//mind a 4 sort ki szeretn�m t�r�lni
		if (!(currentLine == lineToDelete ||
			currentLine == lineToDelete + 1 ||
			currentLine == lineToDelete + 2 ||
			currentLine == lineToDelete + 3)) 
		{
			write(tmpFile, &c, 1); //egy tmp f�jlba m�solunk mindent azt a sort kiv�ve
		}
		/*
		else if (currentLine == lineNumber ||
			currentLine == lineNumber + 1 ||
			currentLine == lineNumber + 2 ||
			currentLine == lineNumber + 3)
		{
			printf("%c", c); //kiirjuk mit t�rl�nk ki
		}
		*/
		
		//ha �j sor karakter van, akkor a sorok sz�ma n�
		if (c == '\n')
		{
			currentLine++;
		}
	}
	close(dataFile);
	close(tmpFile);
	
	//kit�r�lj�k az �tirand� f�jlt, hogy �res f�jlal kezdj�nk
	if (remove(fileName) == -1)
		perror("Error in deleting a file");
		
	//visszam�soljuk a tmpb�l az eredetibe
	readFile(fileName, O_RDONLY | O_WRONLY | O_CREAT, &dataFile);
	readFile("tmpfile.txt", O_RDONLY, &tmpFile);
	
	while (read(tmpFile, &c, 1))
	{
		write(dataFile, &c, 1);
	}
	
	close(dataFile);
	close(tmpFile);
	
	//kit�r�lj�k a tmp f�jlt
	if (remove("tmpfile.txt") == -1)
		perror("Error in deleting a file");
}

//a stringk�nt megadott sz�mb�l megadja a hozz� tartoz� nevet (m�fajn�l m�fajt, szerz�n�l szerz�t)
char* convertToString(char* string, char* fileName)
{
	int f;
	readFile(fileName, O_RDONLY, &f);
	
	char tmp; 
	char* line = (char*)malloc(80*sizeof(char));
	char* prevLine = (char*)malloc(80*sizeof(char)); 
	int i = 0;
	
	while (read(f, &tmp, 1))
	{	
		if (tmp == '\n') //ha �jsort tal�l
		{	
			if (!strcmp(string, line)) //ha a line stimmel a stringel
			{
				return prevLine; //visszaadjuk az el�z� sort
			}
			strcpy(prevLine, line); //beletessz�k a prevlineba a line tartalm�t
			i = 0;
			line = (char*)malloc(80*sizeof(char));
		}
		else
		{
			line[i] = tmp;
			i++;
		}
	}
	close(f);
}

//kilist�zza a f�jl tartalm�t, ha logic == 0 akkor mindent kilist�z,
//egy�bk�nt a toFind felt�tel szerint, ha megegyezik az el�ad�val
void list(char* fileName, int logic, char* toFind)
{
	int dataFile;
	readFile(fileName, O_RDONLY, &dataFile);
	
	char c;
	char* line = (char*)malloc(80*sizeof(char)); //egy sor 80 karakter max
	int lineNumber = 1;
	int i = 0;
	int zenek = 1; //csak arra val�, hogy kitudjam �rni h�nyadik a sz�m a list�ban
	int l = logic; //logikai v�ltoz�, arra hogy ki kell e iratni
	int b = 0;
	while (read(dataFile, &c, 1))
	{
		if (l == 0)
		{
			line[i] = c;
			i++;
			if (c == '\n')
			{
				if (lineNumber % 4 == 1) //ha szerzot tal�l
				{
					removeNewLine(line);
					printf("%d. %s", zenek, convertToString(line, "authors.txt"));
					printf(" ");
				}
				else if (lineNumber % 4 == 0) //ha mufajt tal�l
				{
					removeNewLine(line);
					printf("%s", convertToString(line, "mufajok.txt"));
					printf("\n");
					zenek++; //k�vetkez� zene
				}
				else  //ha nem mufaj �s nem szerzo
				{
					removeNewLine(line);
					printf("%s", line); 
					printf(" ");
				}
				
				line = (char*)malloc(80*sizeof(char)); //linenak uj ter�letet kell foglalni
				i = 0;
				lineNumber++;
			}
		}
		else
		{
			
			line[i] = c;
			i++;
			if (c == '\n')
			{
				if (lineNumber % 4 == 1) //ha szerzot tal�l
				{
					removeNewLine(line);
					//printf("|%s || %s|", convertToString(line, "authors.txt"), toFind);
					if (!strcmp(convertToString(line, "authors.txt"), toFind))
					{
						printf("%d. %s", zenek, convertToString(line, "authors.txt"));
						printf(" ");
						b = 1;
					}
				}
				else if (lineNumber % 4 == 0 && b == 1) //ha mufajt tal�l
				{
					removeNewLine(line);
					printf("%s", convertToString(line, "mufajok.txt"));
					printf("\n");
					zenek++; //k�vetkez� zene
					b = 0;
				}
				else if (b == 1)//ha nem mufaj �s nem szerzo
				{
					removeNewLine(line);
					printf("%s", line); 
					printf(" ");
				}
				
				line = (char*)malloc(80*sizeof(char)); //linenak uj ter�letet kell foglalni
				i = 0;
				lineNumber++;
			}
		}
	}
	
	close(dataFile);
}

//egy music strukt�r�t tud ki�rni a konzolra
void printMusic(struct Music* m)
{
	printf("%s\t", m->author);
	printf("%s\t", m->title);
	printf("%s\t", m->length);
	printf("%s", m->genre);
}

//beolvassa a billentyuzetrol a zene adatait a param�terk�nt megadott struktur�ba
void insertMusic(struct Music* m) 
{
	char* tmp = (char*)malloc(50*sizeof(char));
	
	printf("%s", "Add meg a szerz� nev�t: ");
	m->author = (char*)malloc(80*sizeof(char));
	fgets(m->author, 80, stdin);
	removeNewLine(m->author);
	
	printf("%s", "Add meg a sz�m c�m�t: ");
	m->title = (char*)malloc(80*sizeof(char)); 
	fgets(m->title, 80, stdin);
	removeNewLine(m->title);
	
	printf("%s", "Add meg a sz�m hossz�t: ");
	m->length = (char*)malloc(80*sizeof(char)); 
	fgets(m->length, 80, stdin);
	removeNewLine(m->length);
	
	printf("%s", "Add meg a m�fajt: ");
	m->genre = (char*)malloc(80*sizeof(char)); 
	fgets(m->genre, 80, stdin);
	removeNewLine(m->genre);
}


//ha tal�lt ilyen mufajt vagy szerzot akkor visszaadja a sz�m�t, ha nem, akkor l�trehozza, rendel egy sz�mot hozz� �s azt adja vissza
int convertToNumber(char* string, char* fileName)
{
	int f;
	readFile(fileName, O_RDONLY, &f);
	
	char tmp; 
	char* line = (char*)malloc(80*sizeof(char)); 
	char* str = (char*)malloc(3*sizeof(char)); 
	int i = 0;
	int maxNumber = 0;
	
	while (read(f, &tmp, 1))
	{		
		if (tmp == '\n') //ha �jsort tal�l
		{
			maxNumber = atoi(line);
			if (!strcmp(string, line)) //ha a line stimmel a stringel
			{
				read(f, &tmp, 1); //beolvassuk a sz�m els� jegy�t
				i = 0;
				while (tmp != '\n')
				{
					str[i] = tmp;
					read(f, &tmp, 1); //beolvassuk a sz�m t�bbi jegy�t ha van (mindig a string ut�n van a fileban)
					i++;
				}
				return atoi(str); //visszaadjuk a sz�mot
			}
			i = 0;
		}
		else //am�g nincs �jsor
		{
			line[i] = tmp;
			i++;
		}
	}
	close(f);
	
	//ha nem tal�lta meg a stringet akkor eljut ide, ilyenkor kell l�trehozni
	
	readFile(fileName, O_RDONLY | O_WRONLY | O_CREAT | O_APPEND, &f); //f�jl �r�sra nyit�sa
	
	str = (char*)malloc(3*sizeof(char));
	
	write(f, string, strlen(string)); //bele�rjuk a mufaj nev�t
	write(f, "\n", 1);
	sprintf(str, "%d", maxNumber + 1); //hozz�rendelj�k a k�vetkez� intet
	write(f, str, strlen(str));  //bele�rjuk az intet
	write(f, "\n", 1);
	
	close(f);
	
	return atoi(str);
}


void writeToFile(struct Music m)
{
	//megnyitjuk a f�jlt appendre, �gy tudunk hozz�f�zni
	int dataFile;
	readFile("data.txt", O_WRONLY | O_CREAT | O_APPEND, &dataFile);
	
	char* str = (char*)malloc(3*sizeof(char));
	
	//szerzok
	sprintf(str, "%d", convertToNumber(m.author, "authors.txt"));
	write(dataFile , str, strlen(str));
	write(dataFile, "\n", 1);
	
	//cim
	write(dataFile, m.title, strlen(m.title));
	write(dataFile, "\n", 1);
	
	//hossz
	write(dataFile, m.length, strlen(m.length));
	write(dataFile, "\n", 1);
	
	//mufaj
	sprintf(str, "%d", convertToNumber(m.genre, "mufajok.txt"));
	write(dataFile , str, strlen(str));
	write(dataFile, "\n", 1);
	
	close(dataFile);	
}

//bek�r egy sz�mot a billentyuzetrol
int valasztas()
{
	char* tmp = (char*)malloc(50*sizeof(char));
	fgets(tmp, 50, stdin);
	removeNewLine(tmp);
	return atoi(tmp);
}

//MAIN
int main() 
{
	struct Music m1;
	
	int option = 0;
	int number = 0;
	
	while (option != 6) //ha 5 akkor v�ge a whilenak �s kil�p a program
	{
		printf("%s\n", "1-List�z�s, 2-Besz�r�s, 3-M�dos�t�s, 4-T�rl�s, 5-Keres�s, 6-Kil�p�s");
		printf("%s", "V�laszt�s: ");
		option = valasztas();
		
		if (1 == option) //list�z�s 
		{
			list("data.txt", 0, "");
		}
		
		else if (2 == option) //besz�r�s 
		{
			insertMusic(&m1);
			printf("%s", "Besz�rt sz�m adatai: ");
			printMusic(&m1);
			printf("%s\n", "");
			writeToFile(m1);
		}
		else if (3 == option) //m�dos�t�s
		{
			printf("\nHanyadik bejegyzest szeretned modositani: ");
			number = valasztas();
			insertMusic(&m1);
			writeToFile(m1);
			delete("data.txt", number);
		}
		else if (4 == option) //t�rl�s
		{	
			printf("\nHanyadik bejegyzest szeretned torolni: ");
			number = valasztas();
			delete("data.txt", number);
		}
		else if (5 == option) //keres�s mufajra
		{
			printf("%s", "Milyen eloadora keressunk: ");
			char* tmp = (char*)malloc(50*sizeof(char));
			fgets(tmp, 50, stdin);
			removeNewLine(tmp);
			list("data.txt", 1, tmp);
		}
	}

	return 0;
}