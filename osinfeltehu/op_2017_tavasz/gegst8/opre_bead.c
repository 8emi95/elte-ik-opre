#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio_ext.h>

#define MAX_QUESTIONS 20
#define QUESTION_LENGHT 200
#define OPTION_LENGHT 100

#define TRUE 1

struct Answer{

    char answer[OPTION_LENGHT];
    int length;
    int votes;

};

struct Question{

    char question[QUESTION_LENGHT];
    int num_of_answers;
    int length;
    struct Answer answers[4];
    char time[80];

};

void handler(int signumber){
  signal(SIGTERM,SIG_DFL);
}


void init(struct Question* q){
    int i = 0;
    int j = 0;
    while( i < MAX_QUESTIONS){
        q[i].length = 0;
        q[i].num_of_answers = 2;
        j = 0;
        while(j < 4){
            q[i].answers[j].length = 0;
            q[i].answers[j].votes = 0;
            j++;
        }
        i++;
    }
}

void setTime(char* date){
    time_t     now;
    struct tm *ts;
    now = time(NULL);

    ts = localtime(&now);
    strftime(date, 80, "%a %Y-%m-%d %H:%M:%S %Z", ts);
    puts(date);
}

void print_entity(struct Question* q){
    printf("--------------------------------\nkerdes: %s letrejott: %s\n", q->question, q->time);
    int i = 0;
    while( i < q->num_of_answers ){
        printf("Valasz %d: %s ra erkezett szavazat: %d\n",i+1,q->answers[i].answer,q->answers[i].votes);

        i++;
    }
    printf("--------------------------------\n");
}

int read_string_from_stdin(char* q){
    int c = 0;
    int char_pos = 0;
    __fpurge(stdin);
    //rewind(stdin);
    while ((c = getchar()) != EOF && c != '\n'){
        q[char_pos] = c;
        char_pos++;
    }
    q[char_pos] = '\0';

    return char_pos;
}


void fillAnswers(struct Answer* ans, int* ans_count){
    printf("Kerem adja meg a valaszok szamat! \n");
    //clear_buffer();
    int c;
    scanf("%d",&c);
    while( c > 4 || c < 2 ){
        printf("valaszok szama nem megfelelo adjon meg [2,4] intervallumban! %d \n",c);
        scanf("%d",&c);
    }
    printf("valaszok szama: %d\n",c);

    int i = 0;
    while( i < c ){
        printf("Valasz %d: ",i + 1);
        //clear_buffer();
        ans[i].length = read_string_from_stdin(ans[i].answer);
        printf("%d\n",ans[i].length);
        i++;
    }
    *ans_count = i;
}

void newQuestion(struct Question* q, int* noq){
    printf("Kerem adja meg a kerdest majd usse le az ENTER-t\n");
    //clear_buffer();
    q[*noq].length = read_string_from_stdin(q[*noq].question);
    //clear_buffer();
    fillAnswers(q[*noq].answers, &(q[*noq].num_of_answers));
    setTime(q[*noq].time);
    printf("kerdesek szama: %d", q[*noq].num_of_answers);
    (*noq)++;
}

void printAll(struct Question* q, int size){
    int i = 0;
    while(i < size){
        printf("%d. ",i+1);
        print_entity(&(q[i]));
        i++;
    }
}

int changeStuff(struct Question* q){
    printf("Kerem valasszon melyik kerdest akarja megvaltoztatni!\n");
    int i = -1;
    scanf("%d",&i);

    printf("Kerem valasszon mit szeretne valtoztatni!\n Kerdes: 1. Valaszok: 2.");
    int c = 0;
    scanf("%d",&c);
    if(c == 1){

        printf("Kerem adja meg az uj kerdest!\n");
        q[i-1].length = read_string_from_stdin(q[i-1].question);
        return 1;

    } else if(c == 2){

        printf("Kerem adja meg melyik valaszon szeretne valtoztatni!\n");
        int e = 0;
        scanf("%d",&e);

        if(e < q[i-1].num_of_answers){
            printf("Kerem adja meg az uj valaszt!\n");
            q[i-1].answers[e-1].length = read_string_from_stdin(q[i-1].answers[e-1].answer);
            return 1;
        }

        printf("Nem megfelelo valasztas kezdje ujra!");
        return 0;

    } else {

        printf("Nem megfelelo valasztas kezdje ujra!");
        return 0;
    }
}

void printMenu(){
    write(1,"\n",1);
		write(1,"Magyar Labdajatekok Szervezete\n",31);
		write(1,"Kerdoiv\n\n",9);
		write(1,"1. Uj kerdes felvetele\n",23);
		write(1,"2. Kerdesek listazasa\n",22);
		write(1,"3. Kerdes modositasa\n",21);
		write(1,"4. Kerdes torlese\n",18);
		write(1,"5. Veglegesites es filebairas\n",30);
		write(1,"6. Kerdezobiztos kikuldese\n",27);
		//
		write(1,"7. Kilepes\n",11);
		write(1,"Valasszon a menubol: ",21);
}

