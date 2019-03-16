// TOP -> k -> ID

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


struct data{
    char question[300];
    char answer[4][200];
    char date[20];
    int ID;
};


void menu();
void exitProgram();
void addNewQuestion();
void modifyQuestions();
void deleteQuestions();
void listItems();
void deleteAllItems();
char* timeStamp();
void commitQuestions();
void listCommitedQs();
void countLines(int *lines);
void deleteDefinetive();


int main(int argc, const char * argv[]) {
    int pid,fd;
    menu();
    return 0;
    
}


void menu(){
    int menuPoint;
    do
    {
        printf("== M E N U ==\n");
        
        printf("0. Kilepes \n");
        printf("1. Uj kerdesek es valaszok felvetele \n");
        printf("2. Kerdesek modositasa \n");
        printf("3. Kerdes torlese \n");
        printf("4. Kerdesek es valaszok listazasa \n");
        printf("5. Osszes kerdes torlese a fajlbol \n");
        printf("6. Kerdesek veglegesitese \n");
        printf("7. Veglegesitett kerdoiv listazasa \n");
        printf("8. Veglegesitett fajl torlese\n");
        printf("Valasztott menupont sorszama: \n");
       
        scanf("%d", &menuPoint);
        if (menuPoint < 0 || menuPoint > 8) {
            printf("A megadaott menupont: %d nem megfelelo. Kerem adjon meg valid menupontot (0-7) \n", menuPoint);
        }
    } while ( menuPoint < 0 || menuPoint > 8 );
    
    switch (menuPoint) {
        case 0:
            //exitProgram();
            break;
        case 1:
            addNewQuestion();
            break;
        case 2:
            modifyQuestions();
            break;
        case 3:
            deleteQuestions();
            break;
        case 4:
            listItems();
            break;
        case 5:
            deleteAllItems();
            break;
        case 6:
            commitQuestions();
            break;
        case 7:
            listCommitedQs();
            break;
        case 8:
            deleteDefinetive();
            break;
    }
    
}

void deleteDefinetive(){
    
    FILE *definetive;
    definetive = fopen("definetive.txt", "r");
    
    if (definetive == NULL) {
        printf("Nem letezik a fajl\n");
        
    } else {
        fclose(definetive);
        remove("definetive.txt");
        printf("A fajl torlese sikeres volt\n");
    }
    
    menu();
    
    
    
}

void listCommitedQs(){
    //fork();
    printf("Veglegesitett kerdoiv megjelenitese: \n");
    
    FILE *definetive;
    definetive = fopen("definetive.txt", "r");
    
    char line[256];
    
    if(definetive != NULL){
        while (fgets(line, 256, definetive)) {
            printf("    %s", line);
        }
    } else {
        printf("A fajl nem letezik\n");
    }
    
    printf("1. Vissza a menube\n");
    printf("2. Kilepes\n");
    printf("Valasztas: \n");
    int choice;
    
    do{
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                menu();
                break;
            case 2:
                exitProgram();
                break;
            default:
                printf("A megadott emnupont nem megfelelo. Kerem adjon meg valid menupontot(1-2): \n");
                break;
        }
    
    }while (choice < 1 || choice > 2);
}


