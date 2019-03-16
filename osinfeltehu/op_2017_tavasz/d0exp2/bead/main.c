#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
void clearInput(){
	char c;
	while ((c = getchar()) != '\n' && c != EOF) { }
}



struct Question{
	char question[200];
	char answers[4][80];
	int countAnswers;
	char date[13];
};

//void listQuestions(struct QuestionArray *Q); 

void listAnswers(struct Question *q){
	int i;
	for(i = 0 ; i < q->countAnswers; ++i){
		printf("	%i.) %s", i+1, q->answers[i]);
	}
	printf("\n");
}

void addAnswer(struct Question *q){
	if(q->countAnswers < 4){
		char* answer;
		size_t asize = 80;
		answer = (char *)malloc(80 * sizeof(char));

		if( answer == NULL )
		{
			perror("Unable to allocate buffer");
			exit(1);
		}
	
		printf("Kerem adja meg mely valasz legyen ez:\n");
		getline(&answer, &asize, stdin);
		strcpy(q->answers[q->countAnswers], answer);
		q->countAnswers++;
	}else{
		printf("Nem lehet tobb valaszt hozzaadni, mert mar rendelkezik 4 valaszlehetoseggel.\n");
	}
}

void removeAnswer(struct Question *q){
	int answerNum;
	printf("Melyik valaszt szeretne torolni?\n");
	
	listAnswers(q);
	
	scanf("%i", &answerNum);
	clearInput();
	
	if(q->countAnswers > 2){
		printf("Debug. 2.\n");
		while(answerNum < q->countAnswers){
			strcpy(q->answers[answerNum-1], q->answers[answerNum]);

			++answerNum;
		}
		printf("Debug. 6.\n");
		
		strcpy(q->answers[answerNum-1], "<empty>");
		q->countAnswers--;
	}else{
		printf("Nem lehet valaszt torolni, mert csak 2 valasszal rendelkezik.\n");
	}
}

void modifyAnswer(struct Question *q){}


struct QuestionArray{
	struct Question QArray[100];
	int countQuestions;
};


void listQuestion(struct Question *q){
	printf("%s", q->question);
	printf("\n");
	int i;
	listAnswers(q);
}


void makeQuestion(struct QuestionArray *Q){
	//Debug1.0
	printf("%i", Q->countQuestions);
	
	struct Question q;	
	q.countAnswers = 0;
	
	int qamount, i;
	size_t qsize = 200;
	size_t asize = 80;
	char* question;
	char* answer;
	
	question = (char *)malloc(200 * sizeof(char));
	answer = (char *)malloc(80 * sizeof(char));

	if( question == NULL || answer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }
	
	printf("Kerem adja meg mely kerdest akarja feltenni!\n");
	getline(&question, &qsize, stdin);
	strcpy(q.question, question);
	
	printf("Szukseges, hogy minimum 2 valasz, és maximum 4 valasz keruljon a kerdeshez!\nKerem adja meg hany valaszt akar irni a kerdeshez!\n");
	scanf("%d", &qamount );
	clearInput();
	
	while ( !(qamount > 1 && qamount < 5) ){
		printf("Kerem megfelelo mennyisegu valaszt adjon hozza!\nA valaszok mennyisege:\n");
		scanf("%d", &qamount );
		clearInput();
	}
	
	if(qamount > 1 && qamount < 5){
		for( i = 0; i < qamount; ++i){
			printf("%s %i. %s", "Kerem adja meg az", i+1, "valaszt!\n");
			getline(&answer, &asize, stdin);
			strcpy(q.answers[i], answer);
			answer = NULL;
			q.countAnswers++;
			
			printf("Debug answers toltve. countanswers:  \n");
			printf(q.answers[i]);
			printf("%d", q.countAnswers);
			printf("\n");
		}
	}
		
	Q->QArray[Q->countQuestions] = q;
	++Q->countQuestions;
	printf("%i %s", Q->countQuestions, "-Debug. count");
	free(question);
	free(answer);
}
void listQuestions(struct QuestionArray *Q){
	int i;
	if(Q->countQuestions != 0){
		for(i = 0; i < Q->countQuestions; ++i){
			printf("%i. ", i+1);
			listQuestion(&Q->QArray[i]);
		}	
	}else{
		printf("Nincs meg kerdes hozzaadva!\n");
		return;
	}
	printf("------------------------\n");	
}