int get_menu_input(){
    int i = 0;
    scanf("%d",&i);
    while( i > 7 || i < 1 ){
        printf("Nem megfelelo szam valasszon a [1,7] intervallumbol! input: %d\n",i);
        scanf("%d",&i);
    }
    return i;
}

void writeToFile(struct Question* q, int size){
    int filedesc = open("testfile.txt", O_WRONLY | O_TRUNC);
    if(filedesc < 0){
        printf("Hiba tortent file megnyitaskor probaslja ujra!");
        return;
    }
    char end = '\n';
    char sep = '|';
    int i = 0;
    int j;
    while( i < size ) {
        write(filedesc,q[i].question,q[i].length);
        write(filedesc,&sep,1);
        write(filedesc,q[i].time,sizeof(q[i].time));
        write(filedesc,&end,1);
        j = 0;
        while( j < q[i].num_of_answers ) {
            write(filedesc,q[i].answers[j].answer,q[i].answers[j].length);
            write(filedesc,&end,1);
            j++;
        }
        i++;
    }


}

void deleteQuestion(struct Question* q,int* size){
    printf("Kerem adja meg hanyadik kerdest szeretne torolni: \n");
    int i;
    scanf("%d",&i);

    if(i > *size || i < 1){
        printf("Nem megfelelo indexet adott meg: %d\n", i);
        return;
    }

    q[i-1] = q[(*size)-1];
    (*size)--;

}

void doDaVotin(struct Question* q, int ppl){
    int i = 0;

    while( i < ppl){
        q[0].answers[rand() % q[0].num_of_answers].votes++;
		q[1].answers[rand() % q[1].num_of_answers].votes++;
		q[2].answers[rand() % q[2].num_of_answers].votes++;
        i++;
    }
}


void vote(struct Question* q, int size, int argc, char *argv[]){
    //struct Question a[3];
    //chooseRandom(q,size,a);

    signal(SIGTERM,handler);
	
	srand(time(NULL));
	
	int pipefd1[2];
	int pipefd2[2];
	int pid;
	
	pipe(pipefd1);
	pipe(pipefd2);
	
	pid = fork();
	if (pid == 0){
		printf("Kerdezobiztos: Utazas a helyszinre(%s)...\n", argv[1]);
		sleep(2);
		printf("Kerdezobiztos: Megerkeztem a helyszinre\n");
		printf("Kerdezobiztos: Varom a kerdeseket\n");
		kill(getppid(),SIGTERM);
		
		close(pipefd1[1]);
		close(pipefd2[0]);
		
		struct Question a[3];
		read(pipefd1[0], &a[0], sizeof(struct Question));
		read(pipefd1[0], &a[1], sizeof(struct Question));
		read(pipefd1[0], &a[2], sizeof(struct Question));
		
		close(pipefd1[0]);
		
		printf("Kerdezobiztos: Megkaptam a kerdeseket: \n");

        print_entity(&a[0]);
        print_entity(&a[1]);
        print_entity(&a[2]);

		int people = (rand() % 11) + 10;
		printf("Kerdezobiztos: Kerdoiv kitoltese(%d ember)...\n",people);
		sleep(2);

		doDaVotin(a,people);
		
		printf("Kerdezobiztos: Kerdoiv visszakuldese\n");
		
		write(pipefd2[1], &a[0], sizeof(struct Question));
		write(pipefd2[1], &a[1], sizeof(struct Question));
		write(pipefd2[1], &a[2], sizeof(struct Question));
		
		close(pipefd2[1]);
		
		exit(0);
	} else {
		pause();
		printf("MLSZ: Kerdessor kuldese\n");
		
		close(pipefd1[0]);
		close(pipefd2[1]);
		
		write(pipefd1[1], &(q[0]), sizeof(struct Question));
		write(pipefd1[1], &(q[1]), sizeof(struct Question));
		write(pipefd1[1], &(q[2]), sizeof(struct Question));
		
		close(pipefd1[1]);
		
		read(pipefd2[0], &(q[0]), sizeof(struct Question));
		read(pipefd2[0], &(q[1]), sizeof(struct Question));
		read(pipefd2[0], &(q[2]), sizeof(struct Question));
		
		close(pipefd2[0]);
		
		printf("MLSZ: Megkaptam a kerdoivet\n");
		printf("MLSZ: Az eredmeny:\n");

        print_entity(&(q[0]));
        print_entity(&(q[1]));
        print_entity(&(q[2]));

    }
}

int main(int argc, char *argv[]){

    //inicializ�cio
    struct Question questions[MAX_QUESTIONS];
    int number_of_questions = 0;
    init(questions);

    for(;;){
        printMenu();
        switch(get_menu_input()){
        case 1:

            newQuestion(questions,&number_of_questions);
            break;
        case 2:
            printAll(questions,number_of_questions);
            break;
        case 3:
            while(changeStuff(questions) != TRUE){};
            break;
        case 4:
            deleteQuestion(questions,&number_of_questions);
            break;
        case 5:
            writeToFile(questions,number_of_questions);
            break;
        case 6:
            vote(questions,number_of_questions,argc,argv);
            break;
        default:
            return 0;
        }
    }
    //return 0;
}
