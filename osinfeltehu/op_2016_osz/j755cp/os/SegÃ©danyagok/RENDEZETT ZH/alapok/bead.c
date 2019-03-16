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

//a stringbõl a '\n' karaktert '\0'-ra cseréli
char* removeNewLine(char* str)
{
	if (str != NULL)
    {
		char *newline = strchr(str, '\n'); //visszaadja h hol van az str-ben a '\n', pointerként
		if (newline != NULL)
		{
			*newline = '\0'; 
		}
    }
}

//beolvassa a fájlt, lekezeli a hibákat
void readFile(char* fileName, int mod, int* file)
{
	*file = open(fileName, mod, 0600);
	
	if (errno != 0)
	{
		perror("hiba");
		exit(1);
	}
}

//kitörli a megadott fájl megadott sorát + a 3 utána lévõt, tmp fájl használatával
void delete(char* fileName, int lineNumber)
{
	int dataFile; 
	readFile(fileName, O_RDONLY, &dataFile);
	int tmpFile; 
	readFile("tmpfile.txt", O_RDONLY | O_WRONLY | O_CREAT, &tmpFile);

	char c;
	int currentLine = 1; //elsõ sorról indulunk
	int lineToDelete = (lineNumber-1)*4 + 1;
	while (read(dataFile, &c, 1))
	{
		//mind a 4 sort ki szeretném törölni
		if (!(currentLine == lineToDelete ||
			currentLine == lineToDelete + 1 ||
			currentLine == lineToDelete + 2 ||
			currentLine == lineToDelete + 3)) 
		{
			write(tmpFile, &c, 1); //egy tmp fájlba másolunk mindent azt a sort kivéve
		}
		/*
		else if (currentLine == lineNumber ||
			currentLine == lineNumber + 1 ||
			currentLine == lineNumber + 2 ||
			currentLine == lineNumber + 3)
		{
			printf("%c", c); //kiirjuk mit törlünk ki
		}
		*/
		
		//ha új sor karakter van, akkor a sorok száma nõ
		if (c == '\n')
		{
			currentLine++;
		}
	}
	close(dataFile);
	close(tmpFile);
	
	//kitöröljük az átirandó fájlt, hogy üres fájlal kezdjünk
	if (remove(fileName) == -1)
		perror("Error in deleting a file");
		
	//visszamásoljuk a tmpbõl az eredetibe
	readFile(fileName, O_RDONLY | O_WRONLY | O_CREAT, &dataFile);
	readFile("tmpfile.txt", O_RDONLY, &tmpFile);
	
	while (read(tmpFile, &c, 1))
	{
		write(dataFile, &c, 1);
	}
	
	close(dataFile);
	close(tmpFile);
	
	//kitöröljük a tmp fájlt
	if (remove("tmpfile.txt") == -1)
		perror("Error in deleting a file");
}

