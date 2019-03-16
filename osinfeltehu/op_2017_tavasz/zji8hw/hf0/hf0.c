#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h> 
#include <grp.h>
#include <time.h>

#define N 20

void strmode(mode_t mode, char *p)
{
	// frissen a BSD forráskódból csórva
	
	switch (mode & S_IFMT) {
		case S_IFDIR:			/* directory */
			*p++ = 'd';
			break;
		case S_IFCHR:			/* character special */
			*p++ = 'c';
			break;
		case S_IFBLK:			/* block special */
			*p++ = 'b';
			break;
		case S_IFLNK:			/* symbolic link */
			*p++ = 'l';
			break;
		case S_IFSOCK:			/* socket */
			*p++ = 's';
			break;
		case S_IFIFO:			/* fifo */
			*p++ = 'p';
			break;
		default:				/* unknown */
			*p++ = '-';
			break;
	}
		
	/* usr */
	if (mode & S_IRUSR)
		*p++ = 'r';
	else
		*p++ = '-';
	if (mode & S_IWUSR)
		*p++ = 'w';
	else
		*p++ = '-';
	
	switch (mode & (S_IXUSR | S_ISUID)) {
		case 0:
			*p++ = '-';
			break;
		case S_IXUSR:
			*p++ = 'x';
			break;
		case S_ISUID:
			*p++ = 'S';
			break;
		case S_IXUSR | S_ISUID:
			*p++ = 's';
			break;
	}
		
	/* group */
	if (mode & S_IRGRP)
		*p++ = 'r';
	else
		*p++ = '-';
	if (mode & S_IWGRP)
		*p++ = 'w';
	else
		*p++ = '-';
	
	switch (mode & (S_IXGRP | S_ISGID)) {
		case 0:
			*p++ = '-';
			break;
		case S_IXGRP:
			*p++ = 'x';
			break;
		case S_ISGID:
			*p++ = 'S';
			break;
		case S_IXGRP | S_ISGID:
			*p++ = 's';
			break;
	}
		
	/* other */
	if (mode & S_IROTH)
		*p++ = 'r';
	else
		*p++ = '-';
	if (mode & S_IWOTH)
		*p++ = 'w';
	else
		*p++ = '-';
	
	switch (mode & (S_IXOTH | S_ISVTX)) {
		case 0:
			*p++ = '-';
			break;
		case S_IXOTH:
			*p++ = 'x';
			break;
		case S_ISVTX:
			*p++ = 'T';
			break;
		case S_IXOTH | S_ISVTX:
			*p++ = 't';
			break;
	}

	*p = '\0';
}

// 1. feladat
int length(char *str)
{
	int count = 0;
	while (*str++ != '\0')
		count++;
	return count;
}

// 2. feladat
int equal(char *str1, char *str2)
{
	while (*str1 == *str2)
	{
		if (*str1 == '\0')
			return 1;
		
		str1++;
		str2++;
	}
	
	return 0;
}

// 3. feladat
void lefttrim(char *str, char n)
{
	int l = length(str);
	if (n > l)
		n = l;
	
	if (n == l)
		*str = '\0';
	else
	{
		char *src = str + n;
		while (*src != '\0')
			*str++ = *src++;
	}
	
	int i;
	for (i = 0; i < l - n; i++)
		*str++ = '\0';
}

// 4. feladat
void swap(char *str, char c1, char c2)
{
	while (*str != '\0')
	{
		if (*str == c1)
			*str = c2;
		str++;
	}
}

// 5. feladat
int find(char *str, char *find)
{
	char *_find = find;
	int count = 1;
	int begin = -1;
	while (*str != '\0')
	{
		if (*_find != *str)
		{
			begin = -1;
			_find = find;
		}
		
		if (*_find == *str)
		{
			if (begin == -1)
				begin = count;
			
			_find++;
			
			if (*_find == '\0')
				return begin;
		}
		
		count++;
		str++;
	}
	
	return -1;
}

void feladat6(int argc, char **argv)
{
	int sum = 0, i;
	for (i = 1; i < argc; i++)
		sum += strlen(argv[i]);
	
	printf("(6) Bemeneti parameterek osszege: %d\n", sum);
}

void feladat7(int argc, char **argv)
{
	int i = 0;
	if (argc >= 3)
	{
		char *first = argv[1];
		int b = 1;
		for (i = 2; i < argc; i++)
		{
			if (strcmp(first, argv[i]) != 0)
			{
				b = 0;
				break;
			}
		}
		
		printf("(7) A parameterek %s\n", b ? "megegyeznek" : "nem egyeznek meg");
	}
	else
		printf("(7) Nincs legalabb 2 parameter\n");
	
}

void feladat8(int argc, char **argv)
{
	if (argc == 3)
	{
		char str[strlen(argv[1]) + strlen(argv[2]) + 1];
		strcpy(str, argv[1]);
		strcat(str, argv[2]);
		printf("(8) Osszefuzott string: \"%s\"\n", str);
	}
	else
		printf("(8) Nincs pontosan 2 parameter\n");
}

void feladat10(int argc, char **argv)
{
	FILE *f = fopen("feladat10.txt", "w");
	if (f == NULL)
	{
		perror("(10) Nem sikerult megnyitni a fajlt");
		exit(1);
	}
	
	fputs("Abel", f);
	fclose(f);
	
	printf("(10) Nev sikeresen fajlba irva\n");
}

