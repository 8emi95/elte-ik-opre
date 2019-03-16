#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>

typedef enum { false, true } bool;
// include <stdbool.h>


void s_handler(int signal)
{
    if (signal == SIGUSR1)
    {
        printf("SIGNAL> Received SIGUSR1!\n");
    }
	else if (signal == SIGUSR2)
	{
		printf("SIGNAL> Received SIGUSR2!\n");
	}
	else 
	{
		printf("SIGNAL> %d", signal);
	}
	
}

int random_int(int max)
{
	srand (time(NULL));
	return rand() % max;
}
bool is_id_exists(const int* id_array, int array_size, int checkthis_id)
{
	int index = 0;
	for(index; index<array_size; index++)
	{
		if(id_array[index] == checkthis_id)
		{
			return true;
		}
	}
	return false;
}

int generate_id(const int* id_array, int array_size)
{
	int random_id = random_int(array_size*10);
	while (is_id_exists(id_array, array_size, random_id))
	{
		random_id = random_int(array_size*10);
	}
	return random_id;
}

int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		printf("Nem kaptam meg a szavazok szamat argumentumban!\n");
		exit(1);
	}
	
	int vote_num = (int)(argv[1][0] - '0');
	printf("Szavazok szama: %d\n", vote_num);
	
	int pipe_1[2];
	int pipe_2[2];
	
	if (pipe(pipe_1) == -1)
    {
      printf("ERROR> Nem lehet létrehozni az 1. csövet\n");
      exit(1);
    }
	if (pipe(pipe_2) == -1)
    {
      printf("ERROR> Nem lehet létrehozni a 2. csövet\n");
      exit(1);
    }
	
	pid_t child_1 = fork();
	if (child_1 < 0)
	{
		printf("Nem szuletett meg az 1. gyerek!\n");
		exit(1);
	}
	if (child_1>0) // parent
	{
		
		int mypid = getpid();
		int ppid = getppid();
		
		printf("PARENT> mypid: %d, parentpid: %d\n", mypid, ppid);
		sleep(1);
	
		// megvarjuk h megerkezzen 1_child
		signal(SIGUSR1, s_handler);
		pause();
		signal(SIGUSR1, SIG_DFL);
		printf("PARENT> SIGNAL erkezett: {CHILD_1} kuldte\n");
		sleep(1);
		
		
		pid_t child_2 = fork();
		if(child_2 == 0) // child2
		{
			int mypid = getpid();
			int ppid = getppid();
			printf("CHILD2> mypid: %d, parentpid: %d\n", mypid, ppid);
			sleep(1);
			printf("CHILD1> SIGNALT kuldok, hogy keszenallok.\n");
			sleep(1);
			kill(ppid, SIGUSR2);
			
			// megvarom az 1. gyereket
			// int status;
			// waitpid(child_1, &status,0);
		}
		
		// megvarjuk h megerkezzen 2_child
		signal(SIGUSR2, s_handler);
		pause();
		signal(SIGUSR2, SIG_DFL);
		printf("PARENT> SIGNAL erkezett: {CHILD_2} kuldte\n");
		sleep(1);
		
		
		// generate n id
		int id_array[vote_num];
		int index = 0;
		for(index; index<vote_num; ++index)
		{
			id_array[index] = generate_id(id_array,vote_num);
		}
		// debug kiir idket
		/*printf("PARENT> Generalt ID-k:\n");
		index = 0;
		for(index; index<vote_num; ++index)
		{
			printf("PARENT> %d\n",id_array[index]);
		}*/
		// id-k küldese
		printf("PARENT> Elkuldom a valasztok szamat {CHILD_1}-nek\n");
		write(pipe_1[1], &vote_num, sizeof(vote_num));
		sleep(1);
		
		index = 0;
		printf("PARENT> Elkuldom a valasztok id-jeit {CHILD_1}-nek\n");
		sleep(1);
		for(index;index<vote_num;index++)
		{
			write(pipe_1[1], &id_array[index], sizeof(id_array[0]));
		}
		
		int status;
		//waitpid(child_2,&status,0);
		waitpid(child_1,&status,0);
	}
	else // child1
	{
		int mypid = getpid();
		int ppid = getppid();
		
		printf("CHILD1> mypid: %d, parentpid: %d\n", mypid, ppid);
		sleep(1);
		printf("CHILD1> SIGNALT kuldtem, hogy keszenallok.\n");
		kill(ppid, SIGUSR1);
		sleep(1);
		int vote_num;
		printf("CHILD1> Fogadom a valasztok szamat.\n");
		read(pipe_1[0], &vote_num, sizeof(vote_num));
		printf("CHILD1> Valasztok szama: %d\n", vote_num);
		sleep(1);
		int got_ids[vote_num]; 
		printf("CHILD1> Fogadom a valasztok adatait\n");
		sleep(1);
		int index = 0;
		for (index; index<vote_num;index++)
		{
			read(pipe_1[0], &got_ids[index], sizeof(got_ids[index]));
		}
		
		printf("CHILD1> Fogadott a valasztok adatai:\n");
		sleep(1);
		index = 0;
		for (index; index<vote_num;index++)
		{
			printf("CHILD> id: %d\n", got_ids[index]);
		}
		
		
	}

}
