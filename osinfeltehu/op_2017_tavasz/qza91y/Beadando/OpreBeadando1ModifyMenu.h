#ifndef OPREBEADANDO2_H
#define OPREBEADANDO2_H 

// creating question.
void createQuestion(Question_list *survey){
	// declaring local variables
	Question_list* q = survey;
	Question_list* p = q->next;
	
	// stepping to the last question
	while(p != NULL){
		q = p;
		p = q->next;
	}
	// allocating the next question in the memory
	q->next = (Question_list*)malloc(sizeof(Question_list));
	q = q->next;
	
	// time calculating
	time(&q->time);
	
	// getting question and answers
	printf("Type in the question:\n");
	q->question = inputString(stdin, 1);
	printf("\nType in the answers!");
	printf("\nIf you don't want to give more answers then type in: 'finish' + enter!\n");
	
	// typing in the answers
	q->answers = (Answer_list*)malloc(sizeof(Answer_list));
	Answer_list* ans = q->answers;
	char* tmp = "";
	q->answerNumb = 0;
	int i = 0;
	while( strcmp(tmp , "finish") != 0 ){
		printf("%d. answer: ", q->answerNumb+1);
		tmp = inputString(stdin, 1);
		if ( strcmp(tmp , "finish") != 0 ) {
			ans->next = (Answer_list*)malloc(sizeof(Answer_list));
			ans = ans->next;
			ans->answer = tmp;
			q->answerNumb++;
		}
		ans->next = NULL;
	}
	questionNumb++;
	q->next = NULL;
	printf("\n");
}

// modifying questions.
// copying the current question without the needless stuff and fill it with the new one.
void modify(Question_list *survey){
	// declaring
	Question_list* p = survey;
	Question_list* q = (Question_list*)malloc(sizeof(Question_list));
	q->answers = (Answer_list*)malloc(sizeof(Answer_list));
	int n;
	int i;
	char c;
	char* tmp;
	
	// Choosing the needed question.
	while( n < 1 || n > questionNumb) {
		
		printf("Which question do you want to modify(1 - %d)? ",questionNumb);
		scanf("%d", &n);
		if(n < 1 || n > questionNumb) {
			printf("Doesn't existing question number. you can choose from 1 - %d\n",questionNumb);
		}
	}
	// Stepping to the current question in the list.
	i = 0;
	while(i < n-1){
		p = p->next;
		i++;
	}
	
	// time changing because of the modifying.
	time(&q->time);
	
	while( c != 'Q' && c != 'q' && c != 'A' && c != 'a') {
		printf("What do you want to modify? (Q/A): ");
		fseek(stdin,0,SEEK_END);
		scanf("%c", &c);
	}
	
	if( c == 'Q' || c == 'q' ) { // giving a new question.
		// copying data members.
		q->answerNumb = p->next->answerNumb;
		q->next = p->next->next;
		// copying answers.
		q->answers = p->next->answers;
		//
		free(p->next);
		p->next = q;
		printf("Type in the new question: ");
		q->question = inputString(stdin, 1);
	} 
	else if( c == 'A' || c == 'a' ) { // modifying answer(s).
		// Checking the given answer number's validity.
		n = 0;
		while(n < 1 || n > p->next->answerNumb) {
			printf("Which answer do you want to modify(1-%d?): ",p->next->answerNumb);
			scanf("%d", &n);
			if(n < 1 || n > p->next->answerNumb) {
				printf("Doesn't existing answer number. you can choose from 1 - %d\n",p->next->answerNumb);
			}
		}
		// copying the data members.
		q->answerNumb = p->next->answerNumb;
		q->question = p->next->question;
		q->answers = p->next->answers;
		q->next = p->next->next;
		// modify answer.
		Answer_list* tmp2 = (Answer_list*)malloc(sizeof(Answer_list));
		Answer_list* tmp = q->answers;
		for(i = 0; i<q->answerNumb; i++){
			if(i != n-1){
				tmp = tmp->next;
			} else { // type in the new answer.
				printf("Type in the new answer: ");
				tmp2->answer = inputString(stdin, 1);
				tmp2->next = tmp->next->next;
				free(tmp->next);
				tmp->next = tmp2;
			}
		}
		free(p->next);
		p->next = q;
	} 
	else { // wrong input handling.
		printf("'Q','q': question, 'A','a': answer, other characters are invalid!\n");
	}
	printf("\n");
}

// deleting question or answer.
void deleteQuestion(Question_list *survey){
	char c;
	int i = 0;
	int n;
	Question_list* p = survey;
	Question_list* q = p->next;
	printf("Do you want to delete a question or an answer(Q/A)?: ");
	fseek(stdin,0,SEEK_END);
	scanf("%c", &c);
	if(c == 'Q' || c == 'q') { // delete question.
			while ( n < 1 || questionNumb < n) {
				printf("Which question do you want to delete?(1 - %d) ", questionNumb);
				fseek(stdin,0,SEEK_END);
				scanf("%d", &n);
			}
			while(i < n){
				q = p;
				p = p->next;
				i++;
			}
			q->next = p->next;
			free(p);
			questionNumb--;
	} else if(c == 'A' || c == 'a') { // delete answer.
			while (n < 1 || questionNumb < n) {
				printf("In which question do you want to delete answer?(1 - %d): ", questionNumb);
				fseek(stdin,0,SEEK_END);
				scanf("%d", &n);
				if (n < 1 || p->answerNumb < n) printf("Invalid input!\n");
			}
			while(i < n){
				p = p->next;
				i++;
			}
			while (n < 1 || p->answerNumb < n) {
				printf("Which answer do you want to delete?(1 - %d): ", p->answerNumb);
				fseek(stdin,0,SEEK_END);
				scanf("%d", &n);
				if (n < 1 || p->answerNumb < n) printf("Invalid input!\n");
			}
			Answer_list* a1 = p->answers->next;
			Answer_list* a2 = p->answers->next;
			while(i < n) {
				a2 = a1;
				a1 = a1->next;
			}
			p->answerNumb--;
	} else { // Invalid input.
		printf("'Q','q': question, 'A','a': answer, other characters are invalid!\n");
	}
	printf("\n");
}

#endif