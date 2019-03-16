#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

struct QuestionObject {
	int id;
	char timestamp[128];
	char question[1024];
	char answer1[1024];
	char answer2[1024];
	char answer3[1024];
	char answer4[1024];
};

int countLines(bool isFinalFile){
	FILE * data = NULL;
	if(isFinalFile){
		data = fopen("final.txt", "a");
	} else {
		data = fopen("data.dat", "r");
	}
	int lines=0;
	if(data == NULL && !isFinalFile){
		printf("Figyelmeztetes: nem lett meg letrehozva adatfajl. Adatfajl letrehozasa...\n");
		data=fopen("data.dat", "w");
		if(data == NULL){
			printf("Hiba: nem lehet letrehozni az adatfajlt.\n");
			return;
		} else {
			printf("Adatfajl letrehozva.\n");
		}
	}
	char line[1024];
	while(true){
		if(fscanf(data, " %[^\t\n]s\n", line)!=EOF){
			lines++;
		} else {
			break;
		}
	}
	fclose(data);
	//printf("Lines: %d\n",lines);
	if(lines==0 && !isFinalFile){
		printf("Nem talalhato felvett kerdes.\n\n");
	}else{		
		printf("\n");
	}
	return lines;
}

void finalize(struct QuestionObject qObj){
	char answer;
	printf("\nSzerepeljen a vegleges kerdoivben? (y/n) ");
	scanf(" %c", &answer);
	switch(answer){
		case 'y': case 'Y':
			break;
		default: 
			printf("Nem elfogadott karakter. A kerdes nem fog szerepelni a vegleges kerdoivben (a veglegesites ujra lefuttathato).\n");
			return;
		case 'n': case 'N':
			return;
	}
	FILE * data = NULL;
	data = fopen("final.txt", "a");
	if(data == NULL){
		printf("Hiba: nem lehet megnyitni a szovegfajlt.");
		return;
	}
	
	fprintf(data, "Kerdes: %s\n", qObj.question);
	fprintf(data, "1. valasz: %s\n", qObj.answer1);
	fprintf(data, "2. valasz: %s\n", qObj.answer2);
	if(qObj.answer3[0]!='\0' && qObj.answer3[0]!='-'){
		fprintf(data, "3. valasz: %s\n", qObj.answer3);
		if(qObj.answer4[0]!='\0' && qObj.answer4[0]!='-'){
			fprintf(data, "4. valasz: %s\n", qObj.answer4);
		}
	}
	fprintf(data, "\n");
	fclose(data);
}

struct QuestionObject * listQuestions(bool writeToConsole, bool _finalize){
	FILE * data = NULL;

	int sumNumOfLines = countLines(false);
	data = fopen("data.dat", "r");
	struct QuestionObject qObjArr[sumNumOfLines/7];
	
	int i=0;
	while(i<sumNumOfLines/7){
		int id;
		fscanf(data, " %d\n",&qObjArr[i].id);
		fscanf(data, " %[^\t\n]s\n", &qObjArr[i].timestamp);
		fscanf(data, " %[^\t\n]s\n", &qObjArr[i].question);
		fscanf(data, " %[^\t\n]s\n", &qObjArr[i].answer1);
		fscanf(data, " %[^\t\n]s\n", &qObjArr[i].answer2);
		fscanf(data, " %[^\t\n]s\n", &qObjArr[i].answer3);
		fscanf(data, " %[^\t\n]s\n", &qObjArr[i].answer4);
		i++;
	}
	i=0;
	
