#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <time.h>
#include <sys/time.h>
#include <unistd.h>  //fork
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h> //waitpid
#include <errno.h>
#include <limits.h> 
#include <signal.h>
 struct Data
 {  
    int index;
    char question[100];
    char as_a[100];
    char as_b[100];
    char as_c[100];
    char as_d[100];
    char time[100];
 };

 typedef struct node
{
struct Data data;
struct node *next;
}node_t,*Link;

//menu begin
void menu() 
{   
printf("\t*****************************************************************\n");   printf("\t*                                                               *\n");   printf("\t*     questions and possible answers collection system          *\n");   
printf("\t*                                                               *\n");   
printf("\t*  [1]Add new questions         [2]Delete questions             *\n");

printf("\t*  [3]List questions            [4]Modify questions and answers *\n");   
printf("\t*  [5]Select Questions, output  [0]Quit the System  and save    *\n");   printf("\t*                                                               *\n");   printf("\t*****************************************************************\n");  
} //menu end

//print begin
void print_list(Link l) {
    node_t * current = l->next;

    while (current != NULL) {
	printf("\t%d\n", current->data.index);
        printf("\t%s\n", current->data.question);
	printf("\t%s\n", current->data. as_a);
	printf("\t%s\n", current->data. as_b);
	printf("\t%s\n", current->data. as_c);
	printf("\t%s\n", current->data. as_d);
	printf("\t%s\n", current->data.time);
       current = current->next;
  	}
}//Print end
//remove space start
void RemoveSpaces(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;
}
//remove space end
// time start
char* getTime(){
time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  char* r=asctime (timeinfo);
  RemoveSpaces(r);
  return r;
}//time end


//Push begin
void push(Link l) {
    node_t *p,*r;
    r=l; 
    int ncount = 1;
    while (r->next != NULL) {
        r = r->next;
	ncount ++;
    }

    p=(node_t *)malloc(sizeof(node_t));
    (p->data.index)=ncount;
    printf("\tEnter your question : ");    
    scanf("%s",(p->data. question)); 
    printf("\tEnter your answer a : ");    
    scanf("%s",(p->data. as_a)); 
    printf("\tEnter your answer b : ");    
    scanf("%s",(p->data. as_b)); 
    printf("\tEnter your answer c : ");    
    scanf("%s",(p->data. as_c)); 
    printf("\tEnter your answer d : ");    
    scanf("%s",(p->data. as_d));
    strcpy(p->data.time, getTime());
    p->next=NULL;
    r->next=p;
    }//push end

//Locate begin
node_t* Locate(Link l,int locIndex){
node_t *r; 
r=l->next; 
while(r!=NULL){
	if(r->data.index== locIndex)
	{
		return r;
	}
	r=r->next;}
return 0;
} //Locate end

//Del begin
void Del(Link l){
int del;
node_t *p,*r;
if(!l->next) {
    printf("\n=====>There is nothing to delete!\n");   
    return;   
} 
printf("\tEnter the index for the question you wanna delete :"); 
scanf("%d",&del);
p=Locate(l,del);
if(p){
r=l;
while(r->next!=p){
	r=r->next;
}
r->next=p->next;
free(p);
printf("\tDeleted \n");
while(r->next != NULL){
	r=r->next;
	r->data.index-=1;
}
}
}//Del end

//Modify begin
void Modify(Link l){
node_t *p;
int mo;
if(!l->next) {
    printf("\n=====>There is nothing to delete!\n");   
    return;   
}
printf("\tEnter the index for the question you wanna modify :"); 
scanf("%d",&mo);
p=Locate(l,mo);
if(p){
printf("\tPlease enter the new question(original : %s)",p->data.question);
scanf("%s",p->data.question);
getchar();  
printf("\tPlease enter the new question(original : %s)",p->data. as_a);
scanf("%s",p->data. as_a);
getchar();  
printf("\tPlease enter the new question(original : %s)",p->data. as_b);
scanf("%s",p->data. as_b);
getchar();  
printf("\tPlease enter the new question(original : %s)",p->data. as_c);
scanf("%s",p->data. as_c);
getchar();  
printf("\tPlease enter the new question(original : %s)",p->data. as_d);
scanf("%s",p->data. as_d);
strcpy(p->data.time, getTime());
printf("\tModified\n");
}else{
printf("\tnot find\n");
}

} //Modify end

