#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

struct uzenet { 
	long mtype;
	char mtext [ 1024 ]; 
}; 

// �zenetet k�ld az �zenetsorba. 
int kuld( int uzenetsor ) { 

	const struct uzenet uz = { 8, "Hello!" }; 
	int status; 
	status = msgsnd( uzenetsor, & uz, strlen ( uz.mtext ) + 1 , 0 ); 
	if ( status < 0 )
		perror("msgsnd"); 
	return 0; 
} 
	// �zenetet fogad az �zenetsorb�l. 
int fogad( int uzenetsor ) { 

	struct uzenet uz; 
	int status; 
	
	status = msgrcv( uzenetsor, & uz, 1024, 0, 0 ); 
	
	if ( status < 0 ) 
		perror("msgsnd"); 
	else 
		printf( "A kapott uzenet: %ld, %s\n", uz.mtype, uz.mtext ); 
	
	return 0; 
} 

int main (int argc,char** argv) { 

	pid_t child; 
	int uzenetsor, status; 
	key_t kulcs; // L�trehoz�skor adni kell egy kulcsot, amelyet az ftok 
	// f�ggv�nnyel gener�lhatunk. 
	kulcs = ftok(argv[0],1); 
	uzenetsor = msgget( kulcs, 0700 | IPC_CREAT ); //(azonos�t�, jogosults�g (nekem teljes a msg felett)) 
	
	if ( uzenetsor < 0 ) { 
		perror("msgget"); 
		return 1; 
	} 
	
	child = fork(); 
	
	if ( child > 0 ) { 
		kuld( uzenetsor ); // A sz�l�folyamat k�ldi az �zenetet. 
		wait( NULL );
		// Miut�n a gyerek v�get �rt, t�rli az �zenetsort. 
		status = msgctl( uzenetsor, IPC_RMID, NULL ); //kiny�rja az �zenetsort �gy hogy b�rmilyen �r�-olvas� r�sz van a programban az teljesen emgsz�nik
	
	if ( status < 0 ) 
		perror("msgctl"); 
		return 0; 
	} 
	else if ( child == 0 ) { 
		return fogad( uzenetsor ); 
		
	// A gyerekfolyamat fogadja az �zenetet. 
	} 
	else { 
		perror("fork"); 
		return 1; 
	} 
		
	return 0; 
}