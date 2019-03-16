
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <sys/wait.h>
typedef struct Qs
{
	char question[100];
	char answer1[100];
	char answer2[100];
	char answer3[100];
	char answer4[100];
} questions;

typedef struct TIME
{
	char time[100];
}Time;

void add(questions qsbank[100],int numberofqs,Time times[100]);
void List(questions qsbank[100],int numberofqs,Time times[100]);
void finalsurvey(questions qsbank[100],int numberofqs,Time times[100]);
void modify(questions qsbank[100],Time times[100]);
void delete(questions qsbank[100],int numberofqs,Time times[100]);
void settime(Time times[100],int i);
void QuestionBank(questions qsbank[100],int numberofqs,Time times[100]);
void handler(int signumber);
void fanlist(questions qsbank[100],int fan,Time times[100]);
void fanModify(questions qsbank[100],Time times[100]);
void delN(char * a){
 //逐行读取的srting会带\n,需要变成\0 截止。在char最后一位是\0
 int l;
 l=strlen(a);
 a[l-1]=0;}

int main(){
//part 1 收集问题。提问想要收集问题的数量，根据数量建数组，用循环输入每一个问题和答案
// collect questions and possible answers .	
	int numberofqs=0;
	int i;
	char data;
	
	questions qsbank[100];
	Time time;
	Time times[100];
	

	printf("do you want use the previosly given data?(y/n)\n");
	scanf("%c",&data);getchar();
	if(data=='y'){
			FILE *fp;
		if ((fp=fopen ("QuestionBank.txt", "r")) == NULL)
     		{ fprintf(stderr, "error redirecting stdout\n");}
  		int ch;
   		ch=fgetc(fp);
   		
  		 if(ch==EOF){
  		 	printf("question bank is empty,please make new bank\n");
  		 	fclose(fp);
  		 	goto newbank;
  		 }else{
  		 	while(fgetc(fp)!=EOF){
  		 	fgets(times[numberofqs].time,100,fp);
  			fgets(qsbank[numberofqs].question,100,fp);
  			fgets(qsbank[numberofqs].answer1,100,fp);
  			fgets(qsbank[numberofqs].answer2,100,fp);
  			fgets(qsbank[numberofqs].answer3,100,fp);
  			fgets(qsbank[numberofqs].answer4,100,fp);
  			
  			delN(qsbank[numberofqs].question);
  			delN(qsbank[numberofqs].answer1);
  			delN(qsbank[numberofqs].answer2);
  			delN(qsbank[numberofqs].answer3);
  			delN(qsbank[numberofqs].answer4);
  			 numberofqs++;
  			
  			}
  			 fclose(fp);
  		}
	}
	else{
	newbank:printf("How many  question you got?\n");
		scanf("%d",&numberofqs);
		getchar();
	
		for ( i = 0; i < numberofqs; ++i)
		{
			printf("write the question and answer(2-4) \n");
		
			printf("write the question as xxxx?.\n");
			
			scanf("%[^\n]",&qsbank[i].question);getchar();settime(times,i);
			 
		
			printf("write the answer1.\n");
			scanf("%[^\n]",&qsbank[i].answer1);getchar();

			printf("write the answer2:\n");
			scanf("%[^\n]",&qsbank[i].answer2);getchar();

			printf("write the answer3:\n");
			scanf("%[^\n]",&qsbank[i].answer3);getchar();

			printf("write the answer4:\n");
			scanf("%[^\n]",&qsbank[i].answer4);getchar();			
		}
	}	
//part 2 
//写4个功能的子函数。放入while里，加一个end 来判断是否更改结束break while循环。while 里包括 显示，增加，更改，结束
//list the data, add new questions and answers, modify and delete them.	
	while(1){
		int chose;
		printf("what are you want to do? 1.list,2.add,3.modify,4.delete,5.end,6.exit\n");
		scanf("%d",&chose);
		getchar();
		switch(chose){
			case 1:List(qsbank,numberofqs,times);break;
			case 2:add(qsbank,numberofqs,times);numberofqs++;break;
			case 3:List(qsbank,numberofqs,times);modify(qsbank,times);break;
			case 4:List(qsbank,numberofqs,times);delete( qsbank,numberofqs,times);numberofqs--;break;
			case 5:goto next;break;
			case 6:exit(0);
			default:printf("wrong number of chose. \n");
		}
	}
//part3 
//将最后的结果生产txt文件
//creat the file.
	next:printf("do you want make final survey ?(y/n)\n");
	QuestionBank(qsbank,numberofqs,times); 
	char result;
	scanf("%c",&result);getchar();
	
	if(result == 'y'){
		printf("delete the question which questions should not  belong to it.\n");
		//int end=1;
		while(1){
		int chose1;
		printf("what are you want to do? 1.delete,2.end\n");
		scanf("%d",&chose1);
		getchar();
			switch(chose1){
			case 1:List(qsbank,numberofqs,times);delete( qsbank,numberofqs,times);numberofqs--;break;
			case 2:finalsurvey( qsbank, numberofqs,times);goto hw2;break;
			}
		}
		
	}
	else{
		printf("thank you for use.\n");
	}


//HW2	
	hw2:printf("start homework2.\n");	
	signal(SIGTERM,handler);
			int status; 	int q[3];	 	int pipe_send[2];   int pipe_back[2]; int readrandomnumber[3];
            int fan=1;		int loop=0;     int chose1;    char townNeme[100]; 
            questions readQuestion[3];		int pipe_rand[2];
            questions townquestion[3];  	questions fansurvey[fan*3];
            
			
			if (pipe(pipe_rand) == -1)  {
               perror("Opening error!");
               exit(EXIT_FAILURE);
           }
			
           if (pipe(pipe_send) == -1)  {
               perror("Opening error!");
               exit(EXIT_FAILURE);
           }
            if (pipe(pipe_back) == -1) {
               perror("Opening error!");
               exit(EXIT_FAILURE);
           }
           pid_t  child=fork();
	if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
//parents process
	if (child>0) 
	{	   pause(); //waits till a signal arrive 
 	   
   	
//ReadrandomNumber question
   
  		q[0]=rand()%numberofqs;
	  	for( i=1;i<3;i++){
				int r=rand()%numberofqs;

			  	while(1){
			  		if(r!=q[0] && r!=q[i-1]){
	  				q[i]=r;break;

	  				} else{
	  					r=rand()%numberofqs;
	  				}
	  			}
	  			
	  	}
  		


	  	for(i=0;i<3;i++){
	  		strcpy(townquestion[i].question,qsbank[q[i]].question);
			strcpy(townquestion[i].answer1,qsbank[q[i]].answer1);
			strcpy(townquestion[i].answer2,qsbank[q[i]].answer2);
			strcpy(townquestion[i].answer3,qsbank[q[i]].answer3);
			strcpy(townquestion[i].answer4,qsbank[q[i]].answer4);
	  	}
	  	
	  	
//pipe_send question and readrandomnumber number
  			close(pipe_send[0]); //Usually we close unused read end
            write(pipe_send[1], townquestion,sizeof(townquestion)); 
            close(pipe_send[1]); // Closing write descriptor 
          // printf("send question is good\n");
     		
     		
           
            close(pipe_rand[0]); //Usually we close unused read end
            write(pipe_rand[1], q,sizeof(q)); 
            close(pipe_rand[1]); // Closing write descriptor
           // printf("send rand number is good\n");
            waitpid(child,&status,0); 
           printf("commisioner finished word.\n");
//get  the survey pipe_back
           
           close(pipe_back[1]);  
           read(pipe_back[0],fansurvey,sizeof(fansurvey)); 
           close(pipe_back[0]); 
  		
  		fanlist(fansurvey,fan,times);
 		
	}
	else //child process
	{
		    printf("What is the town's name？\n");
			
		
			scanf("%s",&townNeme);getchar();
			printf("I  arrived the town: %s\n",townNeme);
			
			
			kill(getppid(),SIGTERM); 
			
			sleep(1);
//read question and readrandomnumber number
		   close(pipe_send[1]); 
           read(pipe_send[0],readQuestion,sizeof(readQuestion)); 
           close(pipe_send[0]); 
          // printf("get 1 is good \n");
           
           
            close(pipe_rand[1]); 
           read(pipe_rand[0],readrandomnumber,sizeof(readrandomnumber)); 
           close(pipe_rand[0]);
        // printf("get 2 is good \n");
           

//start  Questionnaire
           printf("how many fan participate this Questionnaire？\n");
           
           scanf("%d",&fan);
           getchar();
           
           questions townbank[fan*3];
           
            while(loop<fan){
           		
           		for(i=0;i<3;i++){
  				strcpy(townbank[i+loop].question,readQuestion[readrandomnumber[i]].question);
				 strcpy(townbank[i+loop].answer1,"__");
				 strcpy(townbank[i+loop].answer2,"__");
				 strcpy(townbank[i+loop].answer3,"__");
				 strcpy(townbank[i+loop].answer4,"__");
  				}
  				
           		printf("survey of %dth fan.\n", loop+1);
				
				while(1){
					printf("what are you want to do? 1.list,2.answer,3.end\n");
					scanf("%d",&chose1);
					getchar();
					switch(chose1){
					case 1:List(townbank,3,times);break;
					case 2:List(townbank,3,times);fanModify(townbank,times);break;
					case 3:goto nextfan;break;
					default:printf("wrong number of chose. \n");
					}
				}
				nextfan:loop++;

  				
          	}
        //Questionnaire done
        //pipe_send pipe_back
           	close(pipe_back[0]); 
            write(pipe_back[1], townbank,sizeof(townbank)); 
            close(pipe_back[1]); 
            exit(2);
	}
	
	return 0;
}
void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}
void add(questions qsbank[100],int numberofqs,Time times[100]){
		
			printf("write the question and answer(2-4) \n");		
			scanf("%[^\n]",&qsbank[numberofqs].question);getchar();settime(times,numberofqs);
			
			printf("answer1.\n");
			scanf("%[^\n]",&qsbank[numberofqs].answer1);getchar();

			printf("answer2:\n");
			scanf("%[^\n]",&qsbank[numberofqs].answer2);getchar();

			printf("answer3:\n");
			scanf("%[^\n]",&qsbank[numberofqs].answer3);getchar();

			printf("answer4:\n");
			scanf("%[^\n]",&qsbank[numberofqs].answer4);getchar();		
}

