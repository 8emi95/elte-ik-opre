/*
This program fulfills the following task. 
Created by Norbert Luksa (YB5TZW), 2017.05.
norbert.luksa@gmail.com

TASK:
A Magyar Labdajátékok Szervezete (MLSZ) nemzeti konzultációt hirdetett a labdajátékok felvirágoztatására!
Első lépésként megalkotjuk azt a kérdéssort és hozzá kérdésenként azt a minimum kettő, 
maximum 4 választási lehetőséget amelyet majd a labdajátékok jobbítása iránt felelősséget érzők kitölthetnek. 
Miután véglegesítettük a kérdéseket az MLSZ (szülő) kérdezőbiztost(gyerek) küld a paraméterként megadott 
labdajáték centrumba, városba.
Az MLSZ véletlenszerűen kiválaszt 3 kérdést, amit csővezetéken kiküld a kérdezőbiztosnak azután, 
hogy az bejelentkezett az MLSZ-hez, hogy készen áll a feladat végrehajtására.
A kérdezőbiztos véletlenszerűen választ 10 és 20 közti konzultálni kívánó polgárt, 
akik kitöltik a kérdőívet (véletlenszerűen). Az eredményt visszaküldi csővezetéken az MLSZ-be, 
majd a kérdezőbiztos befejezi a konzultációt. Az MLSZ fogadja az eredményt és a képernyőre írja a 
konzultáció eredményét.

Készítsünk C programot, amellyel a leírt konzultáció elvégezhető.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h> 		//open,creat
#include <sys/types.h> 	//open
#include <errno.h> 		//perror, errno
#include <unistd.h> 	// for pipe()
#include <signal.h>

//The structure of the questions
struct question{
	int 	id; 			//the identifier of a question, can change every runtime
	char 	question[200];	//questions list
	char* 	answers[4];		//answers list
	int 	answers_c;		//number of answers
	char	date[30]; 		//date of adding the question
	int  	final; 			//boolean value, defines wether it's a final question or not
	int		deleted;		//boolean value, 1 if it has been deleted, will be actually deleted on exit
};

//loads all finalized questions from database, returning the number of them
int load_final_questions(struct question all_fq[100]){
	int c;
	int fc = 0; //finalized question counter
	FILE *file;
	file = fopen("database.txt", "r");
	int all_q_c = 0;
	if (file) {
		//Getting number of questions
		fscanf (file, "%d", &all_q_c);
		if(all_q_c == EOF) {all_q_c = 0;}
		int y;
		//getting all questions
		for(y = 0; y < all_q_c; ++y){
			struct question q;
			int 			id;
			char* 			answers[4];
			int 			answers_c;
			int  			final;
			int 			len;
			fscanf (file, "%d", &id); 	//gets id
			fscanf (file, "%d", &len); 	//gets length of the next question
			
			//reading question
			int x = -1;
			while ((c = getc(file)) != EOF && x < len){
				if(x != -1){ //skipping last line
					q.question[x] = c;
				}
				x++;
			}
			q.question[x] = 0;
			
			//gets the number of answers
			fscanf (file, "%d", &answers_c); 
			
			//gets all answers
			int i;
			for(i = 0; i < answers_c; ++i){
				fscanf (file, "%d", &len); //gets length of next answer
				
				//reading answer
				x = -1;
				q.answers[i] = (char*)malloc(len +1);
				while ((c = getc(file)) != EOF && x < len){
					if(x != -1){ //skipping last line
						q.answers[i][x] = c;
					}
					x++;
				}
				q.answers[i][x] = 0;
			}
			
			//reading date
			x = 0;
			while ((c = getc(file)) != EOF && x < 20){
				q.date[x] = c;
				x++;
			}	
			q.date[x] = 0;
			
			//reading wether a question is final
			fscanf (file, "%d", &final); 
			
			q.id 		= id;
			q.answers_c = answers_c;
			q.final 	= final;
			q.deleted	= 0;
			
			//only adding the finalized questions
			if(final){
				all_fq[fc] = q;
				fc++;
			}
		}
		
		fclose(file);
	}
	
	return fc;
}

//lists all questions
void write_result(struct question all_q[3], int all_q_c, int result[3][4]){
	int i;
	for(i = 0; i < all_q_c; ++i){
		if(!all_q[i].deleted){ //does not list deleted questions
			printf("ID: %d\n", all_q[i].id);
			printf("Question:\n%s\n", all_q[i].question);
			int j;
			for(j = 0; j < all_q[i].answers_c; ++j){
				printf("%d: %s\n", j+1, all_q[i].answers[j]);
			}
			printf("VOTE RESULT: ");
			for(j = 0; j < 4 && result[i][j] != -1; ++j){
				printf("%d: %d\t", j+1, result[i][j]);
			}			
			printf("\n###################################################\n");
		}
	}
}

//selects 3 questions randomly
void select_random(struct question selected[3], struct question all_fq[100], int all_fq_c){
	int sc = 0; //selected counter
	int selected_i[3];
	while (sc != 3){
		int i = rand()%all_fq_c;
		
		int j, f = 0; //f: 'found' is true if the index is already selected
		for(j = 0; j < sc; j++){
			if(selected_i[j] == i) f = 1;
		}
		if(!f) {
			selected_i[sc] = i;
			selected[sc] = all_fq[i];
			sc++;
		}
	}
}

void vote(struct question selected[3], int result[3][4]){
	int voterc = rand()%11 + 10; //getting the number of voters (between 10 and 20)
	
	//result array init
	int i, j;
	for(i = 0; i < 3; i++){
		for(j = 0; j < 4; j++){
			if(j < selected[i].answers_c) result[i][j] = 0;
			else result[i][j] = -1;
		}
	}
	
	//voting
	for(i = 0; i < 3; i++){
		for(j = 0; j < voterc; j++){
			int r =  rand()%selected[i].answers_c;
			result[i][r]++;
		}
	}
}

//signal handler
void handler(int signumber){
  printf("Signal has arrived\n");
}

int main(int argc,char ** argv) {
	srand(time(NULL)); //setting up random seed
	
	if (argc < 2) { 
		perror("Error occured: you have to add an argument (the city) when starting the program"); 
		return -1;
	}
	
	struct question all_fq[100];			//container for all the questions
	int    all_fq_c = load_final_questions(all_fq);	//loads the questions from database and defines the number of questions
	if(all_fq_c < 3) { 
		perror("Error occured during loading finalized questions! Check if there are at least 3."); 
		return -1;
	}	
	//selecting 3 random questions and getting its result
	struct question selected[3];
	int result[3][4];
	int pipefd[2]; // unnamed pipe file descriptor array
	
	//setting signals
	struct sigaction sigact;
	sigact.sa_handler=handler; 
	sigemptyset(&sigact.sa_mask); 
	sigact.sa_flags=0; //nothing special behaviour
	sigaction(SIGTERM,&sigact,NULL);

	sigaction(SIGTERM,&sigact,NULL);
	// creating pipe
	if (pipe(pipefd) == -1) {
	   perror("Error occured while opening pipe!");
	   exit(EXIT_FAILURE);
	}
	// creating parent-child processes
	pid_t pid = fork();	
	if (pid == -1) {
	   perror("Error occured during fork!");
	   exit(EXIT_FAILURE);
	}
	
	if (pid == 0) {	// child process
		printf("%s is waiting for the questions.\n", argv[1]);
		
		read(pipefd[0],selected,sizeof(selected)); // reading from pipe
		close(pipefd[0]);
		printf("The questions arrived, voting...\n");
		kill(getppid(),SIGTERM); //waking up parent
		
		vote(selected, result);
		
		write(pipefd[1], result, sizeof(result));
		close(pipefd[1]); // finally we close the used read end
	} 
	else {    // parent process 
	
		sigset_t sigset;
		sigfillset(&sigset);
		sigdelset(&sigset,SIGTERM);
	
		select_random(selected, all_fq, all_fq_c);
		
		write(pipefd[1], selected,sizeof(selected)); //writing
		close(pipefd[1]); // Closing write descriptor 
		
		sigsuspend(&sigset); //we wait here until the child finished the reading
		
		read(pipefd[0], result, sizeof(result));
		close(pipefd[0]);
		
		printf("The results arrived!\n");
		write_result(selected, 3, result);
		
		fflush(NULL); 	// flushes all write buffers 
		wait();		// waiting for child process 
	}



	return 0;
}