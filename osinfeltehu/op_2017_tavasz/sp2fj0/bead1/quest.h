#ifndef QUEST_H_INCLUDED
#define QUEST_H_INCLUDED
int menu();//menu fv
void createNewFile(char* fileName, int *isCreated);//uj file letrehozasa
void listForMlsz(char a[18][1000]);
void listQuest(char* fileName);
int newQuestion(char* fileName);
void flush();
int countLines(char* fileName);
void writeToFile(char* fileName, char* question, char* answer1, char* answer2, char* answer3, char* answer4);
void modifyQuestion(char* fileName);
void deleteQuestion(char* fileName, int todelete);
void emptyFile(char* fileName);
void finalizeForMlsz(char* fileName);
void finalizeFile(char* fileName, int *isCreated);
void listFinalized();
void deleteFile(char* fileName);

#endif // QUEST_H_INCLUDED
