#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>


int get_random(int lower_bound, int upper_bound){
	int random = rand() % (upper_bound - lower_bound + 1) + lower_bound;
	return random;
}

int contains(int num, int num_of_numbers, int *numbers){
	int i, contains = 0;
	for(i = 0; i < num_of_numbers && contains == 0; ++i){
		if(numbers[i] == num){
			contains = 1;
		}
	}
	return contains;
}

void get_n_random(int lower_bound, int upper_bound, int required_nums, int *numbers){
	if(upper_bound + 1 < required_nums){
		printf("Upper bound too small!");
		exit(5);
	}
	
	int i, tmp_int;
	for(i = 0; i < required_nums; ++i){
		tmp_int = get_random(lower_bound, upper_bound);
		
		while(contains(tmp_int, i, numbers)){
			tmp_int = get_random(lower_bound, upper_bound);
		}
		numbers[i] = tmp_int;
	}
}

int get_num_of_answers(const char *filename){
	FILE *fp = fopen(filename, "r");
	int file_lines = 0;
	if(fp){
		char *lineptr = NULL;
		size_t size = 0;
		while(getline(&lineptr, &size, fp) > 0){
			++file_lines;
		}
	}
	return file_lines - 2;
}

int number_of_files_in_directory(const char *directory){
	int file_count = 0;
	DIR *dirp;
	struct dirent *entry;

	dirp = opendir(directory);
	if(dirp < 0){
		printf("Directory does not exist");
		exit(3);
	}
	
	while ((entry = readdir(dirp)) != NULL) {
		if (entry->d_type == DT_REG) { 
			 file_count++;
		}
	}
	closedir(dirp);
	return file_count;
}

void get_answers(const char *filename, int num_of_answers, char result[][150]){
	FILE *fp = fopen(filename, "r");
	
	if(fp){
		size_t size = 0;
		int i;
		char *lineptr = NULL;
		for(i = 0; i < num_of_answers + 1; ++i){
			getline(&lineptr, &size, fp);
			strcpy(result[i], lineptr);
		}
	}
}

void handler(int sig){
	//printf("%d has arrived\n", sig);
}

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Exactly 1 parameter is required!");
		exit(4);
	}
	srand(time(NULL));
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	
	int pipefd[2], pipefd2[2], status; 
	pid_t kerdezobiztos;
	
	if (pipe(pipefd) == -1)
	{
		printf("Error when opening pipe!");
		exit(EXIT_FAILURE);
	}
	
	if (pipe(pipefd2) == -1)
	{
		printf("Error when opening pipe!");
		exit(EXIT_FAILURE);
	}
	
	kerdezobiztos = fork();
	
	if(kerdezobiztos < 0){
		printf("The fork calling was not succesful\n"); 
		exit(1);
	}	
	
	//
	//Parent
	//
	
	else if(kerdezobiztos > 0){
		pause();
		
		int num_of_questions = number_of_files_in_directory(argv[1]);
		int question_numbers[3], answers[3][4];
		get_n_random(0, num_of_questions - 1, 3, question_numbers);
		
		//printf("Parent: %d %d %d\n", question_numbers[0], question_numbers[1], question_numbers[2]);
		
		close(pipefd[0]);
		write(pipefd[1], question_numbers, 3 * sizeof(int));
		close(pipefd[1]);
		
		fflush(NULL);
		
		//printf("parent uzenetet küld a childnak\n");
		kill(kerdezobiztos, SIGUSR1);
		
		//pause();
		
		close(pipefd2[1]);
		read(pipefd2[0], answers, 12 * sizeof(int));
		close(pipefd2[0]);
		
		char question[5][150];
		char filename[25];
		
		int i, j;
		for(i = 0; i < 3; ++i){
			sprintf(filename, "Final/%df.txt", question_numbers[i]);
			get_answers(filename, get_num_of_answers(filename), question);
			printf("%s", question[0]);
			for(j = 0; j < 4 && answers[i][j] != -1; ++j){
				printf("(%d) %s", answers[i][j], question[j + 1]);
			}
			printf("\n");
		}
		
		waitpid(kerdezobiztos, &status, 0);
		//printf("Parent fininshed\n");
	}
	
	//
	//Child
	//
	
	else if(kerdezobiztos == 0){
		sleep(1);
		kill(getppid(), SIGUSR1);
		
		//pause();
		
		int questions[3];
		int answers[3][4];
		
		close(pipefd[1]);
		read(pipefd[0], questions, 3 * sizeof(int));
		close(pipefd[0]);

		//printf("child: %d %d %d\n", questions[0], questions[1], questions[2]);
		
		int num_of_answers[3];
		
		int i, j, k;
		for(i = 0; i < 3; ++i){
			char filename[18];
			sprintf(filename, "Final/%df.txt", questions[i]);
			num_of_answers[i] = get_num_of_answers(filename);
		}
		
		
		int num_of_people = get_random(10, 20);
		//printf("Child: num of people: %d\n", num_of_people);
		
		for(i = 0; i < 3; ++i){
			for(j = 0; j < num_of_answers[i]; ++j){
				answers[i][j] = 0;
			}
			
			for(; j < 4; ++j){
				answers[i][j] = -1;
			}
		}
		
		for(k = 0; k < num_of_people; ++k){
			for(i = 0; i < 3; ++i){
				j = get_random(0, num_of_answers[i] - 1);
				++answers[i][j];
			}
		}
		
		/*for(i = 0; i < 3; ++i){
			printf("Question %d: ", i);
			for(j = 0; j < 4 && answers[i][j] != -1; ++j){
				printf("Answer %d: %d  ", j, answers[i][j]);
			}
			printf("\n");
		}*/
		
		//printf("pipe előtt");
				
		close(pipefd2[0]);
		write(pipefd2[1], answers, 12 * sizeof(int));
		close(pipefd2[1]);
		
		kill(getppid(), SIGUSR1);
		//printf("Message arrived\n");
		//printf("Child finished");
	}
	
	return 0;
}