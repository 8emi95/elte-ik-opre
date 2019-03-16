#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFERSIZE 512
#define QUESTIONS 3 

void menu();
void ujKerdes();
void kerdesModositas();
void kerdesTorlese();
void kerdesekListazasa();
void veglegesites();
int getValaszokSzama();
int* getRandomKerdesSzamok();
const char* kitoltes(char kerdes[],int kitoltokSzama);

void handler(){
	printf("A biztos keszen all!\n");
}

int main(int argc, char* argv[])
{	
	signal(SIGUSR1,handler);
	srand(time(NULL));

	FILE *file = fopen("temp.txt", "ab+"); //create file if not exists
	fclose(file);
	menu();
	char city[BUFFERSIZE];
	printf("Kerem adja meg a kozpontot: ");
	scanf("%s",&city);
	char text[BUFFERSIZE];
	char survey[QUESTIONS][BUFFERSIZE];
	char filledSurvey[QUESTIONS][BUFFERSIZE];
	pid_t pid;
	int pp[QUESTIONS][2];
	int i=0;
	for (i=0; i<QUESTIONS; ++i){
		if(pipe(pp[i])==-1){perror("Pipe fail! \n");exit(1);}
	}

	pid = fork();

	if(pid==0){
		//child process
		sleep(3);
		kill(getpid(),SIGUSR1);
		int kitoltokSzama = (rand() % (20 + 1 - 10)) + 10;
		printf("Megkezdem a kitoltest %d emberrel\n", kitoltokSzama);
		//read the questions from the parent
		int j=0;
		for (j=0; j<QUESTIONS; ++j){
			read(pp[j][0],survey[j],sizeof(survey[j]));
			close(pp[j][0]);
			strcpy(filledSurvey[j],kitoltes(survey[j],kitoltokSzama));
		}
		//send back the answers	
		int k=0;
		for (k=0; k<QUESTIONS; ++k){
			sprintf(text,filledSurvey[k]);
			write(pp[k][1],text,sizeof(text));
			close(pp[k][1]);
		}
		printf("A biztos elvegezte a munkajat %s varosban.\n",city);
	}
	else if (pid>0){
		//parent process

		int* questionNumbers;
		questionNumbers=getRandomKerdesSzamok();
	   
		FILE* data = fopen("kerdoiv.txt","r");
			char buf[BUFFERSIZE];
			int questionCount=0;
			while(fgets(buf,sizeof(buf),data)){
				questionCount+=1;
				for(i=0; i<3; ++i){
					if(*(questionNumbers+i)==questionCount){
						  strcpy (survey[i],buf);
					}
				}
			}
		fclose(data);

		//send the questions to the child
		int j=0;
		for (j=0; j<QUESTIONS; ++j){
			sprintf(text,survey[j]);
			write(pp[j][1],text,sizeof(text));
			close(pp[j][1]);
		}
		wait(NULL); //wait for the child

		//read the answers from the child
		int k=0;
		for (k=0; k<QUESTIONS; ++k){
			read(pp[k][0],text,sizeof(text));
			close(pp[k][0]);
			printf("A %s kerdesre erkezett valaszok: \n", survey[k]);
			printf("%s\n",text);
		}

		printf("Az MLSZ feldolgozta a valaszokat.\n");
	}

    return 0;
}

void menu(){
	char m[BUFFERSIZE];
	int valasztas;
	do{
		system("clear");
		printf("\t1 - Uj kerdes felvetele\n");
		printf("\t2 - Meglevo kerdes modositasa\n");
		printf("\t3 - Kerdes Torlese\n");
		printf("\t4 - Kerdesek listazasa\n");
		printf("\t5 - Kerdoiv veglegesitese\n");
		printf("\t6 - Kerdoiv elkuldese\n");
		printf("\t0 - Kilepes (vagy nyomja meg a CTRL+C - t)\n");
		printf("\n\nValasszon a menupontok kozul -> ");
		scanf("%s",m);
		valasztas = atoi(m);
		system("clear");
		if(valasztas == 1){
			ujKerdes();
		}
		else if(valasztas == 2){
			kerdesModositas();
		}
		else if(valasztas== 3){
			kerdesTorlese();
		}
		else if(valasztas == 4){
			kerdesekListazasa();
		}
		else if(valasztas == 5){
			veglegesites();
			
		}
		else if(valasztas == 6){
			return;
		}
		printf("\nNyomjon meg egy gombot!\n");
		system("read");
	}while(valasztas!=0);
	exit(0);
}

