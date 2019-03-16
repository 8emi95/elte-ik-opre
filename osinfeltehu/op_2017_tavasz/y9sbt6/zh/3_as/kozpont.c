#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/msg.h>

#define READER (size_t)0
#define WRITER (size_t)1
#define NOTIF_TAXI (long)2
#define RESP_TAXI (long)3

struct message_data {
    pid_t tel;
	char address[100];
	char taxi_text[100];
};

struct uzenet { 
    long mtype; //ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
    struct message_data data; // ĂĽzenet tartalma: itt egy tĂ¶mb, de lehet struktĂşra vagy bĂˇrmi egyĂ©b is 
}; 

int kuld(int uzenetsor, struct uzenet* msg) 
{           
     int status;
     
        // a 3. param ilyen is lehet: sizeof(uz.mtext): ez az ĂĽzenet tartalmĂˇnak mĂ©rete
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos 
     status = msgsnd(uzenetsor, msg, sizeof(msg->data), 0); 
        
     if (status < 0)
     { 
          perror("msgsnd");
     }
     
     return status;
} 
     
// ĂĽzenet fogadĂˇsa 
int fogad(int uzenetsor, long uz_tipus, struct uzenet* msg)
{ 
     int status;
      
    // az utolso parameter(0) a fogadĂˇsra vonatkozĂł opciĂłkat hatĂˇrozza meg
	// ha a 4. paramĂ©ter 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, msg, sizeof(msg->data), uz_tipus, 0); 
     
     if (status < 0)
     { 
          perror("msgsnd"); 
     }
     
     return status; 
}

int main(int argc, char *argv[])
{
	pid_t child;
	int status;
	int addr_pipe[2];
	int uzenetsor; 
    key_t kulcs;
	
	if (argc < 2)
	{
		perror("Add meg parameterkent a cimet\n");
		return 1;
	}
	
	kulcs = ftok(argv[0], 1); 
    uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
	pipe(addr_pipe);
	
	child = fork();	// creating parent-child processes
	if (child == -1) 
	{
	   perror("Fork hiba");
	   return 1;
	}

	if (child > 0) // szülő
	{
		close(addr_pipe[WRITER]);
		char address[100];
		size_t i = 0;
		
		for (i = 0; i < sizeof(address); ++i)
		{
			address[i] = '\0';
		}
		
		printf("kozpont: varom az utast\n");
		read(addr_pipe[READER], address, sizeof(address));
		close(addr_pipe[READER]);
		printf("kozpont: az utas cime: %s\n", address);
		waitpid(child, &status, 0);
		
		pid_t taxi = fork();
		
		if (taxi < 0)
		{
			perror("fork hiba a taxisnal");
			return 1;
		}
		
		if (taxi > 0) // szulo
		{
			struct uzenet msg;
			msg.mtype = NOTIF_TAXI;
			msg.data.tel = taxi;
			strcpy(msg.data.address, address);
			kuld(uzenetsor, &msg);
			fogad(uzenetsor, RESP_TAXI, &msg);
			printf("%s\n", msg.data.taxi_text);
			waitpid(taxi, &status, 0);
			msgctl(uzenetsor, IPC_RMID, NULL);
		}
		else if (taxi == 0) // taxi
		{
			struct uzenet msg;
			fogad(uzenetsor, NOTIF_TAXI, &msg);
			printf("taxi: cim %s, telefon %i\n", msg.data.address, msg.data.tel);
			
			size_t i;
			for (i = 0; i < sizeof(msg.data.taxi_text); ++i)
			{
				msg.data.taxi_text[i] = '\0';
			}
			
			msg.mtype = RESP_TAXI;
			strcpy(msg.data.taxi_text, "taxi: elindultam az utasert");
			kuld(uzenetsor, &msg);
		}
	}
	else if (child == 0) // gyerek
	{
		close(addr_pipe[READER]);
		printf("utas: jelentkezek\n");
		write(addr_pipe[WRITER], argv[1], sizeof(argv[1]));
		close(addr_pipe[WRITER]);
	}
	
	return 0;
}

 
