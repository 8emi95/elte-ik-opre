/*
	1. beadandó feladat. Beadási határidő: 2017-04-10. 
Készítsen C programot, amely rendszerhívások alkalmazásával megoldja a következő feladatot:

A Magyar Labdajátékok Szervezete (MLSZ) nemzeti konzultációt hirdet a labdajátékok felvirágoztatására!
Első lépésként meg kell alkotni azt a kérdéssort és hozzá kérdésenként azt a minimum kettő, maximum 4 választási lehetőséget
amelyet majd a labdajátékok jobbítása iránt felelősséget érzők kitölthetnek.
A készítendő program adjon lehetőséget a kérdéssor megalkotására.
Tudjunk új kérdést felvenni, kérdést módosítani és kérdést törölni.
Módosításhoz, törléshez kínálja fel választásra a kérdést és válaszait a program!
Tudjunk emellett természetesen kérdéseket és válaszaikat listázni!
A kérdésekhez tároljuk automatikusan annak felvételi dátumát (év,hó,nap,óra,perc,másodperc).
Végezetül a program adjon lehetőséget a kérdések véglegesítésére, azaz annak meghatározására,
hogy mely kérdések szerepeljenek a végső konzultációs kérdőíven!

Készítsünk C programot, amellyel a leírt tevékenység elvégezhető. Az adatokat fájlban tároljuk.

*/

//SAVEFINAL(DIFEERENT FIle)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include "functions.h"
#include "Record.h"




const char* tempfile = "questionnairetemp.txt";
const char* file = "questionnaire.txt";
const char* finalfile = "questionnairefinal.txt";

int addQuestion(){
	struct Record r;
	
	FILE* f = fopen(file, "ab");
	
	if (f == 0){
		 perror("Error at opening the file\n");
		 exit(1);
	}

	//question
	do{
		printf("Enter the question!\n");
	}while( readString(r.question,sizeof(r.question)/sizeof(r.question[0]),stdin) );
	
	//number of options
	int qNum;
	readIntInRange(&qNum,"Enter the number of answers!",2,4);
	
	//Options
	int i;
	for(i = 0;i < qNum;i++){
		do{
			printf("Enter %i. option!\n",(i+1));
		}while( readString(r.answers[i],sizeof(r.answers[i])/sizeof(r.answers[i][0]),stdin) );
	}
	for(;i < 4;i++)
	{
		r.answers[i][0] = '\0';
	}
	
	//Date
	time_t rawtime;
	time(&rawtime);
	
	strftime(r.time,sizeof(r.time)/sizeof(r.time[0]),"%Y.%m.%d - %H:%M:%S",localtime(&rawtime));
	
	//Write 
	fwrite(&r,sizeof(struct Record),1,f);
	
	//Close
	fclose(f);
	
	return 0;
};

void writeMenu(){
	printf("1. Add Question\n");
	printf("2. List Questions\n");
	printf("3. Modify Question\n");
	printf("4. Delete Question\n");
	printf("5. Finalize Questionnaire\n");
	printf("6. Quit\n");
}

//Módosításhoz, törléshez kínálja fel választásra a kérdést és válaszait a program!
//Kell még modify és Delete hez hogy töröljük választ
int deleteQuestion(){
	
	//File exists
	if(access(file,F_OK) == -1){
		printf("File not found!\n");
		return 1;
	}
	
	//Open
	FILE* f= fopen(file,"rb");
	FILE* ftemp = fopen(tempfile,"wb");

	//Input
	int qNum;
	do{
		printf("Delete which question?\n");	
	}while( readInt(&qNum,stdin) );
	//Error Check
	if(qNum == 0){
		printf("No question with that number!\n");
		return 0;
	}
		
	
	//Selective copy
	int i = 1;
	struct Record r;
	
	while( fread(&r,sizeof(struct Record),1,f) ){
		if(i != qNum){
			fwrite(&r,sizeof(struct Record),1,ftemp);
		}
		else{
			
			int aCount=writeQuestion(stdout,&r,qNum);
			
			int button;
			readIntInRange(&button,"1. Delete whole question.\n2. Delete answers of question.\n",1,2);
			
			switch(button)
			{
				case 1:
					printf("Question deleted!\n");
				break;
				case 2:
				{
					int quit = 0;
					while(!quit){
						if(aCount>2){
							printf("You can delete %i anwers!\n",aCount-2);
							
							readIntInRange(&button,"Enter the number of the answer you want to delete!\n",1,aCount);
							
							r.answers[button-1][0] = '\0';
							
							if(button != aCount){
								int i;
								for(i =0 ; i < aCount-1;i++){
									if(r.answers[i][0] == '\0'){
										strcpy(r.answers[i],r.answers[i+1]);
										r.answers[i+1][0] = '\0';
									}
								}
							}
							aCount -= 1;
							if(aCount>2){
								char a [3];
								int repeat = 1;
								while(repeat){
									do{
										printf("Delete more answers?\n");
										
									}while( readString(a,3,stdin) );
									if(repeat = !(a[0] == 'y' || a[0] == 'Y' || a[0] == 'n' || a[0] == 'N' ))
										printf("Not a valid option!\n");
								}
								quit = (a[0] == 'N' || a[0] == 'n');
							}
						}
						else{
							printf("No more question can be deleted.\n");
							quit = 1;
						}
						
						
					}
				}
				fwrite(&r,sizeof(struct Record),1,ftemp);
				break;
				default:
					printf("Not an option!\n");
				break;
			}

		}
		i++;
	}
	
	if(qNum >= i)
		printf("No question with that number!\n");
	
	//Close
	fclose(f);
	fclose(ftemp);
	
	//Rename
	rename(tempfile,file); //linux only
	
	return 0;
};

