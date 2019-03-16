#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define P1_READ     0
#define P2_WRITE    1
#define P2_READ     2
#define P1_WRITE    3
#define NUM_PIPES   2

struct Question {
	int id;
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	char question[300];
	int answerCount;
	char answers[4][300];
};

struct QuestionDAO {
	FILE* file;
	char buff[2048];
	char fileName[500];
} questionDAO;

struct result {
	int personId;
	int questionId;
	int choice;
	int size;
};

static struct result randomResults[3][20];
static struct Question questions[100];
static struct Question tempQuestions[3];



static int qCounter = 0;
static int eventCounter = 0;

void signalHandler(int signumber) {
    printf("Signal with number %i has arrived\n",signumber);
	if (signumber == 2)
		exit(1);
}

int random_int(int min, int max) {
   return min + rand() % (max+1 - min);
}
struct Question* selectRandomQuestions(struct Question* currQuestions) {
	
	int selected = 0;
	
	int i = 0;
	int j = 0;
	struct Question selectedQuestions[3];
	int selectedIndexes[3];
	
	//printf("QCounter: %d",qCounter);
	if (qCounter < 3) {
		printf("-------------------------- ERROR ---------------------------\n");
		printf("You need at least 3 questions to generate random answers!\n");
		printf("------------------------------------------------------------\n");
		return NULL;
	}
	srand(time(NULL));
	while(i < 2) {
		//printf("QCounter: %d",qCounter);
		int already = 0;
		int r = random_int(0, qCounter-1); // modulo array size to make sure we dont overindex
		printf("RANDOM INDEX: %d\n",r);
		printf("I: %d", i);
		for (j = 0; j < 3; ++j) {
			if (selectedIndexes[j] == r)
				already = 1;
		}
		if (already)
			continue;
		else
		{
			selectedIndexes[i] = r;
			++i;
		}
	}
	
	printf("Selected questions: (indexes)\n");
	for (i = 0; i < 3; ++i) {
		printf("%d\n",selectedIndexes[i]);
		
		tempQuestions[i] = currQuestions[selectedIndexes[i]];
	}
	struct Question* q = tempQuestions;
	return q;
}

void printRecvdQuestions(struct Question* questions, int size) {
	int j = 0;
    for(j = 0; j < size; ++j)
    {
		if (questions[j].answerCount == 0)
			break;
		
		if (questions[j].answerCount == 2)
			printf("%d: %d.%d.%d %d:%d,%d Question: %s Answers: %s || %s\n", j, questions[j].year, questions[j].month, questions[j].day, questions[j].hour, questions[j].minute, questions[j].second, questions[j].question, questions[j].answers[0], questions[j].answers[1]);
		else
			printf("%d: %d.%d.%d %d:%d,%d Question: %s Answers: %s || %s || %s || %s\n", j, questions[j].year, questions[j].month, questions[j].day, questions[j].hour, questions[j].minute, questions[j].second, questions[j].question, questions[j].answers[0], questions[j].answers[1], questions[j].answers[2],questions[j].answers[3] );
	}
}

