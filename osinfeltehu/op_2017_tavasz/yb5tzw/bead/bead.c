/*
This program fulfills the following task. 
Created by Norbert Luksa (YB5TZW), 2017.04.
norbert.luksa@gmail.comp

TASK:
A Magyar Labdajátékok Szervezete (MLSZ) nemzeti konzultációt hirdet a labdajátékok felvirágoztatására! 
Első lépésként meg kell alkotni azt a kérdéssort és hozzá kérdésenként azt a minimum kettő, maximum 4 választási lehetőséget 
amelyet majd a labdajátékok jobbítása iránt felelősséget érzők kitölthetnek. 
A készítendő program adjon lehetőséget a kérdéssor megalkotására. 
Tudjunk új kérdést felvenni, kérdést módosítani és kérdést törölni. 
Módosításhoz, törléshez kínálja fel választásra a kérdést és válaszait a program! 
Tudjunk emellett természetesen kérdéseket és válaszaikat listázni! 
A kérdésekhez tároljuk automatikusan annak felvételi dátumát (év,hó,nap,óra,perc,másodperc). 
Végezetül a program adjon lehetőséget a kérdések véglegesítésére, azaz annak meghatározására, 
hogy mely kérdések szerepeljenek a végső konzultációs kérdőíven!
Készítsünk C programot, amellyel a leírt tevékenység elvégezhető. Az adatokat fájlban tároljuk.
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <time.h>
#include <string.h>

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
//creates id for a new question. It will be the number of questions including deleted ones.
int get_id(int all_q_c){
	return all_q_c;
}

//returns the current date in %Y.%m.%d. %H:%M:%S format in the given parameter
void get_date(char d[30]){
	time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (d,30,"%Y.%m.%d. %H:%M:%S",timeinfo);
}

//writes all questions to the file (only when there's change)
void write_questions_to_file(struct question* all_q, int all_q_c){
	FILE* f = fopen("database.txt", "w");
	if (f == NULL){ perror("Error occured: file opening was not succesful\n"); exit(1);}
	
	//counting new number of questions
	int i;
	int c = 0;
	for(i = 0; i < all_q_c;i++){
		if(!all_q[i].deleted) { c++;}
	}
	fprintf(f, "%d\n", c);
	
	//writing questions to file
	c = 0;
	for(i = 0; i < all_q_c; ++i){
		if(!all_q[i].deleted){ //does not write deleted questions
			fprintf(f, "%d\n", i - c); //Changing the ID for deleted questions 
			fprintf(f, "%d\n", strlen(all_q[i].question));
			fprintf(f, "%s\n", all_q[i].question);
			fprintf(f, "%d\n", all_q[i].answers_c);
			int j;
			for(j = 0; j < all_q[i].answers_c; ++j){
				fprintf(f, "%d\n", strlen(all_q[i].answers[j]));
				fprintf(f, "%s\n", all_q[i].answers[j]);
			}
			fprintf(f, "%s\n", all_q[i].date);
			fprintf(f, "%d\n", all_q[i].final);
		}else{ c++; }
	}
	
	fclose(f);
}

//adds new question. Returns 1 on success, 0 on fail 
int adding_question(struct question all_q[100], int all_q_c){
	
	struct question q;
	
	q.id 	  = get_id(all_q_c);
	q.final   = 0;
	q.deleted = 0;
	get_date(q.date);
	
	//reading question;
	printf("What is the question?\n");
	char c;
    while ( (c = getchar()) != '\n' && c != EOF ); //clearing up stream
    fgets (q.question, 200, stdin);
    // Remove trailing newline, if there is one
    if ((strlen(q.question)>0) && (q.question[strlen (q.question) - 1] == '\n')) { q.question[strlen (q.question) - 1] = '\0'; }
	
	//reading answers
	printf("How many answers will you add?");
	scanf ("%d", &q.answers_c);
	if (q.answers_c < 2 || q.answers_c > 4) {perror("Error occured: wrong number of answers\n"); return 0; }
	
    while ( (c = getchar()) != '\n' && c != EOF ); //clearing up stream
	int i;
	for(i = 0; i < q.answers_c; ++i){
		printf("Add answer number %d out of %d\n", i+1, q.answers_c);
		q.answers[i] = (char*)malloc(200);
		fgets(q.answers[i], 200, stdin);
		if ((strlen(q.answers[i])>0) && (q.answers[i][strlen(q.answers[i]) - 1] == '\n')) { q.answers[i][strlen (q.answers[i]) - 1] = '\0'; }
	}
	all_q[all_q_c] = q;
	return 1;
}

//loads all questions from database
int load_question(struct question all_q[100]){
	int c;
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
			all_q[y] 	= q;
		}
		
		fclose(file);
	}
	
	return all_q_c;
}

//lists all questions
void list_all(struct question all_q[100], int all_q_c){
	int i;
	for(i = 0; i < all_q_c; ++i){
		if(!all_q[i].deleted){ //does not list deleted questions
			printf("ID: %d\n", all_q[i].id);
			if(all_q[i].final){ printf("Final: YES\n" ); }
			else { printf( "Final: NO\n"); }
			printf("Date: %s\n", all_q[i].date);
			printf("Question:\n%s\n", all_q[i].question);
			int j;
			for(j = 0; j < all_q[i].answers_c; ++j){
				printf("%d: %s\n", j+1, all_q[i].answers[j]);
			}
			printf("###################################################\n");
		}
	}
}

//deleting a question
int delete_question(struct question all_q[100],int all_q_c){
	list_all(all_q, all_q_c); //listing the questions for the user
	printf("Provide the ID of the question you want to delete! ");
	int id;
	scanf ("%d", &id);
	if(id < 0 || id > all_q_c) { printf("There's no such question\n"); return 0; }
	else if(all_q[id].deleted) { printf("You have already deleted this question\n"); return 0;}
	else { all_q[id].deleted = 1; return 1; }
}

//finalize a question
int finalize_question(struct question all_q[100],int all_q_c){
	list_all(all_q, all_q_c); //listing the questions for the user
	printf("Provide the ID of the question you want to finalize! ");
	int id;
	scanf ("%d", &id);
	if(id < 0 || id > all_q_c) { printf("There's no such question\n"); return 0; }
	else if(all_q[id].deleted) { printf("You have already deleted this question\n"); return 0;}
	else { all_q[id].final = 1; return 1; }
}

//unfinalize a question
int unfinalize_question(struct question all_q[100],int all_q_c){
	list_all(all_q, all_q_c); //listing the questions for the user
	printf("Provide the ID of the question you DON'T want to finalize! ");
	int id;
	scanf ("%d", &id);
	if(id < 0 || id > all_q_c) { printf("There's no such question\n"); return 0; }
	else if(all_q[id].deleted) { printf("You have already deleted this question\n"); return 0;}
	else if(!all_q[id].final) { printf("This question is not final\n"); return 0;}
	else { all_q[id].final = 1; return 1; }
}

//returns wether the user wants to modify the question or not
int ask_for_moification(){
	//getting the answer
	int ans;
	scanf ("%d", &ans);
	return ans;
}


//modifies a question. Returns 1 on success, 0 on fail or no change
int modify_question(struct question all_q[100], int all_q_c){
	list_all(all_q, all_q_c); //listing the questions for the user
	
	//getting the id of the question
	printf("Provide the ID of the question you want to modify! ");
	int id;
	scanf ("%d", &id);
	if(id < 0 || id > all_q_c) { printf("There's no such question\n"); return 0; }
	else if(all_q[id].deleted) { printf("You have already deleted this question\n"); return 0;}
	
	int change = 0;
	int modify;
	printf("Now you can modify the selected question.\n");
	
	//modifying question
	printf("Would you like to modify the question? [0 = NO, 1 = YES] ");
	modify = ask_for_moification("question");
	if (modify){
		change = 1;
		//reading question;
		printf("What is the new question?\n");
		char c;
		while ( (c = getchar()) != '\n' && c != EOF ); //clearing up stream
		fgets (all_q[id].question, 200, stdin);
		// Remove trailing newline, if there is one
		if ((strlen(all_q[id].question)>0) 
			&& (all_q[id].question[strlen (all_q[id].question) - 1] == '\n')) { 
				all_q[id].question[strlen (all_q[id].question) - 1] = '\0'; 
		}
	}
	
	//modifying answers
	printf("Would you like to modify the answers? [0 = NO, 1 = YES (one by one), 2 = YES (add all)] ");
	modify = ask_for_moification("question");
	if (modify){
		change = 1;
		if(modify == 1){ //adding all answers again
			int i;
			for(i = 0; i < all_q[id].answers_c; i++){
				printf("Would you like to modify the answer number %d? [0 = NO, 1 = YES] ", i+1);
				modify = ask_for_moification("question");
				if(modify){
					printf("Add answer number %d out of %d\n", i+1, all_q[id].answers_c);
					all_q[id].answers[i] = (char*)malloc(200);
					char c;
					while ( (c = getchar()) != '\n' && c != EOF ); //clearing up stream
					fgets(all_q[id].answers[i], 200, stdin);
					if ((strlen(all_q[id].answers[i])>0) 
						&& (all_q[id].answers[i][strlen(all_q[id].answers[i]) - 1] == '\n')) { 
							all_q[id].answers[i][strlen (all_q[id].answers[i]) - 1] = '\0'; 
					}
				}
			}
		}else{ //modifying answers one by one
			all_q[id].answers_c = 0;
			while(all_q[id].answers_c < 2 || all_q[id].answers_c > 4){
				printf("How many answers will you add? [2, 3, 4]");
				scanf ("%d", &all_q[id].answers_c);
			}
			char c;
			while ( (c = getchar()) != '\n' && c != EOF ); //clearing up stream
			int i;
			for(i = 0; i < all_q[id].answers_c; ++i){
				printf("Add answer number %d out of %d\n", i+1, all_q[id].answers_c);
				all_q[id].answers[i] = (char*)malloc(200);
				fgets(all_q[id].answers[i], 200, stdin);
				if ((strlen(all_q[id].answers[i])>0) 
					&& (all_q[id].answers[i][strlen(all_q[id].answers[i]) - 1] == '\n')) { 
						all_q[id].answers[i][strlen (all_q[id].answers[i]) - 1] = '\0'; 
				}
			}
		}
	}
	
	
	
	return change;
}


int main(){
	struct question all_q[100];				//container for all the questions
	int    all_q_c = load_question(all_q);	//loads the questions from database and defines the number of questions
	int    run    = 1;						//while the menu should run (turned to 0 on exit)
	int    change = 0; 						//only change database if there has been change
	int    selector;   						//selector for the menu
	
	//Menu
	printf("Welcome!\n");
	while(run){		
		int success; //necessary for adding/deleting question, true if it was succesful
		printf("Please select what you would like to do!\n");
		printf("1 - List questions\n");
		printf("2 - Add new questions\n");
		printf("3 - Modify questions\n");
		printf("4 - Delete questions\n");
		printf("5 - Select final questions\n");
		printf("6 - Select final questions\n");
		printf("0 - Exit\n");
		scanf ("%d",&selector);
		switch(selector){
			//exit
			case 0: if(change) {write_questions_to_file(all_q, all_q_c);} run = 0; break; //only change database if there has been change
			//list
			case 1: list_all(all_q, all_q_c); break;
			//add new
			case 2: 
				success = adding_question(all_q, all_q_c);
				if(success){
					change = 1;
					all_q_c += 1;
				}
				break;
			//modify
			case 3: success = modify_question(all_q, all_q_c);
				if(success){ change = 1; }
				break;
			//delete
			case 4: success = delete_question(all_q, all_q_c);
				if(success){ change = 1; }
				break;
			//select final
			case 5: success = finalize_question(all_q, all_q_c);
				if(success){ change = 1; }
				break;
			//unselect final
			case 6: success = unfinalize_question(all_q, all_q_c);
				if(success){ change = 1; }
				break;
			default: printf("That's not an option!");
		}		
	}
	
	return 0;
}