int modifyQuestion(){
	//File exists
	if(access(file,F_OK) == -1){
		printf("File not found!\n");
		return 1;
	}
	
	//Open
	FILE* f= fopen(file,"rb+");
	
	if (f == 0){
		 perror("Error at opening the file\n");
		 exit(1);
	}
	
	//Input
	int qNum;
	do{
		printf("Modify which question?\n");	
	}while( readInt(&qNum,stdin) );
	//Error Check
	if(qNum == 0){
		printf("No question with that number!\n");
		return 0;
	}
	
	if(0 == fseek(f,sizeof(struct Record)*(qNum-1),SEEK_SET) ){
		fpos_t t;
		fgetpos(f,&t);
		struct Record r;
		
		if( fread(&r,sizeof(struct Record),1,f)  ){
			
			fsetpos(f,&t);
			
			int aCount = writeQuestion(stdout,&r,qNum);
			
			//What to modify?
			char a [3];
			int repeat = 1;
			while(repeat){
				do{
					printf("Modify Question?\n");
				}while( readString(a,3,stdin) );
				if(repeat = !(a[0] == 'y' || a[0] == 'Y' || a[0] == 'n' || a[0] == 'N' ))
					printf("Not a valid option!\n");
			}
			
			if( a[0] == 'Y' || a[0] == 'y' ){
				do{
					printf("Enter the new question!\n");
				}while( readString(r.question,sizeof(r.question)/sizeof(r.question[0]),stdin) );
			}
			
			repeat = 1;
			while(repeat){
				do{
					printf("Modify answers?\n");
				}while( readString(a,3,stdin) );
				if(repeat = !(a[0] == 'y' || a[0] == 'Y' || a[0] == 'n' || a[0] == 'N' ))
					printf("Not a valid option!\n");
			}
			
			if(a[0] == 'y' || a[0] == 'Y'){
				int quit = 0;
				while(!quit){ 
					
					readIntInRange(&qNum,"Enter the number of the answer you want to modify!\n",1,aCount);
					
					do{
						printf("Enter the new answer!\n");
					}while( readString(r.answers[qNum-1],sizeof(r.answers[qNum-1])/sizeof(r.answers[qNum-1][0]),stdin) );
					
					
					repeat = 1;
					while(repeat){
						do{
							printf("Modify more answers?\n");
						}while( readString(a,3,stdin) );
						if(repeat = !(a[0] == 'y' || a[0] == 'Y' || a[0] == 'n' || a[0] == 'N' ))
							printf("Not a valid option!\n");
					}
					quit = (a[0] == 'n' || a[0] == 'N');
				}
			}
			
			//modify
			
			fwrite(&r,sizeof(struct Record),1,f);
		}
		else
			printf("No question with that number!\n"); 		
	}
	else
		printf("No question with that number!\n"); 
	
	fclose(f);
	return 0;
};

int listQuestions(){
	
	//File exists
	if(access(file,F_OK) == -1){
		printf("File not found!\n");
		return 1;
	}
	
	//File readable
	FILE* f = fopen(file,"rb");
	
	if (f == 0){
		 perror("Error at opening the file\n");
		 exit(1);
	}
	
	struct Record r;
	
	//print
	int i = 0;
	while( fread(&r,sizeof(r),1,f) ){
		i++;
		writeQuestion(stdout,&r,i);
	}
	
	return 0;
};

int finalizeQuestionnaire(){
	//File exists
	if(access(file,F_OK) == -1){
		printf("File not found!\n");
		return 1;
	}
	
	//File readable
	FILE* f = fopen(file,"rb");
	FILE* ff = fopen(finalfile,"w");
	
	if (f == 0 || ff == 0){
		 perror("Error at opening the file\n");
		 exit(1);
	}
	//question number
	fseek(f, 0, SEEK_END);
	int qMax = ftell(f)/sizeof(struct Record);
    if (qMax == 0){
        printf("File empty!\n");
		return 0;
    }
	
	
	int qsNum;
	readIntInRange(&qsNum,"Enter the number of Questions in the Questionnaire\n",1,qMax);
	
	struct Record r;
	int i;
	for(i=0;i<qsNum;i++){
		
		int qNum;
		char buffer[50];
		sprintf(buffer,"Enter the number of the %i. question\n",i+1);
		readIntInRange(&qNum,buffer,1,qMax);
		
		if(0 == fseek(f,sizeof(struct Record)*(qNum-1),SEEK_SET) )
			if( fread(&r,sizeof(struct Record),1,f)  )
				writeQuestion(ff,&r,i+1);
	}
	fclose(f);
	fclose(ff);
	return 0;
};


int main(){
	int quit = 0,button;
	
	while(!quit){
		writeMenu();

		int repeat;
		while(repeat = readInt(&button,stdin));
		
		switch(button)
		{
			case 1:
				addQuestion();
			break;
			case 2:
				listQuestions();
			break;
			case 3:
				modifyQuestion();
				break;
			case 4:
				deleteQuestion();
			break;
			case 5:
				finalizeQuestionnaire();
			break;
			case 6:
				quit = 1;
			break;
			default:
				printf("Not an option!\n");
			break;
		}
	}
	
	return 0;
};