#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>

#include "MLSZ.h"

#define N 512 
#define K 250 
#define S 7 

#pragma warning( push )
#pragma warning( disable : 4101)

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

char* currentTime()
{
    time_t timer;
	char buffer[30];
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S\n", tm_info);

    return buffer;
}

int checker(char input[], char check[], int meddig)
{
    int i, result = 1;
    for (i = 0; (input[i] != '\0' && check[i] != '\0') || i < meddig; i++) {
        if (input[i] != check[i]) {
            result = 0;
            break;
        }
    }
    return result;
}

void create_survey(Survey* surveys)
{
    int szamlalo = 0;
    int j = 0;
    double i = 0;
	char* end_process;
	/*
    do {
        printf("Number of questions: ");
        scanf("%lf", &i);
        j = (int)i;
        if (i < 0 || j != i)
            printf("Please give a number between 2 and 4!\n");
    } while (i < 0 || j != i);
	*/
    FILE* fp;
    fp = fopen("surveys.txt", "w+");
    time_t current_time;
    char* c_time_string;
    c_time_string = (char*)malloc(26);
    int db = 0;

    //for (szamlalo = 0; szamlalo < i; szamlalo++) {
	do{
        strcpy(c_time_string, currentTime());
        printf("id : %s", c_time_string);
        if (szamlalo == 0)
            gets(&(surveys->id));
        fputs(c_time_string, fp);

        printf("question: ");
        gets(&(surveys->question));
        fprintf(fp, "question: ");
        fputs(&(surveys->question), fp);
        fputs("\n", fp);

        do {
            printf("Number of answers(2..4): ");
            scanf("%i", &db);
        } while (db < 2 || db > 4);

        switch (db) {
        case 2:
            gets(&(surveys->created));

            printf("    (a) - answer: ");
            gets(&(surveys->answer_one));
            fprintf(fp, "\t(a) - ");
            fputs(&(surveys->answer_one), fp);
            fputs("\n", fp);

            printf("    (b) - answer: ");
            gets(&(surveys->answer_two));
            fprintf(fp, "\t(b) - ");
            fputs(&(surveys->answer_two), fp);
            fputs("\n", fp);

            char y = 'Y';
            char n = 'N';
            char c;
            int b = 2;
            do {
                printf("Want to put it in the final survey?(Y/N): ");
                gets(&(surveys->vegleges));
                c = (surveys->vegleges);
                b = c != n;
                b += c != y;
            } while (b == 2);
            fputs(&(surveys->vegleges), fp);
            fputs("\n", fp);
            break;
        case 3:
            gets(&(surveys->created));

            printf("    (a) - answer: ");
            gets(&(surveys->answer_one));
            fprintf(fp, "\t(a) - ");
            fputs(&(surveys->answer_one), fp);
            fputs("\n", fp);

            printf("    (b) - answer: ");
            gets(&(surveys->answer_two));
            fprintf(fp, "\t(b) - ");
            fputs(&(surveys->answer_two), fp);
            fputs("\n", fp);

            printf("    (c) - answer: ");
            gets(&(surveys->answer_three));
            fprintf(fp, "\t(c) - ");
            fputs(&(surveys->answer_three), fp);
            fputs("\n", fp);

            y = 'Y';
            n = 'N';
            b = 2;
           do {
                printf("Want to put it in the final survey?(Y/N): ");
                gets(&(surveys->vegleges));
                c = (surveys->vegleges);
                b = c != n;
                b += c != y;
            } while (b == 2);
            fputs(&(surveys->vegleges), fp);
            fputs("\n", fp);

            break;
        case 4:
            gets(&(surveys->created));

            printf("    (a) - answer: ");
            gets(&(surveys->answer_one));
            fprintf(fp, "\t(a) - ");
            fputs(&(surveys->answer_one), fp);
            fputs("\n", fp);

            printf("    (b) - answer: ");
            gets(&(surveys->answer_two));
            fprintf(fp, "\t(b) - ");
            fputs(&(surveys->answer_two), fp);
            fputs("\n", fp);

            printf("    (c) - answer: ");
            gets(&(surveys->answer_three));
            fprintf(fp, "\t(c) - ");
            fputs(&(surveys->answer_three), fp);
            fputs("\n", fp);

            printf("    (d) - answer: ");
            gets(&(surveys->answer_four));
            fprintf(fp, "\t(d) - ");
            fputs(&(surveys->answer_four), fp);
            fputs("\n", fp);

            y = 'Y';
            n = 'N';
            b = 2;
            do {
                printf("Want to put it in the final survey?(Y/N): ");
                gets(&(surveys->vegleges));
                c = (surveys->vegleges);
                b = c != n;
                b += c != y;
            } while (b == 2);
            fputs(&(surveys->vegleges), fp);
            fputs("\n", fp);

            break;
        }
		
		char y = 'Y';
        char n = 'N';
        char c;
        int b = 2;
        do {
			printf("Are you done?(y/n) ");
            gets(&end_process);
            c = (surveys->vegleges);
            b = c != n;
            b += c != y;
        } while (b == 2);
    }while(end_process != 'y');
	free(c_time_string);
    fclose(fp);
}

