#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#define bool int
#define true 1
#define false 0
#define BoolToStr(boolean) ((boolean) == true ? "true" : "false")

#define New(type) ((type*)(malloc(sizeof(type))))
#define NewArray(type, count) ((type*)(malloc((count) * sizeof(type))))
#define Delete(ptr) free(ptr)
#define ResizeArray(type, ptr, size) ((type*)(realloc(ptr, (size) * sizeof(type))))

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

int main(int argc, char **argv) 
{
	//TestTString();
	//TestTQuestion();
	ReadDatas();
	MainMenu();
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