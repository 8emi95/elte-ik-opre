#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

struct stu{
    char question[100];
    char answer1[100];
    char answer2[100];
    char answer3[100];
    char answer4[100];
    char time[100];
};

void list();

void Delete();

void modify();

void add(char c[100],int number,struct stu questions1[100]);

void end();

void create();

void run(int s,struct stu questions[100]);

int main()
{
	char c[100];
    struct stu questions[100];
    printf("Do you want to build a new question bank? yes or no.\n");
    print:scanf("%[^\n]",&c);
	getchar();
    //printf("%s \n",c);

    if(strcmp(c, "no") == 0)
        goto step2;
    else if(strcmp(c,"yes") == 0)
        goto step1;
    else
        printf("Please write yes or no\n");
        goto print;

    step1: printf("Give a name to the new question bank. name.txt \n");
    char name[100];
    scanf("%[^\n]",&name);
    //printf("%s\n",name);
	getchar();

    int number;
    printf("How many questions you want to add?\n");
    scanf("%d",&number);
    getchar();
    //printf("%d\n",number);

    int i;
    printf("If some answers have nothing to write,please write 'null'.\n");
    for(i=1;i<number+1;i++){
        printf("[%d]question:",i);
        scanf("%[^\n]",&questions[i].question);
		getchar();
        printf("[%d]answer 1:",i);
        scanf("%[^\n]",&questions[i].answer1);
		getchar();
        printf("[%d]answer 2:",i);
        scanf("%[^\n]",&questions[i].answer2);
		getchar();
        printf("[%d]answer 3:",i);
        scanf("%[^\n]",&questions[i].answer3);
		getchar();
        printf("[%d]answer 4:",i);
        scanf("%[^\n]",&questions[i].answer4);
		getchar();
        time_t timep;
        time (&timep);
        strcpy(questions[i].time,ctime(&timep));
        printf("%s\n",questions[i].time);
    }

    goto start;

    step2: printf("Which question bank you want to use?Write it's name.(name.txt)\n");
    system("Dir /b *.txt \n");
    char usename[100];
    scanf("%[^\n]",&usename);
	getchar();
    int number1=0;

    FILE *fp;
    char *name2 = usename;
    if ((fp=fopen(usename,"r"))==NULL)
        {
            printf("File %s does not exist!",name2);
            goto step2;
        }
    while(fscanf(fp,"%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];",&questions[number1+1].question,
               &questions[number1+1].answer1,&questions[number1+1].answer2,
               &questions[number1+1].answer3,&questions[number1+1].answer4,
               &questions[number1+1].time)==6){
        number1++;
    }

    fclose(fp);



    start: printf("Which you want to do? list, 'delete', 'modify', 'add', 'end', 'create the final survey' or 'do a survey'?\n");
    char a[100];
    scanf("%[^\n]",&a);
	getchar();

    int b;
    //scanf("%d",&b);

    //printf("a=%s\n",a);


    if (strcmp(a, "list") == 0)
        b=1;
    else if(strcmp(a, "delete") == 0)
        b=2;
    else if(strcmp(a, "modify") == 0)
        b=3;
    else if(strcmp(a, "add") == 0)
        b=4;
    else if(strcmp(a, "end") == 0)
        b=5;
    else if(strcmp(a, "create the final survey") == 0)
        b=6;
    else if(strcmp(a, "do a survey") == 0)
        b=7;
    else
        b=8;
	
    int d;
    char name1[100];
    if (strcmp(c, "yes") == 0){
        d = number;
        strcpy(name1,name);
    }else if(strcmp(c, "no") == 0){
        d = number1;
        strcpy(name1,usename);
    }

    int s=0;
    while(0<strlen(questions[s+1].question)){
        s++;
    }

    switch (b){
        case 1:list(s,questions);break;
        case 2:Delete(s,questions);break;
        case 3:modify(s,questions);break;
        case 4:add(c,d,questions);break;
        case 5:end(s,name1,questions);break;
        case 6:create(s,questions);break;
		case 7:run(s,questions);break;
        case 8:printf("Do not find please write again. \n");goto start;break;
    }
    //printf("b=%d\n",b);
	
	goto start;

	return 0;
}

void list(int s,struct stu questions[100]){
    //printf("%d",(fp=fopen("student.txt","r")s);
    int i;
    for (i=1;i<s+1;i++){
        printf("[%d]question:%s; answer1:%s; answer2:%s; answer3:%s; answer4:%s; time:%s;\n",
               i,questions[i].question,questions[i].answer1,
               questions[i].answer2,questions[i].answer3,
               questions[i].answer4,questions[i].time);
    }
}