//a stringként megadott számból megadja a hozzá tartozó nevet (mûfajnál mûfajt, szerzõnél szerzõt)
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
		if (tmp == '\n') //ha újsort talál
		{	
			if (!strcmp(string, line)) //ha a line stimmel a stringel
			{
				return prevLine; //visszaadjuk az elõzõ sort
			}
			strcpy(prevLine, line); //beletesszük a prevlineba a line tartalmát
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

//kilistázza a fájl tartalmát, ha logic == 0 akkor mindent kilistáz,
//egyébként a toFind feltétel szerint, ha megegyezik az elõadóval
void list(char* fileName, int logic, char* toFind)
{
	int dataFile;
	readFile(fileName, O_RDONLY, &dataFile);
	
	char c;
	char* line = (char*)malloc(80*sizeof(char)); //egy sor 80 karakter max
	int lineNumber = 1;
	int i = 0;
	int zenek = 1; //csak arra való, hogy kitudjam írni hányadik a szám a listában
	int l = logic; //logikai változó, arra hogy ki kell e iratni
	int b = 0;
	while (read(dataFile, &c, 1))
	{
		if (l == 0)
		{
			line[i] = c;
			i++;
			if (c == '\n')
			{
				if (lineNumber % 4 == 1) //ha szerzot talál
				{
					removeNewLine(line);
					printf("%d. %s", zenek, convertToString(line, "authors.txt"));
					printf(" ");
				}
				else if (lineNumber % 4 == 0) //ha mufajt talál
				{
					removeNewLine(line);
					printf("%s", convertToString(line, "mufajok.txt"));
					printf("\n");
					zenek++; //következõ zene
				}
				else  //ha nem mufaj és nem szerzo
				{
					removeNewLine(line);
					printf("%s", line); 
					printf(" ");
				}
				
				line = (char*)malloc(80*sizeof(char)); //linenak uj területet kell foglalni
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
				if (lineNumber % 4 == 1) //ha szerzot talál
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
				else if (lineNumber % 4 == 0 && b == 1) //ha mufajt talál
				{
					removeNewLine(line);
					printf("%s", convertToString(line, "mufajok.txt"));
					printf("\n");
					zenek++; //következõ zene
					b = 0;
				}
				else if (b == 1)//ha nem mufaj és nem szerzo
				{
					removeNewLine(line);
					printf("%s", line); 
					printf(" ");
				}
				
				line = (char*)malloc(80*sizeof(char)); //linenak uj területet kell foglalni
				i = 0;
				lineNumber++;
			}
		}
	}
	
	close(dataFile);
}

//egy music struktúrát tud kiírni a konzolra
void printMusic(struct Music* m)
{
	printf("%s\t", m->author);
	printf("%s\t", m->title);
	printf("%s\t", m->length);
	printf("%s", m->genre);
}

//beolvassa a billentyuzetrol a zene adatait a paraméterként megadott strukturába
void insertMusic(struct Music* m) 
{
	char* tmp = (char*)malloc(50*sizeof(char));
	
	printf("%s", "Add meg a szerzõ nevét: ");
	m->author = (char*)malloc(80*sizeof(char));
	fgets(m->author, 80, stdin);
	removeNewLine(m->author);
	
	printf("%s", "Add meg a szám címét: ");
	m->title = (char*)malloc(80*sizeof(char)); 
	fgets(m->title, 80, stdin);
	removeNewLine(m->title);
	
	printf("%s", "Add meg a szám hosszát: ");
	m->length = (char*)malloc(80*sizeof(char)); 
	fgets(m->length, 80, stdin);
	removeNewLine(m->length);
	
	printf("%s", "Add meg a mûfajt: ");
	m->genre = (char*)malloc(80*sizeof(char)); 
	fgets(m->genre, 80, stdin);
	removeNewLine(m->genre);
}


//ha talált ilyen mufajt vagy szerzot akkor visszaadja a számát, ha nem, akkor létrehozza, rendel egy számot hozzá és azt adja vissza
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
		if (tmp == '\n') //ha újsort talál
		{
			maxNumber = atoi(line);
			if (!strcmp(string, line)) //ha a line stimmel a stringel
			{
				read(f, &tmp, 1); //beolvassuk a szám elsõ jegyét
				i = 0;
				while (tmp != '\n')
				{
					str[i] = tmp;
					read(f, &tmp, 1); //beolvassuk a szám többi jegyét ha van (mindig a string után van a fileban)
					i++;
				}
				return atoi(str); //visszaadjuk a számot
			}
			i = 0;
		}
		else //amíg nincs újsor
		{
			line[i] = tmp;
			i++;
		}
	}
	close(f);
	
	//ha nem találta meg a stringet akkor eljut ide, ilyenkor kell létrehozni
	
	readFile(fileName, O_RDONLY | O_WRONLY | O_CREAT | O_APPEND, &f); //fájl írásra nyitása
	
	str = (char*)malloc(3*sizeof(char));
	
	write(f, string, strlen(string)); //beleírjuk a mufaj nevét
	write(f, "\n", 1);
	sprintf(str, "%d", maxNumber + 1); //hozzárendeljük a következõ intet
	write(f, str, strlen(str));  //beleírjuk az intet
	write(f, "\n", 1);
	
	close(f);
	
	return atoi(str);
}


void writeToFile(struct Music m)
{
	//megnyitjuk a fájlt appendre, így tudunk hozzáfûzni
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

//bekér egy számot a billentyuzetrol
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
	
	while (option != 6) //ha 5 akkor vége a whilenak és kilép a program
	{
		printf("%s\n", "1-Listázás, 2-Beszúrás, 3-Módosítás, 4-Törlés, 5-Keresés, 6-Kilépés");
		printf("%s", "Választás: ");
		option = valasztas();
		
		if (1 == option) //listázás 
		{
			list("data.txt", 0, "");
		}
		
		else if (2 == option) //beszúrás 
		{
			insertMusic(&m1);
			printf("%s", "Beszúrt szám adatai: ");
			printMusic(&m1);
			printf("%s\n", "");
			writeToFile(m1);
		}
		else if (3 == option) //módosítás
		{
			printf("\nHanyadik bejegyzest szeretned modositani: ");
			number = valasztas();
			insertMusic(&m1);
			writeToFile(m1);
			delete("data.txt", number);
		}
		else if (4 == option) //törlés
		{	
			printf("\nHanyadik bejegyzest szeretned torolni: ");
			number = valasztas();
			delete("data.txt", number);
		}
		else if (5 == option) //keresés mufajra
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