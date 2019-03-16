#include <stdio.h>
#include <sys/types.h>
#include <time.h>

void feladat1()
{
	pid_t child1 = fork();
	if (child1 < 0)
		perror("Creating child1");
	else if (child1 == 0)
	{
		printf("(C1) child1 pid: %d\n", getpid());
		printf("(C1) parent pid: %d\n", getppid());
	}
	else
	{
		pid_t child2 = fork();
		if (child2 < 0)
			perror("Creating child2");
		else if (child2 == 0)
		{
			printf("(C2) child2 pid: %d\n", getpid());
			printf("(C2) parent pid: %d\n", getppid());
		}
		else
		{
			printf("(P) child1 pid: %d\n", child1);
			printf("(P) child2 pid: %d\n", child2);
		}
	}
}

void feladat2()
{
	pid_t child1 = fork();
	if (child1 < 0)
		perror("Creating child1");
	else if (child1 == 0)
	{
		pid_t child2 = fork();
		if (child2 < 0)
			perror("Creating child2");
		else if (child2 == 0)
		{
			printf("(C2) child2 pid: %d\n", getpid());
			printf("(C2) parent pid: %d\n", getppid());
		}
		else
		{
			printf("(C1) child1 pid: %d\n", getpid());
			printf("(C1) parent pid: %d\n", getppid());
		}
	}
	else
	{
		printf("(P) child1 pid: %d\n", child1);
	}
}

void feladat3()
{
	printf("%d\n", getpid());
	execv("./pid", NULL);
}

void feladat4()
{
	printf("%d\n", getpid());
	system("./pid");
	system("ls -l");
}

void feladat5()
{
	time_t t = time(NULL);
	struct tm *ltime = localtime(&t);
	
	char *wd;
	switch (ltime->tm_wday)
	{
		case 0:
			wd = "Vasarnap";
			break;
		case 1:
			wd = "Hetfo";
			break;
		case 2:
			wd = "Kedd";
			break;
		case 3:
			wd = "Szerda";
			break;
		case 4:
			wd = "Csutortok";
			break;
		case 5:
			wd = "Pentek";
			break;
		case 6:
			wd = "Szombat";
			break;
	}
	
	printf(
		"%4d.%2.2d.%2.2d %s %2.2d:%2.2d:%2.2d %s idoszamitas - Az ev %d. napja.\n",
		1900 + ltime->tm_year,
		ltime->tm_mon,
		ltime->tm_mday,
		wd,
		ltime->tm_hour,
		ltime->tm_min,
		ltime->tm_sec,
		(ltime->tm_isdst ? "Nyari" : "Teli"),
		1 + ltime->tm_yday
	);
}

void feladat7()
{
	pid_t child1 = fork();
	if (child1 < 0)
		perror("Creating child1");
	else if (child1 == 0)
	{
		int i;
		for (i = 0; i < 50; i++)
			printf("%d %d\n", getpid(), rand() % 100 + 1);
	}
	else
	{
		pid_t child2 = fork();
		if (child2 < 0)
			perror("Creating child2");
		else if (child2 == 0)
		{
			int i;
			for (i = 0; i < 50; i++)
				printf("%d %d\n", getpid(), rand() % 100 + 1);
		}
		else
		{
			int status;
			waitpid(child1, &status, 0);
			waitpid(child2, &status, 0);
		}
	}
}

void feladat8()
{
	char input[20];
	do {
		printf("shell> ");
		fgets(input, sizeof(input), stdin);
		
		system(input);
	} while (strcmp(input, "exit\n"));
}


int main(int argc, char **argv)
{
	feladat8();
}