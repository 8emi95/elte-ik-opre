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
#include <pwd.h> //megn�zni, mystathoz kapcsol�dik

//FORD�T�S: gcc gyak2.c -o gyak2

/*//fileolvas� f�ggv�ny
void readFile(char* fileName, int mod, int* file)
{
	*file = open(fileName, mod, 0600);
	
	if (errno != 0)
	{
		perror("hiba");
		exit(1);
	}
}
*/

//k�nyvt�r olvas�, rekurz�v
void listaz(char* n)
{
	chdir(n);
	DIR* d;
	struct dirent* p_dirent;
	struct stat mystat;
	
	d = opendir(".");
	while (p_dirent = readdir(d))
	{
		stat(p_dirent->d_name, &mystat); //beolvas statba
		
		printf("%s ", p_dirent->d_name);
		printf("%s\n", ctime(&mystat.st_mtime));
		if (S_ISDIR(mystat.st_mode))
		{
			if (strcmp(p_dirent->d_name, "..") && strcmp(p_dirent->d_name, ".") )
			{
				listaz(p_dirent->d_name);
			}
		}
	}
	
	get
	closedir(d);
	chdir("..");
}


/*//file olvas�s �s �tm�sol�s m�sik f�jlba
int main()
{
	int f, g;
	int *p_f = &f;
	char c;
	
	char* fileName = (char*)malloc(80*sizeof(char));
	
	printf("%s", "Add meg a file nev�t: ");
	scanf("%s", fileName);
	
	readFile(fileName, O_RDONLY, p_f);
	readFile("kimenet.txt", O_RDONLY | O_WRONLY | O_CREAT, &g);
	
	while (read(f, &c, 1))
	{
		write(g, &c, 1);
	}
	
	close(f);
	close(g);
	return 0;
}
*/

//k�nyvt�r olvas�s
int main()
{
	listaz(".");
	
	return 0;
}


