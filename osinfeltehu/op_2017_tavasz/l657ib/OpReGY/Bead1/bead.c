/*Készítsen C programot, amely rendszerhívások alkalmazásával megoldja a következő feladatot:

A Magyar Labdajátékok Szervezete (MLSZ) nemzeti konzultációt hirdet a labdajátékok felvirágoztatására! 
Első lépésként meg kell alkotni azt a kérdéssort és hozzá kérdésenként azt a minimum kettő, maximum 4 
választási lehetőséget amelyet majd a labdajátékok jobbítása iránt felelősséget érzők kitölthetnek. 
A készítendő program adjon lehetőséget a kérdéssor megalkotására. Tudjunk új kérdést felvenni, 
kérdést módosítani és kérdést törölni. Módosításhoz, törléshez kínálja fel választásra a kérdést és válaszait a program!
Tudjunk emellett természetesen kérdéseket és válaszaikat listázni! A kérdésekhez tároljuk automatikusan annak 
felvételi dátumát (év,hó,nap,óra,perc,másodperc). 
Végezetül a program adjon lehetőséget a kérdések véglegesítésére, azaz annak meghatározására, hogy mely 
kérdések szerepeljenek a végső konzultációs kérdőíven!

Készítsünk C programot, amellyel a leírt tevékenység elvégezhető. Az adatokat fájlban tároljuk.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_BUFFER_SIZE 512

void menu();
void addQuestion();
void modifyQuestion();
void deleteQuestion();
void listQuestions();
void commitQuestionList();

void handler(int signum);

int main(){
    /*signal(SIGTERM,handler);

    int pipefd[2];
    if(pipe(pipefd) == -1) {
        perror("Csővezeték létrehozási hiba!\n");
        exit(1);
    }

    pid_t child = fork();
    if(child < 0) {
        perror("Gyerekfolyamat létrehozási hiba!\n");
        exit(1);
    } else {

        FILE *f1 = fopen("vegleges.txt","r");
        FILE *f2 = fopen("id.txt","w");
        if(f1 == NULL) {
            prinft("Fájl megnyitási hiba!\n");
            exit(1);
        }
        
    }*/
    menu();
    return 0;
}

void menu() {
    char answer;
    system("clear");
    while(answer != '6'){
        printf("Elérhető menüpontok:\n");
        printf("1. Kérdés megadása\n");
        printf("2. Kérdés módosítása\n");
        printf("3. Kérdés törlése\n");
        printf("4. Kérdések listázása\n");
        printf("5. Kérdéssor véglegesítése\n");
        printf("6. Kilépés\n");

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