	if(_finalize){
		if(countLines(true)>0){
			//printf("%d\n", countLines(true));
			remove("final.txt");
			FILE * final = NULL;
			final=fopen("final.txt", "a");
			if(final == NULL){
				printf("Nem lehet megnyitni a veglegesitendo szovegfajlt.\n");
			}
			fclose(final);
		}
	}
	while(i<sumNumOfLines/7 && writeToConsole){
		
		printf("Azonosito: %d\n",qObjArr[i].id);
		printf("Felvetel ideje: %s\n",qObjArr[i].timestamp);
		printf("Kerdes: %s\n",qObjArr[i].question);
		printf("Valaszlehetosegek:\n");
		printf("1.: %s\n",qObjArr[i].answer1);
		printf("2.: %s\n",qObjArr[i].answer2);
		if(qObjArr[i].answer3[0] == '-' || qObjArr[i].answer3[0] == '\0'){
			printf("3.: <Nincs megadva>\n");
		} else {
			printf("3.: %s\n",qObjArr[i].answer3);
		}
		if(qObjArr[i].answer4[0] == '-' || qObjArr[i].answer4[0] == '\0'){
			printf("4.: <Nincs megadva>\n");
		} else {
			printf("4.: %s\n",qObjArr[i].answer4);
		}
		printf("---------\n");
		if(_finalize){
			struct QuestionObject qObj = qObjArr[i];
			finalize(qObj);
		}
		i++;
	}
	if(_finalize){
		printf("Vegleges kerdoiv exportalva a 'final.txt' fajlba.\n\n");
	}
	struct QuestionObject * ptr = qObjArr;
	fclose(data);
	return ptr;
}

void newQuestion(){
	FILE * data = NULL;
	data = fopen("data.dat", "a");
	if(data == NULL){
		printf("Hiba: nem lehet megnyitni az adatokat tartalmazo fajlt!");
		return;
	}

	char line[1024];
	bool exit = false;
	while(!exit){
		struct QuestionObject qObj;
		int answer_count = 0;
		printf("Adja meg a kerdest!\n");
		scanf(" %[^\t\n]s",&line);
		//printf("A megadott kerdes: %s\n",line);
		int id;
		srand(time(NULL));
		id = (rand() % (999999 - 100000)) + 100000;
		qObj.id = id;
		char text[128];
		time_t now = time(NULL);
		struct tm *t = localtime(&now);
		strftime(text, sizeof(text)-1, "%Y-%m-%d %H:%M", t);
		strcpy(qObj.timestamp, text);
		strcpy(qObj.question, line);

		while(true){
			if(answer_count>=2){
				char answer;
				if(answer_count >=4){
					printf("Elerte a maximalisan megadthato valaszlehetosegek szamat. Kerdes es valaszok mentese...\n\n");
					//todo save prompt
					answer='n';
				}else{
					printf("Akar meg valaszlehetoseget megadni? (%d/4) (y/n) ", answer_count+1);
					scanf(" %c",&answer);
				}
				switch(answer){		
					case 'y': case 'Y':
						break;
					default:
						printf("Nem elfogadott karakter. Valtoztatasok mentese. A kerdes modosithato lesz.\n");
					case 'n': case 'N':
						fprintf(data, "%d\n", qObj.id);
						fprintf(data, "%s\n", qObj.timestamp);
						fprintf(data, "%s\n", qObj.question);
						fprintf(data, "%s\n", qObj.answer1);
						fprintf(data, "%s\n", qObj.answer2);
						if(qObj.answer3[0] == '\0' || qObj.answer3[0] == '\n'){
							strcpy(qObj.answer3, "-");
						}
						fprintf(data, "%s\n", qObj.answer3);
						if(qObj.answer4[0] == '\0' || qObj.answer4[0] == '\n'){
							strcpy(qObj.answer4, "-");
						}
						fprintf(data, "%s\n", qObj.answer4);
						fclose(data);
						return;
				}
			}
			printf("Adjon meg valaszlehetoseget (%d)\n",answer_count+1);
			scanf(" %[^\t\n]s",&line);
			switch(answer_count){
				case 0:
					strcpy(qObj.answer1, line);
					break;
				case 1:
					strcpy(qObj.answer2, line);
					break;
				case 2:
					strcpy(qObj.answer3, line);
					break;
				case 3:
					strcpy(qObj.answer4, line);
					break;
			}
			answer_count++;
		}
	}
	fclose(data);
}