void addNewQuestion(){
    
    FILE *infile;
    infile = fopen("input.txt", "a");
    //az idFile letrehozasa es egyboli lezarasa azert kell, mert az ID-k keresesenel ezt a fajlt fogjuk vizsgalni de ez jelenleg meg nem letezik
    FILE *idFile;
    idFile = fopen("id.txt", "a");
    fclose(idFile);
    
    struct data newQuestion;
    struct data newID;
    struct data newAnswer;
    struct data modifyDate;
    char *tmp = timeStamp();
    
    int i = 0;
    while (fgets(newQuestion.question, 300, stdin) && i != 1) {
        printf("Add meg a kerdest: \n");
        i++;
    }
    printf("Add meg a kerdeshez tartozo ID-t: \n");
    
    retypeID:scanf("%d", &newID.ID);
    printf("ID: %d\n", newID.ID);
    
    //sorok megszamolasa az id.txt fajlban
    int lines = 0;
    countLines(&lines);
        
        //ujra megnyitjuk olvasasra es ellenorizzuk h a felhasznalo altal megadott ID szerepel e mar a fajlunkban
        FILE *idCheck;
        idCheck = fopen("id.txt", "r");
        
        int idArray[20];
        for (int i = 0; i < lines; i++) {
            int getID = 0;
            fscanf(idCheck, "%d", &getID);
            
            idArray[i] = getID;
            
            if (idArray[i] == newID.ID) {
                printf("Ilyen azonositoval mar szerepel egy kerdes.\n");
                printf("Kerem adjon meg uj azonositot: \n");
                fclose(idCheck);
                goto retypeID;
            }
        }
        fclose(idCheck);
        //ha befejeztuk az ellenorzest lezarjuk a fajlt
        
        //ujramegnyitjuk a fajlt hozzafuzesre es a felhasznalo altal adott ID-t beleirjuk a fajlba
        FILE *idFileR;
        idFileR = fopen("id.txt", "a");
        
        fprintf(idFileR, "%d\n", newID.ID);
        
        int howManyAnswer;
        printf("Mennyi valaszt kivan felvenni?: \n");
        scanf("%d", &howManyAnswer);
        
        while (howManyAnswer < 2 || howManyAnswer > 4) {
            printf("max2 min4\n");
            printf("Valasztasi lehetoseg: \n");
            scanf("%d", &howManyAnswer);
        }
        strcpy(modifyDate.date, tmp);
        fprintf(infile, "%d\n%d\n%s%s\n", newID.ID, howManyAnswer,newQuestion.question, modifyDate.date);
        
        for (int i = 0; i < howManyAnswer; i++) {
            
            int j = 0;
            while (fgets(newAnswer.answer[howManyAnswer], 200, stdin) && j < 1) {
                printf("%d. valasz: \n", i+1);
                j += 2;
            }
            fprintf(infile, "%s", newAnswer.answer[howManyAnswer]);
            
        }
        fprintf(infile, "\n");
        
        fclose(infile);
        fclose(idFileR);
        printf("A kerdes es a valaszok sikeresen hozzaadva!\n");
        
    
    printf("1. Uj kerdes hozzaadasa.\n");
    printf("2. Vissza a menube\n");
    printf("3. Kilepes.\n");
    printf("Valasztas: \n");
    
    int choice;
    
    do{
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                addNewQuestion();
                break;
            case 2:
                menu();
                break;
            case 3:
                exitProgram();
                break;
            default:
                printf("Helytelen menupont. Kerem adjon megg valid menupontot. (1-3): \n");
        }
    }while(choice < 1 || choice > 3);
    
    
}

void commitQuestions(){
    
    FILE *idFile;
    idFile = fopen("id.txt", "r");
    
    //megnezzuk h letezik-e fajl
    if (idFile != NULL) {
        
        fclose(idFile);
        int idCheckArray[20];
        char lineArray[256];
        int idArray[20];
        int qArray[20];
        
        int lines = 0;
        countLines(&lines);
        
        int hit = 0;
        int getID = 0;
        
        if (lines == 0) {
            printf("A kerdosor ures. \n");
            goto whatnow;
        } else if (lines > 0){
            
            printf("Adja meg a kerdes azonositojat (ID): \n");
            scanf("%d", &getID);
            
            FILE *idFile;
            idFile = fopen("id.txt", "r");
            
                for (int i = 0; i < lines; i++) {
                    int idFromFile;
                    fscanf(idFile, "%d", &idFromFile);
                    idCheckArray[i] = idFromFile;
                    
                        if (idCheckArray[i] == getID) {
                            hit++;
                        }
                }
                
                if (hit == 0) {
                    printf("Nincs ilyen azonositoju kerdes\n");
                    fclose(idFile);
                    goto whatnow;
                }
            fclose(idFile);
        }
        
        if (lines > 0 && hit > 0) {
            
            FILE *inFile;
            inFile = fopen("input.txt", "r");
            FILE *definetiveFile;
            definetiveFile = fopen("definetive.txt", "a");
            
            for (int i = 0; i < lines; i++) {
                
                //IDk beolvasasa az input.txt fajlbol
                int ids = 0;
                fscanf(inFile, "%d", &ids);
                idArray[i] = ids;
                
                //kerdeshez tartozo valaszok szama beolvasasa az input.txt fajlbol
                int questions;
                fscanf(inFile, "%d", &questions);
                qArray[i] = questions;
                
                if (idArray[i] != getID) {
                    
                    int j = 0;
                    while (fgets(lineArray, 256, inFile) && j < qArray[i] + 3) {
                        j++;
                    }
                } else {
                    int j = 0;
                    int q = 0;
                    while (fgets(lineArray, 256, inFile) && j < qArray[i] + 3) {
                        
                        if (j == 0 || j == 2) {
                            
                        } else if (j == 1) {
                            fprintf(definetiveFile, "Kerdes: %s", lineArray);
                        } else{
                            fprintf(definetiveFile, "   %d.Valasz: %s", q+1, lineArray);
                            q++;
                        }
                        j++;
                    }//end while
                    fprintf(definetiveFile, "\n");
                }//end else
                
            }//end for loop
            
            fclose(definetiveFile);
            printf("A kerdes sikeresen veglegesitve lett a kerdoivben.\n");
            
        whatnow:printf("1. Vissza a menube\n");
            printf("2. Uj kerdes hozzaadasa a veglegeitett kerdoivhez.\n");
            printf("3. Kilepes\n");
            printf("Valasztas: \n");
            
            int choice;
            do{
                scanf("%d", &choice);
                switch (choice) {
                    case 1:
                        menu();
                        break;
                    case 2:
                        commitQuestions();
                        break;
                    case 3:
                        exitProgram();
                        break;
                    default:
                        printf("Helytelen menupont! Kerem adjon meg valid menupontot (1/3): \n");
                }//end switch
            } while (choice < 1 || choice > 3);
        }//end if
        
        
    } else {
        printf("a fajl nem talalhato\n");
        menu();
    }
    
}

