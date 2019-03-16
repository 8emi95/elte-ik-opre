#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

struct question{
	char q[200];
	time_t time;
	char answer[4][100];
};

int menu(){
	int option;
	char buffer[100]={0};
	for(;;){
		printf("Please choose an option:\n");
		printf("\tPress 0 to exit\n");
		printf("\tPress 1 to write a new question with answers\n");
		printf("\tPress 2 to modify questions or answers\n");
		printf("\tPress 3 to delete questions or answers\n");
		printf("\tPress 4 to list questions with their answers\n");
		printf("\tPress 5 to finalize the questionnaire\n");
		int i=read(0,buffer,100);
		if(i==2 && buffer[0]<54 && buffer[0]>47){
			option=atoi(buffer);
			if(option<6 && option>=0)break;
		}
		printf("Wrong input\n\n");
	}
	return option;
}

void newquestion(int f){
	struct question a={0};
	char buffer[1000];
	int ans;
	lseek(f,0,SEEK_END);
	for(;;){
		printf("Type in the question:\n");
		memset(buffer,0,1000);
		int i=read(0,buffer,1000);
		if(i<200 && i>1){
			strcpy(a.q,buffer);
			break;
		}
		printf("Wrong input, the question can't cant have more than 200 characters\n");
	}
	for(;;){
		printf("How many answers will this question have?\n");
		memset(buffer,0,1000);
		int i=read(0,buffer,1000);
		if(i==2 && buffer[0]<53 && buffer[0]>49){
			ans=atoi(buffer);
			break;
		}
		printf("Wrong input, every question needs to have 2-4 answers.\n");
	}
	int j=1;
	while(ans--){
		printf("Type in the %i. answer\n",j);
		memset(buffer,0,1000);
		int i=read(0,buffer,1000);
		if(i<100 && i>1){
			strcpy(a.answer[j-1],buffer);
			j++;
		}else{
			printf("Wrong input, the answer can't have more than 100 characters\n");
		}
	}
	a.time=time(0);
	write(f,&a,sizeof(a));
	printf("Question saved\n\n");
}

void modquestion(int f){
	char buffer[500];
	struct question a={0};
	listquestions(f,1);
	for(;;){
		printf("Press the number of the question you want to modify\n");
		memset(buffer,0,500);
		int i=read(0,buffer,100);
		if(i>1 && lseek(f,(atoi(buffer)-1)*sizeof(struct question),SEEK_SET)!=-1 && read(f,&a,sizeof(struct question))==sizeof(struct question)){
			for(;;){
				printf("Press 1 to modify the question or press 2 to modify one of the answers\n");
				memset(buffer,0,500);
				int i=read(0,buffer,100);
				if(i==2 && buffer[0]>48 && buffer[0]<51){
					switch(buffer[0]){
						case 49 :
							for(;;){
								printf("Old question: %s\n",a.q);
								printf("Type in the modified question\n");
								memset(buffer,0,500);
								int i=read(0,buffer,500);
								if(i<200 && i>1){
									memset(a.q,0,200);
									strcpy(a.q,buffer);
									break;
								}
								printf("Wrong input, the question can't cant have more than 200 characters\n");
							}
							break;
						case 50 :
							for(;;){
								int j;
								int N=counta(a.answer);
								for(j=0;j<N;j++){
									printf("%i. answer: %s",j+1,a.answer[j]);
								}
								printf("Press the number of the answer you want to modify\n");
								memset(buffer,0,500);
								i=read(0,buffer,100);
								if(i==2 && buffer[0]>48 && buffer[0]<(47+j)){
									for(;;){
										printf("Type in the new answer\n");
										memset(buffer,0,500);
										int i=read(0,buffer,500);
										if(i<100 && i>1){
											memset(a.answer[j-1],0,100);
											strcpy(a.answer[j-1],buffer);
											break;
										}else{
											printf("Wrong input, the answer can't have more than 100 characters\n");
										}
									}
								}
								break;
							}
							break;
					}
					lseek(f,-1*sizeof(struct question),SEEK_CUR);
					write(f,&a,sizeof(a));
					break;
				}
			}
		break;
		}
	}
}

int counta(char**a){
int i=0;
while(a[i++][0]);
return i;
}