void writeObjectsToFile(struct QuestionObject * qObjArr, int length){
	FILE * data = NULL;
	data = fopen("data.dat", "w+");
	if(data == NULL){
		printf("Hiba: nem lehet megnyitni az adatokat tartalmazo fajlt!");
		return;
	}
	int i;
	for(i=0; i<length; i++){
		if(qObjArr[i].id!=-1){
			fprintf(data, "%d\n", qObjArr[i].id);
			fprintf(data, "%s\n", qObjArr[i].timestamp);
			fprintf(data, "%s\n", qObjArr[i].question);
			fprintf(data, "%s\n", qObjArr[i].answer1);
			fprintf(data, "%s\n", qObjArr[i].answer2);
			if(qObjArr[i].answer4[0] == '\0' || qObjArr[i].answer4[0] == '\n'){
				strcpy(qObjArr[i].answer3, "-");
			}
			fprintf(data, "%s\n", qObjArr[i].answer3);
			if(qObjArr[i].answer4[0] == '\0' || qObjArr[i].answer4[0] == '\n'){
				strcpy(qObjArr[i].answer4, "-");
			}
			fprintf(data, "%s\n", qObjArr[i].answer4);
		}
	}
	fclose(data);	
}

void modifyQuestion(){
	struct QuestionObject * qObjArr = listQuestions(true, false);
	int sumNumOfLines = countLines(false);
	char line[1024];
	bool foundEntry = false;
	int i=0;

	int id;
	char str[64]= "";
	bool ok = false;
	
	printf("Adja meg a modositani kivant kerdes azonositojat!\n\nAzonosito: ");
	while(!ok){
		scanf(" %s", str);
		int length=strlen(str);
		bool strOk = true;
		for(i=0; i<length; i++){
			if(!isdigit(str[i]) || length!=6){
				printf("Hibas input!\n\nAzonosito: ");
				strOk=false;
				break;
			}
		}
		if(strOk){
			ok=true;
			id=atoi(str);
			//printf("%d", id);
		}
	}
	i=0;
	while(i<sumNumOfLines/7){
		if(qObjArr[i].id==id){
			foundEntry=true;
			printf("Kerdes (modositando): %s\n",qObjArr[i].question);
			printf("Adja meg a kerdes uj szoveget!\n");
			scanf(" %[^\t\n]s",&qObjArr[i].question);
			//printf("%s\n", qObjArr[i].question);
			//todo timestamp?
			printf("1. valasz (modositando): %s\n", qObjArr[i].answer1);
			printf("Adja meg az 1. valasz uj szoveget!\n");
			scanf(" %[^\t\n]s",&qObjArr[i].answer1);
			printf("2. valasz (modositando): %s\n", qObjArr[i].answer2);
			printf("Adja meg a 2. valasz uj szoveget!\n");
			scanf(" %[^\t\n]s",&qObjArr[i].answer2);
			bool skipA4 = false;
			if(qObjArr[i].answer3[0] == '-' || qObjArr[i].answer3[0] == '\0'){
				printf("Nem volt megadva 3. valasz. Szeretne felvenni? (y/n) ");
				//todo y/n scanf
				char answer;
				scanf(" %c",&answer);
				switch(answer){
					case 'y': case 'Y':
						printf("Adja meg a 3. valasz uj szoveget!\n");
						scanf(" %[^\t\n]s",&qObjArr[i].answer3);
						break;
					default:
						printf("Nem elfogadott karakter. Valtoztatasok mentese.\n");
					case 'n': case 'N': 
						skipA4=true;
						break;
				}
			} else {
				printf("3. valasz (modositando): %s\n", qObjArr[i].answer3);
				printf("Modositja, vagy torli a valaszlehetoseget? (m/d) ");
				char answer;
				scanf(" %c",&answer);
				switch(answer){
					case 'm': case 'M':
						printf("Adja meg a 3. valasz uj szoveget!\n");
						scanf(" %[^\t\n]s",&qObjArr[i].answer3);
						break;
					case 'd': case 'D':
						printf("\n");
						strcpy(qObjArr[i].answer3, "-");
						skipA4=true;
						printf("(Megjegyzes: mivel torolte a 3. valaszlehetoseget, nem vehet fel 4.-et.)");
						break;
				}
			}
			if(!skipA4){
				if(qObjArr[i].answer4[0] == '-' || qObjArr[i].answer4[0] == '\0'){
					printf("Nem volt megadva 4. valasz. Szeretne felvenni? (y/n)");
					//todo y/n scanf
					char answer;
					scanf(" %c",&answer);
					switch(answer){
						case 'y': case 'Y':
							printf("Adja meg a 4. valasz uj szoveget!\n");
							scanf(" %[^\t\n]s",&qObjArr[i].answer4);
							break;
						default:
							printf("Nem elfogadott karakter. Valtoztatasok mentese.\n");
						case 'n': case 'N': 
							skipA4=true;
							break;
					}
				} else {
					printf("4. valasz (modositando): %s\n", qObjArr[i].answer4);
					printf("Modositja, vagy torli a valaszlehetoseget? (m/d) ");
					char answer;
					scanf(" %c",&answer);
					switch(answer){
						case 'm': case 'M':
							printf("Adja meg a 4. valasz uj szoveget!\n");
							scanf(" %[^\t\n]s",&qObjArr[i].answer4);
							break;
						case 'd': case 'D':
							//printf("\n");
							strcpy(qObjArr[i].answer4, "-");
							break;
					}
				}
			}
			break;
		}
		i++;
	}
	if(!foundEntry){
		printf("Nem talalalhato kerdes ezzel az azonositoval: %d\n\n", id);
	}
	writeObjectsToFile(qObjArr,sumNumOfLines/7);
}