void modifyQuestions(){
    
    printf("Kerdesek modositasa\n");
    
    FILE *idFile;
    idFile = fopen("id.txt", "r");
    
    if (idFile != NULL) {
        
        fclose(idFile);

        int idCheckArray[20];
        char *tmp = timeStamp();
        struct data modifyDate;
        
        int lines = 0;
        countLines(&lines);
        
        int hit = 0;
        int getID = 0;
        
        if (lines == 0) {
            printf("A kerdosor ures. A modositas nem lehetseges\n");
        } else if (lines > 0){
            
            FILE *idFile;
            idFile = fopen("id.txt", "r");
            
            printf("Adja meg a modositani kivant kerdes azonositojat (ID): \n");
            scanf("%d", &getID);
            
            for (int i = 0; i < lines; i++) {
                int idFromFile;
                fscanf(idFile, "%d", &idFromFile);
                idCheckArray[i] = idFromFile;
                
                if (idCheckArray[i] == getID) {
                    hit++;
                }
            }
            
            if (hit == 0) {
                printf("Nincs ilyen azonositoju kerdes\n");
            }
            fclose(idFile);
        }
        
        
        if (lines > 0 && hit > 0) {
            
            FILE *inFile;
            inFile = fopen("input.txt", "r");
            FILE *tmpInFile;
            tmpInFile = fopen("tmpInFile.txt", "a");
            
            //kiszedjuk az id-t az input filebol
            int scanID[20];
            int scanHowManyQ[20];
            char lineArray[300];
            for (int i = 0; i < lines; i++) {
                //IDk kiszedese az input fajlbol
                int IDs = 0;
                fscanf(inFile, "%d", &IDs);
                scanID[i] = IDs;
                
                //kerdeshez tartozo valszok kiszedese az input fajlbol
                int questions = 0;
                fscanf(inFile, "%d", &questions);
                scanHowManyQ[i] = questions;
                
                if (scanID[i] == getID) {
                    
                    //regi sorok kiolvasasa a fajlbol
                    int j = 0;
                    while (fgets(lineArray, 300, inFile) && j < scanHowManyQ[i] + 3) {
                        //printf("%s", lineArray);
                        j++;
                    }
                    
                    //azonosito beirasa a fajlba ez marad a regi
                    fprintf(tmpInFile, "%d\n", scanID[i]);
                    //uj kerdes beolvasasa
                    char newQuestionLine[300];
                    int i = 0;
                    while (fgets(newQuestionLine, 300, stdin) && i != 1) {
                        printf("Add meg a kerdest: \n");
                        i++;
                    }
                    
                    //bekerjuk az valaszok szamat a felhasznalotol
                    int howManyAnswer;
                    printf("Mennyi valaszt kivan felvenni a kerdeshez?: \n");
                    scanf("%d", &howManyAnswer);
                    
                    while (howManyAnswer < 2 || howManyAnswer > 4) {
                        printf("max2 min4\n");
                        printf("Valasztasi lehetoseg: \n");
                        scanf("%d", &howManyAnswer);
                    }
                    
                    strcpy(modifyDate.date, tmp);
                    //uj adatok beirasa a fajba
                    fprintf(tmpInFile, "%d\n%s%s\n", howManyAnswer, newQuestionLine, modifyDate.date);
                    
                    struct data newAnswer;
                    for (int i = 0; i < howManyAnswer; i++) {
                        
                        int j = 0;
                        while (fgets(newAnswer.answer[i], 200, stdin) && j < 1) {
                            printf("%d. valasz: \n", i+1);
                            j += 2;
                        }
                        fprintf(tmpInFile, "%s", newAnswer.answer[i]);
                    }
                    fprintf(tmpInFile, "\n");
                    
                }//end if (scanID[i] == getID)
                else {
                    fprintf(tmpInFile, "%d\n%d", scanID[i], scanHowManyQ[i]);
                    
                    int j = 0;
                    while (fgets(lineArray, 300, inFile) && j < scanHowManyQ[i] + 3) {
                        fprintf(tmpInFile, "%s", lineArray);
                        j++;
                    }
                    fprintf(tmpInFile, "\n");
                }
            }//end for (int i = 0; i < lines; i++)
            
            fclose(tmpInFile);
            fclose(inFile);
            
            remove("input.txt");
            rename("tmpInFile.txt", "input.txt");
            remove("tmpInFile.txt");
            
            printf("A kerdes es a valaszok sikeresen modositva!\n");
            
        }//end if (lines > 0 && hit > 0)
        
        
    } else {
        printf("A fajl nem talalhato\n");
    }
    
    
    printf("1. Uj kerdes modositasa.\n");
    printf("2. Vissza a menube\n");
    printf("3. Kilepes.\n");
    printf("Valasztas: \n");
    
    int choice;
    do{
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                modifyQuestions();
                break;
            case 2:
                menu();
                break;
            case 3:
                exitProgram();
                break;
            default:
                printf("Helytelen menupont. Kerem adjon meg valid menupontot. (1-3): \n");
        }
    }while(choice < 1 || choice > 3);
    
}

