#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define QMAX 50
#define QTEXT_SIZE 512
#define ATEXT_SIZE 128
#define MAX 512

struct Question {
	char date[30];
	char text[QTEXT_SIZE];
	int number;
	char answer[4][ATEXT_SIZE];
};


int qnumber=0;
int qmax=QMAX;
struct Question* questions;

void handler(int signumber)
{ 
}


void newQuestion() {
		if ( qnumber == qmax ) {
			struct Question* newq = (struct Question*) malloc(sizeof(struct Question)*qmax*2);
			int i;
			qmax *= 2;
			for (i=0; i<qnumber; ++i){
				newq[i]=questions[i];
			}
			free(questions);
			questions=newq;
		}
		getchar();
		printf("A kerdes:\n");
		struct Question* question = &questions[qnumber];
		size_t qlength = 100;
		size_t alength = 50;
		char *buffer;
		buffer = malloc(QTEXT_SIZE);
		fgets(buffer,qlength,stdin);
		strcpy(questions[qnumber].text,buffer);
		fflush(stdin);
		question->number=0;
		char db[3];
		while (question->number < 2 || question->number > 4){
			printf("Adja meg a valaszok szamat: \n");
			//scanf("%d",&db);
			scanf("%s", &db);
			question->number = atoi(db);
		}
		question->number = atoi(db);
		getchar();
		int i;
		for ( i = 0 ; i<question->number ; ++i){
			printf("%d. valasz: ",(i+1));
			getline(&buffer, &alength, stdin);
			strcpy(questions[qnumber].answer[i],buffer);
		}
		time_t now = time(NULL);
		strftime(question->date, 30, "%Y-%m-%d %H:%M:%S", localtime(&now));
		qnumber++;
}

void listQuestions() {
    int i;
    for ( i = 0 ; i < qnumber ; ++i) {
        printf("\n%d. ",(i+1));
        printf(questions[i].text);
        int j;
        char c;
        for ( j = 0 ; j < questions[i].number ; ++j) {
            printf("%d. valasz ",j+1);
            printf(questions[i].answer[j]);
			printf("\n");
        }  
    }
}

void printQuestion(struct Question *q) {
	printf("A kerdes ");
    printf(q->text);printf("\n");
    int i;
    for ( i = 0; i < q->number ; ++i) {
        printf("A(z) %d. valasz ",(i+1));
        printf(q->answer[i]);
    }
}

void changeQuestion() {
    printf("Modositando kerdes sorszama: ");
    char db[1000];
	scanf("%s", &db);
    int i=atoi(db)-1;
	char c;
    if ( i < qnumber && i>=0){
        struct Question *question = &questions[i];
        printQuestion(question);
		printf("Megváltoztatja a kerdest?(i/n)\n");
		scanf("%c",&c);
		char* buffer;
		buffer = malloc(QTEXT_SIZE);
		if (c == 'i'){
			getchar();
			printf("Kerem a kerdest\n");
			fgets(buffer,QTEXT_SIZE,stdin);
			strcpy(questions[i].text,buffer);

		}
		int j;
		buffer = malloc(ATEXT_SIZE);
		for(j=0; j<question->number; ++j){
			printf("Megvaltoztaja %d. valaszt?(i/n)\n",(j+1));
			scanf(" %c",&c);
			if (c == 'i'){
				getchar();
				printf("Kerem a valaszt\n");
				fgets(buffer,ATEXT_SIZE,stdin);
				strcpy(questions[i].answer[j],buffer);
			}		
		}	
    }
    else {
        printf("Nincs ilyen kerdes!\n");
    }
}

void deleteQuestion() {
    char db[1000];
	printf("Hanyadik kerdest torli?\n");	
	scanf("%s", &db);
	int i;
	i = atoi(db);
	char c;
	while(i <0 || i > qnumber){
		printf("Hanyadik kerdest torli?\n");
		scanf("%s", &db);
		i=atoi(db);	
	}
	if(i==0) return;
	else{	
	printQuestion(&questions[i]);
	int j;	
	for(j=i-1 ; j < qnumber-1; j++) questions[j] = questions[j + 1];	
	struct Question *tmp = realloc(questions, (qnumber - 1) * sizeof(struct Question) );
	--qnumber;
	printf("A kérdés torolve\n");
	}
}

