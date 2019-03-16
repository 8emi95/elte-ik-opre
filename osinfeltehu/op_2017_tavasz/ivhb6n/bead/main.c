#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<time.h>
#include<errno.h>

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

void question_n(char file_name[FILE_SIZE]){

	FILE *file=fopen(file_name, "r");
	FILE *write_file=fopen("filec.txt", "w");

	struct QuestionForm *line=malloc(sizeof(struct QuestionForm));

	int n=1;
	char num[5];

	if(file==NULL || write_file==NULL) { printf("Hiba! Nem lehet megnyitni a fajlt!");}

	while(fread(line, sizeof(struct QuestionForm), 1,file)){

		sprintf(num, "%d.", n);
		strcpy(line->question_number, num);

		fwrite(line, sizeof(struct QuestionForm), 1, write_file);
		n++;
	}

	fclose(write_file);
	fclose(file);

	remove(file_name);
	rename("filec.txt", file_name);
}

void add_question_to_file(struct QuestionForm *input){

	FILE *file=fopen("questions.txt", "a");

	if (file!=NULL){
		fwrite(input, sizeof(struct QuestionForm), 1, file);
		fclose(file);
	}
}

void delete_question_from_file(char input[]){

	FILE *read_file=fopen("questions.txt", "r");
	FILE *write_file=fopen("filec.txt", "w");

	if(read_file==NULL || write_file==NULL) { printf("Hiba! Nem lehet megnyitni a fajlt!");}

	struct QuestionForm *line=malloc(sizeof(struct QuestionForm));

	while(fread(line, sizeof(struct QuestionForm), 1,read_file)){

		if(strcmp(line->question_number, input)!=0) { fwrite(line, sizeof(struct QuestionForm), 1, write_file); }

	}

	fclose(read_file);
	fclose(write_file);

	remove("questions.txt");
	rename("filec.txt", "questions.txt");
}

void modify_question(char num[], struct QuestionForm *input){

	FILE *read_file=fopen("questions.txt", "r");
	FILE *write_file=fopen("filec.txt", "w");

	if(read_file==NULL || write_file==NULL) { printf("Hiba! Nem lehet megnyitni a fajlt!");}

	struct QuestionForm *line=malloc(sizeof(struct QuestionForm));

	while(fread(line, sizeof(struct QuestionForm), 1,read_file)){

		if(strcmp(line->question_number, num)!=0) { fwrite(line, sizeof(struct QuestionForm), 1, write_file); }
		else{
			fwrite(input, sizeof(struct QuestionForm), 1, write_file);
		}
	}

	fclose(read_file);
	fclose(write_file);

	remove("questions.txt");
	rename("filec.txt", "questions.txt");
}

void select_questions(char f_name[FILE_SIZE], char num[INPUT]){

	FILE *read_file=fopen("questions.txt", "r");
	FILE *write_file=fopen(f_name, "a");

	struct QuestionForm *line=malloc(sizeof(struct QuestionForm));

	if(read_file==NULL || write_file==NULL) { printf("Hiba! Nem lehet megnyitni a fajlt!");}

	while(fread(line, sizeof(struct QuestionForm), 1,read_file)){

		if(strcmp(line->question_number, num)==0) { fwrite(line, sizeof(struct QuestionForm), 1, write_file); }
	}

	fclose(read_file);
	fclose(write_file);
}

void list_questions(){

	FILE *read_file=fopen("questions.txt", "r");
	struct QuestionForm *line=malloc(sizeof(struct QuestionForm));

	if(read_file==NULL) { printf("Hiba! Nem lehet megnyitni a fajlt!");}

	while(fread(line, sizeof(struct QuestionForm), 1,read_file)){

		printf("%s%s%s %s %s %s %s\n",line->s_time,line->question_number, line->question, line->answer1, line->answer2, line->answer3, line->answer4);
	}

	fclose(read_file);
}

int main(){

int i;

do{
    printf("Kerem adja meg a vegrehajtani kivant muvelet szamat: \n");
	printf("1. Kerdes hozzaadasa \n");
	printf("2. Kerdes torlese \n");
	printf("3. Kerdes modositasa \n");
	printf("4. Kerdes valasztasa veglegesitesre \n");
	printf("5. Kerdesek listazasa \n");
	printf("6. Kilepes. \n");



	scanf("%d", &i);

	switch(i){
		case 1:
			printf("Kerem a kerdest: \n");
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
			question_n("questions.txt");

			break;
		case 2:
			list_questions();
			printf("Kerem adja meg a torolni kivant kerdes sorszamat : \n");
			char del_q[INPUT_SIZE];
			scanf("%s", del_q);

			delete_question_from_file(del_q);
			question_n("questions.txt");

			break;
		case 3:
			list_questions();
			printf("Kerem adja meg a modositani kivant kerdes sorszamat, az uj kerdest es a valaszokat: \n");
			char mod_input[INPUT_SIZE];
			struct QuestionForm *input_struct1=malloc(sizeof(struct QuestionForm));
			char t[INPUT_SIZE];
			time_t current_time1;
			char* string_time1;
			current_time1=time(NULL);
			string_time1=ctime(&current_time1);
			scanf("%s", mod_input);
			printf("Az uj kerdes es valaszok: \n");
			fgets(t, INPUT, stdin);
			fgets(input_struct1->question, INPUT_SIZE, stdin);
			fgets(input_struct1->answer1, INPUT_SIZE, stdin);
			fgets(input_struct1->answer2, INPUT_SIZE, stdin);
			fgets(input_struct1->answer3, INPUT_SIZE, stdin);
			fgets(input_struct1->answer4, INPUT_SIZE, stdin);
			strcpy(input_struct1->s_time,string_time1);
			modify_question(mod_input, input_struct1);
			question_n("questions.txt");

			break;
		case 4:
			printf("Adja meg a fajlnevet! \n");
			char filename[FILE_SIZE];
			char q_n[INPUT_SIZE];
			scanf("%s", filename);
			list_questions();
			printf("Kerem adja meg a sorszamot , nyomjon Enter-t és irjon uj szamot. Majd Enter es Ctrl D lenyomasaval lezarhatja: \n");
			while(scanf(" %s", q_n)==1){
				select_questions(filename, q_n);
			}
			question_n(filename);
			break;
		case 5:
			list_questions();
			break;
	}

    } while(i != 6);



	return 0;
}
