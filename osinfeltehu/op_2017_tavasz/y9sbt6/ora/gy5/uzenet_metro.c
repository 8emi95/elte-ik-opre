#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

// SYSTEM V-ös példa

struct message_data {
    int id;
    int train_num;
    char text[512];
};

struct uzenet { 
    long mtype; //ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
    struct message_data data; // üzenet tartalma: itt egy tömb, de lehet struktúra vagy bármi egyéb is 
}; 

// üzenet küldése
int kuld(int uzenetsor, struct uzenet* msg) 
{           
     int status;
     
        // a 3. param ilyen is lehet: sizeof(uz.mtext): ez az üzenet tartalmának mérete
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos 
     status = msgsnd(uzenetsor, msg, sizeof(msg->data), 0); 
        
     if (status < 0)
     { 
          perror("msgsnd");
     }
     
     return status;
} 
     
// üzenet fogadása 
int fogad(int uzenetsor, long uz_tipus, struct uzenet* msg)
{ 
     int status;
      
    // az utolso parameter(0) a fogadásra vonatkozó opciókat határozza meg
	// ha a 4. paraméter 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, msg, sizeof(msg->data), uz_tipus, 0); 
     
     if (status < 0)
     { 
          perror("msgsnd"); 
     }
     
     return status; 
} 

void train_arrived(int signumber)
{
    
}

int main (int argc, char* argv[])
{ 
    if (argc < 2)
    {
        perror("Masodik parameternek adj meg egy szamot!");
        return 1;
    }
    
    srand(getpid()); // véletlenszám generálás inicializálása
    pid_t child; 
    int uzenetsor, status; 
    key_t kulcs; 
    
    //signal(SIGUSR1, <jelzéskezelő fv>);
    
    // msgget needs a key, amelyet az ftok generál 
    //. 
    kulcs = ftok(argv[0], 1); 
    printf("A kulcs: %d\n", kulcs);
    uzenetsor = msgget(kulcs, 0600 | IPC_CREAT); 

    if (uzenetsor < 0)
    { 
        perror("msgget"); 
        return 1; 
    } 

    child = fork(); 
    if (child > 0) // szülő folyamat
    {         
        // szülő üzenetet küld
        struct uzenet msg;        
        kuld(uzenetsor, &msg);
        waitpid(child, &status, 0);

        // a gyerekfolyamat terminálását követően töröljük az üzenetsort 
        status = msgctl(uzenetsor, IPC_RMID, NULL); 
        if (status < 0)
        { 
           perror("msgctl"); 
        }
    }
    else if (child == 0) // gyerek folyamat
    {
        // a gyerek fogad egy üzenetet
        struct uzenet msg;
        //fogad(uzenetsor, <uzenet tipusa>, &msg);
    }
    else
    { 
        perror("fork"); 
        return 1; 
    } 

    return 0; 
} 