void deleteQuestion(){
	struct QuestionObject * qObjArr = listQuestions(true, false);
	int sumNumOfLines = countLines(false);
	bool foundEntry = false;
	int i=0;
	int id;
	char str[64]= "";
	bool ok = false;
	
	printf("Adja meg a torolni kivant kerdes azonositojat!\n\nAzonosito: ");
	while(!ok){
		scanf(" %s", str);
		int length=strlen(str);
		bool strOk = true;
		for(i=0; i<length; i++){
			if(!isdigit(str[i]) || length!=6){
				printf("Hibas input!\n\nAzonosito: ");
				strOk=false;
				break;
			}
		}
		if(strOk){
			ok=true;
			id=atoi(str);
			//printf("%d", id);
		}
	}
	i=0;
	while(i<sumNumOfLines/7){
		if(qObjArr[i].id==id){
			foundEntry=true;
			printf("A kivalasztott kerdes: %s\n", qObjArr[i].question);
			printf("Biztosan torli? (y/n) ");
			char answer;
			scanf(" %c",&answer);
			switch(answer){
				case 'y': case 'Y':
					qObjArr[i].id=-1; //"delete" flag
					break;
				default:
					printf("Nem elfogadott karakter. Torles megszakitva.\n");
				case 'n': case 'N': break;
			}
			break;
		}
		i++;
	}
	if(!foundEntry){
		printf("Nem talalalhato kerdes ezzel az azonositoval: %d\n\n", id);
	}
	writeObjectsToFile(qObjArr,sumNumOfLines/7);
}

int main(){
	bool exit = false;
	while(!exit){
		printf("%s", "Menu:\nn - Uj kerdes\nl - Kerdesek listazasa\nd - Kerdes torlese\nm - Kerdes modositasa\nf - Veglegesites\ne - Kilepes\n\nValasz: ");
		char answer;
		scanf(" %c",&answer);
		switch(answer){
			case 'n': case 'N':
				newQuestion();
				break;
			case 'l': case 'L':
				listQuestions(true,false);
				break;
			case 'd': case 'D':
				deleteQuestion();
				break;
			case 'm': case 'M':
				modifyQuestion();
				break;
			case 'f': case 'F':
				listQuestions(true,true);
				break;
			case 'e': case 'E':
				exit = true;
				break;
			default:
				printf("Hibas keres.\n");
				break;
		}
	}
	return 0;
}
