#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>

// Boolean
#define bool               int
#define true               1
#define false              0
#define BoolToStr(boolean) ((boolean) == true ? "true" : "false")

// Memory
#define New(type)                    ((type*)(malloc(sizeof(type))))
#define NewArray(type, count)        ((type*)(malloc((count) * sizeof(type))))
#define Delete(ptr)                  free(ptr)
#define ResizeArray(type, ptr, size) ((type*)(realloc(ptr, (size) * sizeof(type))))

// Process
#define TProcID           pid_t
#define GetParentProcID() getppid()
#define GetProcID()       getpid()
#define CreateProcess()   fork()
#define IsSucceeded(pid)  ((pid) < 0 ? false : true)
#define IsChild(pid)      ((pid) == 0 ? true : false)
#define IsParent(pid)     ((pid) > 0 ? true : false)
#define WaitForChildren() wait(NULL)
#define Sleep(tick)       sleep(tick)

// Signal
#define TSigSet                        sigset_t
#define ClearSigSet(sigset)            sigemptyset(&(sigset))
#define FillSigSet(sigset)             sigfillset(&(sigset))
#define AddToSigSet(sigset, sig)       sigaddset(&(sigset), sig) /* SIGTERM SIGUSR1 SIGUSR2   SIGKILL SIGSTOP */
#define RemoveFromSigSet(sigset, sig)  sigdelset(&(sigset), sig)
#define SetSignalMasking(sigset, mode) sigprocmask(mode, &(sigset), NULL) /* SIG_BLOCK SIG_UNBLOCK SIG_SETMASK */
#define TSigHndl                       struct sigaction
#define InitSigHndl(sighndl, handler)  { sighndl.sa_handler = handler; sigemptyset(&sighndl.sa_mask); sighndl.sa_flags = 0; } /* SIG_DFL SIG_IGN */
#define SetSigHndl(sighndl, sig)       sigaction(sig, &sighndl, NULL);
#define Suspend(sig)                   { sigset_t __sigset__; sigfillset(&__sigset__); sigdelset(&__sigset__, sig); sigsuspend(&__sigset__); }
#define SendSignal(pid, sig)           kill(pid, sig)

// Timer
#define TTimer                               struct itimerval
#define SetSigHndlForTimer(sighndl, handler) { sigemptyset(&sighndl.sa_mask); sighndl.sa_handler = handler; sighndl.sa_flags = 0; sigaction(SIGALRM, &sighndl, NULL); }
#define InitTimer(timer, interval)           { timer.it_interval.tv_sec = 0; timer.it_interval.tv_usec = interval * 1000; timer.it_value.tv_sec = 0; timer.it_value.tv_usec = timer.it_interval.tv_usec; }
#define StartTimer(timer)                    setitimer(ITIMER_REAL, &timer, NULL)
#define SetTimerInterval(timer, interval)    { timer.it_interval.tv_sec = 0; timer.it_interval.tv_usec = interval * 1000; }
#define SetTimerTime(timer, timeval)         { timer.it_value.tv_sec = 0; timer.it_value.tv_usec = timeval * 1000; }
#define StopTimer(timer)                     { timer.it_interval.tv_sec = 0; timer.it_interval.tv_usec = 0; timer.it_value.tv_sec = 0; timer.it_value.tv_usec = 0; }
#define GetTimer(timer)                      getitimer(ITIMER_REAL, &timer)
#define GetTimerInterval(timer)              (timer.it_interval.tv_usec / 1000)
#define GetTimerTime(timer)                  (timer.it_value.tv_usec / 1000)

// Pipe
#define TPipe(pipename)                       int pipename[2]
#define CreatePipe(pipename)                  (pipe(pipename) < 0 ? false : true)
#define ReadFromPipe(pipename, buffer, count) read(pipename[0], buffer, count)
#define WriteToPipe(pipename, buffer, count)  write(pipename[1], buffer, count)
#define ClosePipeRead(pipename)               close(pipename[0])
#define ClosePipeWrite(pipename)              close(pipename[1])
#define ClosePipe(pipename)                   { close(pipename[0]); close(pipename[1]); }

char GetByte(int value, int nth)
{
	return (char)((value & (0xFF << (nth * 8))) >> (nth * 8));
}

int MergeBytes2(char byte2, char byte1)
{
	return (((int)(byte2) & 0xFF) << 8) | ((int)(byte1) & 0xFF);
}

