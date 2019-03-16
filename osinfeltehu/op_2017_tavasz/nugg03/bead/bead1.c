#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct questions
{
 char question[160];
 char answers[4][100];
 int answerNumber;
 char creationDate[100];
};

struct questions qs[100];
int questionNumber = 0;
int scanfNewLineRemover;

void newLineToNull(char *line){
	char *pos;
	if ((pos=strchr(line, '\n')) != NULL){
    		*pos = '\0';
	}
}

int isNumber(char *input) {
  int i;
  for (i = 0; input[i] != '\0'; i++)
    if (isalpha(input[i]))
      return 0;
  return 1;
}

void addQuestion(){
	printf("Kérem adja meg az új kérdést!\n");
	fgets(qs[questionNumber].question, sizeof(qs[questionNumber].question), stdin);
	newLineToNull(qs[questionNumber].question);
	char tempqnumber[10];
	int qnumber=0;
	int found=0;
	do{
			printf("Kérem adja meg a válaszok számát!(2-4)\n");
			scanf("%s", &tempqnumber);
			if(isNumber(tempqnumber)){
				found=1;
				qnumber=atoi(tempqnumber);
			}
			while ( (scanfNewLineRemover = getchar()) != '\n' && scanfNewLineRemover != EOF );
		}while(!((qnumber == 2 || qnumber == 3 || qnumber == 4) && found==1));
	qs[questionNumber].answerNumber=qnumber;

	int i;
	for (i = 0; i < qs[questionNumber].answerNumber; ++i) {
		printf("Kérem adja meg az %d. válaszlehetőséget!\n",i+1);
		fgets(qs[questionNumber].answers[i], sizeof(qs[questionNumber].answers[i]), stdin);
		newLineToNull(qs[questionNumber].answers[i]);
	}

	time_t timer;
	time(&timer);
	strftime(qs[questionNumber].creationDate, 100, "%Y-%m-%d %H:%M:%S", localtime(&timer));
	questionNumber++;
} 

void listQuestions(){
  int i;
  for (i = 0; i < questionNumber; ++i) {
      printf("%d. Kérdés: \n%s\n", i+1, qs[i].question);
	  int j;
	  printf("Válaszok: \n");
	  for (j = 0; j < qs[i].answerNumber; ++j) {
		printf("%d. %s\n", j+1, qs[i].answers[j]);
	  }
	  printf("Létrehozva: \n%s\n", qs[i].creationDate);
  }
}

void deleteQuestion(){
	listQuestions();
	char tempqid[10];
	int qid=0;
	int found=0;
	do{
			printf("Hanyadik kérdést szeretné törölni?\n");
			scanf("%s", &tempqid);
			if(isNumber(tempqid)){
				found=1;
				qid=atoi(tempqid);
			}
			while ( (scanfNewLineRemover = getchar()) != '\n' && scanfNewLineRemover != EOF );
		}while(!(qid<=questionNumber && found==1 && qid!=0));
	int i;
	for (i = qid ; i < questionNumber; ++i) {
		qs[i - 1] = qs[i];
	}
	questionNumber--;
} 

void modifyQuestion(){
	listQuestions();
	char tempqid[10];
	int qid=0;
	int found=0;
	do{
			printf("Hanyadik kérdést szeretné módosítani?\n");
			scanf("%s", &tempqid);
			if(isNumber(tempqid)){
				found=1;
				qid=atoi(tempqid);
			}
			while ( (scanfNewLineRemover = getchar()) != '\n' && scanfNewLineRemover != EOF );
		}while(!(qid<=questionNumber && found==1 && qid!=0));
	qid--;
	printf("Régi kérdés:\n%s\n", qs[qid].question);
	printf("Új kérdés:\n");
	fgets(qs[qid].question, sizeof(qs[qid].question), stdin);
	newLineToNull(qs[qid].question);
	char tempqnumber[10];
	int qnumber=0;
	found=0;
	do{
			printf("Kérem adja meg a válaszok számát!(2-4)\n");
			scanf("%s", &tempqnumber);
			if(isNumber(tempqnumber)){
				found=1;
				qnumber=atoi(tempqnumber);
			}
			while ( (scanfNewLineRemover = getchar()) != '\n' && scanfNewLineRemover != EOF );
		}while(!((qnumber == 2 || qnumber == 3 || qnumber == 4) && found==1));
	qs[qid].answerNumber=qnumber;
	int i;
	for (i = 0; i < qs[qid].answerNumber; ++i) {
		printf("Régi %d. válasz : %s\n", i+1, qs[qid].answers[i]);
		printf("Új %d. válasz : ", i+1);
		fgets(qs[qid].answers[i], sizeof(qs[qid].answers[i]), stdin);
		newLineToNull(qs[qid].answers[i]);
	}
}

