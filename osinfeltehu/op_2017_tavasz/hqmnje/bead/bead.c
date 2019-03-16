/*Kérdéssor
Legalább 2 válaszlehetőség egy kérdésnél,
Maximum 4.

Funkciók:
	- Új kérdés felvétele
		* Automatikusan felveszi a kérés felvételi dátumát (év, hó, nap, óra, perc, másodperc)
	- Kérdést módosítani
		* Kínálja fel, melyik kérdést szeretné módosítani a felhasználó
		(Kérdést és válaszokat is jelenítse meg)
	- Kérdést törölni
		* Kínálja fel, melyik kérdést szeretné törölni a felhasználó
		(Kérdést és válaszokat is jelenítse meg)
	- Kérdések, válaszok listázása
	- Kérdések véglegesítés
		* Kiírjuk a kérdéseket, hozzátartozó válaszlehetőségeket, majd
		  megkérjük, hogy válassza ki azokat a kérdéseket, amik szerepeljenek a végső konzultációs kérdőíven.
*/

/*
	EXIT
	10 : Nincs megnyitható file
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define _GNU_SOURCE
#define fileName "questions.txt";
#define MAX_ANSWER 15;

typedef struct Form{
	int _n;								//Kérdések száma
	Question* _questions;				//Kérdések	
}
typedef struct Question{
	int _numberOfAnswers;
	char* _question;					//Kérdés
	char* _answers[MAX_ANSWER];			//Válaszok
}

void menu();
Form readFileByLines(){
	Form input;
	input->_questions->numberOfAnswers = 0;
	FILE *f;
	char *line = null;
	size_t len = 0;
	ssize_t read;
	fp = fopen(fileName, "r");
	if(fp == NULL){
		exit(10);
	}
	while((read = getline(&line, &len, fp)) != -1){	//Soronként olvasunk be, mert egy sorba tároljuk a kérdést és a hozzá tartozó válaszokat szőközzel elválasztva
		int k = 0;		//A válaszok és a kérdés maga is nem tartalmaz szóközt, hanem _ jelet csak.
		char **result = str_split(line," ");
		input->_questions[k]->_question = result[0];
		for(int i=1;i<sizeof(result)/sizeof(char*); i++){
			input->_questions[k]->_answers[i-1] = result[i];
			input->_questions->numberOfAnswers++;
		}
		k++;
	}
	input->_n = k;
	return input;
}

void newQuestion();
void changeQuestion();
void deleteQuestion();
void showQuestion();
void submitQuestions();
char** str_split(char*, const char);


int main(int argc, char *argv[]){
	menu();
	return 0;
}

void menu(){
	char *menu;
	system("clear");
	printf("----------------------------------------------------\n");
	printf("Kerdessor szerkeszto\n");
	printf("----------------------------------------------------\n");
	printf("Valasszon az alabbi menupontok kozul:\n");
	printf("\t(1) Uj kerdes felvetele\n");
	printf("\t(2) Meglevo kerdes modositasa\n");
	printf("\t(3) Meglevo kerdes torlese\n");
	printf("\t(4) Meglevo kerdesek es valaszok listazasa\n");
	printf("\t(5) Kerdesek veglegesitese\n");
	printf("\t(0) Kilepes\n");
	printf("----------------------------------------------------\n");
	printf("Menu: ");
	scanf("%s",menu)
	system("clear");
	if(strcmp(menu,"1")){
		newQuestion();
	}else if(strcmp(menu,"2"){
		changeQuestion();
	}else if(strcmp(menu,"3"){
		deleteQuestion();
	}else if(strcmp(menu,"4"){
		showQuestion();
	}else if(strcmp(menu,"5"){
		submitQuestions();
	}else if(strcmp(menu,"0"){
		exit();
	}else{
		printf("Nincs ilyen menupont")
	}
}

void newQuestion(){
	FILE *questions;
	questions = fopen(fileName);
	char *quest;
	printf("Kerlek fogalmazd meg a kerdest ugy, hogy szokoz helyett _ (alulvonas) jelet használsz.\n");
	scanf("%s",quest);
	system("clear");
	printf("A kerdes amit megadtal:\n");
	char **result = str_split(quest,"_");
	for(int i=0;i<sizeof(result)/sizeof(char*); i++){
		printf("%s ",result[i]);
	}
	printf("\nAdd meg, hogy hany db valaszlehetoseget szeretnel hozzaadni a kerdeshez:\n");
	int numberOfAnswers;
	char **answers;
	scanf("%d",numberOfAnswers);
	answers = malloc(sizeof(char*)*numberOfAnswers);
	for(int i=0;i<numberOfAnswers;i++){
		printf("%d. Valasz",i);
		scanf("%s",answers[i]);
	}
	
}
void changeQuestion(){
	
}
void deleteQuestion(){
	
}
void showQuestion(){
	Form input = readFileByLines();
	for(int i=0;i<input->_n;i++){
		printf("%d. kerdes:\n%s\n",i,input->_questions->_question);
		for(int j=0;j<input->_questions->numberOfAnswers;j++){
			printf("\t%d. valasz: %s\n",j,input->_questions->_answers[j]);
		}
	}
}
void submitQuestions(){
	printf("A kerdessorod keszen van, megtalalod a %s fileban",fileName);
}

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}