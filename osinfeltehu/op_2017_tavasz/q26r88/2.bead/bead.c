#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <signal.h>

typedef int bool;
#define true 1
#define false 0

void handler(int signumber)
{
  signal(SIGTERM,SIG_DFL);
}

struct answer
{
	char text[50];
	int votes;
};

struct question
{
	char text[50];
	int answercount;
	struct answer answers[4];
	time_t time;
};


void init(struct question question_arr[], int *questioncount, int final_arr[], int *finalcount);
void add_question(struct question question_arr[], int *questioncount, int final_arr[], int *finalcount);
void list_all(struct question question_arr[], int *questioncount);
void modify_question(struct question question_arr[], int *questioncount, int final_arr[], int *finalcount);
void save_data(struct question question_arr[], int *questioncount, int final_arr[], int *finalcount);
void delete_question(struct question question_arr[], int *questioncount, int final_arr[], int *finalcount);
void modify_final(int final_arr[], int *finalcount, struct question question_arr[],int *questioncount);
void initiate_poll(struct question question_arr[], int *questioncount, int final_arr[], int *finalcount,int *argc, char *argv[]);

int main(int argc, char *argv[]) 
{
	struct question question_arr[50];
	int questioncount = 0;
	
	int final_arr[50];
	int finalcount = 0;
	
	init(question_arr,&questioncount,final_arr,&finalcount);
	
	int i;
	int d;
	
	for(;;)
	{	
		write(1,"\n",1);
		write(1,"Magyar Labdajatekok Szervezete\n",31);
		write(1,"Kerdoiv\n\n",9);
		write(1,"1. Uj kerdes felvetele\n",23);
		write(1,"2. Kerdesek listazasa\n",22);
		write(1,"3. Kerdes modositasa\n",21);
		write(1,"4. Kerdes torlese\n",18);
		write(1,"5. Vegleges kerdoiv modositasa\n",31);
		write(1,"6. Kerdezobiztos kikuldese\n",27);
		write(1,"0. Kilepes\n",11);
		write(1,"Valasszon a menubol: ",21);
		
		char c;
		
		scanf("%c",&c);
		switch (c) 
		{
			case '1':
				add_question(question_arr,&questioncount,final_arr,&finalcount);
				break;
			case '2':
				list_all(question_arr,&questioncount);
				break;
			case '3':
				list_all(question_arr,&questioncount);
				modify_question(question_arr,&questioncount,final_arr,&finalcount);
				break;
			case '4':
				list_all(question_arr,&questioncount);
				delete_question(question_arr,&questioncount,final_arr,&finalcount);
				break;
			case '5':
				list_all(question_arr,&questioncount);
				modify_final(final_arr,&finalcount,question_arr,&questioncount);
				save_data(question_arr,&questioncount,final_arr,&finalcount);
				break;
			case '6':
				initiate_poll(question_arr,&questioncount,final_arr,&finalcount,&argc,argv);
				break;
			case '0':
				exit(0);
				break;
			default : break;
		}
	}
	return 0;
}

void init(struct question question_arr[], int *questioncount, int final_arr[], int *finalcount)
{
	FILE * file;
	file = fopen("data.txt","r");
	
	char text[50];
	int answercount;
	char time[30];
	
	int temp;
	
	char line[100];
	if (fgets(line,sizeof(line),file) != NULL)
	{
		int i;
		for (i = 0;line[i]!='\n';i++)
		{
			final_arr[i] = line[i] - '0';
			*finalcount = *finalcount + 1;
		}
	}
	while(fgets(line,sizeof(line),file) != NULL)
	{
		sscanf(line,"%d %[^\n]", &answercount, text);
		fgets(line,sizeof(line),file);
		sscanf(line,"%[^\n]", time);
		
		strcpy(question_arr[*questioncount].text,text);
		question_arr[*questioncount].answercount = answercount;
		
		struct tm tm;
		strptime(time,"%a %b %d %T %Y",&tm);
		question_arr[*questioncount].time = mktime(&tm);
		
		int i;
		for(i = 0; i < answercount; i++)
		{
			fgets(line,sizeof(line),file);
			sscanf(line,"%[^\n]", text);
			strcpy(question_arr[*questioncount].answers[i].text,text);
		}
		
		*questioncount = *questioncount + 1;
	}
	
	fclose(file);
}