int survey_list(Survey* surveys)
{
    FILE* fp;
    fp = fopen("surveys.txt", "r");
    char line[N];
    int i = 0;
    int j = 0;
    while (fgets(line, N, fp) != NULL) {
        if (checker(line, "question: ", 7) > 0) {
            printf("%i. ", i);
            printf("%s", line);
            
            ++i;
        }
        else {
            printf("%s", line);
        }
        ++j;
    }
    fclose(fp);
    return j;
}

void add_question(Survey* surveys)
{
    FILE* fp;
    fp = fopen("surveys.txt", "a");
    time_t current_time;
    char* c_time_string;
    c_time_string = (char*)malloc(26);
    int db = 0;

    strcpy(c_time_string, currentTime());
    printf("id : %s", c_time_string);
    gets(&(surveys->id));
    fputs(c_time_string, fp);

    printf("question: ");
    gets(&(surveys->question));
    fprintf(fp, "question: ");
    fputs(&(surveys->question), fp);
    fputs("\n", fp);

    do {
        printf("Number of questions? (2..4)");
        scanf("%i", &db);
    } while (db < 2 || db > 4);

    switch (db) {
    case 2:
        gets(&(surveys->id));

        printf("    (a) - answer: ");
        gets(&(surveys->answer_one));
        fprintf(fp, "\t(a) - ");
        fputs(&(surveys->answer_one), fp);
        fputs("\n", fp);

        printf("    (b) - answer: ");
        gets(&(surveys->answer_two));
        fprintf(fp, "\t(b) - ");
        fputs(&(surveys->answer_two), fp);
        fputs("\n", fp);

        char y = 'Y';
        char n = 'N';
        char c;
        int b = 2;
        do {
            printf("Want to put it in the final survey?(Y/N): ");
            gets(&(surveys->vegleges));
            c = (surveys->vegleges);
            b = c != n;
            b += c != y;
        } while (b == 2);
        fputs(&(surveys->vegleges), fp);
        fputs("\n", fp);

        break;

    case 3:
        gets(&(surveys->id));

        printf("    (a) - answer: ");
        gets(&(surveys->answer_one));
        fprintf(fp, "\t(a) - ");
        fputs(&(surveys->answer_one), fp);
        fputs("\n", fp);

        printf("    (b) - answer: ");
        gets(&(surveys->answer_two));
        fprintf(fp, "\t(b) - ");
        fputs(&(surveys->answer_two), fp);
        fputs("\n", fp);

        printf("    (c) - answer: ");
        gets(&(surveys->answer_three));
        fprintf(fp, "\t(c) - ");
        fputs(&(surveys->answer_three), fp);
        fputs("\n", fp);

        y = 'Y';
        n = 'N';
        b = 2;
        do {
           printf("Want to put it in the final survey?(Y/N): ");
            gets(&(surveys->vegleges));
            c = (surveys->vegleges);
            b = c != n;
            b += c != y;
        } while (b == 2);
        fputs(&(surveys->vegleges), fp);
        fputs("\n", fp);

        break;
    case 4:
        gets(&(surveys->id));

        printf("    (a) - answer: ");
        gets(&(surveys->answer_one));
        fprintf(fp, "\t(a) - ");
        fputs(&(surveys->answer_one), fp);
        fputs("\n", fp);

        printf("    (b) - answer: ");
        gets(&(surveys->answer_two));
        fprintf(fp, "\t(b) - ");
        fputs(&(surveys->answer_two), fp);
        fputs("\n", fp);

        printf("    (c) - answer: ");
        gets(&(surveys->answer_three));
        fprintf(fp, "\t(c) - ");
        fputs(&(surveys->answer_three), fp);
        fputs("\n", fp);

        printf("    (d) - answer: ");
        gets(&(surveys->answer_four));
        fprintf(fp, "\t(d) - ");
        fputs(&(surveys->answer_four), fp);
        fputs("\n", fp);

        y = 'Y';
        n = 'N';
        b = 2;
        do {
            printf("Want to put it in the final survey?(Y/N): ");
            gets(&(surveys->vegleges));
            c = (surveys->vegleges);
            b = c != n;
            b += c != y;
        } while (b == 2);
        fputs(&(surveys->vegleges), fp);
        fputs("\n", fp);

        break;
    }
	free(c_time_string);
    fclose(fp);
}