void Delete(int s,struct stu questions[100]){
    list(s,questions);
    c: printf("Do you want to delete? yes or no\n");    //printf("%d",(fp=fopen("student.txt","r")s);

    char sure[100];
    scanf("%s",&sure);
    getchar();

    if(strcmp(sure, "no") == 0)
        goto a;
    else if(strcmp(sure,"yes") == 0)
        goto b;
    else
        printf("Please write yes or no\n");
        goto c;

    b: printf("Which question you want to delete?please write it's number.\n");
    int d;
    int i;
    scanf("%d",&d);
    getchar();

    for (i=d;i<s+2;i++){
        strcpy(questions[i].question,questions[i+1].question);
        strcpy(questions[i].answer1,questions[i+1].answer1);
        strcpy(questions[i].answer2,questions[i+1].answer2);
        strcpy(questions[i].answer3,questions[i+1].answer3);
        strcpy(questions[i].answer4,questions[i+1].answer4);
        strcpy(questions[i].time,questions[i+1].time);
    }
    s = s - 1;
    list(s,questions);
    goto c;
    a:  printf("Will back menu.\n");
}

void modify(int s,struct stu questions[100]){
    list(s,questions);
    c: printf("Do you want to modify? yes or no\n");
    char sure[100];
    scanf("%s",&sure);
    getchar();

    if(strcmp(sure, "no") == 0)
        goto a;
    else if(strcmp(sure,"yes") == 0)
        goto b;
    else
        printf("Please write yes or no\n");
        goto c;

    b: printf("Which question you want to modify?please write it's number.\n");
    int d;
    scanf("%d",&d);
    getchar();

    printf("What you want to modify ?question,answer1,answer2,answer3,answer4?\n");
    char a[100];
    d: scanf("%s",&a);
    getchar();
    int i;
    if(strcmp(a, "question") == 0)
        i=1;
    else if(strcmp(a, "answer1") == 0)
        i=2;
    else if(strcmp(a, "answer2") == 0)
        i=3;
    else if(strcmp(a, "answer3") == 0)
        i=4;
    else if(strcmp(a, "answer4") == 0)
        i=5;
    else
        i=6;
    char e[100];

    switch(i){
        case 1:scanf("%[^\n]",&e);getchar();strcpy(questions[d].question,e);
        time_t timep;
        time (&timep);
        strcpy(questions[d].time,ctime(&timep));
        printf("%s\n",questions[d].time);break;
        case 2:scanf("%[^\n]",&e);getchar();strcpy(questions[d].answer1,e);break;
        case 3:scanf("%[^\n]",&e);getchar();strcpy(questions[d].answer2,e);break;
        case 4:scanf("%[^\n]",&e);getchar();strcpy(questions[d].answer3,e);break;
        case 5:scanf("%[^\n]",&e);getchar();strcpy(questions[d].answer4,e);break;
        case 6:printf("Please write question, answer1, answer2, answer3, answer4.\n");goto d;
    }
    s = s;
    time_t timep;
        time (&timep);
        strcpy(questions[d].time,ctime(&timep));
        printf("%s\n",questions[d].time);
    list(s,questions);
    goto c;
    a:  printf("Will back menu.\n");
}

void add(char c[100],int number,struct stu questions[100]){
    int num;
    int num1;
    int i;
    printf("How many questions you want to add?\n");
    scanf("%d",&num);
    getchar();
    num1 = number + num;
    //printf("%d",num1);
    printf("If some answers have nothing to write,please write 'null'.\n");
    for(i=number+1;i<num1+1;i++){
        printf("[%d]question:",i);
        scanf("%[^\n]",&questions[i].question);
		getchar();
        printf("[%d]answer 1:",i);
        scanf("%[^\n]",&questions[i].answer1);
		getchar();
        printf("[%d]answer 2:",i);
        scanf("%[^\n]",&questions[i].answer2);
		getchar();
        printf("[%d]answer 3:",i);
        scanf("%[^\n]",&questions[i].answer3);
		getchar();
        printf("[%d]answer 4:",i);
        scanf("%[^\n]",&questions[i].answer4);
		getchar();
        time_t timep;
        time (&timep);
        strcpy(questions[i].time,ctime(&timep));
        printf("%s\n",questions[i].time);
    }
}

void end(int s,char name[100],struct stu questions[100]){
    FILE *fp;
    fp = fopen(name,"w+");

    int i;
    for (i=1;i<s+1;i++){
        fprintf(fp,"%s;%s;%s;%s;%s;%s;",
               questions[i].question,questions[i].answer1,
               questions[i].answer2,questions[i].answer3,
               questions[i].answer4,questions[i].time);
    }
    fclose(fp);
    exit(0);
}

