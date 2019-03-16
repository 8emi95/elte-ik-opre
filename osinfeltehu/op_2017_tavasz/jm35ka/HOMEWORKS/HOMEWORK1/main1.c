#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>  
#include<sys/types.h> 
#include<sys/stat.h>
#include<errno.h> 
#include<string.h>
#include<time.h>

#define INPUT_SIZE 256
#define FILE_SIZE 30
#define INPUT 5


struct QuestionForm{
	char s_time[INPUT_SIZE];
	char question_number[INPUT];
	char question[INPUT_SIZE];
	char answer1[INPUT_SIZE];
	char answer2[INPUT_SIZE];
	char answer3[INPUT_SIZE];
	char answer4[INPUT_SIZE];
};	

int number_of_answers(struct QuestionForm *input){
	
	int count=0;
	
	if(input->answer1[0]!='\n'){
		count++;
	}
	
	if(input->answer2[0]!='\n'){
		count++;
	}
	
	if(input->answer3[0]!='\n'){
		count++;
	}
	
	if(input->answer4[0]!='\n'){
		count++;
	}
	
	return count;
}

void question_n(char file_name[FILE_SIZE]){
	
	FILE *file=fopen(file_name, "r");
	FILE *write_file=fopen("filecopy.txt", "w");
	
	struct QuestionForm *line=malloc(sizeof(struct QuestionForm));
	
	int n=1;
	char number[5];
	
	if(file==NULL || write_file==NULL) { printf("Open error");}
	
	while(fread(line, sizeof(struct QuestionForm), 1,file)){
			
		sprintf(number, "%d.", n);
		strcpy(line->question_number, number);
		
		fwrite(line, sizeof(struct QuestionForm), 1, write_file);
		n++;
	}

	fclose(write_file);
	fclose(file);
	
	remove(file_name);
	rename("filecopy.txt", file_name);
	
	free(line);
}

void add_question_to_file(struct QuestionForm *input){
	
	FILE *file=fopen("questionbank.txt", "a");
	
	if (file!=NULL){
		fwrite(input, sizeof(struct QuestionForm), 1, file);
		fclose(file);
	}	
}

void delete_question_from_file(char input[]){

	FILE *read_file=fopen("questionbank.txt", "r");
	FILE *write_file=fopen("filecopy.txt", "w");

	if(read_file==NULL || write_file==NULL) { printf("Open error");}

	struct QuestionForm *line=malloc(sizeof(struct QuestionForm));

	while(fread(line, sizeof(struct QuestionForm), 1,read_file)){
		
		if(strcmp(line->question_number, input)!=0) { fwrite(line, sizeof(struct QuestionForm), 1, write_file); }
		
	}

	fclose(read_file);
	fclose(write_file);
	
	remove("questionbank.txt");
	rename("filecopy.txt", "questionbank.txt");
	
	free(line);
}

void modify_question(char num[], struct QuestionForm *input){
	
	FILE *read_file=fopen("questionbank.txt", "r");
	FILE *write_file=fopen("filecopy.txt", "w");
	
	if(read_file==NULL || write_file==NULL) { printf("Open error");}	

	struct QuestionForm *line=malloc(sizeof(struct QuestionForm));
	
	while(fread(line, sizeof(struct QuestionForm), 1,read_file)){
		
		if(strcmp(line->question_number, num)!=0) { fwrite(line, sizeof(struct QuestionForm), 1, write_file); }
		else{
			fwrite(input, sizeof(struct QuestionForm), 1, write_file);
		}
	}

	fclose(read_file);
	fclose(write_file);
	
	remove("questionbank.txt");
	rename("filecopy.txt", "questionbank.txt");
	
	free(line);
}

void select_questions(char f_name[FILE_SIZE], char num[INPUT]){
	
	FILE *read_file=fopen("questionbank.txt", "r");
	FILE *write_file=fopen(f_name, "a");
	
	struct QuestionForm *line=malloc(sizeof(struct QuestionForm));
	
	if(read_file==NULL || write_file==NULL) { printf("Open error");}
	
	while(fread(line, sizeof(struct QuestionForm), 1,read_file)){
		
		if(strcmp(line->question_number, num)==0) { fwrite(line, sizeof(struct QuestionForm), 1, write_file); }
	}
	
	fclose(read_file);
	fclose(write_file);
	
	free(line);
}

