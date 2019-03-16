#include "t_header.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <errno.h> //perror, errno
#include <time.h> //system time

//_GLOBALS_//////////////////////////////////////////////////
char GlobalBuffer[QUESTION_LENGTH];

int NumQuestions = 0;
struct Question Questions[MAX_QUESTIONS];
/////////////////////////////////////////////////////////////

void loadPollIfExists(){
    int FileDescriptor;
    FileDescriptor = open(POLLFILENAME, O_RDONLY);    
    bool OpeningError = FileDescriptor < 0;
    if (OpeningError){
        perror("Nincs előző kérdőív, amit meg lehetne nyitni.\n");
    }
    else{
        //First line is the umber of questions
        read(FileDescriptor, &NumQuestions);
		printf("\nA beolvasott kérdések száma: %d\n", NumQuestions);
		
		/*
		int TempId;
		read(FileDescriptor, &TempId, sizeof(TempId));
		printf("\n%d\n", &TempId);
		*/
		
        //Additional lines are the questions themselves
		char Text[512];
		read(FileDescriptor, Text, sizeof(Text));
		printf("\n%s\n", Text);
        
		/*
		int Idx;
		struct Question TempQuestion;
        for (Idx = 0; Idx < NumQuestions; Idx++){
            
			read(FileDescriptor, &Questions[Idx], sizeof(struct Question));
			printf("\n%s\n", Questions[Idx].Text);
			
			read(FileDescriptor, &TempQuestion, sizeof(struct Question));
			printf("\n%s\n", TempQuestion.Text);
			
			
			read(FileDescriptor, TempQuestion.Text, sizeof(TempQuestion.Text));
			printf("\n%s\n", TempQuestion.Text);
        }
		*/
		
        close(FileDescriptor);
        printf("Az előző kérdőív betöltésre került.\n");
    }
}

void menu(){
	int cmd;
	do{
		displayMainMenu();
			
		scanf("%d", &cmd);
		switch (cmd){   
			case 0:
				break;
			case 1:
				listQuestions();
				break;
			case 2:
				newQuestion();
				break;
			case 3:
				modifyQuestion();
				break;
			case 4:
				deleteQuestion();
				break;
			case 5:
				finalisePoll();
				break;
		}
	}while(cmd != 0);
}

void displayMainMenu(){
    printf("\no-Kérdőív---------------------------o\n");
	printf("|[0] - Kilépés                      |\n");
	printf("o-Kérdések--------------------------o\n");
	printf("|[1] - Kérdések listázása           |\n");
	printf("|[2] - Új kérdés                    |\n");
	printf("|[3] - Kérdés módosítása            |\n");
	printf("|[4] - Kérdés törlése               |\n");
	printf("o-----------------------------------o\n");
	printf("|[5] - Kérdőív véglegesítése        |\n");
	printf("o-----------------------------------o\n\n");
}

void waitForInput(){
    int cmd;
    printf("Nyomj [0]-t a menü megjelenítéséhez.");
    do{
        scanf("%d", &cmd);
    }while(cmd != 0);
    printf("\n");
}

void newQuestion(){
    printf("A kérdés: ");
    char* QuestionText = getText();
    
    addQuestion(QuestionText);
}

char* getText(){
    //copying to Global Buffer to have a valid
    //location in the memory upon function return
    scanf("%s", GlobalBuffer);
    printf("\n");
    
    return GlobalBuffer;
}

void addQuestion(char* Name){
    Questions[NumQuestions].Id = NumQuestions;
    
    strcpy(Questions[NumQuestions].Text, Name);
    
    struct tm* TimeInfo;
    time_t RawTime;
    time(&RawTime);
    TimeInfo = localtime(&RawTime);
    Questions[NumQuestions].TimeOfAddition = asctime(TimeInfo);
    
    //Initialising the answer count to 0
    Questions[NumQuestions].TheAnswers.Count = 0;
    
    NumQuestions++;
}

void modifyQuestion(){
    listQuestions();
    
    printf("Módosítandó kérdés száma: ");
    int QuestionId = getId();
    
    if(QuestionId <= NumQuestions){
        modificationMenu(QuestionId);
    }
    else{
        printf("Nincs ilyen számú kérdés.\n");
    }
}

void listQuestions(){
    if (NumQuestions > 0){
        int i;
        for (i = 0; i < NumQuestions; i++){
            printf("[%d]: %s?\n", Questions[i].Id, Questions[i].Text);
        }
    }
    else{
        printf("Még nincs megadva kérdés\n");
    }
}

int getId(){
    int Id;
    scanf("%d", &Id);
    printf("\n");
    return Id;
}

void modificationMenu(int QuestionId){
    int cmd;
	do{
		displayModificationMenu(QuestionId);
			
		scanf("%d", &cmd);
		switch (cmd){   
			case 0:
			    //Returns to main menu
				break;
			case 1:
				listAnswers(QuestionId);
				break;
			case 2:
				newAnswer(QuestionId);
				break;
			case 3:
				deleteAnswer(QuestionId);
				break;
			case 4:
				modifyQuestionText(QuestionId);
				break;
		}
	}while(cmd != 0); 
}