void add_question(struct question question_arr[], int *questioncount, int final_arr[], int *finalcount)
{
	char text[50];
	int answercount;
	int c;
	
	do{
		c = getchar();
	}while(c != EOF && c != '\n');
	
	write(1,"Adja meg a kerdest: ",20);
	scanf("%[^\n]", &text);
	
	write(1,"Adja meg a valaszok szamat: ",28);
	scanf("%d", &answercount);
	
	while (answercount < 2 || answercount > 4)
	{
		write(1,"A valaszok szamanak ketto es negy kozott kell lennie!\n",54);
		write(1,"Adja meg a valaszok szamat: ",28);
		scanf("%d", &answercount);
	}
	
	strcpy(question_arr[*questioncount].text,text);
	question_arr[*questioncount].answercount = answercount;
	int i;
	for(i = 0; i < answercount; i++)
	{	
		do{
			c = getchar();
		}while(c != EOF && c != '\n');
		
		char answertext[50];
		write(1,"Adja meg a valaszt: ",20);
		scanf("%[^\n]", &answertext);
		strcpy(question_arr[*questioncount].answers[i].text,answertext); 
	}
	question_arr[*questioncount].time = time(NULL);
	*questioncount = *questioncount + 1;
	
	save_data(question_arr,questioncount,final_arr,finalcount);
	
	// FILE * file;
	// file = fopen("data.txt","a+");
	
	// char w_data[90] = "";
	// char temp[50];
	// sprintf(temp,"%d",answercount);
	// strcat(w_data,temp);
	// strcat(w_data," ");
	
	// strcat(w_data,text);
	
	// strcat(w_data,"\n");
	
	// fputs (w_data,file);
	
	// time_t mytime;
	// time(&mytime);
	// fputs (ctime(&mytime),file);
		
	// for(i = 0; i < answercount; i++)
	// {
		// char answer_w_data[50] = "";
		// strcat(answer_w_data,question_arr[*questioncount - 1].answers[i].text);
		// strcat(answer_w_data,"\n");
		// fputs(answer_w_data,file); 
	// }
	
	// fclose(file);
	
	do{
		c = getchar();
	}while(c != EOF && c != '\n');
	
}

void list_all(struct question question_arr[], int *questioncount)
{
	printf("\n");
	int i;
	for(i = 0; i < *questioncount; i++)
	{
		printf("%d. ", i+1);
		printf("%s ", question_arr[i].text);
		printf("%d ", question_arr[i].answercount);
		printf("%s", ctime(&question_arr[i].time));
		int j;
		for (j = 0; j < question_arr[i].answercount; j++)
		{
			printf(" %d. ", j+1);
			printf("%s ", question_arr[i].answers[j].text);
			printf("\n");
		}
	}
	
	char c;
	do{
		c = getchar();
	}while(c != EOF && c != '\n');
}

