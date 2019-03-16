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

void handler(int sig){
	printf("%d has arrived\n", sig);
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

int main(int argc, char *argv[]){
	
	char first[5][150];
	get_answers("Final/0f.txt", get_num_of_answers("Final/0f.txt"), first);
	printf("%s%s%s%s", first[0], first[1], first[2], first[3]);
	return 0;
}