void displayModificationMenu(int QuestionId){
    printf("o-%d. kérdés-------------------------o\n", QuestionId);
	printf("|[0] - Visszalépés a főmenübe       |\n");
	printf("|[1] - Válaszok listázása           |\n");
	printf("|[2] - Új válasz a kérdésre         |\n");
	printf("|[3] - Válasz törlése               |\n");
	printf("|[4] - Kérdés szövegének módosítása |\n");
	printf("o-----------------------------------o\n\n");
}

void deleteQuestion(){
    listQuestions();
    
    printf("Törlendő kérdés száma: ");
    int QuestionId = getId();
    
    //Making temporary storage
    struct Question TempQuestions[MAX_QUESTIONS];
    int Idx;
    int TempIdx = 0;
    for (Idx = 0; Idx < NumQuestions; Idx++){
        if (QuestionId != Idx){
            TempQuestions[TempIdx] = Questions[Idx];
            TempIdx++;
        }
    }
    NumQuestions--;
    
    //Copying modified data back to source container
    for (Idx = 0; Idx < NumQuestions; Idx++){
        Questions[Idx] = TempQuestions[Idx];
        Questions[Idx].Id = Idx;
    }
}

void listAnswers(int QuestionId){
    int AnswerCount = getAnswerCount(QuestionId);
    if (AnswerCount > 0){
        int i;
        for (i = 0; i < AnswerCount; i++){
            printf("%d. %s\n", i, getAnswerText(QuestionId, i));
        }
    }
    else{
        printf("A %d. kérdéshez nem tartoznak válaszok.\n", QuestionId);
    }
}

int getAnswerCount(int QuestionId){
    return Questions[QuestionId].TheAnswers.Count;
}

char* getAnswerText(int QuestionId, int Idx){
    return Questions[QuestionId].TheAnswers.Text[Idx];
}

void newAnswer(int QuestionId){
    int AnswerCount = getAnswerCount(QuestionId);
    if (AnswerCount < MAX_ANSWERS){
        printf("A válasz: ");
        strcpy(Questions[QuestionId].TheAnswers.Text[AnswerCount], getText());
        
        Questions[QuestionId].TheAnswers.Count++;
    }
    else {
        printf("A %d. kérdéshez már maximális számú válasz tartozik.\n", QuestionId);
    }
}

void deleteAnswer(int QuestionId){
    int AnswerCount = getAnswerCount(QuestionId);
    if (AnswerCount > 0){
        listAnswers(QuestionId);
        
        printf("Törlendő válasz száma: ");
        int AnswerId = getId();
        
        //Making temporary storage
        struct Answers TempAnswers;
        int Idx;
        int TempIdx = 0;
        for (Idx = 0; Idx < AnswerCount; Idx++){
            if (AnswerId != Idx){
                //strcpy?
                strcpy(TempAnswers.Text[TempIdx], 
                    Questions[QuestionId].TheAnswers.Text[Idx]);
                TempIdx++;
            }
        }
        AnswerCount--;
    
        //Copying modified data back to source container
        for (Idx = 0; Idx < AnswerCount; Idx++){
            strcpy(Questions[QuestionId].TheAnswers.Text[Idx],
                TempAnswers.Text[Idx]);
        }
        Questions[QuestionId].TheAnswers.Count = AnswerCount;
    }
    else {
        printf("A %d. kérdéshez nem tartozik válasz.\n", QuestionId);
    }
}


void modifyQuestionText(int QuestionId){
    printf("Az új kérdés: ");
    char* QuestionText = getText();
    strcpy(Questions[QuestionId].Text, QuestionText);
}

void finalisePoll(){
    if (questionsHaveCorrectAnswerCount()){
        writePollToFile();
    }
}

bool questionsHaveCorrectAnswerCount(){
    bool ret = true;
    int Idx;
    if (NumQuestions > 0){
        for (Idx = 0; Idx < NumQuestions; Idx++){
            int AnswerCount = getAnswerCount(Idx);
            if (AnswerCount < MIN_ANSWERS){
                ret = false;
            }
            if (AnswerCount > MAX_ANSWERS){
                ret = false;
            }
        }
    }
    else{
        printf("Még nincs megadva kérdés.\n");
        ret = false;
    }
    
    return ret;
}

void writePollToFile(){
    int FileDescriptor;
    FileDescriptor =
        open(POLLFILENAME, O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
    bool OpeningError = FileDescriptor < 0;
    if (OpeningError){
        perror("Hiba a fájl megnyitásánál.\n");
        exit(1);
    }
    
    //First line is the number of questions
    bool WritingError =
            write(FileDescriptor, &NumQuestions, sizeof(NumQuestions))
            !=
            sizeof(NumQuestions);
    printf("\nA kérdések száma: %d\n", NumQuestions);
	if (WritingError){
        perror("Hiba a fájlba írásnál.\n");
        exit(1);
    }
    
    //Additional lines are the questions themselves
    int Idx;
    for (Idx = 0; Idx < NumQuestions; Idx++){
        bool WritingError =
            write(FileDescriptor, &Questions[Idx], sizeof(struct Question))
            !=
            sizeof(struct Question);
        if (WritingError){
            perror("Hiba a fájlba írásnál.\n");
            exit(1);
        }
    }
    
    close(FileDescriptor);
    printf("A kérdőív sikeresen mentve lett.\n");
}
