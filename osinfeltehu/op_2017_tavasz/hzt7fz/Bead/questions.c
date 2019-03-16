#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//Checks if there is a data file, if not, creates one
void set_up_data_file(){
	FILE *fp = fopen("data.txt", "a+");
	if(fp){
		fseek(fp, 0L, SEEK_END);
		int sz = (int)ftell(fp);
		if(sz == 0){
			char num[3];
			sprintf(num, "%d", 0);
			fputs(num, fp);
		}
		fclose(fp);
	}
}

//Reads number of questions from the data file
int get_num_of_questions(){
	int num_of_questions = -1;
	FILE *fp = fopen("data.txt", "r");
	if(fp){
		fscanf(fp, "%d", &num_of_questions);
		fclose(fp);
		
		return num_of_questions;
	}
}

//Prints a given string to the given file in the given mode
void write_to_file(const char *text, const char *filename, const char *mode){
	FILE* fp;
	fp = fopen(filename, mode);
	if(fp){
		fputs(text, fp);
		fclose(fp);
	}
}

//Modifies the data file
//-1 : Incrementing the number of files by 1
//int > 0 : Deleteing the given file
void questions_modified(const int file){
	int num_of_questions = get_num_of_questions(), i;
	FILE *fp = fopen("data.txt", "w+");
	char new_val[4], old_name[7], new_name[7];
	
	if(file == -1){
		sprintf(new_val, "%d", num_of_questions + 1);
	}else if(file >= 0){
		sprintf(new_val, "%d", num_of_questions - 1);
		
		for(i = file + 1; i < num_of_questions; ++i){
			sprintf(old_name, "%d.txt", i);
			sprintf(new_name, "%d.txt", i - 1);
			rename(old_name, new_name);
		}
	}
	
	if(fp){
		fputs(new_val, fp);
		fclose(fp);
	}
}

//Deleting the given question
void delete_question(const int question){
	char name[7];
	sprintf(name, "%d.txt", question);
	remove(name);
	questions_modified(question);
}


void modify_question(const int line_num, const int file, const char *new_line){
	char tmp[11], original[7], *line = NULL;
	int i, num_of_chars;
	size_t size;
	
	sprintf(tmp, "%d_tmp.txt", file);
	sprintf(original, "%d.txt", file);
	
	rename(original, tmp);
	
	//FILE *dest = fopen(original, "w+");
	FILE *source = fopen(tmp, "r");

	for(i = 0; i < line_num;++i){
		getline(&line, &size, source);
		write_to_file(line, original, "a+");
	}
	
	write_to_file(new_line, original, "a+");
	
	num_of_chars = getline(&line, &size, source);
	num_of_chars = getline(&line, &size, source);
	
	while(num_of_chars != -1){
		write_to_file(line, original, "a+");
		num_of_chars = getline(&line, &size, source);
	}
	remove(tmp);
}

