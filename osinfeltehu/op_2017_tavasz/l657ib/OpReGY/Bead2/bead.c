#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 512

void menu();
void addQuestion();
void modifyQuestion();
void deleteQuestion();
void listQuestions();
void commitQuestionList();
void fillSurvey();

int signalFlag = 0;

void handler(int signum) {
    signalFlag = signum;
}

char* convertSignalToStr(int signum) {
    switch(signum) {
        case SIGTERM: return "SIGTERM";
        default: return "UNKWNOWN";
    }
}

int main(){
    srand(time(NULL));
    menu();
    return 0;
}

void menu() {
    char answer;
    system("clear");
    while(answer != '7'){
        printf("Elérhető menüpontok:\n");
        printf("1. Kérdés megadása\n");
        printf("2. Kérdés módosítása\n");
        printf("3. Kérdés törlése\n");
        printf("4. Kérdések listázása\n");
        printf("5. Kérdéssor véglegesítése\n");
        printf("6. Kérdőív elküldése\n");
        printf("7. Kilépés\n");

        printf("A választott menüpont: ");
        scanf(" %c",&answer);

        switch(answer) {
            case '1':
                addQuestion();
                break;
            case '2':
                modifyQuestion();
                break;
            case '3':
                deleteQuestion();
                break;
            case '4':
                listQuestions();
                break;
            case '5':
                commitQuestionList();
                break;
            case '6':
                fillSurvey();
                break;
        }
    }
}

void addQuestion() {
    FILE* f = fopen("kerdesek.txt","a+");
    char id[MAX_BUFFER_SIZE];
    printf("A kérdés azonosítója (ez alapján kereshető vissza a későbbiekben:\n");
    scanf("%s",&id);
    char question[MAX_BUFFER_SIZE];
    printf("A kérdés:\n");
    scanf(" %[^\n]s",&question);
    int numOfAnswers;
    printf("A lehetséges válaszok száma:\n");
    scanf("%d",&numOfAnswers);
    while (numOfAnswers < 2 || numOfAnswers > 4)
    {
        printf("Kérem 2-4 közötti számot adjon meg!\n");
        scanf("%d", &numOfAnswers);
    }
    char answers[numOfAnswers][MAX_BUFFER_SIZE];
    int i;
    for (i = 0; i < numOfAnswers; i++)
    {
        printf("%d. válasz:\n", i + 1);
        scanf(" %[^\n]s",&answers[i]);
    }
    printf("A megadott kérdés:\n%s\n",question);
    printf("A megadott válaszok:\n");
    int j;
    for(j=0;j<numOfAnswers;j++) {
        printf("%d.: %s\n",j+1,answers[j]);
    }
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    strftime(s,sizeof(s),"%c",tm);
    
    fputs(id,f);
    fputs("\t",f);
    fputs(question,f);
    fputs("\t",f);
    char buffer[10];
    sprintf(buffer,"%d",numOfAnswers);
    fputs(buffer,f);
    fputs("\t",f);
    for(i = 0;i<numOfAnswers;i++) {
        fputs(answers[i],f);
        fputs("\t",f);
    }
    fputs(s,f);
    fputs("\n",f);

    fclose(f);
}

