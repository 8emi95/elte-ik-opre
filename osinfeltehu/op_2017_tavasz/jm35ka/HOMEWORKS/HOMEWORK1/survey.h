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

int number_of_answers(struct QuestionForm input){
	
	int count=0;
	
	if(input.answer1[0]!='\n'){
		count++;
	}
	
	if(input.answer2[0]!='\n'){
		count++;
	}
	
	if(input.answer3[0]!='\n'){
		count++;
	}
	
	if(input.answer4[0]!='\n'){
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