void List(questions qsbank[100],int numberofqs,Time times[100]){
	int j;
	if(numberofqs==0){
		printf("still have no question.\n");
	}
	for( j=0;j<numberofqs;j++){
	printf("question %d is:%s|%s",j+1,qsbank[j].question,times[j].time);
	printf("A.%s  ",qsbank[j].answer1);
	printf("B.%s  ",qsbank[j].answer2);
	printf("C.%s  ",qsbank[j].answer3);
	printf("D.%s  ",qsbank[j].answer4);
	printf("\n");
	}
}


void fanlist(questions qsbank[100],int fan,Time times[100]){
		int j; int n;
	if(fan==0){
		printf("no one want participate this questionnaire.\n");
	}
	for( j=0;j<fan;j++){
	printf("The %dth fan's survey!!!!!\n",j+1);
		for(n=0;n<3;n++){ 
	printf("question %d is:%s|%s",j+1,qsbank[(j*3)+n].question,times[(j*3)+n].time);
	printf("A.%s  ",qsbank[(j*3)+n].answer1);
	printf("B.%s  ",qsbank[(j*3)+n].answer2);
	printf("C.%s  ",qsbank[(j*3)+n].answer3);
	printf("D.%s  ",qsbank[(j*3)+n].answer4);
	printf("\n");}
	}
}