void delquestion(int f){
	char buffer[500];
	struct question a={0};
	listquestions(f,1);
	for(;;){
		printf("Press the number of the question you want to delete\n");
		memset(buffer,0,500);
		int i=read(0,buffer,100);
		if(i>1 && lseek(f,(atoi(buffer)-1)*sizeof(struct question),SEEK_SET)!=-1 && read(f,&a,sizeof(struct question))==sizeof(struct question)){
				if(counta(a.answer)>2){
					printf("Press 1 to delete the whole the question or press 2 to delete one of the answers\n");
					memset(buffer,0,500);
					int i=read(0,buffer,100);
					if(i==2 && buffer[0]>48 && buffer[0]<51){
						switch(buffer[0]){
							case 49 :
								for(;;){
									while(read(f,&a,sizeof(struct question))==sizeof(struct question)){
										lseek(f,-2*sizeof(struct question),SEEK_CUR);
										write(f,&a,sizeof(a));
										lseek(f,sizeof(struct question),SEEK_CUR);
									}
								}
								break;
							case 50 :
								for(;;){
									int j;
									int N=counta(a.answer);
									for(j=0;j<N;j++){
										printf("%i. answer: %s",j+1,a.answer[j]);
									}
									printf("Press the number of the answer you want to delete\n");
									memset(buffer,0,500);
									i=read(0,buffer,100);
									if(i==2 && buffer[0]>48 && buffer[0]<(47+j)){
										memset(a.answer[atoi(buffer[0])],0,100);
										lseek(f,-1*sizeof(struct question),SEEK_CUR);
										write(f,&a,sizeof(a));
									}
									break;
								}
								break;
						}
					}
				}else{
					while(read(f,&a,sizeof(struct question))==sizeof(struct question)){
						lseek(f,-2*sizeof(struct question),SEEK_CUR);
						write(f,&a,sizeof(a));
						lseek(f,sizeof(struct question),SEEK_CUR);
					}
				}
		break;
		}
	}
}


void listquestions(int f,int numb){
	printf("\n");
	struct question qbuffer[100]={0};
	lseek(f,0,SEEK_SET);
	int i=100*sizeof(struct question);
	int page=0;
	while(i==100*sizeof(struct question)){
		i=read(f,qbuffer,100*sizeof(struct question));
		int j;
		for(j=0;j<(i/sizeof(struct question));j++){
			if(numb==1)printf("%i. question: ",j+1+100*page);
			printf("%s",qbuffer[j+100*page].q);
			int k;
			for(k=0;k<4;k++){
				printf("\t%s",qbuffer[j+100*page].answer[k]);
			}
			printf("\n");
		}
		if(i<sizeof(struct question))printf("There aren't any questions\n");
	}
}

void makequestionnaire(int f){
	struct question qbuffer1[100]={0};
	struct question qbuffer2[100]={0};
	char buffer[500];
	int o;
	int l=0;
	lseek(f,0,SEEK_SET);
	int i=100*sizeof(struct question);
	int page=0;
	while(i==100*sizeof(struct question) && l<100){
		i=read(f,qbuffer1,100*sizeof(struct question));
		int j;
		for(j=0;j<(i/sizeof(struct question));j++){
			printf("%s",qbuffer1[j+100*page].q);
			int k;
			for(k=0;k<4;k++){
				printf("\t%s",qbuffer1[j+100*page].answer[k]);
			}
			printf("\n");
			for(;;){
				printf("Press 1 if you want to put this question into the questionnaire or 2 if not\n");
				memset(buffer,0,500);
				int i=read(0,buffer,100);
				if(i==2 && buffer[0]>48 && buffer[0]<51){
					if(buffer[0]==49){
						qbuffer2[l++]=qbuffer1[j+100*page];
					}
					break;
				}
			}
		}
		if(i<sizeof(struct question))printf("There aren't any questions\n");
	}
	o=open("questionnaire.txt",O_RDWR|O_TRUNC|O_CREAT,S_IRUSR|S_IWUSR);
	write(o,&qbuffer2,(l+1)*sizeof(struct question));
	close(o);
	printf("The questionnaire is done");
}

int main(){
	int f;
	int run=1;
	f=open("data.txt",O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
	while(run){
		run=menu();
		switch(run){
			case 1 :
				newquestion(f);
				break;
			case 2 :
				modquestion(f);
				break;
			case 3 :
				delquestion(f);
				break;
			case 4 :
				listquestions(f,0);
				break;
			case 5 :
				makequestionnaire(f);
				break;
		}
	}
	close(f);
	return 0;
}}