void modifyQuestion() {
    FILE* f;
    f = fopen("kerdesek.txt","r");
    FILE* modified;
    modified = fopen("temp.txt","w");
    char id[MAX_BUFFER_SIZE];
    listQuestions();
    printf("Adja meg a módosítandó kérdés azonosítóját:\n");
    scanf("%s",id);
    char line[MAX_BUFFER_SIZE];
    while(fgets(line,sizeof(line),f) != NULL) {
        if((strstr(line,id)) != NULL) {
            char question[MAX_BUFFER_SIZE];
            printf("A kérdés:\n");
            scanf(" %[^\n]s",&question);
            int numOfAnswers;
            printf("A lehetséges válaszok száma:\n");
            scanf("%d",&numOfAnswers);
            while (numOfAnswers < 2 || numOfAnswers > 4)
            {
                printf("Kérem 2-4 közötti számot adjon meg!\n");
                scanf("%d", &numOfAnswers);
            }
            char answers[numOfAnswers][MAX_BUFFER_SIZE];
            int i;
            for (i = 0; i < numOfAnswers; i++)
            {
                printf("%d. válasz:\n", i + 1);
                scanf(" %[^\n]s",&answers[i]);
            }
            printf("A megadott kérdés:\n%s\n",question);
            printf("A megadott válaszok:\n");
            int j;
            for(j=0;j<numOfAnswers;j++) {
                printf("%d.: %s\n",j+1,answers[j]);
            }
            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            char s[64];
            strftime(s,sizeof(s),"%c",tm);
            
            fputs(id,modified);
            fputs("\t",modified);
            fputs(question,modified);
            fputs("\t",modified);
            char buffer[10];
            sprintf(buffer,"%d",numOfAnswers);
            fputs(buffer,modified);
            fputs("\t",modified);
            for(i = 0;i<numOfAnswers;i++) {
                fputs(answers[i],modified);
                fputs("\t",modified);
            }
            fputs(s,modified);
            fputs("\n",modified);
        }
        else {
            fputs(line,modified);
        }
    }
    fclose(f);
    fclose(modified);
    remove("kerdesek.txt");
    rename("temp.txt","kerdesek.txt");
}

void deleteQuestion() {
    FILE* f;
    f = fopen("kerdesek.txt","r");
    FILE* temp;
    temp = fopen("temp.txt","w");
    char id[MAX_BUFFER_SIZE];
    listQuestions();
    printf("Adja meg a törlendő kérdés azonosítóját:\n");
    scanf("%s",id);
    char line[MAX_BUFFER_SIZE];
    while(fgets(line,sizeof(line),f) != NULL) {
        if((strstr(line,id)) == NULL) {
            fputs(line,temp);
        }
    }
    fclose(f);
    fclose(temp);
    remove("kerdesek.txt");
    rename("temp.txt","kerdesek.txt");
}

void listQuestions() {
    FILE* f;
    f = fopen("kerdesek.txt","r");
    char line[MAX_BUFFER_SIZE];
    int i = 1;
    char* s;
    while(fgets(line,sizeof(line),f) != NULL) {
        printf("%d. kérdés: \n",i);
        for(s = strtok(line,"\t");s;s = strtok(NULL,"\t")) {
            printf("%s\n",s);
        }
        i++;
    }
    fclose(f);
}

void commitQuestionList() {
    FILE* f;
    FILE* final;
    f = fopen("kerdesek.txt","r");
    final = fopen("vegleges.txt","w");
    char line[MAX_BUFFER_SIZE];
    
    int i = 1;
    char answer;
    while(fgets(line,sizeof(line),f) != NULL) {
        printf("%d. kérdés: \n",i);
        printf("%s\n",line);
        printf("Meg akarja tartani a kérdést? (i/n) ");
        scanf(" %c",&answer);
        if(answer == 'i') {
            fputs(line,final);
        }
        i++;
    }

    fclose(f);
    fclose(final);
    remove("kerdesek.txt");
}