int generateToSpecific(struct Question q, struct result* returnArray) {
	
	int citizens = random_int(11, 20);
	int i = 0;
	for (i = 0; i < citizens; ++i) {	
		int personId = i;
		int questionId = q.id;
		int acount = q.answerCount;
		int choice = random_int(1, acount);
		// hard debug only
		//printf("CHILD DEBUG:> person id generated: %d, questionId: %d, acount: %d, choice %d\n", i, questionId, acount, choice);
		returnArray[i].personId = personId;
		returnArray[i].questionId = questionId;
		returnArray[i].choice = choice;
		returnArray[i].size = citizens;
	}
	return citizens;
}
void printResults(struct result results[3][20]) {
	
	int i = 0;
	int j = 0;
	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 1024; ++j) {
			if (results[i][j].choice != 0)
				printf("Result for question id: %d, person id: %d, choice: %d\n", results[i][j].questionId, results[i][j].personId, results[i][j].choice);
		}
	}
}
void waitForChild(int signum) {
	if (signum == SIGUSR1) {
		printf("PARENT:> A kerdezobiztos megerkezett!\n");
	}
	if (signum == SIGUSR2) {
		printf("CHILD:> A MLSZ megkapta a kitoltott kerdoiveket! Befejeztem a dolgom!\n");
		kill(getpid(), SIGKILL);
	}
}
void getRandomResultsFromCity(char* cityName, struct Question* selectedQuestions) {
    int i = 0;
    int counter = 0;
   
    for (i = 0; i < 3; ++i) {
        tempQuestions[i] = selectedQuestions[i];
    }
    int cityPipe[2];
    int questionPipe[2];
	
    int p1[2];
	int p2[2];
	
    char cityBuffer[1024];
    struct Question genericBuffer[1];
    struct result resultBuffer[1];
   
    struct Question* q = selectRandomQuestions(selectedQuestions);
    pid_t cpid;
    int processCount = 0;
    int n;
	char *messageText = "ARRIVED";
	struct sigaction act;
	union sigval value;
	
	act.sa_flags = SA_SIGINFO;

	
    if (q == NULL) {
        return;
    }
    printf("Getting random results in city %s...\n", cityName);
   
   
    printf("Selected questions: (titles)\n");
    for (i = 0; i < 3; ++i) {
        printf("%d: %s answerCount:%d\n",q[i].id, q[i].question, q[i].answerCount);
    }
    eventCounter = 0;
   
    /* initing pipes */
    if (pipe(cityPipe) == -1)
    {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }
    if (pipe(questionPipe) == -1) {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }
    if (pipe(p1) == -1)
	{
		perror("pipe error");
        exit(EXIT_FAILURE);
	}
	if (pipe(p2) == -1) {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }
	
    signal(SIGTERM,signalHandler);
    signal(SIGINT,signalHandler);
   
    /* creating children */
    cpid = fork();
   
    if (cpid == -1)
    {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
	
    if(cpid == 0) // child
    {
		/* 2-way comm here */
		close(p1[1]);
        close(p2[0]);
        /* accepting city name from parent*/
        close(cityPipe[1]);
		
		/* waiting 5 seconds to arrive */
		//value.sival_ptr = messageText;
		sleep(5);
		
		//sigqueue(getppid(), SIGUSR1, value);
		
        read(cityPipe[0], cityBuffer, sizeof(cityBuffer));
        close(cityPipe[0]);
		
        printf("CHILD:> I arrived in %s\n", cityBuffer);
        close(questionPipe[1]);
       
        int i = 0;
        struct result finalResults[3][20];
		int generatedNumbers[3];
		
        /* accepting questions from parents and generating answers */
        while ((n = read(questionPipe[0], genericBuffer, sizeof(genericBuffer)) > 0)) {
            printf("CHILD: DEBUG> received: %d: %s\n", genericBuffer[0].id, genericBuffer[0].question);
            generatedNumbers[i] = generateToSpecific(genericBuffer[0],finalResults[i]);
            ++i;
        }
		close(questionPipe[0]);
		
       
        printf("CHILD: DEBUG> Finished generating! Sending data back to parent...\n");
		/*
        write(p2[1], &finalResults[0], sizeof(finalResults[0]));
        write(p2[1], &finalResults[1], sizeof(finalResults[1]));
        write(p2[1], &finalResults[2], sizeof(finalResults[2]));
		*/
		int j = 0;
		for (i = 0; i < 3; ++i) {
			for (j = 0; j < generatedNumbers[i]; ++j) {
				write(p2[1], &finalResults[i][j], sizeof(finalResults[i][j]));
			}
		}
        //write(questionPipe[0], &finalResults, sizeof(finalResults));
		close(p1[0]);
        close(p2[1]);
		
		int status;
        //pause();
		printf("CHILD:> Waiting for MLSZ to receive my generated answers...\n");
		wait(NULL);
		//signal(SIGUSR2, waitForChild);
    }
    else // parent
    {
		close(p1[0]);
        close(p2[1]);
        close(cityPipe[0]);
		close(questionPipe[0]);
		
		printf("MLSZ:> Waiting for child to arrive...\n");
		
        int status;
        wait(NULL);
		
		//signal(SIGUSR1, waitForChild);
		//pause();
		
        write(cityPipe[1], cityName, sizeof(cityBuffer));
        close(cityPipe[1]);

        write(questionPipe[1], &q[0], sizeof(q[0]));
        write(questionPipe[1], &q[1], sizeof(q[1]));
        write(questionPipe[1], &q[2], sizeof(q[2]));
        close(questionPipe[1]);
		
		sleep(5);
		printf("PARENT: debug> Receiving generated data from child...\n");
		
        while ((n = read(p2[0], resultBuffer, sizeof(resultBuffer)) > 0)) {
            //int i = 0;
            //for (i = 0; i < 60; ++i) { // 3*20 maximum
			if (resultBuffer[i].choice != 0)
				printf("MLSZ:> Result for question id: %d, person id: %d, choice: %d\n", resultBuffer[0].questionId, resultBuffer[0].personId, resultBuffer[0].choice);
            //}
        }
		
		//sigqueue(cpid, SIGUSR2, value);
		
		printf("PARENT: debug> Receiving DONE...Signaling back to child to kill...\n");
		kill(cpid, SIGKILL);
		sleep(2);
		close(p1[1]);
       	close(p2[0]);
    }
}

struct Question* getQuestionsFromSheet(FILE* fp) {
	qCounter = 0;
	char line [128];
	
	while(fgets(line, sizeof(line) ,fp) != NULL) {
		char* token;
		struct Question temp;
		int counter = -1;
		char* l = line;
		while((token = strsep(&l,"|")) != NULL) {
			switch( counter ) {
				case -1:
				temp.id = atoi(token);
				break;
				case 0:
				temp.year = atoi(token);
				break;
				case 1:
				temp.month = atoi(token);
				break;
				case 2:
				temp.day = atoi(token);
				break;
				case 3:
				temp.hour = atoi(token);
				break;
				case 4:
				temp.minute = atoi(token);
				break;
				case 5:
				temp.second = atoi(token);
				break;
				case 6:
				strcpy(temp.question, token);
				break;
				case 7:
				temp.answerCount = atoi(token);
				break;
				case 8:
				strcpy(temp.answers[0], token);
				break;
				case 9:
				strcpy(temp.answers[1], token);
				break;
				case 10:
				if (temp.answerCount == 4)
					strcpy(temp.answers[2], token);
				break;
				case 11:
				if (temp.answerCount == 4)
					strcpy(temp.answers[3], token);
				break;
			}
			++counter;
		}
		questions[qCounter] = temp;
		++qCounter;
		//printf("Qcounter: %d\n", qCounter);
	}
	
	if (qCounter == 0) {
		printf("\n");
		printf("INFO:> The sheet is currently EMPTY!\n");
		printf("*****************************\n");
	} else {
		printf("\n");
		printf("INFO:> The sheet now: \n");
		printf("****************\n");
		int j;
		for (j = 0; j < qCounter; ++j) {
			if (questions[j].answerCount == 2)
				printf("%d: %d.%d.%d %d:%d,%d Question: %s Answers: %s || %s\n", j, questions[j].year, questions[j].month, questions[j].day, questions[j].hour, questions[j].minute, questions[j].second, questions[j].question, questions[j].answers[0], questions[j].answers[1]);
			else
				printf("%d: %d.%d.%d %d:%d,%d Question: %s Answers: %s || %s || %s || %s\n", j, questions[j].year, questions[j].month, questions[j].day, questions[j].hour, questions[j].minute, questions[j].second, questions[j].question, questions[j].answers[0], questions[j].answers[1], questions[j].answers[2],questions[j].answers[3] );
		}
	printf("****************\n");
	}
	
	struct Question* q = questions;
	return q;
	
}
int deleteQuestion(struct Question* questionPointer) {
	questionDAO.file = fopen(questionDAO.fileName, "rw+");
	FILE* fp = fopen("replica.dat", "ab+"); // replica, creates if doesnt exists: ab+
	
	int index;
	printf("INFO:> Question you want to delete: (index)");
	scanf("%d", &index);
	printf("\n");
	
	if (index >= qCounter || index < 0) {
		printf("ERROR:> bad index!");
		return -1;
	}
	
	struct Question questionToModify = questions[index];
	
	//printf("qCount: %d", questionToModify.answerCount);

	if (questionToModify.answerCount == 4)
		printf("INFO:> The question you want to modify: %s with answer count: %d || Answers: %s | %s | %s | %s\n", questionToModify.question, questionToModify.answerCount, questionToModify.answers[0], questionToModify.answers[1], questionToModify.answers[2], questionToModify.answers[3]);
	else
		printf("INFO:> The question you want to modify: %s with answer count: %d || Answers: %s | %s\n", questionToModify.question, questionToModify.answerCount, questionToModify.answers[0], questionToModify.answers[1]);
	
	
	if (fp == NULL || questionDAO.file == NULL) {
		printf("ERROR:> Error opening the files!");
		return -1;
	}
	char line [128];
	int lineIndex = 0;
	
	while(fgets(line, sizeof(line) ,questionDAO.file) != NULL) { // giving index here, if index matches, we don't copy that row
		//printf("%s LINE!\n", line);
		if (lineIndex != index) {
			fprintf(fp, line);
		}
		++lineIndex;
	}
	//--qCounter; // removed one question
	
	int ret = rename("replica.dat", questionDAO.fileName);
	fclose(questionDAO.file);
	
	if(ret != 0) {
 		printf("ERROR:> unable to rename the file");
		return -1;
	}
	printf("INFO:> Question with index %d deleted successfully!\n", index);
	fclose(fp);
	questionDAO.file = fopen(questionDAO.fileName, "rw+");
	return 0;
}
int modifyQuestion(struct Question* questionPointer) {
	questionDAO.file = fopen(questionDAO.fileName, "rw+");
	FILE* fp = fopen("replica.dat", "ab+");
	
	int index;
	char question[300];
	int aCount;
	char answers[4][300];
	char tempAnswer[300];
	printf("INFO:> Question you want to modify: (index)");
	scanf("%d", &index);
	printf("\n");
	
	struct Question questionToModify = questions[index];
	
	if (index >= qCounter || index < 0) {
		printf("ERROR:> bad index!");
		return -1;
	}
	
	if (questionToModify.answerCount == 4)
		printf("INFO:> The question you want to modify: %s with answer count: %d || Answers: %s | %s | %s | %s\n", questionToModify.question, questionToModify.answerCount, questionToModify.answers[0], questionToModify.answers[1], questionToModify.answers[2], questionToModify.answers[3]);
	else
		printf("INFO:> The question you want to modify: %s with answer count: %d || Answers: %s | %s\n", questionToModify.question, questionToModify.answerCount, questionToModify.answers[0], questionToModify.answers[1]);
	
	printf("INFO:> Modification:\n");
	printf("INFO:> Question?\n");
	scanf(" %[^\n]s", question);
	//fgets(question, 300, stdin);
	//getLine("Question?", question, sizeof(question));
	printf("INFO:> Answer count?\n");
	scanf("%d", &aCount);
	if (aCount != 2 && aCount != 4)
	{
		printf("ERROR:> Answer count must be 2 or 4!");
		return -1;
	}
	
	//printf("ACOUNT: %d", aCount);
	int i;
	for (i = 0; i < aCount; ++i) {
		printf("INFO:> %d: Answer: \n", i+1);
		scanf(" %[^\n]s", tempAnswer);
		strcpy(answers[i], tempAnswer);
	}
	if (aCount == 4)
		printf("INFO:> Your question is: %s with answer count: %d || Answers: %s | %s | %s | %s\n", question, aCount, answers[0], answers[1], answers[2], answers[3]);
	else
		printf("INFO:> Your question is: %s with answer count: %d || Answers: %s | %s\n", question, aCount, answers[0], answers[1]);
	
	printf("INFO:> Correct? (0/1)");
	int choice;
	scanf("%d", &choice);
	
	if (choice == 1) {
		strcpy(questionToModify.question, question);
		questionToModify.answerCount = aCount;
		strcpy(questionToModify.answers[0], answers[0]);
		strcpy(questionToModify.answers[1], answers[1]);
		if (aCount == 4)
			strcpy(questionToModify.answers[2], answers[2]);
		if (aCount == 4)
			strcpy(questionToModify.answers[3], answers[3]);
		char line [128];
	
		int currentLine = 0;
		while(fgets(line, sizeof(line) ,questionDAO.file) != NULL) {
			printf("line: %s", line);
			if (currentLine == index) {
				if (aCount == 2)
					fprintf(fp, "%d|%d|%d|%d|%d|%d|%d|%s|%d|%s|%s", 
					questionToModify.id,
					questionToModify.year, 
					questionToModify.month, 
					questionToModify.day, 
					questionToModify.hour, 
					questionToModify.minute, 
					questionToModify.second, 
					questionToModify.question,
					questionToModify.answerCount,
					questionToModify.answers[0],
					questionToModify.answers[1]
				);
				else
				fprintf(fp, "%d|%d|%d|%d|%d|%d|%d|%s|%d|%s|%s|%s|%s", 
					questionToModify.id,
					questionToModify.year, 
					questionToModify.month, 
					questionToModify.day, 
					questionToModify.hour, 
					questionToModify.minute, 
					questionToModify.second, 
					questionToModify.question,
					questionToModify.answerCount,
					questionToModify.answers[0],
					questionToModify.answers[1],
					questionToModify.answers[2],
					questionToModify.answers[3]
				);
			} else {
				fprintf(fp, line);
			}
			++currentLine;
		}
		int ret = rename("replica.dat", questionDAO.fileName);
		fclose(questionDAO.file);
	
		if(ret != 0) {
			printf("ERROR:> Error: unable to rename the file");
			return -1;
		}
		//printf("Question with index %d deleted successfully!\n", index);
		fclose(fp);
		questionDAO.file = fopen(questionDAO.fileName, "rw+");
	
		printf("INFO:> Successfully modified question!\n");
	}
}
int addQuestion(struct Question* questionPointer) {
	
	
	fclose(questionDAO.file);
	questionDAO.file = fopen(questionDAO.fileName, "a"); // opened with append, no need to fseek
	
	char question[300];
	int aCount;
	char answers[4][300];
	char tempAnswer[300];
	
	printf("INFO:> Question?\n");
	scanf(" %[^\n]s", question);
	
	printf("INFO:> Answer count?");
	scanf("%d", &aCount);
	if (aCount != 2 && aCount != 4)
	{
		printf("ERROR:> Answer count must be 2 or 4!\n");
		return -1;
	}
	//printf("ACOUNT: %d", aCount);
	int i;
	for (i = 0; i < aCount; ++i) {
		printf("INFO:> %d: Answer: ", i+1);
		scanf(" %[^\n]s", tempAnswer);
		strcpy(answers[i], tempAnswer);
	}
	if (aCount == 4)
		printf("INFO:> Your question is: %s with answer count: %d || Answers: %s | %s | %s | %s\n", question, aCount, answers[0], answers[1], answers[2], answers[3]);
	else
		printf("INFO:> Your question is: %s with answer count: %d || Answers: %s | %s\n", question, aCount, answers[0], answers[1]);
	
	printf("INFO:> Correct? (0/1)");
	int choice;
	scanf("%d", &choice);
	
	if (choice == 1) {
		time_t rawtime; 
		time (&rawtime); 
		struct tm *tm_struct = localtime(&rawtime);
		questionPointer[qCounter].id = qCounter;
		questionPointer[qCounter].year = 1900+tm_struct->tm_year;
		questionPointer[qCounter].month = tm_struct->tm_mon+1;
		questionPointer[qCounter].day = tm_struct->tm_mday;
		questionPointer[qCounter].hour = tm_struct->tm_hour;
		questionPointer[qCounter].minute = tm_struct->tm_min;
		questionPointer[qCounter].second = tm_struct->tm_sec;
		strcpy(questionPointer[qCounter].question, question);
		questionPointer[qCounter].answerCount = aCount;
		strcpy(questionPointer[qCounter].answers[0], answers[0]);
		strcpy(questionPointer[qCounter].answers[1], answers[1]);
		if (aCount == 4)
			strcpy(questionPointer[qCounter].answers[2], answers[2]);
		if (aCount == 4)
			strcpy(questionPointer[qCounter].answers[3], answers[3]);
		
		// 2017 03 01 12 38 00 OK? 2 NO NO
		if (aCount == 2)
			fprintf(questionDAO.file, "%d|%d|%d|%d|%d|%d|%d|%s|%d|%s|%s\n", 
			questionPointer[qCounter].id,
			questionPointer[qCounter].year, 
			questionPointer[qCounter].month, 
			questionPointer[qCounter].day, 
			questionPointer[qCounter].hour, 
			questionPointer[qCounter].minute, 
			questionPointer[qCounter].second, 
			questionPointer[qCounter].question,
			questionPointer[qCounter].answerCount,
			questionPointer[qCounter].answers[0],
			questionPointer[qCounter].answers[1]
			);
		else
			fprintf(questionDAO.file, "%d|%d|%d|%d|%d|%d|%d|%s|%d|%s|%s|%s|%s\n", 
			questionPointer[qCounter].id,
			questionPointer[qCounter].year, 
			questionPointer[qCounter].month, 
			questionPointer[qCounter].day, 
			questionPointer[qCounter].hour, 
			questionPointer[qCounter].minute, 
			questionPointer[qCounter].second, 
			questionPointer[qCounter].question,
			questionPointer[qCounter].answerCount,
			questionPointer[qCounter].answers[0],
			questionPointer[qCounter].answers[1],
			questionPointer[qCounter].answers[2],
			questionPointer[qCounter].answers[3]
			);
		printf("INFO:> Successfully added question to sheet!\n");
		++qCounter;
		fclose(questionDAO.file);
		questionDAO.file = fopen(questionDAO.fileName, "rw+");
		return 1;
	} else {
		fclose(questionDAO.file);
		questionDAO.file = fopen(questionDAO.fileName, "rw+");
		return -1;
	}
	
}
struct Question* getQuestionsFromFinalsheet(FILE* fp) {
	int question_counter = 0;
	char line [128];
	struct Question question_temp[20];
	
	while(fgets(line, sizeof(line) ,fp) != NULL) {
		char* token;
		struct Question temp;
		int counter = -1;
		char* l = line;
		while((token = strsep(&l,"|")) != NULL) {
			switch( counter ) {
				case -1:
				temp.id = atoi(token);
				break;
				case 0:
				temp.year = atoi(token);
				break;
				case 1:
				temp.month = atoi(token);
				break;
				case 2:
				temp.day = atoi(token);
				break;
				case 3:
				temp.hour = atoi(token);
				break;
				case 4:
				temp.minute = atoi(token);
				break;
				case 5:
				temp.second = atoi(token);
				break;
				case 6:
				strcpy(temp.question, token);
				break;
				case 7:
				temp.answerCount = atoi(token);
				break;
				case 8:
				strcpy(temp.answers[0], token);
				break;
				case 9:
				strcpy(temp.answers[1], token);
				break;
				case 10:
				if (temp.answerCount == 4)
					strcpy(temp.answers[2], token);
				break;
				case 11:
				if (temp.answerCount == 4)
					strcpy(temp.answers[3], token);
				break;
			}
			++counter;
		}
		question_temp[question_counter] = temp;
		++question_counter;
		//printf("Qcounter: %d\n", qCounter);
	}
	
	if (question_counter == 0) {
		printf("\n");
		printf("INFO:> The FINAL sheet is currently EMPTY!\n");
		printf("*****************************\n");
	} else {
		printf("\n");
		printf("INFO:> The FINAL sheet now: \n");
		printf("****************\n");
		int j;
		for (j = 0; j < question_counter; ++j) {
			if (question_temp[j].answerCount == 2)
				printf("%d: %d.%d.%d %d:%d,%d Question: %s Answers: %s || %s\n", j, question_temp[j].year, question_temp[j].month, question_temp[j].day, question_temp[j].hour, question_temp[j].minute, question_temp[j].second, question_temp[j].question, question_temp[j].answers[0], question_temp[j].answers[1]);
			else
				printf("%d: %d.%d.%d %d:%d,%d Question: %s Answers: %s || %s || %s || %s\n", j, question_temp[j].year, question_temp[j].month, question_temp[j].day, question_temp[j].hour, question_temp[j].minute, question_temp[j].second, question_temp[j].question, question_temp[j].answers[0], question_temp[j].answers[1], question_temp[j].answers[2],question_temp[j].answers[3] );
		}
	printf("****************\n");
	}
	
	struct Question* q = question_temp;
	return q;
}
struct Question* saveSheet() {
	fclose(questionDAO.file);
	questionDAO.file = fopen(questionDAO.fileName, "r");
	char fname[200];
	char questionIndexesString[200];
	int cnt;
	char line[300];
	printf("INFO:> Save sheet as: (filename)\n");
	scanf("%s", fname);
	FILE* fp = fopen(fname, "ab+");
	printf("INFO:> How many questions you want to add to your sheet?\n");
	scanf("%d", &cnt);
	if (cnt <= 0) {
		printf("ERROR:> You have to add at least ONE question to your final sheet!");
		return NULL;
	}
	printf("INFO:> Which questions you want to add to your final sheet? (indexes, sperated by spaces)\n");
	scanf(" %[^\n]s", questionIndexesString);
	
	int questions[100];
	char* q = questionIndexesString;
	
	char* token;
	int i = 0;
	while((token = strsep(&q," ")) != NULL) {
		questions[i] = atoi(token);
		++i;
	}
	int lineCounter = 0;
	while(fgets(line, sizeof(line) ,questionDAO.file) != NULL) { // giving index here, if index matches, we do copy that row
		int toCopy = 0; // forgot that there is no bool in C :(
		int i;
		for (i = 0; i < cnt; ++i) {
			if (questions[i] == lineCounter) {
				toCopy = 1;
			}
		}
		if (toCopy == 1) {
			fprintf(fp, line);
		}
		++lineCounter;
	}
	fclose(fp);
	printf("INFO:> Successfully saved your sheet!\n");
	fp = fopen(fname, "r");
	struct Question* _q = getQuestionsFromFinalsheet(fp);
	fclose(fp);
	fclose(questionDAO.file);
	
	return _q;
}

int main(int argc, char** argv) { 

	if (argc < 3) {
		printf("ERROR:> First and second launch parameter must be given! (sheet name)");
		return -1;
	}
	
	if (argc > 3) {
		printf("ERROR:> Too many launch parameters!");
		return -1;
	}
	
	int choice = -1;
	do {
		strcpy(questionDAO.fileName, argv[1]);
		questionDAO.file = fopen(questionDAO.fileName, "ab+");
		struct Question* questionPointer = getQuestionsFromSheet(questionDAO.file); // getting the questions from file
	
		//printf("%d", qCounter);
		printf("INFO:> Sheet processed... What do we want to do next?\n");
		printf("**********************************************\n");
		printf("MENU:> Add question: 1\n");
		printf("MENU:> Delete question: 2\n");
		printf("MENU:> Modify question: 3\n");
		printf("MENU:> Finalize sheet: 4\n");
		printf("MENU:> Exit: 5\n");
	
		scanf("%d", &choice);
		struct Question* temp_questionPointer;
		
		switch(choice) {
			case 1: 
			if (addQuestion(questionPointer) != -1);	 
				//getQuestionsFromSheet();
			else
				printf("ERROR:> Error adding question!");
			break;
			
			case 2: 
			if (deleteQuestion(questionPointer) != -1);
				//getQuestionsFromSheet(); 
			else
				printf("ERROR:> Error deleting question!");
			break;
			
			case 3: 
			if (modifyQuestion(questionPointer) != -1);
				//getQuestionsFromSheet();
			else
				printf("ERROR:> Error modifying question!");
			break;
			
			case 4:
		    temp_questionPointer = saveSheet(questionPointer);
			getRandomResultsFromCity(argv[2], temp_questionPointer);
			break;
			
			case 5:
			printf("INFO:> Thank you for using our software! Good bye!\n");
			break;
		}
	} while (choice != 5);
}