void modify_question(struct question question_arr[], int *questioncount, int final_arr[], int *finalcount)
{
	char text[50];
	int id;
	int answerid;
	int d;

	write(1,"Modositando kerdes szama: ",26);
	scanf("%d", &id);
	
	do{
		d = getchar();
	}while(d != EOF && d != '\n');
	
	write(1,"\n",1);
	write(1,"1. Kerdes modositasa\n",21);
	write(1,"2. Valasz modositasa\n",21);
	write(1,"3. Valasz torlese\n",18);
	write(1,"4. Valasz hozzadasa\n",20);
	write(1,"0. Kilepes/mentes\n",18);
	write(1,"Adja meg mit szeretne tenni: ",29);
	char c;
	scanf("%c",&c);
	while(c != '0')
	{
		switch (c) 
		{
			case '1':
				do{
					d = getchar();
				}while(d != EOF && d != '\n');
				
				write(1,"Adja meg az uj kerdest: ",24);
				scanf("%[^\n]", &text);
				strcpy(question_arr[id-1].text,text);
				break;
			case '2':
				write(1,"Adja meg a valasz szamat: ",26);
				scanf("%d", &answerid);
				
				do{
					d = getchar();
				}while(d != EOF && d != '\n');
				
				write(1,"Adja meg az uj valaszt: ",24);
				scanf("%[^\n]", &text);
				strcpy(question_arr[id-1].answers[answerid-1].text,text);
				break;
			case '3':
				if (question_arr[id-1].answercount == 2)
				{
					write(1,"Legalabb 2 valasznak kell lennie\n",33);
				}
				else
				{
					write(1,"Adja meg a valasz szamat: ",26);
					scanf("%d", &answerid);
					int i;
					for(i = answerid-1;i < question_arr[id-1].answercount - 1; i++)
					{
						strcpy(question_arr[id-1].answers[i].text,question_arr[id-1].answers[i+1].text);
					}
					question_arr[id-1].answercount --;
				}
				break;
			case '4':
				if (question_arr[id-1].answercount == 4)
				{
					write(1,"Maximum 4 valasz lehet\n",23);
				}
				else
				{
					do{
						d = getchar();
					}while(d != EOF && d != '\n');
					
					char answertext[50];
					write(1,"Adja meg a valaszt: ",20);
					scanf("%[^\n]", &answertext);
					strcpy(question_arr[id-1].answers[question_arr[id-1].answercount].text,answertext);
					question_arr[id-1].answercount++;
				}
				break;
			default : break;
		}
		write(1,"\n",1);
		write(1,"1. Kerdes modositasa\n",21);
		write(1,"2. Valasz modositasa\n",21);
		write(1,"3. Valasz torlese\n",18);
		write(1,"4. Valasz hozzadasa\n",20);
		write(1,"0. Kilepes/mentes\n",18);
		write(1,"Adja meg mit szeretne tenni: ",29);
		scanf(" %c",&c);
	}
	
	save_data(question_arr,questioncount,final_arr,finalcount);
	
	do{
		d = getchar();
	}while(d != EOF && d != '\n');
}

void save_data(struct question question_arr[], int *questioncount, int final_arr[], int *finalcount)
{
	FILE * file;
	file = fopen("data.txt","w");
	
	int i;
	
	char final_data[100] = "";
	for (i = 0; i < *finalcount; i++)
	{
		char temp[50];
		sprintf(temp,"%d",final_arr[i]);
		strcat(final_data,temp);
	}
	strcat(final_data,"\n");
	fputs (final_data,file);
	
	for (i = 0; i < *questioncount; i++)
	{
		char w_data[90] = "";
		char temp[50];
		sprintf(temp,"%d",question_arr[i].answercount);
		strcat(w_data,temp);
		strcat(w_data," ");
		
		strcat(w_data,question_arr[i].text);
		
		strcat(w_data,"\n");
		
		fputs (w_data,file);
		
		fputs (ctime(&question_arr[i].time),file);
		
		int j;
		for(j = 0; j < question_arr[i].answercount; j++)
		{
			char answer_w_data[50] = "";
			strcat(answer_w_data,question_arr[i].answers[j].text);
			strcat(answer_w_data,"\n");
			fputs(answer_w_data,file); 
		}
	}
	fclose(file);
}