void createSurvey () {
  FILE *f;
  f = fopen("kerdoiv.txt", "w");
  if (f == NULL) {
    f = fopen("kerdoiv.txt", "a");
  }
  listQuestions();
  printf("Mely kérdéseket szeretné beletenni a kérdőívbe?\n");
  printf("Addja meg a sorszámukat![max 10]\n");
  printf("Ha végzett a kérdések megadásával,nyomjon 0-át!\n");
  char tmpselectedqs[10];
  int selectedQs[10];
  int i=0;
  int end=0;
  int found=0;
  do{
	  do{
			printf("%d. kérdés: \n",i+1);
			scanf("%s", &tmpselectedqs);
			if(isNumber(tmpselectedqs)){
				found=1;
				selectedQs[i]=atoi(tmpselectedqs);
			}
			while ( (scanfNewLineRemover = getchar()) != '\n' && scanfNewLineRemover != EOF );
		}while(!(selectedQs[i]<=questionNumber && found==1));
		if(selectedQs[i]==0){
			end=1;
		}
		i++;
  }while(i<10 && end==0);
  int j;
  int index;
  for (j=0;j<i;++j){
	index=selectedQs[j];
	index--;
	if(index!=-1){
		fprintf(f, "%d. Kérdés:\n%s\n", j+1,qs[index].question);
		int k;
		for (k = 0; k < qs[index].answerNumber; ++k) {
		  fprintf(f, "%d. Válasz: %s\n", k+1,qs[index].answers[k]);
		}
		fprintf(f, "\n");
	}
  }
  fclose(f);
}

void readFromFile () {
  FILE *f;
  f=fopen("kerdesek.txt", "r");
  if (f == NULL) {
    f=fopen("kerdesek.txt", "a");
    return;
  }
  char line[200];
  while (fgets(line, sizeof(line), f)) {
	newLineToNull(line);
    strcpy(qs[questionNumber].question, line);
    fgets(line, sizeof(line), f);
	newLineToNull(line);
    qs[questionNumber].answerNumber = atoi(line);
    int i;
    for (i = 0; i < qs[questionNumber].answerNumber; ++i) {
		fgets(line, sizeof(line), f);
		newLineToNull(line);
		strcpy(qs[questionNumber].answers[i], line);
    }
    fgets(line, sizeof(line), f);
	newLineToNull(line);
    strcpy(qs[questionNumber].creationDate, line);
    questionNumber++;
  }
  fclose(f);
}

void writeToFile () {
  FILE *f = fopen("kerdesek.txt", "w");
  if (f == NULL) {
    printf("Hiba a fájl megnyitásakor!\n");
    return;
  }
  int i;
  for (i = 0; i < questionNumber; ++i) {
    fprintf(f, "%s\n", qs[i].question);
    fprintf(f, "%d\n", qs[i].answerNumber);

    int j;
    for (j = 0; j < qs[i].answerNumber; ++j) {
      fprintf(f, "%s\n", qs[i].answers[j]);
    }
    fprintf(f, "%s\n", qs[i].creationDate);
  }
  fclose(f);
}

void mainMenu(){
	char input;
	printf("MLSZ kérőív Manager\n");
	printf("<------------------->\n");
	printf("Kérdés felvétele - A\n");
	printf("Kérdés módosítása - M\n");
	printf("Kérdés törlése - D\n");
	printf("Kérdések listázása - L\n");
	printf("Kérdőív létrehozása - K\n");
	printf("Kilépés a programból - E\n");
	scanf(" %s",&input);
	while ( (scanfNewLineRemover = getchar()) != '\n' && scanfNewLineRemover != EOF );
	printf("<------------------->\n");
	switch(input){
		case 'a' : addQuestion();
					break;
		case 'm' : modifyQuestion();
					break;
		case 'd' : deleteQuestion();
					break;
		case 'l' : listQuestions();
					break;
		case 'k' : createSurvey();
					break;
		case 'e' : writeToFile();
					exit(1);
					break;
	}
}

int main(int argc,char ** argv) 
{
	readFromFile();
	while(1){
		mainMenu();
	}
	
 return 0;
}