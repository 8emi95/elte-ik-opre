#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>  // for errno, the number of last error
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>

#define QUESTIONLIMIT 100
#define MAXANSWERS 4
#define MINANSWERS 2

void handler(int signumber){}

struct Question{
	char question[100];
	char Answers[MAXANSWERS][100];
	int aCount;
	time_t tm;
	bool Commited; //felhasználjuk a kérdések kiválasztására
};

int QuestionCount = 0;
struct Question Questions[QUESTIONLIMIT];

void List();
void LoadFile();


int main(int argc,char ** argv)
{
	if(argc != 2){
		printf("\nNem adta meg melyik varosba keszuljon a felmeres.\n");
		return 1;
	}
	LoadFile();
	int i=0;
	for(i; i<QuestionCount;i++){
		Questions[i].Commited = false;
	}
	signal(SIGRTMAX,handler);
	printf("\nÜdvözöljük az MLSZ nemzeti konzltaciojan!\n");
	int fd1;
	unlink("pipe");
    int fid1=mkfifo("pipe", S_IRUSR|S_IWUSR );
	if (fid1==-1){
        printf("Error number: %i",errno); 
        exit(EXIT_FAILURE);
    }	
	srand(time(NULL)); //the starting value of random number generation
	pid_t pid = fork();
	
	if(pid>0){ //parent
		pause(); //waits till a signal arrive 
		printf("A kerdezobiztos megerkezett a(z) %s varos labdajáték centrumba\n",argv[1],SIGRTMAX);
		int questionCount = 0;		
		do
		{
			int r=rand()%QuestionCount;
			if(!Questions[r].Commited){
				Questions[r].Commited = true;
				++questionCount;
			}
		}while(questionCount!=3);
		fd1=open("pipe",O_WRONLY);
		write(fd1,Questions,sizeof(Questions));
		close(fd1);
		fd1=open("pipe",O_RDONLY);
		struct Question AnsweredQuestions[QUESTIONLIMIT];
		read(fd1,AnsweredQuestions,sizeof(AnsweredQuestions));
		close(fd1);		
		
		i = 0;
		for(i; i<QuestionCount;i++){
			if(AnsweredQuestions[i].Commited){
				printf("A(z) %d. kerdesre a kitoltok valaszai: %s \n    Valaszok:\n",i+1,Questions[i].question);
				int j=0;
				for(j; j<AnsweredQuestions[i].aCount;j++){
					printf("\t -%-15s \t\t%s db valaszado\n",Questions[i].Answers[j],AnsweredQuestions[i].Answers[j]);
				}
			}
		}
		
	}
	else
	{
		kill(getppid(),SIGRTMAX); 
		fd1=open("pipe",O_RDONLY);
		struct Question SelectedQuestions[QUESTIONLIMIT];
		read(fd1,SelectedQuestions,sizeof(SelectedQuestions));
		close(fd1);		
		int respondentsNo = 10 + (rand() % 11);
		int i = 0;
		for(i; i<QuestionCount;i++){
			int j =0;
			for(j; j<SelectedQuestions[i].aCount;j++){
				memset(&SelectedQuestions[i].Answers[j], 0, sizeof(SelectedQuestions[i].Answers[j]));
				sprintf(SelectedQuestions[i].Answers[j],"%d",0);
			}
		}
		i=0;
		for(i; i<QuestionCount;i++){
			if(SelectedQuestions[i].Commited){
				int k =0;
				for(k; k< respondentsNo;k++){
					int ans = rand() % SelectedQuestions[i].aCount;
					int count = atoi(SelectedQuestions[i].Answers[ans]);
					sprintf(SelectedQuestions[i].Answers[ans],"%d",++count);
				}
			}
		}
		fd1=open("pipe",O_WRONLY);
		write(fd1,SelectedQuestions,sizeof(SelectedQuestions));
		close(fd1);		
	}
    return 0;
}


void List(){
	printf("\n");
	int i=0;
	for(i; i<QuestionCount;i++){
		printf("A(z) %d. kerdes(veglegesitett: %s): %s \t hozzaadas datuma: %s",i+1,Questions[i].Commited ? "igen" : "nem",Questions[i].question,ctime(&Questions[i].tm));
		int j=0;
		for(j; j<Questions[i].aCount;j++){
			printf("\t -%s\n",Questions[i].Answers[j]);
		}
	}
}


void LoadFile(){
    FILE *fp;
    fp=fopen("tmp.txt", "r");
    
    if (fp == NULL){
        printf("Betoltes sikertelen!!");
        return;
    }
    
    fread(&QuestionCount, sizeof(int), 1, fp);
    int i;
    for (i = 0; i < QuestionCount; i++) {
        fread(&Questions[i], sizeof(struct Question), 1, fp);        
    }
    fclose(fp);
};