int MergeBytes4(char byte4, char byte3, char byte2, char byte1)
{
	return (MergeBytes2(byte4, byte3) << 16) | MergeBytes2(byte2, byte1);
}

char* TimeToStr(time_t t)
{
	char *chs = NewArray(char, 80);
	strftime(chs, 80, "%Y.%m.%d. %H:%M", localtime(&t));
	return chs;
}

#define PATH "datas.dat"

//--------------------------------------------------------------------------------------------------
// TString

#define TString struct _TString

struct _TString
{
	int length;
	char *chars;
};

void DeleteTString(TString *pstr)
{
	if (pstr->chars != NULL)
	{
		Delete(pstr->chars);
		pstr->chars = NULL;
	}
	Delete(pstr);
}

TString* NewTString(char *pchs)
{
	TString *str = New(TString);
	str->length = strlen(pchs);
	str->chars = NewArray(char, str->length);
	strcpy(str->chars, pchs);
	return str;
}

void TestTString()
{
	TString *str = NewTString("Hello World!");
	printf("TestTString: %s %d\n", str->chars, str->length);
	DeleteTString(str);
	str = NULL;
}

//--------------------------------------------------------------------------------------------------
// TQuestion

#define TQuestion struct _TQuestion

struct _TQuestion
{
	TString *question, **answers;
	int answercount;
	time_t creationtime;
	bool finalized;
};

void DeleteTQuestion(TQuestion *pquest)
{
	if (pquest->question != NULL)
	{
		DeleteTString(pquest->question);
		pquest->question = NULL;
	}
	if (pquest->answers != NULL)
	{
		int i;
		for (i = 0; i < pquest->answercount; ++i)
			DeleteTString(pquest->answers[i]);
		Delete(pquest->answers);
		pquest->answers = NULL;
	}
	Delete(pquest);
}

TQuestion* NewTQuestion(char *pquest, char **panss, int panscount)
{
	TQuestion* quest = New(TQuestion);
	quest->question = NewTString(pquest);
	quest->answercount = panscount;
	quest->answers = NewArray(TString*, quest->answercount);
	int i;
	for (i = 0; i < quest->answercount; ++i)
		quest->answers[i] = NewTString(panss[i]);
	time(&quest->creationtime);
	quest->finalized = false;
	return quest;
}

void TQuestionToBinary(TQuestion *pquest, char **pbytes, int *plength)
{
	int length = 2 + pquest->question->length;
	int i;
	length += 1;
	for (i = 0; i < pquest->answercount; ++i)
		length += 2 + pquest->answers[i]->length;
	length += 4 + 1;
	*plength = length;
	
	*pbytes = NewArray(char, length);	
	int pos = 0;
	(*pbytes)[pos++] = GetByte(pquest->question->length, 0);
	(*pbytes)[pos++] = GetByte(pquest->question->length, 1);
	for (i = 0; i < pquest->question->length; ++i)
		(*pbytes)[pos++] = pquest->question->chars[i];
	(*pbytes)[pos++] = GetByte(pquest->answercount, 0);
	for (i = 0; i < pquest->answercount; ++i)
	{
		(*pbytes)[pos++] = GetByte(pquest->answers[i]->length, 0);
		(*pbytes)[pos++] = GetByte(pquest->answers[i]->length, 1);
		int j;
		for (j = 0; j < pquest->answers[i]->length; ++j)
			(*pbytes)[pos++] = pquest->answers[i]->chars[j];
	}
	(*pbytes)[pos++] = GetByte(pquest->creationtime, 0);
	(*pbytes)[pos++] = GetByte(pquest->creationtime, 1);
	(*pbytes)[pos++] = GetByte(pquest->creationtime, 2);
	(*pbytes)[pos++] = GetByte(pquest->creationtime, 3);
	(*pbytes)[pos++] = GetByte(pquest->finalized, 0);
}

TQuestion* BinaryToTQuestion(char *pbytes)
{
	int pos = 0;
	int l = MergeBytes2(pbytes[pos++], pbytes[pos++]);
	char *chsquest = NewArray(char, l + 1);
	int i;
	for (i = 0; i < l; ++i)
		chsquest[i] = pbytes[pos++];
	chsquest[l] = '\0';
	
	int anscount = pbytes[pos++];
	char **anss = NewArray(char*, anscount);
	for (i = 0; i < anscount; ++i)
	{
		int j;
		l = MergeBytes2(pbytes[pos++], pbytes[pos++]);
		anss[i] = NewArray(char, l + 1);
		for (j = 0; j < l; ++j)
			anss[i][j] = pbytes[pos++];
		anss[i][l] = '\0';
	}
	
	time_t t = MergeBytes4(pbytes[pos++], pbytes[pos++], pbytes[pos++], pbytes[pos++]);
	bool fin = pbytes[pos++];
	
	TQuestion *quest = NewTQuestion(chsquest, anss, anscount);
	quest->creationtime = t;
	quest->finalized = fin;
	return quest;
}

