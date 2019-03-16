#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Question{
	char* que;
	char** answers;
	int ansc;
	int final;
};

char* gl(void){
    char *line = malloc(100), *linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}

void printmenu(){
	//printf("\033[2J\033[1;1H");
	printf("\nNyomja le a megfelelo szamokat majd nyomjon enter-t\n1. uj kerdesek hozzaadasa\n2. kerdesek modositasa\n3. kerdesek torlese\n4. kerdesek listazasa\n5. kerdesek veglegesitese\n0. Veglegesitett kerdesek fajlba irasa es kilepes\n");
};


void amodify(struct Question* ques, int qc){

	
};

void aadd(char** at){
	printf("alma");
	
};

void adelete(struct Question* ques, int qc){

	
};

void qlist(struct Question* ques, int qc){
	//printf("\033[2J\033[1;1H");
	int i=0;
	int q=1;
	for(;i<qc;i++){
		if(ques[i].final>0){
			printf("%d.kerdes: %s?\n",q, ques[i].que);
			int a=0;
			for(;a<ques[i].ansc;a++){
				printf("-%d.valasz: %s.\n",a+1, ques[i].answers[a]);
			}
			q++;
		}
	}
};

void qmodify(struct Question* ques, int qc){
	qlist(ques, qc);
	

	
};

void qdelete(struct Question* ques, int qc){
	qlist(ques, qc);
	

	
};

void qadd(struct Question* ques, int qc){
	//printf("\033[2J\033[1;1H");
	qlist(ques, qc);
	printf("\nAdja meg a listahoz adando kerdest: ");
	char *be=malloc(255);
	scanf("%s", &be);
	char **at;
	int i=1;
	while(i<5&&i){
		aadd(at);
		i++;
	}
	struct Question qu = { .que=be, .answers=at, .ansc=i, .final=0 };
	ques[qc]=qu;
	qc++;
};

void qfinalize(struct Question* ques, int qc){
	qlist(ques, qc);
	int i=0;
	printf("\nAdja meg a veglegesitendo kerdes szamat: ");
	scanf("%d",&i);
	while(i>0){
		ques[i-1].final=1;
		scanf("%d",&i);
	}
};

void pfin(struct Question* ques, int qc){
	FILE *f = fopen("kerdoiv.txt", "w");
	int i=0;
	int q=1;
	for(;i<qc;i++){
		if(ques[i].final>0){
			fprintf(f,"%d.kerdes: %s?\n",q, ques[i].que);
			int a=0;
			for(;a<ques[i].ansc;a++){
				fprintf(f,"-%d.valasz: %s.\n",a+1, ques[i].answers[a]);
			}
			q++;
		}
	}
	fclose(f);
};

int main(){
	//char alma=NULL;
	int menu=1;
	int qc=0;
	struct Question* ques;
			
	while(menu>0){
		printmenu();
		usleep(100);
		scanf("%d",&menu);
		//getline(&alma, (size_t*)255, stdin);
		
		usleep(100);
		//menu=getchar();
		switch(menu){
		case 1:
			qadd(ques, qc);

			break;
		case 2:

			break;
		case 3:

			break;
		case 4:

			qlist(ques, qc);
			break;
		case 5:

			qfinalize(ques, qc);
			break;
		case 0:
			//menu=0;
			pfin(ques, qc);
			break;	
		default:
			printf("\nNem valid opcio!");
		}
		usleep(100);
	}
	
}