void list_questions(){
	
	FILE *read_file=fopen("questionbank.txt", "r");
	struct QuestionForm *line=malloc(sizeof(struct QuestionForm));
	
	if(read_file==NULL) { printf("Open error");}
	
	while(fread(line, sizeof(struct QuestionForm), 1,read_file)){
		
		printf("%s%s%s %s %s %s %s\n",line->s_time,line->question_number, line->question, line->answer1, line->answer2, line->answer3, line->answer4); 
	}
	
	fclose(read_file);
	
	free(line);
}

int main(){

	printf("Welcome, please enter a corresponding number: \n");
	printf("1. Add new questions \n");
	printf("2. Delete questions \n");
	printf("3. Modify questions \n");
	printf("4. Select questions for survey \n");
	printf("5. List questions from the question bank. \n");
	
	int i;
	
	scanf("%d", &i);
	
	switch(i){
		case 1:
			printf("You entered 1. Example of question format:\n How are you: \n a) good\n b) great\n c) awesome.\nYou can have at most 4 answers for question. Write your question: \n");
			struct QuestionForm *input_struct=malloc(sizeof(struct QuestionForm));
			char tester[INPUT_SIZE];
			time_t current_time;
			char* string_time;
			current_time=time(NULL);
			string_time=ctime(&current_time);
			fgets(tester, INPUT, stdin);
			fgets(input_struct->question, INPUT_SIZE, stdin);
			fgets(input_struct->answer1, INPUT_SIZE, stdin);
			fgets(input_struct->answer2, INPUT_SIZE, stdin);
			fgets(input_struct->answer3, INPUT_SIZE, stdin);
			fgets(input_struct->answer4, INPUT_SIZE, stdin);
			strcpy(input_struct->s_time,string_time);
			add_question_to_file(input_struct);
			question_n("questionbank.txt");
			printf("Finished adding question.\n");
			free(input_struct);
			break;
		case 2:
			list_questions();
			printf("To delete a question, enter the number of a question followed by . (dot). For example 3. : \n");
			char del_q[INPUT_SIZE];
			scanf("%s", del_q);
			delete_question_from_file(del_q);
			question_n("questionbank.txt");
			printf("Question %s was deleted. \n", del_q);
			break;
		case 3:
			list_questions();
			printf("To modify a question first enter the number of the question followed by a dot, for example 3. , and after the new question with answers: \n");
			char mod_input[INPUT_SIZE];
			struct QuestionForm *input_struct1=malloc(sizeof(struct QuestionForm));
			char t[INPUT_SIZE];
			time_t current_time1;
			char* string_time1;
			current_time1=time(NULL);
			string_time1=ctime(&current_time1);
			scanf("%s", mod_input);
			printf("Enter the modified question and answers: \n");
			fgets(t, INPUT, stdin);
			fgets(input_struct1->question, INPUT_SIZE, stdin);
			fgets(input_struct1->answer1, INPUT_SIZE, stdin);
			fgets(input_struct1->answer2, INPUT_SIZE, stdin);
			fgets(input_struct1->answer3, INPUT_SIZE, stdin);
			fgets(input_struct1->answer4, INPUT_SIZE, stdin);
			strcpy(input_struct1->s_time,string_time1);
			modify_question(mod_input, input_struct1);
			question_n("questionbank.txt");
			printf("Modified question was added.\n");
			free(input_struct1);
			break;
		case 4:
			printf("Enter the new file name for the survey. \n");
			char filename[FILE_SIZE];
			char q_n[INPUT_SIZE];
			scanf("%s", filename);
			list_questions();
			printf("To select the questions, enter the number of a question followed by dot, for example 3. , press Enter and write another number. When you are done writing the numbers press Enter and Ctrl ^ D: \n");
			while(scanf(" %s", q_n)==1){	
				select_questions(filename, q_n);
			}
			question_n(filename);			
			break;
		case 5:
			list_questions();
			break;
		default:
			printf("Do nothing.\n");
	}
	return 0;
}