void TestTQuestion()
{
	printf("TestTQuestion:\n");
	
	char **anss = NewArray(char*, 3);
	anss[0] = NewArray(char, 256); strcpy(anss[0], "Bla");
	anss[1] = NewArray(char, 256); strcpy(anss[1], "Bla bla");
	anss[2] = NewArray(char, 256); strcpy(anss[2], "Bla bla bla");
	TQuestion *quest = NewTQuestion("Bla bla bla?", anss, 3);
	Delete(anss[0]); Delete(anss[1]); Delete(anss[2]); Delete(anss);
	quest->finalized = true;
	
	printf("  %s\n", quest->question->chars);
	int i;
	for (i = 0; i < quest->answercount; ++i)
		printf("    %s\n", quest->answers[i]->chars);
	char *strt = TimeToStr(quest->creationtime);
	printf("  %s\n", strt);
	Delete(strt);
	printf("  %s\n", BoolToStr(quest->finalized));
	
	char *bytes = NULL;
	int l = 0;
	TQuestionToBinary(quest, &bytes, &l);
	TQuestion *quest2 = BinaryToTQuestion(bytes);
	Delete(bytes);
	printf("  -----------------\n");
	
	printf("  %s\n", quest2->question->chars);
	for (i = 0; i < quest2->answercount; ++i)
		printf("    %s\n", quest2->answers[i]->chars);
	strt = TimeToStr(quest2->creationtime);
	printf("  %s\n", strt);
	Delete(strt);
	printf("  %s\n", BoolToStr(quest2->finalized));
	
	DeleteTQuestion(quest);
	DeleteTQuestion(quest2);
}

//--------------------------------------------------------------------------------------------------
// Datas

TQuestion **quests = NULL;
int questcount = 0;

void ReleaseDatas()
{
	if (quests != NULL)
	{
		int i;
		for (i = 0; i < questcount; ++i)
			DeleteTQuestion(quests[i]);
		Delete(quests);
		quests = NULL;
		questcount = 0;
	}
}

void AddToDatas(TQuestion *quest)
{
	quests = ResizeArray(TQuestion*, quests, questcount + 1);
	quests[questcount] = quest;
	questcount++;
}

void RemoveFromDatas(int index)
{
	TQuestion **nquests = NewArray(TQuestion*, questcount - 1);
	int i;
	for (i = 0; i < index; ++i)
		nquests[i] = quests[i];
	DeleteTQuestion(quests[index]);
	for (i = index + 1; i < questcount; ++i)
		nquests[i - 1] = quests[i];
	Delete(quests);
	quests = nquests;
	questcount--;
}

void ReadDatas()
{
	if (access(PATH, F_OK) < 0)
	{
		ReleaseDatas();
		return;
	}
	
	int f = open(PATH, O_RDONLY);
	if (f <= 0)
	{
		printf("ERROR: Could not open the \"%s\" file!\n", PATH);
		exit(EXIT_FAILURE);
	}
	
	int count;
	if (read(f, &count, sizeof(int)) != sizeof(int))
	{
		printf("ERROR: Could not read the \"%s\" file properly!\n", PATH);
		exit(EXIT_FAILURE);
	}
	
	if (count > 0)
	{
		ReleaseDatas();
		questcount = count;		
		quests = NewArray(TQuestion*, questcount);
		int i;
		for (i = 0; i < questcount; ++i)
		{
			int l = 0;
			if (read(f, &l, sizeof(int)) != sizeof(int))
			{
				printf("ERROR: Could not read the \"%s\" file properly!\n", PATH);
				exit(EXIT_FAILURE);
			}
			char *bytes = NewArray(char, l);
			if (read(f, bytes, l) != l)
			{
				printf("ERROR: Could not read the \"%s\" file properly!\n", PATH);
				exit(EXIT_FAILURE);
			}
			quests[i] = BinaryToTQuestion(bytes);
			Delete(bytes);
		}
	}
	else
		ReleaseDatas();
	
	close(f);
}