void finalizeQuestions(char* city){
		signal(SIGTERM,handler);
		char c;
		int f=open("survey.bin",O_WRONLY|O_CREAT|O_TRUNC);
		int i;
		int qs=0;
		for(i=0; i<qnumber; ++i){
			printQuestion(&questions[i]);
			printf("Veglegesiti a kerdest?(i/n)\n");
			scanf("%c", &c);
			if (c == 'i'){
				printf("A kerdes veglegesitve\n");
				write(f,&questions[i],sizeof(struct Question));
				qs=qs+1;
			}				
		}
		close(f);
		printf("A kerdessor veglegesitve\n");
		if(qs<0){
			printf("Tul keves kerdes\n");
			return;
		}  
		int pipefd[2];
		if (pipe(pipefd) < 0){
			perror("pipe error!");
			exit(1);
		  }

		pid_t child = fork();
		if (child < 0){
			perror ("fork error!");
			exit(-1);
		}

	    // parent process
	    if (0 > child) {
			sleep(1);
			kill(child, SIGTERM);
			pause();
			char buffer[MAX];
			write(pipefd[1], city, MAX);
			read(pipefd[0], buffer, MAX);

			srand(time(NULL));
			int rand1 = rand() % qnumber;
			int rand2 = rand() % qnumber;
			int rand3 = rand() % qnumber;
			struct Question q1= questions[rand1];
			struct Question q2= questions[rand2];
			struct Question q3= questions[rand3];
			
			int j;
			for(j=0;j<3;++j){
				write(pipefd[1], &q1, MAX);
				sleep(1);
				kill(child, SIGTERM);
				pause();
			}	
			
			int number;
			read(pipefd[1], &number, MAX);
			sleep(1);
			kill(getppid(), SIGTERM);
			int results[number][3];
			int k;
			read(pipefd[1], &results, MAX);
			sleep(1);
			kill(getppid(), SIGTERM);
			printf("A konzultacio eredmenye\n");
			for(i=0; i<number; i++){
				printf(questions[rand1].text);
				printf(questions[rand1].answer[results[i][0]]);
				printf(questions[rand2].text);
				printf(questions[rand2].answer[results[i][1]]);	
				printf(questions[rand3].text);
				printf(questions[rand3].answer[results[i][2]]);				
			}
		}		
		else{
			pause();
			sleep(1);
			kill(getppid(), SIGTERM);

			int h;
			struct Question MLSZqs[3];
			for(h=0; h<3; ++h){
				pause();			
				read(pipefd[0], &MLSZqs[h], MAX);
				sleep(1);
				kill(getppid(), SIGTERM);
			}
    
			int surveys = rand() % (11) + 10;
			int answers[surveys][3];
			
			int j;
			for (i = 0; i < surveys; i++){
				for	(j=0; j <3; j++){
					answers[i][j] = rand()% MLSZqs[j].number;
				}
				
			}
			write(pipefd[0], &surveys, MAX);
            kill(getppid(), SIGTERM);
            pause();			
			write(pipefd[0], &answers, MAX);
			kill(getppid(), SIGTERM);
            pause();
		}	
}

void printMenu(void) {
    printf("\n");
    printf("1 - Uj kerdes\n");
    printf("2 - Kerdes szerkesztese\n");
    printf("3 - Kerdes torlese\n");
    printf("4 - Kerdesek listazasa \n");
    printf("5 - Kerdessor veglegesitese\n");
    printf("0 - Kilepes\n");
}

int main(int argc, char **argv) {
questions = (struct Question*)malloc(qmax*sizeof(struct Question));
    int choice = 9;
    while (choice != 0) {
        printMenu();
        printf(" > ");
        scanf("%d",&choice);
        switch (choice) {
            case 1:
                newQuestion();
                break;
            case 2:
                changeQuestion();
                break;
            case 3:
                deleteQuestion();
                break;
            case 4:
                listQuestions();
                break;
            case 5:
				finalizeQuestions(argv[0]);
                break;
        }
    }

    return 0;
}