void delete_question(struct question question_arr[], int *questioncount, int final_arr[], int *finalcount)
{
	char text[50];
	int id;

	write(1,"Adja meg a torolni kivant kerdes szamat: ",41);
	scanf("%d", &id);
	
	int i = 0;
	while(final_arr[i] != id-1 && i < *finalcount) i++;
	if (i < *finalcount)
	{
		for(;i < *finalcount - 1; i++)
		{
			final_arr[i] = final_arr[i+1];
		}
		*finalcount = *finalcount - 1;
	}
	
	for (i = 0; i < *finalcount; i++)
	{
		if (final_arr[i] > id-1)
		{
			final_arr[i] = final_arr[i] - 1;
		}
	}
	
	for(i = id-1;i < *questioncount - 1; i++)
	{
		strcpy(question_arr[i].text,question_arr[i+1].text);
		question_arr[i].answercount = question_arr[i+1].answercount;
		question_arr[i].time = question_arr[i+1].time;
		int j;
		for (j = 0; j < question_arr[i].answercount; j++)
		{
			strcpy(question_arr[i].answers[j].text,question_arr[i+1].answers[j].text);
		}
	}
	*questioncount = *questioncount - 1;
		
	save_data(question_arr,questioncount,final_arr,finalcount);
	
	char c;
	do{
		c = getchar();
	}while(c != EOF && c != '\n');
}

void modify_final(int final_arr[], int *finalcount, struct question question_arr[], int *questioncount)
{
	int id;
	int i;
	
	write(1,"\n",1);
	write(1,"1. Kerdes hozzaadasa\n",21);
	write(1,"2. Kerdes torlese\n",18);
	write(1,"3. Vegleges kerdoiv listazasa\n",30);
	write(1,"0. Kilepes/mentes\n",18);
	write(1,"Adja meg mit szeretne tenni: ",29);
	char c;
	scanf("%c",&c);
	while(c != '0')
	{
		switch (c) 
		{
			case '1':
				write(1,"Adja meg a hozzaadni kivant kerdes szamat: ",43);
				scanf("%d", &id);
				if (id >= 1 && id <= *questioncount)
				{
					i = 0;
					while (i < *finalcount && final_arr[i] != id-1) i++;
					if (i >= *finalcount)
					{
						final_arr[*finalcount] = id-1;
						*finalcount = *finalcount + 1;
					}
				}
				else
				{
					write(1,"Nem megfelelo kerdesszam!\n",28);
				}
				do{
					c = getchar();
				}while(c != EOF && c != '\n');
				break;
			case '2':
				write(1,"Adja meg a torolni kivant kerdes szamat: ",41);
				scanf("%d", &id);
				
				i = 0;
				while(final_arr[i] != id-1 && i < *finalcount) i++;
				for(;i < *finalcount - 1; i++)
				{
					final_arr[i] = final_arr[i+1];
				}
				*finalcount = *finalcount - 1;
				do{
					c = getchar();
				}while(c != EOF && c != '\n');
				break;
			case '3':
				printf("\n");
				for (i = 0; i < *finalcount; i++)
				{
					printf("%d. ", final_arr[i]+1);
					printf("%s ", question_arr[final_arr[i]].text);
					printf("\n");
				}
				do{
					c = getchar();
				}while(c != EOF && c != '\n');
				break;
			default : break;
		}
		write(1,"\n",1);
		write(1,"1. Kerdes hozzaadasa\n",21);
		write(1,"2. Kerdes torlese\n",18);
		write(1,"3. Vegleges kerdoiv listazasa\n",30);
		write(1,"0. Kilepes/mentes\n",18);
		write(1,"Adja meg mit szeretne tenni: ",29);
		scanf("%c",&c);
	}
		
	do{
		c = getchar();
	}while(c != EOF && c != '\n');
}

