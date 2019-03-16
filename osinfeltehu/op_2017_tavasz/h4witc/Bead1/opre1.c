#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define STRING_LENGTH (32)


typedef struct{
	char QText[STRING_LENGTH];
	char Answer1[STRING_LENGTH];
	char Answer2[STRING_LENGTH];
	char Answer3[STRING_LENGTH];
	char Answer4[STRING_LENGTH];
	time_t CreationTime;
	int deleted;
	int finalized;
} Question;

const char filename[]= "data";


Question createQuestion();
Question* findQuestion(Question* q, int length,int num);
void modifyQuestion(Question* q);
void addQtoArray(Question** qarray, int length);


Question createQuestion(){
	
	Question q;
	modifyQuestion(&q);
	q.deleted=0;
	return q;
}

void modifyQuestion(Question* q)
{	
	*q->QText=0;
	do{
	printf("Question text:");
	scanf("%s", q->QText);
	}while(strlen(q->QText)==0);
	
	
	do{
		printf("Answer 1:");
		scanf("%s", q->Answer1);
	}while(strlen(q->Answer1)==0);
	
	do{
		printf("Answer 2:");
		scanf("%s", q->Answer2);
	}while(strlen(q->Answer2)==0);

	
	printf("Answer 3:");
	scanf("%s", q->Answer3);
	
	printf("Answer 4:");
	scanf("%s", q->Answer4);
	
	time(&q->CreationTime);
	
};

void addQtoArray(Question** qarray, int length)
{
	*qarray =(Question*) realloc(*qarray,(length+1) * sizeof(Question));
	(*qarray)[length]= createQuestion();
	
}

void printQuestion(Question q)
{
	if(q.deleted){return;}
	
	char buff[20];
	strftime(buff,20,"%Y-%m-%d %H:%M:%S",localtime(&q.CreationTime));
	
		printf("Question: %s\n"
		"Answer1: %s\n"
		"Answer2: %s\n"
		"Answer3: %s\n"
		"Answer4: %s\n"
		"Finalized: %s\n"
		"CreationTime: %s\n",
		q.QText,q.Answer1,q.Answer2,q.Answer3,q.Answer4,q.finalized ? "Yes" : "No" ,buff
		);
}

void removeQuestion(Question* q){
	q->deleted=1;
}

void finalizeQuestion(Question* q){
	q->finalized=1;
}

Question* findQuestion(Question* q, int length,int num){
	for(int i=num; i < length; ++i ){
		if(! q[i].deleted){
			return &q[i];
		}
	}
	return NULL;
}

void listQuestions(Question* qarray, int length){
	for(int i=0; i < length; ++i)
	{
		printQuestion(qarray[i]);
	}
}

void writeoutQuestion(Question* qarray, int length, int notdelcount){
	FILE* outf= fopen(filename, "w");
	int f;
	printf("Notdelcount erteke: %d", notdelcount);
	if((f=fwrite(&notdelcount, sizeof(notdelcount),1,outf)) != 1)
	{
		printf("Rosszul irta ki az elemet : %d", f);
		
	}
	for(int i=0; i < length; ++i){
		if(!qarray[i].deleted){
			if((f=fwrite(&qarray[i],sizeof(*qarray),1,outf)) != 1)
			{
				printf("Nem megfeleloen irt fajlba elemet : %d", f);
			}
		}
	}
	fflush(outf);
	fclose(outf);
}
int readQuestion(Question** qarray){
	int qcount=0;
	FILE* inf= fopen(filename, "r");
	int h;
	if((h=fread(&qcount, sizeof(qcount),1,inf)) != 1){
		printf("nem megfelelo beolvasott kerdes szam: %d", h);
	}
	printf("qcount erteke: %d", qcount);
	*qarray= realloc(*qarray, qcount * sizeof(Question));
	if((h=fread(*qarray, sizeof(Question),qcount,inf)) != qcount){
		printf("nem megfelelo beolvasott kerdes szam: %d", h);
	}
	fclose(inf);
	return qcount;
}

int main(int argc, char *argv[]){
	
	printf("MLSZ Nemzeti konzultacio\n");
	printf("===================\n");
	char end= 0;
	Question* qarray = 0;
	int length = 0;
	int notdelcount= 0;
	
	while(!end){
		printf(
		"Kilepes: 0\n"
		"Uj kerdes: 1\n"
		"Kerdes modositas: 2\n"
		"Kerdes torles: 3\n"
		"Kerdesek listazasa: 4\n"
		"Kerdes veglegesitese: 5\n"
		"Fajlba kiiras: 6\n"
		"Fajlbol olvasas: 7\n"
		);
		
		printf("\nValassz egy opciot: ");
		
		int option;
		scanf("%d",&option);
		Question* q1;
		
		switch(option){
			case 0:
				end = 1;
				printf("Viszlat!\n");
				break;
			case 1:
				qarray=realloc(qarray, (length+1) * sizeof(Question));
				qarray[length] = createQuestion();
				qarray[length].deleted = 0;
				qarray[length].finalized = 0;
				++length;
				++notdelcount;
				break;
			case 2:
				printf("Hanyadik kerdes? \n");
				int num2;
				scanf("%d",&num2);
				if(q1 = findQuestion(qarray,length,num2)){modifyQuestion(q1);}
				else{
					printf("Nem jo az index! \n");
				}
				break;
			case 3:
				printf("Hanyadik kerdes? \n");
				int num3;
				scanf("%d",&num3);
				if(q1 = findQuestion(qarray,length,num3)){removeQuestion(q1);}
				else{
					printf("Nem jo az index! \n");
				}
				--notdelcount;
			case 4:
				listQuestions(qarray, length);
				break;
			case 5:
				printf("Hanyadik kerdes? \n");
				int num4;
				scanf("%d",&num4);
				if(q1 = findQuestion(qarray,length,num4)){finalizeQuestion(q1);}
				else{
					printf("Nem jo az index! \n");
				}
				break;
			case 6:
				writeoutQuestion(qarray, length, notdelcount);
				break;
			case 7: 
				length=notdelcount=readQuestion(&qarray);
				break;
			default:
				printf("Nincs ilyen opcio!\n\n");
		}	
	}
}