void Save(Link l){
	FILE* fp;
	node_t *p;
	int count=0;
	
	fp=fopen("record.txt","w");
	if(fp==NULL){
		printf("/n=====>error when reopen the file");
		return;
	}
	p=l;
	while(p->next != NULL){
	p=p->next;
	fprintf(fp, "%d\n", p->data.index);
	fprintf(fp, "%s\n", p->data.question);
	fprintf(fp, "%s\n", p->data. as_a);
	fprintf(fp, "%s\n", p->data. as_b);
	fprintf(fp, "%s\n", p->data. as_c);
	fprintf(fp, "%s\n", p->data. as_d);
	fprintf(fp, "%s\n", p->data.time);
	count++;
	}
	if(count>0){
		printf("\t=====>save success.(%d quesions saved)\n",count);

	}else{
		printf("\t=====>save failed.(%d quesions saved)\n",count);	
	}
	fclose(fp);
}
int CountN(Link l){
	FILE* fp;
	node_t *p;
	int count=0;
	p=l;
	while(p->next != NULL){
	p=p->next;
	count++;
	}
	return count;
}

void Select(Link l){
FILE *f;
f = fopen("survey.txt", "a");
int mo;
print_list(l);
printf("\tEnter the number of question you wanna select for a survey :"); 
scanf("%d",&mo);
int i;
for(i=0;i<mo;i++){
node_t *p;
int se;
printf("\tEnter the index for the question you wanna include in survey :"); 
scanf("%d",&se);
p=Locate(l,se);
if(p){
	fprintf(f, "%d\n", p->data.index);
	fprintf(f, "%s\n", p->data.question);
	fprintf(f, "%s\n", p->data. as_a);
	fprintf(f, "%s\n", p->data. as_b);
	fprintf(f, "%s\n", p->data. as_c);
	fprintf(f, "%s\n", p->data. as_d);
	fprintf(f, "%s\n", p->data.time);
}
}
}

void handler(int signumber){
	printf("Parent know child is ready");
	
}