void edit_survey(Survey* surveys)
{
    FILE* fp;
    fp = fopen("surveys.txt", "r");
    char line[N];
    int i_kerdes[K]; 
    int i = 0;
    int db = 1; 
    for (i = 0; i < K; ++i) {
        i_kerdes[i] = 0;
    }
    i = 0;
    while (fgets(line, N, fp) != NULL) {
        if (checker(line, "question: ", 7) > 0) {
            printf("%i. ", i);
            printf("%s", line);
            
            ++i;
        }
        i_kerdes[i] = ++db;
    }

    long menu;
    do {
        printf("Select a question by index: ");
        scanf("%i", &menu);
        if (menu < 0 || menu > i - 1)
            printf("Please select a correct number!\n");
    } while (menu < 0 || menu > i - 1);
    

    char question[N];
    printf("New question: ");
    gets(&question);
    gets(&question);
    fclose(fp);

    fp = fopen("surveys.txt", "r");
    FILE* fp_w;
    fp_w = fopen("surveys_2.txt", "w");

    line[N];
    int line_counter = 1;
    while (fgets(line, N, fp) != NULL && line_counter < db) {
        if (checker(line, "question: ", 7) > 0 && line_counter == i_kerdes[menu]) {
            fputs("Question: ", fp_w);
            fputs(&question, fp_w);
            fputs("\n", fp_w);
        }
        else {
            fputs(&line, fp_w);
        }
        line_counter++;
    }

    fclose(fp_w);
    fclose(fp);

    remove("surveys.txt");
    rename("surveys_2.txt", "surveys.txt");
}

