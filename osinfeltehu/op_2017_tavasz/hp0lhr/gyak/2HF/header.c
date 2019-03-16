#include "header.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <errno.h> //perror, errno
#include <time.h> //system time

#include <unistd.h> // for pipe()
#include <sys/time.h> //srand

//_GLOBALS_//////////////////////////////////////////////////
char GlobalBuffer[QUESTION_LENGTH];

int NumQuestions = 0;
struct Question Questions[MAX_QUESTIONS];
/////////////////////////////////////////////////////////////

bool loadPollIfExists(){
    bool hasLoaded = false;
    int FileDescriptor;
    FileDescriptor = open(POLLFILENAME, O_RDONLY);    
    bool OpeningError = FileDescriptor < 0;
    if (OpeningError){
        perror("Nincs előző kérdőív, amit meg lehetne nyitni.\n");
    }
    else{
        //First line is the umber of questions
        read(FileDescriptor, &NumQuestions, sizeof(NumQuestions));
    
        //Additional lines are the questions themselves
        int Idx;
        for (Idx = 0; Idx < NumQuestions; Idx++){
            read(FileDescriptor, &Questions[Idx], sizeof(struct Question));
        }
    
        close(FileDescriptor);
        printf("Az előző kérdőív betöltésre került.\n");
        
        hasLoaded = true;
    }
    
    return hasLoaded;
}

void menu(){
	int cmd;
    bool exit = false;
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
			case 6:
			    displayPoll();
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
	printf("|[6] - Kérdőív megjelenítése        |\n");
	printf("o-----------------------------------o\n\n");
}