void WriteDatas()
{
	int f = open(PATH, O_WRONLY | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR);
	if (f <= 0)
	{
		printf("ERROR: Could not open the \"%s\" file!\n", PATH);
		exit(EXIT_FAILURE);
	}
	
	if (write(f, &questcount, sizeof(int)) != sizeof(int))
	{
		printf("ERROR: Could not write the \"%s\" file properly!\n", PATH);
		exit(EXIT_FAILURE);
	}
	
	int i;
	for (i = 0; i < questcount; ++i)
	{
		int l = 0;
		char *bytes = NULL;
		TQuestionToBinary(quests[i], &bytes, &l);
		if (write(f, &l, sizeof(int)) != sizeof(int))
		{
			printf("ERROR: Could not write the \"%s\" file properly!\n", PATH);
			exit(EXIT_FAILURE);
		}
		if (write(f, bytes, l) != l)
		{
			printf("ERROR: Could not write the \"%s\" file properly!\n", PATH);
			exit(EXIT_FAILURE);
		}
		Delete(bytes);
	}

	close(f);
}

//--------------------------------------------------------------------------------------------------

char* GetLineFromConsole()
{
    int maxlength = 4,
	    length = 0;
    char *line = NewArray(char, maxlength);
    while (true)
	{
        int ch = getchar();
        if (ch == EOF || ch == '\n')
            break;
        if(length >= maxlength - 1)
		{
			maxlength *= 2;
            line = ResizeArray(char, line, maxlength);
        }
		line[length++] = ch;
    }
    line[length] = '\0';
    return line;
}

int GetIntFromConsole()
{
	int integer;
	char *line = GetLineFromConsole();
	sscanf(line, "%d", &integer);
	Delete(line);
	return integer;
}

char GetCharFromConsole()
{
	char ch;
	char *line = GetLineFromConsole();
	sscanf(line, "%c", &ch);
	Delete(line);
	return ch;
}

void MainMenu();
void ListQuestions();
void AddQuestion();
void ModifyQuestion();
void DeleteQuestion();

void DoTask();
void ParentTask();
void ChildTask();

int main(int argc, char **argv) 
{
	//TestTString();
	//TestTQuestion();
	ReadDatas();
	MainMenu();
	DoTask();
	return 0;
}

void MainMenu()
{
	bool run = true;
	while (run)
	{
		printf("Options:\n");
		printf("0. List questions\n");
		printf("1. Add question\n");
		printf("2. Modify question\n");
		printf("3. Delete question\n");
		printf("4. Exit\n");
		printf("Choice: ");
		int choice = GetIntFromConsole();
		printf("\n");
		switch (choice)
		{
			case 0: ListQuestions(); break;
			case 1: AddQuestion(); break;
			case 2: ModifyQuestion(); break;
			case 3: DeleteQuestion(); break;
			case 4: run = false; break;
			default: printf("I love apples too!\n\n"); break;
		}
	}
}

void ListQuestions()
{
	if (questcount <= 0)
	{
		printf("There is no questions.\n\n");
		return;
	}
	
	int i;
	for (i = 0; i < questcount; ++i)
	{
		printf("%d. Question:\n  %s\n", i + 1, quests[i]->question->chars);
		int j;
		for (j = 0; j < quests[i]->answercount; ++j)
			printf("    %d. %s\n", j + 1, quests[i]->answers[j]->chars);
		char *strt = TimeToStr(quests[i]->creationtime);
		printf("  Creation time: %s\n", strt);
		Delete(strt);
		printf("  Finalized: %s\n", BoolToStr(quests[i]->finalized));
	}
	printf("\n");
}

void AddQuestion()
{
	printf("Question: ");
	char *chsquest = GetLineFromConsole();

	printf("Answer count: ");
	int anscount = GetIntFromConsole();
	if (anscount < 2 || 4 < anscount)
	{
		printf("You can add only 2-4 answers!\n\n");
		Delete(chsquest);
		return;
	}
	
	char **anss = NewArray(char*, anscount);
	int i;
	for (i = 0; i < anscount; ++i)
	{
		printf("Answer %d.: ", i + 1);
		anss[i] = GetLineFromConsole();
	}
	
	printf("Finalized? (y/n): ");
	char ch = GetCharFromConsole();
	bool fin = ch == 'y' || ch == 'Y' ? true : false;

	TQuestion *quest = NewTQuestion(chsquest, anss, anscount);
	quest->finalized = fin;
	AddToDatas(quest);
	WriteDatas();
	printf("New question successfully added!\n\n");
}