//Recording a question from satndard input, modifying data file
void write_question(const int num_of_questions){
	int num_of_answers = 0, i, c;
	char *question = NULL, *answer = NULL, filename[7];
	size_t size_of_question = 0, size_of_answer = 0;
	
	//Converting the number of questions to a string
	sprintf(filename, "%d.txt", num_of_questions);
	
	printf("Type in a question please:\n");
	getline(&question, &size_of_question, stdin);
	write_to_file(question, filename, "w+");
	
	while(num_of_answers < 2 || num_of_answers > 4){
		printf("How many answers would you like(2-4): ");
		scanf("%d", &num_of_answers);
		while ((c = getchar()) != '\n' && c != EOF) { }
	}

	for(i = 0; i < num_of_answers; ++i){
		printf("Answer %d: ", i + 1);
		getline(&answer, &size_of_answer, stdin);
		write_to_file(answer, filename, "a+");
	}
	
	//Recording the date of question
	char date_to_print[25];
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	sprintf(date_to_print, "%d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	
	write_to_file(date_to_print, filename, "a+");
	
	questions_modified(-1);
}

//Prints a file to the standard output
void print_file(const char *filename){
	FILE *fp = fopen(filename, "r");
	int character;
	
	if (fp) {
		while((character = getc(fp)) != EOF){
			putchar(character);
		}
		fclose(fp);
	}	
}


void copy_file(const char* to, const char *filename, const char *new_name){
	char *line = NULL, dest[20], dest_folder[30];
	struct stat st = {0};
	size_t size = 0, num_of_chars = 0;
	FILE *fp = fopen(filename, "r");
	
	sprintf(dest, "%s/%s", to, new_name);
	sprintf(dest_folder, "%s/", to);
	
	rmdir(dest_folder);
	mkdir(dest_folder, 0777);
	
	num_of_chars = getline(&line, &size, fp);
	
	while(num_of_chars != -1){
		write_to_file(line, dest, "a+");
		num_of_chars = getline(&line, &size, fp);
	}
	
	fclose(fp);
}

//***********
//**  Main
//***********

int main(int argc, char *argv[]){
	printf("%s\n", "Welcome to MLSZ!");
	set_up_data_file();
	int is_over = 0;
	
	
	while(is_over != 1){
		int num_of_questions = get_num_of_questions();
		int c, option;
		
		printf("What would you like to do?\nRead Questions[1]\nAdd new question[2]\nModify questions[3]\nDelete question[4]\nFinalize questions[5]\n");
		
		scanf("%d", &option);
		while ((c = getchar()) != '\n' && c != EOF) { }
		
		if(option == 1){		//Printing question
			
			int question_num;
			char filename[7];
			printf("Which question would you like to read?(%d-%d)", 0, num_of_questions - 1);
			scanf("%d", &question_num);
			while ((c = getchar()) != '\n' && c != EOF) { }
			if(question_num >= 0 && question_num < num_of_questions){
				sprintf(filename, "%d.txt", question_num);
				print_file(filename);
			}else{
				printf("Wrong question number!\n");
			}
			
		}else if(option == 2){		//Adding new question
			
			write_question(num_of_questions);
			
		}else if(option == 3){		//Modifying question
			
			char *modified_line;
			size_t size;
			int question_num, option_num, line_num;
			printf("Which question would you like to modify?(%d-%d)\n", 0, num_of_questions - 1);
			scanf("%d", &question_num);
			while ((c = getchar()) != '\n' && c != EOF) { }
			
			if(question_num >= 0 && question_num < num_of_questions){
				
				printf("Would you like to modify a question or an answer, or both? Question[0] Answer[1] Both[2]\n");
				scanf("%d", &option_num);
				while ((c = getchar()) != '\n' && c != EOF) { }
				
				if(option_num == 0){
					
					printf("Please provide the new question:");
					getline(&modified_line, &size, stdin);
					
					modify_question(0, question_num, modified_line);
					
				}else if(option_num == 1){
					
					printf("Which answer would you like to modify?");
					scanf("%d", &line_num);
					while ((c = getchar()) != '\n' && c != EOF) { }
					
					printf("Please provide the new answer:");
					
					getline(&modified_line, &size, stdin);
					
					modify_question(line_num + 1, question_num, modified_line);
					
				}else if(option_num == 2){
					delete_question(question_num);
					write_question(num_of_questions - 1);
				}
				
			}else{
				printf("Wrong question number!\n");
			}
			
			
		}else if(option == 4){		//Deleting question
			
			int question_num;
			printf("Which question would you like to delete?(%d-%d)", 0, num_of_questions - 1);
			scanf("%d", &question_num);
			while ((c = getchar()) != '\n' && c != EOF) { }
			
			if(question_num >= 0 && question_num < num_of_questions){
				delete_question(question_num);
			}else{
				printf("Wrong question number!\n");
			}
		}else if(option == 5){		//Finalizing questions
			int questions = 0;
			char old_filename[10], new_filename[10];
			printf("Which questions would you like to finalize? (-1 when done)");
			
			scanf("%d", &questions);
			while ((c = getchar()) != '\n' && c != EOF) { }
			
			while(questions != -1){
				if(questions < -1 || questions >= num_of_questions){ 
					scanf("%d", &questions);
					while ((c = getchar()) != '\n' && c != EOF) { }
					continue; 
				}
				sprintf(old_filename, "%d.txt", questions);
				sprintf(new_filename, "%df.txt", questions);
				copy_file("Final", old_filename, new_filename);
				scanf("%d", &questions);
				while ((c = getchar()) != '\n' && c != EOF) { }
			}
		}else{		//Bad option
			printf("Please provide a number between 1 and 4.\n");
			continue;
		}
		
		printf("Would you like to do anything else? Yes[0]  No[1]\n");
		scanf("%d", &is_over);
		while ((c = getchar()) != '\n' && c != EOF) { }
	}
	
	return 0;
}