void edit_answers(Survey* surveys)
{
    FILE* fp;
    fp = fopen("surveys.txt", "r");
    char line[N];
    int i_kerdes[K]; 
    int answer_numbers[K];
    int i = 0;
    int db = 1; 
    for (i = 0; i < K; ++i) {
        answer_numbers[i] = 0;
        i_kerdes[i] = 0;
    }
    i = 0;
    int menuok = 0;
    while (fgets(line, N, fp) != NULL) {
        if (checker(line, "question: ", 7) > 0) {
            printf("%i. ", i);
            printf("%s", line);
            
            ++i;
            menuok = 1;
        }
        if (checker(line, "\t(", 2) > 0) {
            printf("%s", line);
            answer_numbers[i - 1] = menuok++;
        }
        i_kerdes[i] = ++db;
    }

    int answer_number = 0;
    int kerdes_szama;
    char answer_letter;
    do {
        printf("Pick a number by index \n");
        printf("    question: ");
        scanf("%i", &kerdes_szama);
        int kerdes_szam = kerdes_szama;
        
        printf("    menu: ");
        scanf("%c", &answer_letter);
        scanf("%c", &answer_letter);
        
        if (kerdes_szama < 0 || kerdes_szama > db - 1) {
            printf("Please give a correct number..\n");
            answer_number = 0;
        }
        else {
            
            switch (answer_numbers[kerdes_szama]) {
            case 2:
                if (answer_letter == 'a')
                    answer_number = 1;
                if (answer_letter == 'b')
                    answer_number = 2;
                break;
            case 3:
                if (answer_letter == 'a')
                    answer_number = 1;
                if (answer_letter == 'b')
                    answer_number = 2;
                if (answer_letter == 'c')
                    answer_number = 3;
                break;
            case 4:
                if (answer_letter == 'a')
                    answer_number = 1;
                if (answer_letter == 'b')
                    answer_number = 2;
                if (answer_letter == 'c')
                    answer_number = 3;
                if (answer_letter == 'd')
                    answer_number = 4;
                break;
            }
            if (answer_number < 1) {
                printf("Pick a number from 1 to 4!");
                answer_number = 0;
            }
        }
    } while (answer_number == 0);

    char menu[N];
    printf("New answer: ");
    gets(&menu);
    gets(&menu);

    fclose(fp);
    
    
    fp = fopen("surveys.txt", "r");
    FILE* fp_w;
    fp_w = fopen("surveys_2.txt", "w+");

    int line_counter = 1;
    while (fgets(line, N, fp) != NULL && line_counter < db) {
        if (line_counter == i_kerdes[kerdes_szama] + answer_number) {
            fprintf(fp_w, "	(%c) - %s\n", answer_letter, &menu);
        }
        else {
            fputs(&line, fp_w);
        }
        line_counter++;
    }

    fclose(fp_w);
    fclose(fp);

    remove("surveys.txt");
    rename("surveys_2.txt", "surveys.txt");
}

void delete_question(Survey* surveys)
{
    FILE* fp;
    fp = fopen("surveys.txt", "r");
    char line[N];
    int i_kerdes[K]; 
    int answer_numbers[K];
    int i = 0;
    int db = 1; 
    for (i = 0; i < K; ++i) {
        answer_numbers[i] = 0;
        i_kerdes[i] = 0;
    }
    i = 0;
    int menuok = 0;
    while (fgets(line, N, fp) != NULL) {
        if (checker(line, "question: ", 7) > 0) {
            printf("%i. ", i);
            printf("%s", line);
            
            ++i;
            menuok = 1;
        }
        if (checker(line, "\t(", 2) > 0) {
            
            answer_numbers[i - 1] = menuok++;
        }
        i_kerdes[i] = ++db;
    }

    int answer_number = 1;
    int kerdes_szama;
    char answer_letter;
    do {
        printf("Pick a number by index \n");
        printf("    question: ");
        scanf("%i", &kerdes_szama);
        int kerdes_szam = kerdes_szama;
        if (kerdes_szama < 0 || kerdes_szama > db - 1) {
            printf("Please give a correct number..\n");
            answer_number = 0;
        }
    } while (answer_number == 0);

    fclose(fp);
    
    
    fp = fopen("surveys.txt", "r");
    FILE* fp_w;
    fp_w = fopen("surveys_2.txt", "w+");

    int line_counter = 1;
    while (fgets(line, N, fp) != NULL && line_counter < db) {
        if (line_counter > i_kerdes[kerdes_szama] - 2 && line_counter < i_kerdes[kerdes_szama] + answer_numbers[kerdes_szama] + 2) {
        }
        else {
            fputs(&line, fp_w);
        }
        line_counter++;
    }

    fclose(fp_w);
    fclose(fp);

    remove("surveys.txt");
    rename("surveys_2.txt", "surveys.txt");
}