void fillSurvey() {
    signal(SIGTERM,handler);
    int pipefd[2];
    pid_t pid;
  
    if(pipe(pipefd) == -1) {
        perror("Pipe nyitási hiba!\n");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if(pid == -1) {
        perror("Forkolási hiba!\n");
        exit(EXIT_FAILURE);
    }
    if(pid == 0) {
        pause();
        sleep(1);
        kill(getppid(),SIGTERM);
        //printf("GY: A kérdezőbiztos bejelentkezett.\n");
        char survey[3][MAX_BUFFER_SIZE];
        int j = 0;
        for(j=0;j<3;j++) {
            pause();
            read(pipefd[0],survey[j],sizeof(survey[j]));
            sleep(1);
            kill(getppid(),SIGTERM);
        }
        printf("Megkaptam a kérdéseket.\n");
        int fillersNum = rand()%11+10;
        printf("A kérdőívet %i polgár fogja kitölteni.\n",fillersNum);
        for(j=0;j<3;j++) {
            char line[MAX_BUFFER_SIZE];
            strcpy(line,survey[j]);
            char *tokens[10];
            int k = 0;
            tokens[k] = strtok(line,"\t");
            while(tokens[k] != NULL) {
                tokens[++k] = strtok(NULL,"\t");
            }
            int numOfAnswers = atoi(tokens[2]);
            char answers[numOfAnswers][MAX_BUFFER_SIZE];
            for(k = 3;k<3+numOfAnswers;k++) {
                strcpy(answers[k-3],tokens[k]);
            }
            char givenAnswers[fillersNum][MAX_BUFFER_SIZE];
            for(k = 0;k<fillersNum;k++) {
                int choice = rand()%numOfAnswers;
                char choiceString[MAX_BUFFER_SIZE];
                strcpy(givenAnswers[k],answers[choice]);
            }
            int db[4];
            for(k = 0;k<4;k++) {
                db[k] = 0;
            }
            if(numOfAnswers == 2) {
                for(k = 0;k<fillersNum;k++) {
                    if(strcmp(tokens[3],givenAnswers[k]) == 0) {
                        db[0]++;
                    } else if(strcmp(tokens[4],givenAnswers[k]) != 0) {
                        db[1]++;
                    }
                }
            }
            if(numOfAnswers == 3) {
                for(k = 0;k<fillersNum;k++) {
                    if(strcmp(tokens[3],givenAnswers[k]) == 0) {
                        db[0]++;
                    } else if(strcmp(tokens[4],givenAnswers[k]) == 0) {
                        db[1]++;
                    } else if(strcmp(tokens[5],givenAnswers[k]) == 0) {
                        db[2]++;
                    }
                }
            }
            if(numOfAnswers == 4) {
                for(k = 0;k<fillersNum;k++) {
                    if(strcmp(tokens[3],givenAnswers[k]) == 0) {
                        db[0]++;
                    } else if(strcmp(tokens[4],givenAnswers[k]) == 0) {
                        db[1]++;
                    } else if(strcmp(tokens[5],givenAnswers[k]) == 0) {
                        db[2]++;
                    } else if(strcmp(tokens[6],givenAnswers[k]) == 0) {
                        db[3]++;
                    }
                }
            }
            write(pipefd[1],db,sizeof(db));
        }
        printf("A kérdezőbiztos befejezte a munkát.\n");
    } else {
        char questions[100][MAX_BUFFER_SIZE];
        char line[MAX_BUFFER_SIZE];
        FILE* file = fopen("vegleges.txt","r");
        if(file == NULL) {
            printf("Fájl megnyitási hiba!\n");
            exit(1);
        }
        int i = 0;
        while(fgets(line,sizeof(line),file) != NULL) {
            strcpy(questions[i],line);
            i++;
        }
        char survey[3][MAX_BUFFER_SIZE];
        int j = 0;
        int a = 0;
        for(j = 0;j<3;j++) {
            int place = rand()%i;
            strcpy(survey[j],questions[place]);
        }
        fclose(file);
        printf("Az MLSZ vár a kérdezőbiztos bejelentkezésére.\n");
        sleep(1);
        kill(pid,SIGTERM);
        pause();
        printf("SZ: A kérdezőbiztos bejelentkezett.\n");
        for(j=0;j<3;j++) {
            printf("Az %i. kérdés küldése...\n",j+1);
            printf(survey[j]);
            write(pipefd[1],survey[j],sizeof(survey[j]));
            //sleep(1);
            kill(pid,SIGTERM);
            //pause();
        }
        int db[4];
        for(j=0;j<3;j++) {
            printf("Az %i. kérdésre érkezett válaszok eloszlása: \n",j+1);
            read(pipefd[0],db,sizeof(db));
            int k = 0;
            for(k = 0;k<4;k++) {
                printf("%i. válasz: %i\n",k+1,db[k]);
            }
            kill(getppid(),SIGTERM);
        }
        printf("A kérdőívek feldolgozása megtörtént.\n");
    }
    exit(EXIT_SUCCESS);
}