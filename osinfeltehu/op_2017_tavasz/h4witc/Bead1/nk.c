#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

#define STRING_LENGTH (32)

typedef int bool;
enum { false, true };

typedef struct{
	char QText[STRING_LENGTH];
	char Answer1[STRING_LENGTH];
	char Answer2[STRING_LENGTH];
	char Answer3[STRING_LENGTH];
	char Answer4[STRING_LENGTH];
	time_t CreationTime;
	int Id;
	int finalized;
} Question;


void addQuestion();
void listQuestions();
void modifyQuestion();
void removeQuestion();
void finalizeQuestion();

int main(int argc, char *argv[]){
	
	printf("MLSZ Nemzeti konzultáció\n");
	printf("===================\n");
	bool end = false;
	Question* qarray = 0;
	int length = 0;
	
	while(!end){
		printf(
		"Kilepes: 0\n"
		"Uj kerdes: 1\n"
		"Kerdes modositas: 2\n"
		"Kerdes torles: 3\n"
		"Kerdesek listazasa: 4\n"
		"Kerdes veglegesitese: 5\n"
		);
		
		printf("\nValassz egy opciot: ");
		
		int option;
		scanf("%d",&option);
		
		switch(option){
			case 0:
				end = true;
				printf("Viszlat!\n");
				break;
			case 1:
				addQuestion();
				break;
			case 2:
				modifyQuestion();
				break;
			case 3:
				removeQuestion();
			case 4:
				listQuestions();
				break;
			case 5:
				finalizeQuestion();
				break;
			default:
				printf("Nincs ilyen opcio!\n\n");
		}	
	}
}