void DeleteQuestion()
{
	if (questcount <= 0)
	{
		printf("There is no questions.\n\n");
		return;
	}
	
	ListQuestions();
	
	printf("Question's index: ");
	int index = GetIntFromConsole() - 1;
	if (index < 0 || questcount <= index)
	{
		printf("Incorrect index!\n\n");
		return;
	}
	RemoveFromDatas(index);
	WriteDatas();
	printf("Question successfully deleted!\n\n");
}

void ModifyQuestion()
{
	if (questcount <= 0)
	{
		printf("There is no questions.\n\n");
		return;
	}
	
	ListQuestions();
	
	printf("Question's index: ");
	int index = GetIntFromConsole() - 1;
	if (index < 0 || questcount <= index)
	{
		printf("Incorrect index!\n\n");
		return;
	}
	
	printf("Options:\n");
	printf("0. Modify the question\n");
	printf("1. Modify an answers\n");
	printf("2. Delete an answers\n");
	printf("3. Add a new answers\n");
	printf("4. Modify finalization\n");
	printf("5. Return to main menu\n");
	printf("Choice: ");
	int choice = GetIntFromConsole();
	if (choice == 0)
	{
		printf("Question: ");
		char *chsquest = GetLineFromConsole();
		DeleteTString(quests[index]->question);
		quests[index]->question = NewTString(chsquest);
	}
	else if (choice == 1)
	{
		printf("Answer's index: ");
		int ansind = GetIntFromConsole() - 1;
		if (ansind < 0 || quests[index]->answercount <= ansind)
		{
			printf("Incorrect index!\n\n");
			return;
		}
		printf("Answer: ");
		char *ans = GetLineFromConsole();
		DeleteTString(quests[index]->answers[ansind]);
		quests[index]->answers[ansind] = NewTString(ans);
	}
	else if (choice == 2)
	{
		if (quests[index]->answercount <= 2)
		{
			printf("Answer count must remain minimum 2!\n\n");
			return;
		}
		printf("Answer's index: ");
		int ansind = GetIntFromConsole() - 1;
		if (ansind < 0 || quests[index]->answercount <= ansind)
		{
			printf("Incorrect index!\n\n");
			return;
		}
		TString **anss = NewArray(TString*, quests[index]->answercount - 1);
		int i;
		for (i = 0; i < ansind; ++i)
			anss[i] = quests[index]->answers[i];
		DeleteTString(quests[index]->answers[ansind]);
		for (i = ansind + 1; i < quests[index]->answercount; ++i)
			anss[i - 1] = quests[index]->answers[i];
		Delete(quests[index]->answers);
		quests[index]->answers = anss;
		quests[index]->answercount--;
	}
	else if (choice == 3)
	{
		if (quests[index]->answercount >= 4)
		{
			printf("Answer count must remain maximum 4!\n\n");
			return;
		}
		printf("Answer: ");
		char *ans = GetLineFromConsole();
		quests[index]->answers = ResizeArray(TString*, quests[index]->answers, quests[index]->answercount + 1);
		quests[index]->answers[quests[index]->answercount] = NewTString(ans);
		quests[index]->answercount++;
	}
	else if (choice == 4)
	{
		printf("Finalized? (y/n): ");
		char ch = GetCharFromConsole();
		bool fin = ch == 'y' || ch == 'Y' ? true : false;
		quests[index]->finalized = fin;
	}
	else if (choice == 5)
	{
		printf("\n");
		return;
	}
	else
	{
		printf("Oh! Apple pies! Nom nom nom...\n\n");
		return;
	}
	WriteDatas();
	printf("Question successfully modified!\n\n");
}

void SignalHandler()
{ }

TPipe(channel);

void DoTask()
{
	TSigHndl sighndl;
	InitSigHndl(sighndl, SignalHandler);
	SetSigHndl(sighndl, SIGTERM);
	
	if (!CreatePipe(channel))
	{
		printf("ERROR: Could not create pipe!\n");
		exit(EXIT_FAILURE);
	}
	
	TProcID pid = CreateProcess();
	if (!IsSucceeded(pid))
	{
		printf("ERROR: Could not create child process!\n");
		exit(EXIT_FAILURE);
	}
	if (IsParent(pid))
		ParentTask(pid);
	else
		ChildTask();
}