void deleteQuestions(){
    
    printf("Kerdes torlese\n");
    
    FILE *idFile;
    idFile = fopen("id.txt", "r");
    
    if (idFile != NULL) {
        
        char lineArray[256];
        int idArray[20];
        int idChechArray[20];
        int qArray[20];
        fclose(idFile);
        
        int lines = 0;
        countLines(&lines);
        
        int hit = 0;
        int getID = 0;
        
        if (lines == 0) {
            printf("A kerdosor ures. A torles nem lehetseges\n ");
        } else if (lines > 0){
            FILE *idFile;
            idFile = fopen("id.txt", "r");
            printf("Adja meg a torolni kivant kerdes azonositojat (ID): \n");
            scanf("%d", &getID);
            
            
            for (int i = 0; i < lines; i++) {
                int idk;
                fscanf(idFile, "%d", &idk);
                idChechArray[i] = idk;
                
                if (idChechArray[i] == getID) {
                    hit++;
                }
            }
            
            if (hit == 0) {
                printf("Nincs ilyen azonosito\n");
            }
            fclose(idFile);
        }
        
        if(lines > 0 && hit > 0){
            
            FILE *infile;
            FILE *tmpInput;
            FILE *idFile;
            FILE *tmpID;
            infile = fopen("input.txt", "r");
            tmpInput = fopen("tmpInput.txt", "a");
            idFile = fopen("id.txt", "r");
            tmpID = fopen("tmpID.txt", "a");
            
            for (int i = 0; i < lines; i++) {
                
                int ids = 0;
                fscanf(infile, "%d", &ids);
                idArray[i] = ids;
                
                int questions;
                fscanf(infile, "%d", &questions);
                qArray[i] = questions;
                
                if (idArray[i] == getID) {
                    
                    int j = 0;
                    while (fgets(lineArray, 256, infile) && j < qArray[i] + 3) {
                        j++;
                    }
                } else {
                    fprintf(tmpInput, "%d\n%d", idArray[i], qArray[i]);
                    fprintf(tmpID, "%d\n", idArray[i]);
                    
                    int j = 0;
                    while (fgets(lineArray, 256, infile) && j < qArray[i] + 3) {
                        fprintf(tmpInput, "%s", lineArray);
                        j++;
                    }
                    fprintf(tmpInput, "\n");
                }//end else
                
            }//end for loop*/
            printf("A kerdessor torlese sikeresen megtortent\n");
            
            fclose(infile);
            fclose(tmpInput);
            fclose(idFile);
            fclose(tmpID);
            
            remove("input.txt");
            rename("tmpInput.txt", "input.txt");
            remove("tmpInput.txt");
            
            remove("id.txt");
            rename("tmpID.txt", "id.txt");
            remove("tmpID.txt");
            
        }

    } else {
        printf("A falj nem talalhato\n");
    }
    
    
    printf("1. Vissza a menube\n");
    printf("2. Uj kerdes torlese\n");
    printf("3. Kilepes\n");
    
    printf("Valasztas: \n");
    int choice;
    
    do{
        scanf("%d", &choice);
        if (choice == 1) {
            menu();
        }else if(choice == 2){
            deleteQuestions();
        }else if(choice == 3){
            exitProgram();
        } else {
            printf("Hibas menupont. Kerem valid menupontot adjon meg: \n");
        }
    }while (choice < 1 || choice > 3);
    
}