void modify(questions qsbank[100],Time times[100]){
	int change;
	int change1;
	char modify[100];
	printf("which you want change?(1:question1 and answer 2:question2....)\n");
	scanf("%d",&change);getchar();
	printf("what are you want modify?1.qs,2.answer1...5.answer4\n");
	scanf("%d",&change1);getchar();
	printf("give new question or answer\n");
	
	
	scanf("%[^\n]",&modify);getchar();

	
	switch(change1){
			case 1:strcpy(qsbank[change-1].question,modify);settime(times,change-1);break;
			case 2:strcpy(qsbank[change-1].answer1,modify);settime(times,change-1);break;
			case 3:strcpy(qsbank[change-1].answer2,modify);settime(times,change-1);break;
			case 4:strcpy(qsbank[change-1].answer3,modify);settime(times,change-1);break;
			case 5:strcpy(qsbank[change-1].answer4,modify);settime(times,change-1);break;
			default:printf("wrong number of modify \n");
	}
}

void fanModify(questions qsbank[100],Time times[100]){
	int change;
	int change1;
	char modify[100];
	printf("which you want answer?(1:question1 and answer 2:question2....)\n");
	scanf("%d",&change);getchar();
	printf("what are you want answer?1.answer1...4.answer4\n");
	scanf("%d",&change1);getchar();
	printf("give the answer\n");
	
	scanf("%[^\n]",&modify);getchar();

	switch(change1){
			case 1:strcpy(qsbank[change-1].answer1,modify);settime(times,change-1);break;
			case 2:strcpy(qsbank[change-1].answer2,modify);settime(times,change-1);break;
			case 3:strcpy(qsbank[change-1].answer3,modify);settime(times,change-1);break;
			case 4:strcpy(qsbank[change-1].answer4,modify);settime(times,change-1);break;
			default:printf("wrong number of modify \n");
	}
}