void ParentTask(TProcID child)
{
	printf("Parent: Starting...\n");
	
	printf("Parent: Waiting for signal...\n");
	Suspend(SIGTERM);
	printf("Parent: Signal received!\n");
	
	int i, count = 0;
	for (i = 0; i < questcount; ++i)
	{
		if (quests[i]->finalized)
			count++;
	}
	if (count < 3)
	{
		printf("ERROR: There is not enough finalized questions!\n");
		SendSignal(child, SIGKILL);
		WaitForChildren();
		exit(EXIT_FAILURE);
	}
	int *indices = NewArray(int, count);
	int j = 0;
	for (i = 0; i < questcount; ++i)
	{
		if (quests[i]->finalized)
			indices[j++] = i;
	}
	int *selectedquests = NewArray(int, 3);
	for (i = 0; i < 3; ++i)
	{
		bool ok = false;
		while (!ok)
		{
			selectedquests[i] = rand() % count;
			int j;
			for (j = 0; j < i; ++j)
			{
				if (selectedquests[j] == selectedquests[i])
					break;
			}
			if (j == i)
				ok = true;
		}
	}
	Delete(indices);
	
	for (i = 0; i < 3; ++i)
	{
		int l = 0;
		char *bytes = NULL;
		TQuestionToBinary(quests[selectedquests[i]], &bytes, &l);
		WriteToPipe(channel, &l, 4);
		WriteToPipe(channel, bytes, l);
		Delete(bytes);
	}
	//ClosePipe(channel);
	printf("Parent: Sending signal...\n");
	SendSignal(child, SIGTERM);
	printf("Parent: Signal sent!\n");
	
	printf("Parent: Waiting for signal...\n");
	Suspend(SIGTERM);
	printf("Parent: Signal received!\n");
	
	char *data = NewArray(char, 20 * 3 + 1);
	ReadFromPipe(channel, data, 20 * 3 + 1);
	ClosePipe(channel);
	int maxi = data[0];
	printf("Results: %d\n", maxi);
	for (i = 0; i < 3; ++i)
	{
		printf("  %d:%d. question: %s\n", i + 1, selectedquests[i] + 1, quests[selectedquests[i]]->question->chars);
		int j;
		for (j = 0; j < quests[selectedquests[i]]->answercount; ++j)
		{
			int k, c = 0;
			for (k = i; k < maxi * 3; k += 3)
			{
				if (data[k + 1] == j)
					c++;
			}
			printf("    %d. answer: %d\n", j + 1, c);
		}
	}
	Delete(selectedquests);
	
	WaitForChildren();
	printf("Parent: Done!\n");
}

void ChildTask()
{
	printf("Child: Starting...\n");
	Sleep(1);
	
	printf("Child: Sending signal...\n");
	SendSignal(GetParentProcID(), SIGTERM);
	printf("Child: Signal sent!\n");

	printf("Child: Waiting for signal...\n");
	Suspend(SIGTERM);
	printf("Child: Signal received!\n");
	
	char *data = NewArray(char, 8192),
	     *ptr;
	ReadFromPipe(channel, data, 8192);
	//ClosePipe(channel);
	ptr = data;
	TQuestion **cquests = NewArray(TQuestion*, 3);
	int i;
	for (i = 0; i < 3; ++i)
	{
		int l = MergeBytes4(ptr[3], ptr[2], ptr[1], ptr[0]);
		cquests[i] = BinaryToTQuestion(ptr + 4);
		ptr += l + 4;
	}
	ptr = NULL;
	Delete(data);
	
	char maxi = (rand() % 11) + 10;
	char *answers = NewArray(char, maxi * 3);
	for (i = 0; i < maxi; ++i)
	{
		int j;
		for (j = 0; j < 3; ++j)
			answers[i * 3 + j] = rand() % cquests[j]->answercount;
	}
	for (i = 0; i < 3; ++i)
		DeleteTQuestion(cquests[i]);
	Delete(cquests);
	WriteToPipe(channel, &maxi, 1);
	WriteToPipe(channel, answers, maxi * 3);
	ClosePipe(channel);
	Delete(answers);
	
	printf("Child: Sending signal...\n");
	SendSignal(GetParentProcID(), SIGTERM);
	printf("Child: Signal sent!\n");
	
	printf("Child: Done!\n");
}