void ujKerdes(){
	printf("Uj kerdes hozzaadsa\n");
	char kerdes[BUFFERSIZE];
	int valaszokSzama;
	printf("Adja meg a kerdest:\n -> ");
   	scanf(" %[^\t\n]s",&kerdes);
	printf("A megadott kerdes: %s\n",kerdes);
	valaszokSzama=getValaszokSzama();
	char valaszok[valaszokSzama][BUFFERSIZE];
	int i;
	for (i=0; i<valaszokSzama; i = i+ 1){
		printf("Adja meg a(z) %d. valaszt:\n ->",i+1);
   		scanf(" %[^\t\n]s",&valaszok[i]);
		printf("A beirt valasz: %s \n", valaszok[i]);
	}
	FILE* data = fopen("temp.txt","a+");
	switch(valaszokSzama){
		case 2:
			fprintf(data,"%s\t%s\t%s\t%s\t%s\n",kerdes,valaszok[0],valaszok[1],__DATE__,__TIME__);
		break;
		case 3:
			fprintf(data,"%s\t%s\t%s\t%s\t%s\t%s\n",kerdes,valaszok[0],valaszok[1],valaszok[2],__DATE__,__TIME__);
		break;
		case 4:
			fprintf(data,"%s\t%s\t%s\t%s\t%s\t%s\t%s\n",kerdes,valaszok[0],valaszok[1],valaszok[2],valaszok[3],__DATE__,__TIME__);
		break;
	}
	fclose(data);
}

void kerdesModositas(){
	printf("Kerdes modositasa\n");
	kerdesekListazasa();
	printf("Adja meg a modositani kivant kerdes sorszamat\n");
	char kerdes[BUFFERSIZE];
	int kerdesSzam;
	scanf("%s",kerdes);
	kerdesSzam = atoi(kerdes);
	FILE *newfile=fopen("out.txt","w");
	FILE* data = fopen("temp.txt","r");
	char buf[BUFFERSIZE];
	int i=1;
	while(fgets(buf,sizeof(buf),data)){
		if(i==kerdesSzam){
			char kerdes[BUFFERSIZE];
			int valaszokSzama;
			printf("Adja meg az uj kerdest:\n -> ");
		   	scanf(" %[^\t\n]s",&kerdes);
			printf("A megadott kerdes: %s\n",kerdes);
			valaszokSzama=getValaszokSzama();
			char valaszok[valaszokSzama][BUFFERSIZE];
			int i;
			for (i=0; i<valaszokSzama; i = i+ 1){
				printf("Adja meg a(z) %d. valaszt:\n ->",i+1);
		   		scanf(" %[^\t\n]s",&valaszok[i]);
				printf("A beirt valasz: %s \n", valaszok[i]);
			}
		switch(valaszokSzama){
			case 2:
				fprintf(newfile,"%s\t%s\t%s\t%s\t%s\n",kerdes,valaszok[0],valaszok[1],__DATE__,__TIME__);
			break;
			case 3:
				fprintf(newfile,"%s\t%s\t%s\t%s\t%s\t%s\n",kerdes,valaszok[0],valaszok[1],valaszok[2],__DATE__,__TIME__);
			break;
			case 4:
				fprintf(newfile,"%s\t%s\t%s\t%s\t%s\t%s\t%s\n",kerdes,valaszok[0],valaszok[1],valaszok[2],valaszok[3],__DATE__,__TIME__);
			break;
		}
	}
	else{
			fprintf(newfile,"%s",buf);
		}
		i=i+1;
	}
	fclose(newfile);
	fclose(data);
	remove("temp.txt");
	rename("out.txt","temp.txt");
}

void kerdesTorlese(){
	printf("Kerdes torlese\n\n");
	char kerdes[BUFFERSIZE];
	int kerdesSzam;
	kerdesekListazasa();
	printf("Adja meg a torolni kivant kerdes sorszamat\n");
	scanf("%s",kerdes);
	kerdesSzam = atoi(kerdes);
	FILE *newfile=fopen("out.txt","w");
	FILE* data = fopen("temp.txt","r");
	char buf[BUFFERSIZE];
	int i=1;
	while(fgets(buf,sizeof(buf),data)){
		if(i==kerdesSzam){
			printf("%d. kerdes torolve\n",i);
		}else{
			fprintf(newfile,"%s",buf);
		}
		i=i+1;
	}
	fclose(newfile);
	fclose(data);
	remove("temp.txt");
	rename("out.txt","temp.txt");
}