void addQuestion(){
	int f = open("data.txt", O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
	
	char QText[STRING_LENGTH];
	char Answer1[STRING_LENGTH];
	char Answer2[STRING_LENGTH];
	char Answer3[STRING_LENGTH];
	char Answer4[STRING_LENGTH];
	time_t currTime;
	int id;
	int c;
	
	if(read(f,&c,sizeof(int)) < 0){
		close(f);
		f = open("data.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		c = 0;
		id = 0;
		
		write(f,&c,sizeof(c));
		write(f,&id,sizeof(id));
	}

	c += 1;
	
	read(f,&id,sizeof(int));
	id += 1;
	close(f);
	f = open("data.txt", O_WRONLY, S_IRUSR | S_IWUSR);
	
	printf("Question text:");
	scanf("%s", QText);
	
	do{
		printf("Answer 1:");
		scanf("%s", Answer1);
	}while(strlen(q->Answer1)==0);
	
	do{
		printf("Answer 2:");
		scanf("%s", Answer2);
	}while(strlen(q->Answer2)==0);

	
	printf("Answer 3:");
	scanf("%s", Answer3);
	
	printf("Answer 4:");
	scanf("%s", Answer4);
	
	time(&currTime);
	
	f = open("data.txt", O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
	
	write(f,QText,sizeof(QText));
	write(f,Answer1,sizeof(Answer1));
	write(f,Answer2,sizeof(Answer2));
	write(f,Answer3,sizeof(Answer3));
	write(f,Answer4,sizeof(Answer4));
	write(f,&currTime,sizeof(time_t));
	write(f,&id,sizeof(id));
	
	close(f);
	
}

void modifyQuestion(){
	int f = open("data.txt",O_RDONLY);
	
	int num;
	int all;
	int modifyId;
	int id;
	
	if(read(f,&num,sizeof(int)) < 0 || num==0){
		close(f);
		
		printf("Meg nincsenek kerdesek!\n");
		return;
	}
	
	read(f,&all,sizeof(int));
	
	questions = (Question*) realloc(questions,num*sizeof(Question));
	read(f,questions,num*sizeof(Question));
	
	printf("\nMelyik kerdest modositja? Adjon meg egy sorszamot");
	scanf("%i", &modifyId);
	
	int i = 0;
	bool questionFound = false;

	while(i<num){
		if(questions[i].Id == modifyId){
			questionFound = true;
			printf("\nAdd meg az uj kerdest: ");
			scanf("%s", questions[i].QText);
			
			printf("\nAdj meg az uj elso valaszt: ");
			scanf("%s", questions[i].Answer1);
			
			printf("\nAdj meg az uj masodik valaszt: ");
			scanf("%s", questions[i].Answer2);
			
			printf("\nAdj meg az uj harmadik valaszt: ");
			scanf("%s", questions[i].Answer3);
			
			printf("\nAdj meg az uj negyedik valaszt: ");
			scanf("%s", questions[i].Answer4);
		}
		++i;
	}
	close(f);
	
	if(questionFound){
		f = open("data.txt",O_WRONLY);
		
		write(f,&num,sizeof(int));
		write(f,&all,sizeof(int));
		write(f,questions,num*sizeof(Question));
		
		close(f);
	}
	else{
		printf("Nincs ilyen vendeg!\n\n");
	}
	
	free(questions);
}

void removeQuestion(){
	int f = open("data.txt",O_RDONLY);
	
	int num;
	int all;
	int removeId;
	
	Question* questions = NULL;
	bool* removeData = NULL;

	if(read(f,&num,sizeof(int))<0 || num==0){
		close(f);
		
		printf("Meg nincs kerdes!\n");
		return;
	}
	read(f,&all,sizeof(int));
	
	int newNum = num;
	
	questions = (Question*) realloc(questions,num*sizeof(Question));
	read(f,questions,num*sizeof(Question));
	
	removeData = (bool*) realloc(removeData,num*sizeof(bool));
	
	printf("\nMelyik kerdest torli? Adjon egy sorszamot: ");
	scanf("%i", &removeId);
	
	int i = 0;

	while(i<num){
		if(questions[i].Id == removeId){
			removeData[i] = true;
			newNum -= 1;
		}
		else{
			removeData[i] = false;
		}
		++i;
	}
	close(f);
	f = open("data.txt",O_WRONLY | O_TRUNC);
	
	i = 0;
	
	write(f,&newNum,sizeof(int));
	write(f,&all,sizeof(int));
	
	while(i<num){
		if(!removeData[i]){
			write(f,&questions[i],sizeof(Question));
		}
		++i;
	}
	free(questions);
	free(removeData);
	close(f);
}

void listQuestions(){
	int f = open("data.txt", O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
	
	char QText[STRING_LENGTH];
	char Answer1[STRING_LENGTH];
	char Answer2[STRING_LENGTH];
	char Answer3[STRING_LENGTH];
	char Answer4[STRING_LENGTH];
	
	char* currTime;
	struct tm* timeInfo;
	
	int num;
	int all;

	if(read(f,&num,sizeof(int)) < 0 || num==0){
		close(f);
		printf("\nMeg ures a lista!\n\n");
		return;
	}
	read(f,&all,sizeof(int));
	printf("\n");
	int i=0;
	
	printf("\nKerdesek listazasa: \n");
	
	while(i<num){
		read(f,QText,sizeof(QText));
		read(f,Answer1,sizeof(Answer1));
		read(f,Answer2,sizeof(Answer2));
		read(f,Answer3,sizeof(Answer3));
		read(f,Answer4,sizeof(Answer4));
		read(f,&t,sizeof(time_t));
		timeInfo = localtime(&t);
		
		printf("Kerdes: %s\n",QText);
		printf("Valasz1: %s\n",Answer1);
		printf("Valasz2: %s\n",Answer2);
		printf("Valasz3: %s\n",Answer3);
		printf("Valasz4: %s\n",Answer4);
		printf("Regisztracio ideje: %s",asctime(timeInfo));
		++i;
	}
	
	printf("\n");
	
	close(f);
}

void finalizeQuestion(){
	
}