void confirm_survey(Survey* surveys)
{
    FILE* fp;
    fp = fopen("surveys.txt", "r");
    char line[N];
    int i_kerdes[K]; 
    int answer_numbers[K];
    int i = 0;
    int db = 1; 
    for (i = 0; i < K; ++i) {
        i_kerdes[i] = 0;
    }
    i = 0;
    int menuok = 0;
    while (fgets(line, N, fp) != NULL) {
        if (checker(line, "question: ", 7) > 0) {
            printf("%i. ", i);
            printf("%s", line);
            
            ++i;
            menuok = 0;
        }
        else if (checker(line, "N", 1) > 0 || checker(line, "Y", 1) > 0) {
            printf("Confirmed: %s", line);
        }
        else if (checker(line, "\t(", 2) > 0) {
            printf("%s", line);
            answer_numbers[i - 1] = ++menuok;
        }
        i_kerdes[i] = ++db;
    }

    int sorszam_menu = 0;
    do {
        printf("answer: ");
        scanf("%i", &sorszam_menu);
        if (sorszam_menu < 0 || sorszam_menu > i - 1)
            printf("Please give a correct number..\n");
    } while (sorszam_menu < 0 || sorszam_menu > i - 1);
    

    int sorszam = sorszam_menu;
    char menu[1];
    char y = 'Y';
    char n = 'N';
    char c[1];
    do {
        printf("Do you want this question on the final survey?(Y/N)");
        scanf("%s", &menu);
        c[0] = menu[0];
    } while (checker(c, "Y ", 1) > 0 || checker(c, "N ", 1) > 0);

    fclose(fp);

    fp = fopen("surveys.txt", "r");
    FILE* fp_w;
    fp_w = fopen("surveys_2.txt", "w");

    line[N];
    int line_counter = 1;
    while (fgets(line, N, fp) != NULL && line_counter < db) {
        
        if ((checker(line, "Y", 1) > 0 || checker(line, "N", 1) > 0) && line_counter == (i_kerdes[sorszam] + answer_numbers[sorszam] + 1)) {
            fprintf(fp_w, "%s\n", &menu);
        }
        else {
            fputs(&line, fp_w);
        }
        line_counter++;
    }

    fclose(fp_w);
    fclose(fp);

    remove("surveys.txt");
    rename("surveys_2.txt", "surveys.txt");
	
	
}

int confirmed_survey(Survey* surveys)
{
    FILE* fp;
    fp = fopen("surveys.txt", "r");
    int db = 1; 
    int menuok = 0;
    char line[N];
    while (fgets(line, N, fp) != NULL) {
        ++db;
    }
    fclose(fp);

    fp = fopen("surveys.txt", "r");
    FILE* fp_w;
    fp_w = fopen("confirm_surveys.txt", "w+");

    
    int line_counter = 1;
    
    char vegleges[S][K];
    int i = 0;
    int j = 0;
	int kerdes_szam = 0;
    while (fgets(line, N, fp) != NULL && line_counter < db) {
        strcpy(vegleges[i], line);
        if (checker(line, "Y", 1) > 0) {
            for (j = 0; j < i; ++j) {
                fputs(&vegleges[j], fp_w);
                
            }
			kerdes_szam++;
            i = 0;
        }
        else if (checker(line, "N", 1) > 0) {
			
            i = 0;
        }
        else {
            i++;
            line_counter++;
        }
    }

    fclose(fp_w);
    fclose(fp);
	printf("DB question : %i \n", kerdes_szam);
	
	return kerdes_szam;
}

void chosen_questions(int random1, int random2, int random3) {
	FILE* fp;
	fp = fopen("confirm_surveys.txt", "r");
    FILE* fp_w;
	fp_w = fopen("confirmed_survey.txt", "w+");
	char line[N];
	int i = 0;
	while (fgets(line, N, fp) != NULL) {
		if (checker(line, "2", 1) > 0) ++i;
		if ( random1 == i) {
			fputs(&line, fp_w);
		}
		if ( random2 == i) {
			fputs(&line, fp_w);
		}
		if ( random3 == i) {
			fputs(&line, fp_w);
		}
		
	}
	fclose(fp_w);
    fclose(fp);
}