void kerdesekListazasa(){
	printf("Kerdesek listazasa:\n");

	FILE* data = fopen("temp.txt","r");
	char buf[BUFFERSIZE];
	int i=1;
	while(fgets(buf,sizeof(buf),data)){
		printf("%d. kerdes: ",i);
		printf("%s \n",buf);
		i=i+1;
	}
	fclose(data);
}

void veglegesites(){
	printf("Kerdesek veglegesitese.\n");
	remove("kerdoiv.txt");
	FILE* data = fopen("temp.txt","r");
	FILE *newfile=fopen("kerdoiv.txt","w");
	char buf[BUFFERSIZE];
	char igenNem[BUFFERSIZE];
	int i=1;
	int j=0;
	while(fgets(buf,sizeof(buf),data)){
		printf("%d. kerdes: ",i);
		printf("%s \n",buf);
		printf("Szerepeljen a vegleges listaban? (i/n)""\n");
		scanf("%s",&igenNem);
		if(strcmp(igenNem, "i") == 0){
			j=j+1;
			fprintf(newfile,"%d. kerdes %s",j,buf);
		}
		i=i+1;
	}
	fclose(newfile);
	fclose(data);
}

int getValaszokSzama(){
	char valaszok[BUFFERSIZE];
	int valaszokSzama;
	int error=1;
	do{
		printf("Adja meg a valaszok szamat:\n -> ");
		scanf("%s",valaszok);
		valaszokSzama=atoi(valaszok);
		printf("A valaszok szama %d\n",valaszokSzama);
		if(valaszokSzama<2 || valaszokSzama>4){
			error=0;
			printf("Hiba a valaszoknak 2 es 4 kozott kell lenniuk \n");
		}else{
			error=1;
		}
	}while(error==0);
	return valaszokSzama;
}

int* getRandomKerdesSzamok(){
	FILE* data = fopen("kerdoiv.txt","r");
			char buf[BUFFERSIZE];
			int questionCount=0;
			while(fgets(buf,sizeof(buf),data)){
				questionCount+=1;
			}
		fclose(data);

		static int questionNumbers[3];
	    questionNumbers[0] =(rand() % (questionCount + 1 - 1)) + 1;
	    do{
	    	questionNumbers[1] = (rand() % (questionCount + 1 - 1)) + 1;
	    }while(questionNumbers[1]==questionNumbers[0]);

	    do{
	    	questionNumbers[2] =(rand() % (questionCount + 1 - 1)) + 1;
	    }while(questionNumbers[2]==questionNumbers[0] || questionNumbers[2]==questionNumbers[1]);
	    
	    return questionNumbers;
}

const char * kitoltes(char kerdes[],int kitoltokSzama){
	int answerCount=0;
	int valaszok[kitoltokSzama];
	char * pch;
	char copy[BUFFERSIZE];
	static char response[BUFFERSIZE];
	strcpy(response,""); //kiuritjuk az eddigi valaszokat
	memset(copy, '\0', sizeof(copy));
   	strcpy(copy, kerdes);
    pch = strtok (kerdes,"\t");
    while (pch != NULL)
    {	answerCount+=1;
        pch = strtok (NULL, "\t");
    }
    answerCount-=3;
    char answers[answerCount][BUFFERSIZE];
    int i;
    int megoszlas[answerCount];
    for(i=0; i<answerCount; ++i){
    	megoszlas[i]=0;
    }
    
    for(i=0; i<kitoltokSzama; ++i){
    	int temp = (rand() % (answerCount + 1 - 1)) + 1;
    	valaszok[i]=temp;
    	megoszlas[temp-1]++;
    }

    i=0;
    int j=0;
   	pch = strtok (copy,"\t");
    while (pch != NULL)
    {	i+=1;
        pch = strtok (NULL, "\t");
        if(i>=1 && i<=answerCount+1){
        	strcpy(answers[j],pch);
        	++j;
        }
    }

    for(i=0; i<answerCount; ++i){
    	char text[BUFFERSIZE];
    	snprintf(text, sizeof(text),"%d kitolto mondta hogy %s \n",megoszlas[i], answers[i]);
    	strcat(response,text);
    }
    return response;
}