void deleteQuestion(struct QuestionArray *Q){
	int i, questionNum, n;
	
	printf("Melyik kerdest szeretne torolni?\n");
	listQuestions(Q);
	scanf("%i", &questionNum);
	
	printf("Debug. 21.\n");
	if(questionNum <= Q->countQuestions && questionNum > 0){
		printf("Debug. 2.\n");
		while(questionNum < Q->countQuestions){
		printf("Debug. 33.\n");
			Q->QArray[questionNum-1] = Q->QArray[questionNum];
		printf("Debug. 44.\n");
		
	printf("%i. ", questionNum);
			++questionNum;
			printf("%s, %i", "DEbug55.", questionNum);
		}
		printf("Debug. 6.\n");
		
		strcpy(Q->QArray[questionNum-1].question, "<empty>");
		printf("Debug. 7.\n");
		for (i = 0; i < Q->QArray[questionNum-1].countAnswers; ++i){
			strcpy(Q->QArray[questionNum-1].answers[i], "<empty>");		
		}
		Q->countQuestions--;
	}else{
		printf("Nem lehet kerdest torolni.\n");
	}
}

void modifyQuestion(struct QuestionArray *Q){
	int i, questionNum, n, answerNum;
	
	printf("Melyik kerdest szeretne modositani?\n");
	listQuestions(Q);
	scanf("%i", &questionNum);
	clearInput();
	
	printf("Debug. 31.\n");
	if(questionNum <= Q->countQuestions && questionNum > 0){
		printf("Debug. 32.\n");
		printf("Modositsam a kerdest?\n0. igen\n1. nem\n");
		
		scanf("%i", n);
		clearInput();
		
		printf("Debug. 31.\n");
		while( !(n == 0 || n == 1) ){
			if( n == 0){
				char* question;
				size_t qsize = 200;
				question = (char *)malloc(200 * sizeof(char));
					
				if( question == NULL)
				{
					perror("Unable to allocate buffer");
					exit(1);
				}
	
				printf("Kerem adja meg az uj kerdest:\n");
				getline(&question, &qsize, stdin);
				strcpy(Q->QArray[questionNum-1].question, question);
				free(question);
			}
			else if( n == 1 ){
				return;
			}else{
				printf("Ez nem lehetoseg! Adja meg ujra, mit szeretne tenni!\n");
				scanf("%i", n);
				clearInput();
			}
		}
		printf("Modositsam a valaszokat?\n0. igen\n1. nem\n");
		
		scanf("%i", n);
		clearInput();
		while( !(n == 0 || n == 1) ){
			if( n == 0){
				//char* answer;
				//size_t qsize = 80;
				//question = (char *)malloc(80 * sizeof(char));
				/*	
				if( answer == NULL)
				{
					perror("Unable to allocate buffer");
					exit(1);
				}
				*/
				printf("  1. Válasz törlése\n  2.Válasz hozzáadás\n  3.Válasz átírása");
				scanf("%i", &questionNum);
				clearInput();
				switch(answerNum){
					case 1:
							removeAnswer(&Q->QArray[questionNum-1]);
							break;
					case 2:
							addAnswer(&Q->QArray[questionNum-1]);
							break;
					case 3:
							modifyAnswer(&Q->QArray[questionNum-1]);
							break;
				}
			}
			else if( n == 1 ){
				return;
			}else{
				printf("Ez nem lehetoseg! Adja meg ujra, mit szeretne tenni!\n");
				scanf("%i", n);
				clearInput();
			}
		}
	
	}else{
		printf("Nincs ilyen kerdes.");
	}
}

void finalizeQuestions(){
	
}

void MenuWriteOut(){
	int x = 0;
	struct QuestionArray Q;
	Q.countQuestions = 0;
	
	printf("------------------------\n");
	printf("Kerdoiv keszito program |\n");
	printf("------------------------\n");
	printf("\n");
	
	while(x == 0){
		printf("Mit kivan tenni?\n");
		printf("\n");
		printf("1. Kerdes megadasa\n");
		printf("2. Kerdes modositas\n");
		printf("3. Kerdes torlese\n");
		printf("4. Kerdesek listazasa\n");
		printf("5. Kerdesek veglegesitese\n");
		printf("6. Kilepes");
		printf("\n");

		int choice;
		scanf("%d", &choice );
		clearInput();
		switch(choice){
			case 1:
				makeQuestion(&Q);
				printf("\n");
				break;
			case 2:
				modifyQuestion(&Q);
				printf("\n");
				break;
			case 3:
				deleteQuestion(&Q);
				printf("\n");
				break;
			case 4:
				listQuestions(&Q);
				printf("\n");
				break;
			case 5:
				//finalizeQuestions();
				printf("\n");
				break;
			case 6:
				printf("|||Kilepes. A program bezarul.|||\n");
				x = 4;
				break;
			default:
				printf("Helytelen opcio. Valasszon masikat\n");
		}	
	}
}



int main(){
	MenuWriteOut();
	
	return 0;
}