void waitForInput(){
    int cmd;
    printf("Nyomj [0]-t a továbblépéshez.");
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
    scanf(" %[^\n]s", GlobalBuffer);
    //fgets(GlobalBuffer, sizeof(GlobalBuffer), stdin);
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
    strcpy(Questions[NumQuestions].TimeOfAddition, asctime(TimeInfo));
    
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
            printf("[%d]: %s? - %s", Questions[i].Id, Questions[i].Text, Questions[i].TimeOfAddition);
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
        char* AnswerText = getText();
        strcpy(Questions[QuestionId].TheAnswers.Text[AnswerCount], AnswerText);
        
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
                printf("Az %d. kérdésnek túl kevés válasza van.\n", Idx);
            }
            if (AnswerCount > MAX_ANSWERS){
                ret = false;
                printf("Az %d. kérdésnek túl sok válasza van.\n", Idx);
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

void displayPoll(){
    if (NumQuestions > 0){
        int i;
        for (i = 0; i < NumQuestions; i++){
            //Questions
            printf("\n%d. %s\n", Questions[i].Id, Questions[i].Text);
            
            //Answers
            int AnswerCount = getAnswerCount(i);
            if (AnswerCount > 0){
                int j;
                for (j = 0; j < AnswerCount; j++){
                    printf("   [] %s\n", getAnswerText(i, j));
                }
            }
            else{
                printf("A %d. kérdéshez nem tartoznak válaszok.\n", i);
            }
        }
    }
}
/////////////////////////////////////////////////////////////
void startPolling(){
    char ReadBuffer[PIPE_BUFFER_SIZE];

    int MLSZToQueryPipe[2];
    createPipe(MLSZToQueryPipe);
    int QueryToMLSZPipe[2];
    createPipe(QueryToMLSZPipe);

    int ReadEnd = 0;
    int WriteEnd = 1;
    pid_t PId = fork();
    bool ForkError = PId == -1;
    if (ForkError){
        perror("Fork hiba!");
        exit(EXIT_FAILURE);
    }
    
    bool isChild = PId == 0;
    if (isChild){
        close(MLSZToQueryPipe[WriteEnd]);
        close(QueryToMLSZPipe[ReadEnd]);

        //QueryMan();
        //receiveDestination();
        read(MLSZToQueryPipe[ReadEnd], ReadBuffer, sizeof(ReadBuffer));
        printf("KB: Megérkeztem ide: %s\n", ReadBuffer);
        char msg[] = "megérkeztem";
        printf("KB: Visszajelzek hogy helyszínen vagyok.\n");
        write(QueryToMLSZPipe[WriteEnd], msg, sizeof(msg));

        //receiveQuestions();
        struct Question ChoosenQuestions[QUESTIONS_TO_SEND_COUNT];
        int i;
        for (i = 0; i < QUESTIONS_TO_SEND_COUNT; ++i){
            read(MLSZToQueryPipe[ReadEnd], &ChoosenQuestions[i], sizeof(struct Question));
            printf("KB: Megérkezett a %d kérdés.\n", ChoosenQuestions[i].Id);
        }
        waitForInput();

        //queryPeople();
        int ConsultantCount = generateIntBetween(CONSULTANT_COUNT_MIN, CONSULTANT_COUNT_MAX);
        printf("KB: Konzultánsok száma: %d\n", ConsultantCount);
        Poll KBPoll[CONSULTANT_COUNT_MAX];
        fillQuery(KBPoll, ChoosenQuestions, ConsultantCount);
        waitForInput();
        
        //sendResult();
        write(QueryToMLSZPipe[WriteEnd], &ConsultantCount, sizeof(ConsultantCount));
        write(QueryToMLSZPipe[WriteEnd], &KBPoll, sizeof(Poll)* ConsultantCount);
        
        printf("KB: Befejeztem a konzultációt.\n");
        close(MLSZToQueryPipe[ReadEnd]);
        close(QueryToMLSZPipe[WriteEnd]);

    } else if (!isChild){
        close(MLSZToQueryPipe[ReadEnd]);
        close(QueryToMLSZPipe[WriteEnd]);
        
        //MLSZ();
        printf("MLSZ: A kérdezőbiztos úticélja: ");
        char* Destination = getText();
        printf("MLSZ: Elküldöm a kérdezőbiztost ide: %s\n", Destination);
        write(MLSZToQueryPipe[WriteEnd], Destination, sizeof(Destination));
        read(QueryToMLSZPipe[ReadEnd], ReadBuffer, sizeof(ReadBuffer));
        if (strcmp(ReadBuffer, "megérkeztem") == 0){
            printf("MLSZ: Megkaptam a visszajelzést.\n");
            waitForInput();

            //sendQuestions();
            //chooseQuestions();
            printf("MLSZ: Kiválasztom a kérdéseket.\n");
            int InitValue = -1;
            //initArray(RandInts, InitValue);
            int RandInts[QUESTIONS_TO_SEND_COUNT];
            int j;
            for (j = 0; j < QUESTIONS_TO_SEND_COUNT; ++j){
                RandInts[j] = InitValue;
            }

            int i = 0;
            bool HasDuplicates = true;
            do{
                int RandInt = generateIntBetween(0, NumQuestions - 1);
                addRandInt(RandInts, RandInt, i++);
                HasDuplicates = checkDuplicates(RandInts, InitValue);
            } while (HasDuplicates );//&& i >= QUESTIONS_TO_SEND_COUNT);
            printf("MLSZ: A kiválasztott kérdések: %d., %d., %d.\n", RandInts[0], RandInts[1], RandInts[2]);
            
            //send()
            for (i = 0; i < QUESTIONS_TO_SEND_COUNT; ++i){
                write(MLSZToQueryPipe[WriteEnd], &Questions[RandInts[i]], sizeof(struct Question));
            }
            
            //receivePoll()
            int ConsultantCount;
            read(QueryToMLSZPipe[ReadEnd], &ConsultantCount, sizeof(ConsultantCount));
            Poll MLSZPoll[CONSULTANT_COUNT_MAX];
            read(QueryToMLSZPipe[ReadEnd], &MLSZPoll, sizeof(Poll)* ConsultantCount);
            
            //displayFilledPoll();
            printf("MLSZ: A kitöltött kérdőívek:");
            for (i = 0; i < ConsultantCount; ++i){
                int j;
                for (j = 0; j < QUESTIONS_TO_SEND_COUNT; ++ j){
                    //Questions
                    printf("\n%d. %s\n", MLSZPoll[i].FQS[j].Id, MLSZPoll[i].FQS[j].Text);
            
                    //Answers
                    int k;
                    for (k = 0; k < MLSZPoll[i].FQS[j].TheAnswers.Count; ++k){
                        printf("   [x] %s\n", MLSZPoll[i].FQS[j].TheAnswers.Text[k]);
                    }
                }
                printf("______________________________\n");
            }
        }
        close(MLSZToQueryPipe[WriteEnd]);
        close(QueryToMLSZPipe[ReadEnd]);
        //int status;
        //waitpid(PId,&status,0);
        wait();
        printf("MLSZ: Befejeztem mára.\n"); 
    }
}

void createPipe(int* Descriptor){
    int PipeReturnValue = pipe(Descriptor);
    bool PipeError = PipeReturnValue == -1;
    if (PipeError){
        perror("Hiba a pipe nyitáskor!\n");
        exit(EXIT_FAILURE);
    }
}

int generateIntBetween(int LowerBound, int UpperBound){
    //srand(time(NULL)); //uncomment for real results
    int RandInt = rand()%(UpperBound - LowerBound) + LowerBound;
    printf("    generated: %d\n", RandInt);
    return RandInt;
}

void addRandInt(int* RandInts, int RandInt, int Idx){
    if (Idx >= QUESTIONS_TO_SEND_COUNT){
        int RandIndex = generateIntBetween(0, QUESTIONS_TO_SEND_COUNT - 1);
        RandInts[RandIndex] = RandInt; //hope for the best
        printf("    added: %d to idx: %d\n", RandInt, RandIndex);
    } else {
        RandInts[Idx] = RandInt;
        printf("    added: %d to idx: %d\n", RandInt, Idx);
    }
}

bool checkDuplicates(int* RandInts, int InitValue){
    bool HasDuplicate = false;
    if (QUESTIONS_TO_SEND_COUNT >= 2){
        int i;
        for (i = 0; i < QUESTIONS_TO_SEND_COUNT - 1; ++i){
            printf("    comparing: %d -> %d\n", RandInts[i], RandInts[i + 1]);
            bool initValue1 = RandInts[i] == InitValue;
            bool initValue2 = RandInts[i + 1] == InitValue;
            bool Duplicate = RandInts[i] == RandInts[i + 1];
            if (Duplicate || initValue1 || initValue2){
                HasDuplicate = true;
            }
        }
    }
    printf("    %d (0: done, 1: continue)\n", HasDuplicate);
    return HasDuplicate;
}

void fillQuery(Poll* KBPoll, struct Question* ChoosenQuestions, int ConsultantCount){
    int i;
    for (i = 0; i < ConsultantCount; ++i){
        int j;
        for (j = 0; j < QUESTIONS_TO_SEND_COUNT; ++j){
            strcpy(KBPoll[i].FQS[j].Text, ChoosenQuestions[j].Text);
            strcpy(KBPoll[i].FQS[j].TimeOfAddition, ChoosenQuestions[j].TimeOfAddition);

            int Id;
            Id = ChoosenQuestions[j].Id;
            KBPoll[i].FQS[j].Id = Id;

            //answer(i, j)
            int AnswerCount;
            AnswerCount = getAnswerCount(Id);
            int RandAnswer = generateIntBetween(0, AnswerCount);

            KBPoll[i].FQS[j].TheAnswers.Count = 0;
            int k;
            int l = 0;
            for (k = 0; k < AnswerCount; ++k){
                if (k == RandAnswer){
                    strcpy(KBPoll[i].FQS[j].TheAnswers.Text[l++], ChoosenQuestions[j].TheAnswers.Text[k]);
                    KBPoll[i].FQS[j].TheAnswers.Count++;
                    printf("    %d. consultant answered %d to question %d\n", i, RandAnswer, Id);
                    //printf("    the answer was: %s\n", KBPoll[i].FQS[j].TheAnswers.Text[l - 1]);
                }
            }
        }
    }
}