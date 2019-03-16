#ifndef QUESTION_H
#define QUESTION_H

struct question {
	char question[128];
	int qnum;
	char answers[4][128];
	char date[128];
};

#endif // !QUESTION_H