int main()     { 
srand(time(NULL));
int count=0;
signal(SIGTERM,handler);
FILE *fp;
menu();
Link l =NULL;
l = malloc(sizeof(node_t));

//read from file
node_t *p,*r;
r =l;
if ( !( fp = fopen( "record.txt", "r" ))){
    printf( "\tFile open failed\n" );
}
while(!feof(fp)){
	p=(node_t *)malloc(sizeof(node_t));
	fscanf(fp, "%d\n", &(p->data.index));
	fscanf(fp, "%s\n", p->data.question);
	fscanf(fp, "%s\n", p->data. as_a);
	fscanf(fp, "%s\n", p->data. as_b);
	fscanf(fp, "%s\n", p->data. as_c);
	fscanf(fp, "%s\n", p->data. as_d);
	fscanf(fp, "%s\n", p->data.time);
	p->next=NULL;
	r->next=p;
	r=p;
	count++;
}
int pid,fd;
printf("\t=====>Fifo start!\n");
unlink("fifo1.ftc");
unlink("fifo2.ftc");
int fid1=mkfifo("fifo1.ftc", S_IRUSR|S_IWUSR ); 
int fid2=mkfifo("fifo2.ftc", S_IRUSR|S_IWUSR );

if (fid1==-1||fid2==-1)
{
printf("\t=====>Error number: %i",errno);
exit(EXIT_FAILURE);
}
printf("\t=====>Mkfifo ends, fork starts!\n");
pid = fork();

if(pid>0) {
printf("\t=====>already read %d quesions from file\n",count);
//read file end

while(1)
{
int choose;
printf("\t=====>Please choose："); 
scanf("%d",&choose);
 if(choose==0){Save(l);break;}
	switch(choose){
	case 3:print_list(l);
	break;
	case 1:push(l);
	break;
	case 2:Del(l);
	break;
	case 4:Modify(l);
	break;
	case 5:Select(l);
	}
}
pause();
 //write to Pipe
printf("parent process:writing \n");
fd=open("fifo1.ftc",O_WRONLY );
char buffer[PIPE_BUF + 1];
int cou = CountN(l);
int q2,q3;
int q1=rand()%cou+1;
do{
	q2=rand()%cou+1;
}while(q2==q1);
do{
	q3=rand()%cou+1;
}while(q3==q1||q3==q2);
char c1[10];
sprintf(c1, "%d", q1);
char c2[10];
sprintf(c2, "%d", q2);
char c3[10];
sprintf(c3, "%d", q3);
strncat(buffer,c1,10);
strncat(buffer,"And",10);
strncat(buffer,c2,10);
strncat(buffer,"And",10);
strncat(buffer,c3,10);
write(fd,buffer,PIPE_BUF+1);

// node_t *p1,*p2,*p3;
// p1=Locate(l,q1);
// if(p1){
// 	write(fd,&(p1->data.index),12);
// 	write(fd,p1->data.question,12);
// 	write(fd,p1->data. as_a,12);
// 	write(fd,p1->data. as_b,12);
// 	write(fd,p1->data. as_c,12);
// 	write(fd,p1->data. as_d,12);
// 	write(fd,p1->data.time,22);
// }
// p2=Locate(l,q2);
// if(p2){
// 	write(fd,&(p2->data.index),12);
// 	write(fd,p2->data.question,12);
// 	write(fd,p2->data. as_a,12);
// 	write(fd,p2->data. as_b,12);
// 	write(fd,p2->data. as_c,12);
// 	write(fd,p2->data. as_d,12);
// 	write(fd,p2->data.time,22);
// }
// p3=Locate(l,q3);
// if(p3){
// 	write(fd,&(p3->data.index),12);
// 	write(fd,p3->data.question,12);
// 	write(fd,p3->data. as_a,12);
// 	write(fd,p3->data. as_b,12);
// 	write(fd,p3->data. as_c,12);
// 	write(fd,p3->data. as_d,12);
// 	write(fd,p3->data.time,22);
// }
close(fd);
//read
char s[PIPE_BUF + 1]="nothing";
fd=open("fifo2.ftc",O_RDONLY);
printf("parent process: reading: \n");
read(fd,s,sizeof(s));
printf("result(10 samples) \n%s\n",s);
close(fd);
//end of parent
}else{
	//child
	//read
	char dest[20] ;
	scanf("%s",dest); 
	printf("Child process: Working to %s: \n",dest);
	kill(getppid(),SIGTERM);
	char s[PIPE_BUF + 1]="nothing";
	fd=open("fifo1.ftc",O_RDONLY);
	printf("Child process: reading: \n");
	read(fd,s,sizeof(s));
	int q1,q2,q3;
	int n = sscanf(s,"%dAnd%dAnd%d",&q1,&q2,&q3);
	if(n){
	printf("Child process: first quetion index : %d \n",q1);
	printf("Child process: second quetion index : %d \n",q2);
	printf("Child process: third quetion index : %d \n",q3);
	}else{
		printf("read question index error");
	}
	int a1,a2,a3;
	int q1a=0,q1b=0,q1c=0,q1d=0,q2a=0,q2b=0,q2c=0,q2d=0,q3a=0,q3b=0,q3c=0,q3d=0;
	int i;
	for(i=0;i<10;i++){
		a1=rand()%4+1;
		if(a1==1){
			q1a++;
		}else if(a1==2){
			q1b++;
		}else if(a1==3){
			q1c++;
		}else if(a1==4){
			q1d++;
		}
		
		a2=rand()%4+1;
		if(a2==1){
			q2a++;
		}else if(a2==2){
			q2b++;
		}else if(a2==3){
			q2c++;
		}else if(a2==4){
			q2d++;
		}
		
		a3=rand()%4+1;
		if(a3==1){
			q3a++;
		}else if(a3==2){
			q3b++;
		}else if(a3==3){
			q3c++;
		}else if(a3==4){
			q3d++;
		}
	}
	close(fd);
	
	//write
	fd=open("fifo2.ftc",O_WRONLY );
	char buffer[PIPE_BUF + 1];
	char c1a[10];
	sprintf(c1a, "%d", q1a);
	char c1b[10];
	sprintf(c1b, "%d", q1b);
	char c1c[10];
	sprintf(c1c, "%d", q1c);
	char c1d[10];
	sprintf(c1d, "%d", q1d);
	char c2a[10];
	sprintf(c2a, "%d", q2a);
	char c2b[10];
	sprintf(c2b, "%d", q2b);
	char c2c[10];
	sprintf(c2c, "%d", q2c);
	char c2d[10];
	sprintf(c2d, "%d", q2d);
	char c3a[10];
	sprintf(c3a, "%d", q3a);
	char c3b[10];
	sprintf(c3b, "%d", q3b);
	char c3c[10];
	sprintf(c3c, "%d", q3c);
	char c3d[10];
	sprintf(c3d, "%d", q3d);
	
	strncat(buffer,"First question:\na:",25);
	strncat(buffer,c1a,10);
	strncat(buffer,"\nb:",10);
	strncat(buffer,c1b,10);
	strncat(buffer,"\nc:",10);
	strncat(buffer,c1c,10);
	strncat(buffer,"\nd:",10);
	strncat(buffer,c1d,10);
	
	strncat(buffer,"\nSecond question:\na:",25);
	strncat(buffer,c2a,10);
	strncat(buffer,"\nb:",10);
	strncat(buffer,c2b,10);
	strncat(buffer,"\nc:",10);
	strncat(buffer,c2c,10);
	strncat(buffer,"\nd:",10);
	strncat(buffer,c2d,10);
	
	strncat(buffer,"\nThird quesiont:\na:",25);
	strncat(buffer,c3a,10);
	strncat(buffer,"\nb:",10);
	strncat(buffer,c3b,10);
	strncat(buffer,"\nc:",10);
	strncat(buffer,c3c,10);
	strncat(buffer,"\nd:",10);
	strncat(buffer,c3d,10);
	printf("Child process: writing \n");
	write(fd,buffer,PIPE_BUF+1);
	close(fd);
		
}//end of child
return 0;
}