void QuestionBank(questions qsbank[100],int numberofqs,Time times[100]){
	int j;
	FILE *fp1;
	
     fp1=fopen("QuestionBank.txt","w+");
    fprintf(fp1,"*");
    for( j=0;j<numberofqs;j++){	

    fprintf(fp1,"*%s",times[j].time);
	fprintf(fp1,"%s\n",qsbank[j].question);
	fprintf(fp1,"%s\n",qsbank[j].answer1);
	fprintf(fp1,"%s\n",qsbank[j].answer2);
	fprintf(fp1,"%s\n",qsbank[j].answer3);
	fprintf(fp1,"%s\n",qsbank[j].answer4);
	}   
    fclose(fp1);
 }

void finalsurvey(questions qsbank[100],int numberofqs,Time times[100]){
	int j;
	FILE *fp1;
	
     fp1=fopen("FinalSurvey.txt","w+");
    fprintf(fp1,"The Society of Hungarian Ball Games advertise a consultation \n");
    for( j=0;j<numberofqs;j++){
    	
	fprintf(fp1,"question %d is:%s%s",j+1,qsbank[j].question,times[j].time);
	fprintf(fp1,"A.%s  ",qsbank[j].answer1);
	fprintf(fp1,"B.%s  ",qsbank[j].answer2);
	fprintf(fp1,"C.%s  ",qsbank[j].answer3);
	fprintf(fp1,"D.%s  ",qsbank[j].answer4);
	fprintf(fp1,"\n");
	}
    fclose(fp1);
 }

void delete(questions qsbank[100],int numberofqs,Time times[100]){
	int delete;
	int i;
	printf("which question you want delete?1,2,3,4..\n");
	scanf("%d",&delete);getchar();
	for(i=delete-1;i<numberofqs;i++){
			strcpy(times[i].time,times[i+1].time);
			strcpy(qsbank[i].question,qsbank[i+1].question);
			strcpy(qsbank[i].answer1,qsbank[i+1].answer1);
			strcpy(qsbank[i].answer2,qsbank[i+1].answer2);
			strcpy(qsbank[i].answer3,qsbank[i+1].answer3);
			strcpy(qsbank[i].answer4,qsbank[i+1].answer4);
	}
}

void settime(Time times[100],int i){
	time_t timep;
    time (&timep);
	strcpy(times[i].time,ctime(&timep));
}