void feladat11(int argc, char **argv)
{
	typedef struct
	{
		int x;
		int y;
	} point;
	
	point p;
	if (argc >= 3)	// 11. b feladat
	{
		p.x = atoi(argv[1]);
		p.y = atoi(argv[2]);
	}
	else
	{
		p.x = 5;
		p.y = 8;
	}
	
	FILE *f = fopen("feladat11.bin", "wb");
	if (f == NULL)
	{
		perror("(11) Nem sikerult irasra megnyitni a fajlt");
		exit(1);
	}
	
	fwrite(&p, sizeof(point), 1, f);
	fclose(f);
	
	f = fopen("feladat11.bin", "rb");
	if (f == NULL)
	{
		perror("(11) Nem sikerult olvasasra megnyitni a fajlt");
		exit(1);
	}
	
	point p2;
	fread(&p2, sizeof(point), 1, f);
	
	if (p.x == p2.x && p.y == p2.y)
		printf("(11) Ugyanazt kaptuk vissza, mint amit kiirtunk\n");
	else
		printf("(11) Nem ugyanazt kaptuk vissza, mint amit kiirtunk\n");
}

void feladat12()
{
	FILE *f = fopen("feladat12.txt", "r");
	if (f == NULL)
	{
		perror("(12) Nem sikerult megnyitni a fajlt");
		exit(1);
	}
	
	int count = 0;
	char ch = '\0';
	while (!feof(f))
	{
		ch = fgetc(f);
		if (ch == '\n')
			count++;
	}
	
	fseek(f, 0, SEEK_SET);
	
	char *lines[count];
	char line[512];
	int j = 0;
	int i;
	for (i = 0; i < count; i++)
	{
		if (fgets(line, sizeof(line), f))
		{
			lines[i] = malloc(sizeof(line));
			strcpy(lines[i], line);
		}
	}
	
	char *swap;
	for (i = 0; i < count - 1; i++)
	{
		for (j = 0; j < count - i - 1; j++)
		{
			if (strcmp(lines[j], lines[j + 1]) > 0)
			{
				swap = lines[j];
				lines[j] = lines[j + 1];
				lines[j + 1] = swap;
			}
		}
	}
	
	printf("(12) Fajl sorai rendezve:\n");
	for (i = 0; i < count; i++) {
		printf("%s", lines[i]);
		free(lines[i]);
	}
}

void feladat13()
{
	typedef struct
	{
		char chr;
		int num;
	} valami;
	
	valami tomb[N];
	int i;
	for (i = 0; i < N; i++)
	{
		valami *elem = &tomb[i];
		elem->chr = i + 'a';
		elem->num = i + 1;
	}
	
	FILE *f = fopen("feladat13.bin", "wb");
	if (f == NULL)
	{
		perror("(13) Nem sikerult irasra megnyitni a fajlt");
		exit(1);
	}
	
	fwrite(&tomb, sizeof(valami), N, f);
	fclose(f);
	
	f = fopen("feladat13.bin", "rb");
	if (f == NULL)
	{
		perror("(13) Nem sikerult olvasasra megnyitni a fajlt");
		exit(1);
	}
	
	valami tomb2[N];
	for (i = 0; i < N; i += 2)
		fread(tomb2 + N - i - 2, sizeof(valami), 2, f);
	
	printf("(13) ");
	
	for (i = 0; i < N; i++)
		printf("[%c, %d] ", tomb2[i].chr, tomb2[i].num);
	
	printf("\n");
}

void feladat14(int argc, char **argv)
{
	struct dirent *ep;
	struct stat stat;
	struct passwd *pw;
	struct group *g;
	struct tm *timeinfo;
	
	char *dir = argc > 1 ? argv[1] : ".";

	DIR *dp = opendir(dir);
	if (dp != NULL)
	{
		while (ep = readdir(dp))
		{
			char path[strlen(dir) + strlen(ep->d_name) + 2];
			strcpy(path, dir);
			strcat(path, "/");
			strcat(path, ep->d_name);

			int fd = open(path, O_RDONLY);
			fstat(fd, &stat);
			
			char mode[10];
			strmode(stat.st_mode, mode);
			
			pw = getpwuid(stat.st_uid);
			g = getgrgid(stat.st_gid);
			
			timeinfo = localtime(&stat.st_mtime);
			char time[13];
			strftime(time, 13, "%b %d %H:%M", timeinfo);
			
			printf("%s\t%d\t%s\t%s\t%d\t%s\t%s\n", mode, stat.st_nlink, pw->pw_name, g->gr_name, stat.st_size, time, ep->d_name);
			
			close(fd);
		}
		closedir(dp);
	}
	else
		perror("(14) Nem sikerult megnyitni a konyvtarat");
}

// 15. feladat
void listdir(char *str)
{
	struct dirent *ep;
	struct stat stat;
	
	DIR *dp = opendir(str);
	if (dp != NULL)
	{
		printf("\n\n%s:\n", str);
		
		while (ep = readdir(dp))
		{
			if (ep->d_name[0] == '.')
				continue;
			
			printf("%s ", ep->d_name);
		}
		
		rewinddir(dp);
		while (ep = readdir(dp))
		{
			if (ep->d_name[0] == '.')
				continue;
			
			char path[strlen(str) + strlen(ep->d_name) + 2];
			strcpy(path, str);
			strcat(path, "/");
			strcat(path, ep->d_name);
			
			int fd = open(path, O_RDONLY);
			fstat(fd, &stat);
			
			if ((stat.st_mode & S_IFMT) == S_IFDIR)
				listdir(path);
			
			close(fd);
		}
	}
}

int main(int argc, char **argv)
{
	listdir(argv[1]);
	return 0;
}