void create(int s,struct stu questions[100]){
    list(s,questions);
    printf("How many questions you want to choice?\n");
    int m;
    scanf("%d",&m);
    getchar();
    struct mem{
        int t;
    }p[100];
    printf("Write the questions' numbers which you want.\n");
    int j;
    for (j=1;j<m+1;j++)
    {
        scanf("%d",&p[j].t);
        getchar();
    }
    printf("Give a name to the new survey.\n");
    char name[100];
    scanf("%[^\n]",&name);
	getchar();

    FILE *fp;
    fp = fopen(name,"w+");

    int i;
    int z=0;
    for (i=1;i<s+1;i++){
        for(j=1;j<m+1;j++){
            if (i=p[j].t){
                z++;
                fprintf(fp,"[%d]question:%s; answer1:%s; answer2:%s; answer3:%s; answer4:%s; time:%s;\n",
                    z,questions[i].question,questions[i].answer1,
                    questions[i].answer2,questions[i].answer3,
                    questions[i].answer4,questions[i].time);
            }
        }
    }
    fclose(fp);
}

struct str{
	char question[100];
    char answer1[100];
    char answer2[100];
    char answer3[100];
    char answer4[100];
};

struct stp{
	char question[100];
	char answer[100];
};

void handler(){
  printf("The commisioner arrived Budapest.\n");
}

void hand(){
  printf("The survey arrived.\n");
}

void run(int s,struct stu questions[100]){
	signal(SIGTERM,handler);
	signal(SIGUSR1,hand);
	int status;
	//建立管道
	int pipefd[2];
	int pipehd[2];
	pid_t pid;
	//确定pipe有没有错误
	if (pipe(pipefd) == -1)
	   {
            perror("Hiba a pipe nyitaskor!");
            exit(EXIT_FAILURE);
        }
		
	if (pipe(pipehd) == -1)
	   {
            perror("Hiba a pipe nyitaskor!");
            exit(EXIT_FAILURE);
        }

	//建立父子进程
	pid_t commisioner=fork();
	if (commisioner < 0){
		//报错
		perror("The fork calling was not succesful\n");
		exit(1);
	}

	if (commisioner > 0){
		//Society of Hungarian Ball Games.父进程
		printf("Wait commisioner arrive.\n");
		pause();
		printf("work!\n");
		//随机三个问题
		int q[3];
		q[0]=rand()%s+1;
		q[1]=rand()%s+1;
		while(q[0]==q[1]){
			q[1]=rand()%s+1;
		}

		q[2]=rand()%s+1;

		while(q[2]==q[0] || q[2]==q[1]){
			q[2]=rand()%s+1;
		}
		
		//关闭不用管道
		close(pipefd[0]);
		close(pipehd[1]);
		//通过题号找问题
		int i;
		struct str qu[100];
		for (i=0;i<3;i++){
			strcpy(qu[i].question,questions[q[i]].question);
			strcpy(qu[i].answer1,questions[q[i]].answer1);
			strcpy(qu[i].answer2,questions[q[i]].answer2);
			strcpy(qu[i].answer3,questions[q[i]].answer3);
			strcpy(qu[i].answer4,questions[q[i]].answer4);
		}

		for (i=0;i<3;i++){
			write(pipefd[1], &qu[i],sizeof(qu[i]));
		}

		close(pipefd[1]);
		//等待调查结果
		pause();
		struct stp qm[100];
		//获取结果
		
		
		read(pipehd[0],&qm,sizeof(qm));
		int n=0;
		
		printf("Get the survey!\n");
		while(0<strlen(qm[n].question)){
			n++;
		}
		
		int j;
		for(j=0;j<n;j++){
			printf("person %d: question:%s; answer:%s;\n",
               j+1,qm[j].question,qm[j].answer);
		}
		
		close(pipehd[0]);
		
		kill(commisioner,SIGKILL);

	} else{
		//commisioner。子进程
		sleep(1);
		kill(getppid(),SIGTERM);
		sleep(1);

		//关闭不用管道
		close(pipefd[1]);
		close(pipehd[0]);
		
		struct str qu[100];
		struct stp qm[100];
		int i;
		for(i=0;i<3;i++){
			read(pipefd[0],&qu[i],sizeof(qu[i]));
		}
		
		close(pipefd[0]);

		int pn;
		pn=rand()%11+10;
		int qs;
		
		int j;
		printf("%d persons wishes to fill the survey.\n ",pn);
		for (j=0;j<pn;j++){
			qs = rand()%2;
			printf("person %d: \n",j+1);
			
			int answer;
			answer = rand()%3+1;
			switch(answer){
				case 1:strcpy(qm[j].question,qu[qs].question);strcpy(qm[j].answer,qu[qs].answer1);break;
				case 2:strcpy(qm[j].question,qu[qs].question);strcpy(qm[j].answer,qu[qs].answer2);break;
				case 3:strcpy(qm[j].question,qu[qs].question);strcpy(qm[j].answer,qu[qs].answer3);break;
				case 4:strcpy(qm[j].question,qu[qs].question);strcpy(qm[j].answer,qu[qs].answer4);break;
			}
		}
		
		kill(getppid(),SIGUSR1);
		
		write(pipehd[1],&qm,sizeof(qm));
		
		close(pipehd[1]);
		
		pause();
	}
}
