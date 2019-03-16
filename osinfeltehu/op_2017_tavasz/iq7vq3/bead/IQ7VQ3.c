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


struct Question{
	char question[100];
	char Answers[MAXANSWERS][100];
	int aCount;
        time_t tm;
        bool Commited;
};

int QuestionCount = 0;
struct Question Questions[QUESTIONLIMIT];

void NewAnswer();
void EditQuestion();
void DeleteQuestion();
void List();
void SaveFile();
void LoadFile();
void CommitQuestions();

int main()
{
LoadFile();
	 printf("\nÜdvözöljük az MLSZ nemzeti konzltaciojanak kerdoiv szerkeszto alkalmazasaban!\n");
	 printf("(Kerem vegye figyelembe hogy a program legfeljebb 100 kerdes felvetelere kepes.)\n");
    char menu[10];
    do {
        printf("\nKerem válasszon az alabbi menupontok kozul:\n");
        printf("1, Uj kerdes felvetele.\n");
        printf("2, Kerdes modositasa.\n");
        printf("3, Kerdes torlese.\n");
        printf("4, Kerdesek listazasa.\n");
        printf("5, Kerdoiv kerdeseinek kivalasztasa.\n\n");
        printf("0, Kilepes!\n");
        scanf("%s",&menu);
        switch(menu[0]){
            case '1' : 
               NewAnswer();
                break;
            case '2' : 
                EditQuestion();
                break;
            case '3' : 
                DeleteQuestion();
                break;
            case '4' : 
                List();
                break;
            case '5' : 
                CommitQuestions();
                break;
        }

    }while (*menu!='0');
    SaveFile();
    return 1;
}

void NewAnswer(){
        Questions[QuestionCount].Commited = false;
        time(&Questions[QuestionCount].tm);
	char question[100];
	int aCount;
	printf("\nGepelje be a kerdest: ");
	fgets(question, 100, stdin); 
        scanf("%[^\n]*s", question);
	strcpy(Questions[QuestionCount].question, question);
	do{
		printf("Adja meg hany darab valasz lesz a kerdeshez (minimum %d, legfeljebb %d): ",MINANSWERS,MAXANSWERS);
		scanf("%d",&aCount);
	}while(aCount<MINANSWERS || aCount>MAXANSWERS);
	Questions[QuestionCount].aCount=aCount;
	int i=0;
	for(i; i<aCount;i++){
		//char tmp[100];
		printf("Kerem adja meg a(z) %d. valaszt: ",i+1);
		fgets(Questions[QuestionCount].Answers[i], 100, stdin); 
		scanf("%[^\n]*s", Questions[QuestionCount].Answers[i]);
	}	
	++QuestionCount;
}

void EditQuestion(){
	printf("\n");
	char tmp[10];
	int id;
	int i=0;
	List();
	do{
		printf("Valassza ki a modositani kivant kerdest: ");
		scanf("%d",&id);
	}while(id< 1 || id > QuestionCount);
	--id;
	printf("A kerdest vagy a valaszt szeretne modositani(k/v): ");
	scanf("%s",&tmp);
	if(tmp[0] == 'k'){
		printf("A valasztott kerdes: %s\n",Questions[id].question);
		printf("A modositani kivant kerdes: ");
		fgets(Questions[id].question, 100, stdin); 
		scanf("%[^\n]*s", Questions[id].question);
	}
	if(tmp[0] == 'v'){
		int aid;
		i= 0;
		for(i; i<Questions[id].aCount;i++){
			printf("%d. valasz: %s\n",i+1,Questions[id].Answers[i]);
		}
		do{
			printf("A valassza ki melyik valaszt szeretne modositani: ");
			scanf("%d",&aid);
		}while(aid<MINANSWERS || aid > Questions[id].aCount);
		printf("A modositani kivant valasz: ");
		fgets(Questions[id].Answers[--aid], 100, stdin); 
		scanf("%[^\n]*s", Questions[id].Answers[--aid]);
	}
}

void DeleteQuestion(){
	int id;
	int i=0;
	List();
	do{
		printf("Valassza ki a torolni kivant kerdest: ");
		scanf("%d",&id);
	}while(id< 1 || id > QuestionCount);
        Questions[--id]=Questions[--QuestionCount];
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
void CommitQuestions(){
	List();
        printf("A kerdoiv kerdeseinek veglegesitehez valassza ki a kerdes sorszamat.\nVeglegesitett kerdes ujboli kivalasztasa eseten visszavonhatja a veglegesitest.\nA veglegesites befejezesehez valassza a 0 sorszamot.\n");
        int id;
        do{
                printf("-> ");
        	scanf("%d",&id);
                if(id-->QuestionCount)
                    printf("Hibas azonosito");
                Questions[id].Commited = !Questions[id].Commited;
        }while(id != -1);
}

void SaveFile(){
    FILE* fp;
    fp = fopen("tmp.txt", "w");
    
    if (fp == NULL){
        printf("Hiba a mentesnel!!!");
        return;
    }
    
    fwrite( &QuestionCount, sizeof(int), 1, fp);
    
    int i;
    for (i = 0; i < QuestionCount; i++) {
        fwrite(&Questions[i], sizeof(struct Question), 1, fp);       
    }
    
    fclose(fp);
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