void write_confirmed_survey(int random1, char question[]) {
	FILE* fp;
	fp = fopen("confirm_surveys.txt", "r");
	char line[N];
	int i = 0;
	while (fgets(line, N, fp) != NULL) {
		if ( random1 == i) {
			strcpy(question, line);
			break;
		}
		if (checker(line, "2", 1) > 0) ++i;
	}
    fclose(fp);
}

void people_answer(int question, int answer[]){
	
	
	FILE* fp_w;
	fp_w = fopen("confirmed_survey.txt", "r");
	char line[N];
	int i = 0; 
	int db = -1; 
	while (fgets(line, N, fp_w) != NULL) {
		if (question < i) break;
		if (checker(line, "2", 1) > 0){
			++i;
		}else if( question == i) {
			db++;
		}
		
	}
	
	
	for ( i = 0; i < db; ++i){
		if (answer[i] == -1) answer[i] = 0;
	}
	
	
	
	
	srand(time(NULL));
	int p = rand()%db;
	answer[p]++;
	
	
	fclose(fp_w);
}

int * random_numbers(int question_number){
	int numbers[3];
	numbers[0] = rand()%question_number + 1;
	while(numbers[1] == numbers[0] || numbers[2] == numbers[0] || numbers[1] == numbers[2]){
		numbers[1] = rand()%question_number + 1;
		numbers[2] = rand()%question_number + 1;
	}
	
	return numbers;
}

