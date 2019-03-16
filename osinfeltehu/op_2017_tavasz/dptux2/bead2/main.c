/*A Magyar Labdajátékok Szervezete (MLSZ) nemzeti konzultációt hirdetett a labdajátékok felvirágoztatására!
Első lépésként megalkotjuk azt a kérdéssort és hozzá kérdésenként azt a minimum kettő, maximum 4 választási lehetőséget
amelyet majd a labdajátékok jobbítása iránt felelősséget érzők kitölthetnek. Miután véglegesítettük a kérdéseket az MLSZ
(szülő) kérdezőbiztost(gyerek) küld a paraméterként megadott labdajáték centrumba, városba.

Az MLSZ véletlenszerűen kiválaszt 3 kérdést, amit csővezetéken kiküld a kérdezőbiztosnak azután, hogy az bejelentkezett
az MLSZ-hez, hogy készen áll a feladat végrehajtására. A kérdezőbiztos véletlenszerűen választ 10 és 20 közti konzultálni
kívánó polgárt, akik kitöltik a kérdőívet (véletlenszerűen). Az eredményt visszaküldi csővezetéken az MLSZ-be,
majd a kérdezőbiztos befejezi a konzultációt. Az MLSZ fogadja az eredményt és a képernyőre írja a konzultáció eredményét.

Készítsünk C programot, amellyel a leírt konzultáció elvégezhető.
*/
#include <stdio.h>
#include <stdlib.h>//exit
#include <sys/msg.h> 
#include <sys/types.h>
#include <string.h> 
#include <unistd.h> //fork,pipe
#include "poll.h"





struct mymsg {
    long int mtype;       /* message type */
    char mtext[1024];    /* message text */
};

int sending(int msgbuffer, char texttwrt[],char texttsnd[],long int t){
	
	int status;
	struct mymsg message;
	strcpy(message.mtext,texttsnd);
	message.mtype = t; 
	
	printf("%s",texttwrt);
	
	status = msgsnd( msgbuffer,&message,strlen ( message.mtext ) + 1, 0); 
	
	if ( status < 0 ) 
          perror("Error: Message could not be sent!\n"); 
     return 0; 
};

int receiving(int msgbuffer,long int t){
	 struct mymsg message; 
     	int status; 
     
     status = msgrcv(msgbuffer, &message, 1024, t, 0 ); 
     
     if ( status < 0 ) 
          perror("Error: Message could not be received!\n");  
     else
          printf( "%s",message.mtext); //del
	  
     return 0; 
};

int main(int argc, char *argv[]){
	srand(time(NULL));
	//parameter check
	if(argc != 3){
		printf("Error: Missing parameter!\n");
		return 1;
	}
	
	//pipe
	int pipe1[2];
	
	if(pipe(pipe1) == -1){
		perror("The pipe calling was not successful\n");
        exit(1);
	}
	
	//message
	int status; 
    key_t key = ftok(argv[0],1);	
	int msgbuffer = msgget( key, 0600 | IPC_CREAT );//?? 
     if ( msgbuffer < 0 ) { 
          perror("Error: Message buffer could not be created!\n"); 
          return 1; 
     } 
	 
	//polling
	
	poll();
	printf("Questionnaire completed!\n");
	//file exists
	if(access("questionnairefinal.txt",F_OK) == -1){
		printf("Error: Final file not found!\n");
		return 1;
	}
	
	//file readable
	FILE* f = fopen("questionnairefinal.txt","rb");
	
	if (f == 0){
		 perror("Error: Could not open file\n");
		 exit(1);
	}
	
	
	//choosing questions
	int questionCount = 0;
	struct Record r;
	
	while( fread(&r,sizeof(r),1,f) ){
		questionCount++;
	}
	
	fclose(f);
	
	int chosenQuestions[3] = {0};
	
	
	if(questionCount < 3){
		printf("Error: Not enough question!\n");
		return 1;
	}else if(questionCount == 3){
		chosenQuestions[0] = 1;
		chosenQuestions[1] = 2;
		chosenQuestions[2] = 3;
	}else{
			chosenQuestions[0] = genRandom(questionCount);
			do{
				chosenQuestions[1] = genRandom(questionCount);
			}while(chosenQuestions[0] == chosenQuestions[1]);
			do{
				chosenQuestions[2] = genRandom(questionCount);
			}while(chosenQuestions[0] == chosenQuestions[2] ||chosenQuestions[1] == chosenQuestions[2] );	
			qsort(chosenQuestions,3,sizeof(int),cmpfunc);
	}
	
	//fork
	pid_t child= fork();
	
	if(child < 0){
		perror("The fork calling was not successful\n");
		exit(1);
	}
	else if(child > 0){
	//parent - MLSZ
		char* s1 = "MLSZ has sent out the pollster!\n";
		char s2[40]; 
		sprintf(s2,"The pollster arrived at %s %s\n",argv[1],argv[2]);
		sending(msgbuffer,s1,s2,1);
		
		//file exists
		if(access("questionnairefinal.txt",F_OK) == -1){
			printf("Error: Final file not found!\n");
			return 1;
		}

		//file readable
		FILE* f = fopen("questionnairefinal.txt","rb");
		
		if (f == 0){
			 perror("Error: Could not open file\n");
			 exit(1);
		}
		int i = 0,j = 0;
		struct Record r[3];
		while( fread(&r[j],sizeof(r[j]),1,f) || j < 3 ){
			i++;
			if(i == chosenQuestions[j])
				j++;	
			
		}
		write(pipe1[1],r,sizeof(r));
		close(pipe1[1]);
		fclose(f);
		sending(msgbuffer,"MLSZ has sent the chosen questions!\n","Pollster has received the chosen questions!\n",2);
		//waiting for citizens to complete the questions
		receiving(msgbuffer,3);
		int citizens;
		read(pipe1[0],&citizens,sizeof(int));
		int answer[citizens*3];
		read(pipe1[0],&answer,sizeof(answer));
		close(pipe1[0]);
		i = 0;
		for(;i < 3;i++){
			printf("%i.Question: %s \n",i+1,r[i].question);
			int j = 0;
			
			while(r[i].answers[j][0] != '\0' && j < 4){
				int sum = 0;
				int z = 0;
				for(;z < citizens;z++){
					if(answer[i + (z*3)] == j+1)
						sum++;
				}
				printf("Answer %i. : %s Chosen by: %i\n",j+1,r[i].answers[j],sum);	
				j++;
			}
			printf("Date: %s \n\n",r[i].time);
			
		}
		
		printf("MLSZ has finished the job!\n");
	}
	else{
		//child - kérdezőbiztos
		//To city
		receiving(msgbuffer,1);
		//Selected questions
		receiving(msgbuffer,2);
		struct Record r[3];
		read(pipe1[0],r,sizeof(r));
		close(pipe1[0]);
		
		int citizens = genRandom(10) + 10;
		int answers[citizens*3];
		int j = 0;
		for(;j < citizens;j++){
			int i = 0;
			for(;i<3;i++){
				int anum = 2;
				if(r[i].answers[2][0] != '\0'){
					anum++;
					if(r[i].answers[3][0] != '\0')
						anum++;
				}
				
				answers[i+(3*j)] = genRandom(anum);
			}
		}
		write(pipe1[1],&citizens,sizeof(int));
		write(pipe1[1],answers,sizeof(answers));
		close(pipe1[1]);
		sending(msgbuffer,"Questionarie answered and sent!\n","MLSZ has received the answers!\n",3);
		printf("Pollster finished the job!\n");
	}
	
	
	
	return 0;
};