void listItems(){

    FILE *infile;
    FILE *idFile;
    infile = fopen("input.txt", "r");
    idFile = fopen("id.txt", "r");
    
    if (infile != NULL && idFile != NULL) {
        
        int lines = 0;
        countLines(&lines);
        
        if (lines == 0) {
            printf("A fajl nem tartalmaz adatokat. A listazas nem lehetseges. Elobb adjon meg adatokat az 1. menupontban.\n");
        } else {
            
            char line[256];
            int idArray[20];
            int qArray[20];
            
            for (int i = 0; i < lines; i++) {
                int ids = 0;
                fscanf(infile, "%d", &ids);
                idArray[i] = ids;
                printf("ID: %d", idArray[i]);
                
                int questions = 0;
                fscanf(infile, "%d", &questions);
                qArray[i] = questions;
                
                int j = 0;
                while (fgets(line, 256, infile) && j < qArray[i] + 3) {
                    printf("%s", line);
                    j++;
                }
                printf("\n");
            }
            
        }
        fclose(infile);
        fclose(idFile);
    } else {
        printf("A fajl nem talalhato.\n");
    }
    
    

    printf("\n1. Vissza a menube\n");
    printf("2. Kilepes\n");
    int choice;
    printf("Valasztas: \n");
    
    do{
        scanf("%d", &choice);
        if (choice == 1) {
            menu();
        } else if(choice == 2){
            exitProgram();
        } else {
            printf("Hibas menupont. Kerem valid menupontot adjon meg: \n");
        }
    }while (choice < 1 || choice > 2);
    
}




void deleteAllItems(){
    
    FILE *infile;
    infile = fopen("input.txt", "r");
    FILE *idFile;
    idFile = fopen("id.txt", "r");
    
    if (NULL != infile || NULL != idFile) {
        fseek(infile, 0, SEEK_END);
        fseek(idFile, 0, SEEK_END);
        long size = ftell(infile);
        long sizeid = ftell(idFile);
        if (size == 0 && sizeid == 0) {
            printf("A fajlok uresek. A torles nem lehetseges\n");
        } else {
            infile = fopen("input.txt", "w");
            idFile = fopen("id.txt", "w");
            printf("Az adatok sikeresen torolve lettek a fajlbol\n");
        }
    }
    
    fclose(infile);
    fclose(idFile);
    
    printf("Szeretni vissza lepni a menube?\n");
    printf("1. Igen\n");
    printf("2. Nem\n");
    printf("Valasztas: \n");
    int choice;
    
    do{
        scanf("%d", &choice);
        if (choice == 1) {
            menu();
        } else if (choice == 2){
            exitProgram();
        }
        if (choice < 1 || choice > 2) {
            printf("A megadaott valasztasi lehetoseg: %d nem megfelelo. Kerem adjon meg valid valaszt (1-2) \n", choice);
        }
    } while(choice < 1 || choice > 2);
    
}

void exitProgram(){
    
    printf("Biztos ki kivan lepni a programbol? \n");
    printf("1. Igen \n");
    printf("2. Nem \n");
    printf("Valasztas: \n");
    int choice;
    do{
        scanf("%d", &choice);
        if (choice == 1) {
            printf("A program futasa leall\n");
            exit(1);
        } else if (choice == 2){
            printf("Visszalepes a menube\n\n");
            menu();
        }
        if (choice < 1 || choice > 2) {
            printf("A megadaott valasztasi lehetoseg: %d nem megfelelo. Kerem adjon meg valid valaszt (1-2) \n", choice);
        }
    } while(choice < 1 || choice > 2);
}

void countLines(int *lines){
    FILE *lineCheck;
    lineCheck = fopen("id.txt", "r");
    
    while (!feof(lineCheck)) {
        int ch = fgetc(lineCheck);
        if (ch == '\n') {
            *lines += 1;
        }
    }
    fclose(lineCheck);
}

char* timeStamp()
{
    time_t t = time(NULL);
    struct tm *stt;
    int const dateLen = strlen("xxxx.xx.xx xx:xx:xx") + 1;
    const char* const timeFormat = "%Y.%m.%d %H:%M:%S";
    char* x = (char*)malloc(sizeof(char) * dateLen);
    
    stt = localtime(&t);
    strftime(x, dateLen, timeFormat, stt);
    
    return x;
}