int main(int argc, char* argv[])
{
	unlink("fifo.ftc");
	int fd=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); 
	if (fd==-1)
    {
		printf("Error number: %i",errno);
		exit(EXIT_FAILURE);
    }
	unlink("fifo2.ftc");
	int fd2=mkfifo("fifo2.ftc", S_IRUSR|S_IWUSR ); 
	if (fd2==-1)
    {
		printf("Error number: %i",errno);
		exit(EXIT_FAILURE);
    }
	
	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);
	char sz[100];
	
	pid_t pid = fork();
	if (pid == 0) { 
		
		pause(); 
		
		printf("Parent is ready! But child is not!\n");
		
		sleep(1);
		
		printf("Child sends the sign that I arrived!\n",SIGUSR1);
		kill(getppid(),SIGUSR1);
		
		
		int random1 = 0;
		int random2 = 0;
		int random3 = 0;
		
		fd=open("fifo.ftc",O_RDONLY);
		read(fd,&random1,sizeof(random1));
		read(fd,&random2,sizeof(random2));
		read(fd,&random3,sizeof(random3));
		
		chosen_questions(random1,random2,random3);
		
		srand(time(NULL));
		int people = rand()%10 +10;
		
		int answer_1[4] = {-1,-1,-1,-1};
		int answer_2[4] = {-1,-1,-1,-1};
		int answer_3[4] = {-1,-1,-1,-1};
		
		int k = 0;
		while (people > k){
			people_answer(1, answer_1);
			people_answer(2, answer_2);
			people_answer(3, answer_3);
			++k;
		}
		
		int j;
		
		printf("First question's answers :\n");
		for (j = 0; j < 4; ++j){
			if (answer_1[j] != -1)
				printf("%i : %d\n",j,answer_1[j]);
		}
		printf("\n");
		printf("Second question's answers :\n");
		for (j = 0; j < 4; ++j){
			if(answer_2[j] != -1)
				printf("%i : %d\n",j,answer_2[j]);
		}
		printf("Third question's answers :\n");
		for (j = 0; j < 4; ++j){
			if(answer_3[j] != -1)
				printf("%i : %d\n",j,answer_3[j]);
		}
		close(fd);
		
		fd2=open("fifo2.ftc",O_WRONLY);
		
		for (j = 0; j < 4; ++j){
				write(fd2,&answer_1[j],sizeof(answer_1[j]));
		}
		for (j = 0; j < 4; ++j){
				write(fd2,&answer_2[j],sizeof(answer_2[j]));
		}
		for (j = 0; j < 4; ++j){
				write(fd2,&answer_3[j],sizeof(answer_3[j]));
		}
		close(fd2);

	}else{
		int menu;
		Survey* surveys = (Survey*)malloc(K * sizeof(Survey));
		surveys->id = (char*)malloc(N * sizeof(Survey));
		surveys->question = (char*)malloc(N * sizeof(Survey));
		surveys->answer_one = (char*)malloc(N * sizeof(Survey));
		surveys->answer_two = (char*)malloc(N * sizeof(Survey));
		surveys->answer_three = (char*)malloc(N * sizeof(Survey));
		surveys->answer_four = (char*)malloc(N * sizeof(Survey));
		surveys->vegleges = (char*)malloc(N * sizeof(Survey));
		
		int question_number = -1;
		int v = 1;
		do {
			printf("--------------------- M L SZ ---------------------\n");
			printf("	(1) - Create survey \n");
			printf("	(2) - List survey \n");
			printf("	(3) - New question \n");
			printf("	(4) - Edit question \n");
			printf("	(5) - Edit answers \n");
			printf("	(6) - Delete question \n");
			printf("	(7) - Confirm survey \n");
			printf("	(8) - Create confirmed survey \n");
			printf("	(0) - Quit \n");

			scanf("%i", &menu);
			switch (menu) {
			case 1:
				create_survey(surveys);
				break;
			case 2:
				survey_list(surveys);
				break;
			case 3:
				add_question(surveys);
				break;
			case 4:
				edit_survey(surveys);
				break;
			case 5:
				edit_answers(surveys);
				break;
			case 6:
				delete_question(surveys);
				break;
			case 7:
				confirm_survey(surveys);
				break;
			case 8:
				question_number = confirmed_survey(surveys);
				printf("Kapott szam : %i\n",question_number);
				v = 0;
				break;
			case 0:
				
				break;
			}
		}while (menu != 0);
		free(surveys->id);
		free(surveys->question);
		free(surveys->answer_one);
		free(surveys->answer_two);
		free(surveys->answer_three);
		free(surveys->answer_four);
		free(surveys->vegleges);
		free(surveys);

		printf("PARENT sends the message that I'm ready!\n",SIGUSR2);
		kill(pid,SIGUSR2);

		pause();
		printf("Men arrived! So Parent sends three question\n",SIGUSR1);

		fd=open("fifo.ftc",O_WRONLY);
	
		srand(time(NULL));
		int* randoms = random_numbers(question_number);
		int rnd1 = randoms[0];
		int rnd2 = randoms[1];
		int rnd3= randoms[2];
		
        write(fd,&rnd1,sizeof(rnd1));
        write(fd,&rnd2,sizeof(rnd2));
        write(fd,&rnd3,sizeof(rnd3));
		
		close(fd);
		
		fd2=open("fifo2.ftc",O_RDONLY);
		char a[4] = {'a','b','c','d'};
		int b,j;
		printf("Elso question re adott menu :\n");
		char question[N];
		write_confirmed_survey(rnd1, question);
		printf("%s ",question);
		
		for (j = 0; j < 4; ++j){
			read(fd2,&b,sizeof(b));
			printf("%c : %i\n",a[j],b);
		}
		printf("Masodik question re adott menu :\n");
		write_confirmed_survey(rnd2, question);
		printf("%s ",question);
		for (j = 0; j < 4; ++j){
			read(fd2,&b,sizeof(b));
			printf("%c : %i\n",a[j],b);
		}
		printf("Harmadik question re adott menu :\n");
		write_confirmed_survey(rnd3, question);
		printf("%s ",question);
		for (j = 0; j < 4; ++j){
			read(fd2,&b,sizeof(b));
			printf("%c : %i\n",a[j],b);
		}
		
		close(fd2);	
		printf("\nMLSZ exit........\n");
	}

    return 0;
}