void initiate_poll(struct question question_arr[], int *questioncount, int final_arr[], int *finalcount,int *argc, char *argv[])
{
	//2.Beadandó//
	
	int c;
	do{
		c = getchar();
	}while(c != EOF && c != '\n');
	
	if (*finalcount < 3)
	{
		printf("\nLegalabb 3 kerdesnek kell lennie a vegleges kerdessorban!\n");
		do{
			c = getchar();
		}while(c != EOF && c != '\n');
		return;
	}
	if (*argc != 2)
	{
		printf("\nAdja meg parameterkent a celhelyet!\n");
		do{
			c = getchar();
		}while(c != EOF && c != '\n');
		return;
	}
	
	signal(SIGTERM,handler);
	
	srand(time(NULL));
	
	int pipefd1[2];
	int pipefd2[2];
	int pid;
	
	pipe(pipefd1);
	pipe(pipefd2);
	
	pid = fork();
	if (pid == 0) //child / partner
	{
		printf("Kerdezobiztos: Utazas a helyszinre(%s)...\n", argv[1]);
		sleep(2);
		printf("Kerdezobiztos: Megerkeztem a helyszinre\n");
		printf("Kerdezobiztos: Varom a kerdeseket\n");
		kill(getppid(),SIGTERM);
		
		close(pipefd1[1]);
		close(pipefd2[0]);
		
		struct question first,second,third;
		read(pipefd1[0], &first, sizeof(struct question));
		read(pipefd1[0], &second, sizeof(struct question));
		read(pipefd1[0], &third, sizeof(struct question));
		
		close(pipefd1[0]);
		
		printf("Kerdezobiztos: Megkaptam a kerdeseket\n");
		
		int people = (rand() % 11) + 10;
		printf("Kerdezobiztos: Kerdoiv kitoltese(%d ember)...\n",people);
		sleep(2);
		
		int i;
		for (i = 0; i < people; i++)
		{
			first.answers[rand() % first.answercount].votes++;
			second.answers[rand() % second.answercount].votes++;
			third.answers[rand() % third.answercount].votes++;
		}
		
		printf("Kerdezobiztos: Kerdoiv visszakuldese\n");
		
		write(pipefd2[1], &first, sizeof(struct question));
		write(pipefd2[1], &second, sizeof(struct question));
		write(pipefd2[1], &third, sizeof(struct question));
		
		close(pipefd2[1]);
		
		exit(0);
	}
	else //parent / központ
	{
		pause();
		printf("MLSZ: Kerdessor kuldese\n");
		
		int first = rand() % *finalcount;
		int second = rand () % *finalcount;
		int third = rand() % *finalcount;
		while (second == first) second = rand () % *finalcount;
		while (third == first || third == second) third = rand() % *finalcount;
		
		close(pipefd1[0]);
		close(pipefd2[1]);
		
		write(pipefd1[1], &question_arr[final_arr[first]], sizeof(struct question));
		write(pipefd1[1], &question_arr[final_arr[second]], sizeof(struct question));
		write(pipefd1[1], &question_arr[final_arr[third]], sizeof(struct question));
		
		close(pipefd1[1]);
		
		struct question first_q,second_q,third_q;
		read(pipefd2[0], &first_q, sizeof(struct question));
		read(pipefd2[0], &second_q, sizeof(struct question));
		read(pipefd2[0], &third_q, sizeof(struct question));
		
		close(pipefd2[0]);
		
		printf("MLSZ: Megkaptam a kerdoivet\n");
		printf("MLSZ: Az eredmeny:\n");
		
		int i;
		printf("%s:\n",first_q.text);
		for (i = 0;i < first_q.answercount;i++)
		{
			printf(" %s: %d db\n",first_q.answers[i].text,first_q.answers[i].votes);
		}
		printf("%s:\n",second_q.text);
		for (i = 0;i < second_q.answercount;i++)
		{
			printf(" %s: %d db\n",second_q.answers[i].text,second_q.answers[i].votes);
		}
		printf("%s:\n",third_q.text);
		for (i = 0;i < third_q.answercount;i++)
		{
			printf(" %s: %d db\n",third_q.answers[i].text,third_q.answers[i].votes);
		}
	}
}