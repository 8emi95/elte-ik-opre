#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct Answer{
  int id;
  char *answer;
}Answer;

typedef struct Question{
  char *question;
  Answer *answers;
  time_t date;
}Question;

char *read_(FILE *fp, size_t size, size_t max){
  char *input;
  int ch;
  size_t len = 0;
  //input = realloc(NULL, sizeof(char)*size);
  input = malloc(sizeof(char)*size);
  if(!input) return input;
  
  while(EOF != (ch=fgetc(fp)) && ch != '\n'){
    input[len++] = ch;
    if(len == size){
		if(max == 0){
        	input = realloc(input, sizeof(char)*(size+=20));
		}
      if(!input) return input;
    }
  }
  
  input[len+2] = '\0';
  //printf("INPUT: %s\n", input);
  return realloc(input, sizeof(char)*len);
}

void main_menu(int menu){
  switch(menu){
    case(0):
      printf("1. Új kérdéssor készítése. \n");
      printf("2. Kérdéssor szerkesztése. \n");
      printf("3. Kérdéssor megtekintése. \n");
      printf("4. Kilépés. \n");
      break;
    case(1):
      printf("1. Új kérdés felvétele. \n");
      printf("2. Kérdéssor mentése. \n");
      printf("3. Vissza. \n");
      break;
    case(2):
      printf("1. Kérdés szerkesztése.\n");
      printf("2. Kérdés törlése.\n");
      printf("3. Válasz megadása kérdéshez\n");
      printf("4. Vissza. \n");
      break;
    case(3):
      printf("1. Kérdéssor szerkesztése. \n");
      printf("2. Vissza. \n");
	  break;
    default:
		break;
  }
}

Question* new_question(){
  Question *q;
  printf("Adjon meg egy kérdést: \n");	
	char *c = realloc(NULL, sizeof(char));
	c = read_(stdin, 200, 0);
	q->question = c;
  q->date = time(NULL);
	q->answers = malloc(sizeof(Answer)*4);	
  return q;
}

void save_question(Question *questionArray, int size){
  FILE *file;
  int i,j;
  char *printed_line;  
  printed_line = malloc(sizeof(char));

  //printf("Kérdés első eleme: %s\n", questionArray->question);
  printf("Adja meg a fájl nevét. \n");
  //char *fname = realloc(NULL, sizeof(char));
  char *fname/* = malloc(sizeof(char))*/;
  fname = read_(stdin, 200, 0);
  printf("Ide bemegy.\n");
  file = fopen(fname, "wt");

  for(i = 0; i < size; ++i){    
		if(i > 0){
			fprintf(file, "\n");
		}
		printf("Van kérdés. Size: %i, Question: %s\n", size, questionArray[i].question);
		printed_line = realloc(NULL, strlen(questionArray[i].question));
		snprintf(printed_line, strlen(questionArray[i].question), "%s", questionArray[i].question);
		fprintf(file, printed_line);   
  }
  free(printed_line);
  //free(fname);
  //free(questionArray);
  fclose(file);
}

Question* open_question(char *fname){
	FILE *file;	
	ssize_t read;
	size_t len = 200;
	char *line = malloc(len);
	int size = 0;
	Question *q = malloc(sizeof(Question));
	Question *temp = malloc(sizeof(Question));
	if(access(fname, F_OK) != -1){
		file = fopen(fname, "r");
		//q = realloc(NULL, sizeof(Question));
		while(!feof(file)){	
			if(fgets(line, len, file) != NULL){	
					Question *tempp = malloc(sizeof(q));
					tempp = realloc(q, 30);
					if(tempp == NULL){
						printf("De null.\n");
					}else{
						q = tempp;
					}
				temp = malloc(sizeof(Question));				
				temp->question = realloc(NULL, sizeof(line));			
				strcpy(temp->question, line);
				
				temp->answers = malloc(sizeof(Answer)*4);
				
				temp->date = time(NULL);
				
				q[size] = *temp;	
				
				if(temp != NULL){
					free(temp);		
					size++;
				}
				
				printf("Sor: %s\n", line);
				if(feof(file)){
					fclose(file);
					break;
				}
			free(tempp);
			}
		}
	}else{
		printf("Nincs ilyen fájl.\n");	
		//free(q);
		//free(line);
		return NULL;
	}
	//free(fname);
	//free(line);
	//fclose(file);
	//q++;
	//printf("Size: %i, Elso kerdes: %s\n",size, q[0].question);
	/*Question *tempp = malloc(sizeof(q));
	tempp = realloc(q, sizeof(q));
	if(tempp != NULL){
		printf("NEm nulla.\n");
		return realloc(tempp, sizeof(tempp));	
	}else{
		printf("De nulla.\n");
		return NULL;
	}*/
	return realloc(q, sizeof(Question)*10);	
}

char *edit_question(){
	printf("Adja meg az új kérdést. \n");
	char *asd = malloc(sizeof(char));
	asd = read_(stdin, sizeof(char), 0);
	return realloc(asd, strlen(asd));
}

int main(){
  Question *d;
  Question *edit;
  int decision = 1;
  int size = 0;
  int loop = 1;
  //Question questions[4];
  Question *questions = malloc(sizeof(Question));
  //Question questions[4];
  while(decision != 4){
	  MAINMENU: main_menu(0);
	  decision =(int) *read_(stdin, sizeof(int), 1);
      switch(decision){
        case('1'):
		 SUBMENU: main_menu(1);
		  decision = (int) *read_(stdin, sizeof(int), 1);
          switch(decision){
             case('1'):                           
			   d = realloc(NULL,sizeof(Question));
			   d = new_question(size);			   
			   Question *temp = malloc(sizeof(questions));
			   temp = realloc(questions, sizeof(questions)+sizeof(Question));
			   /*if(size > 0){

				   questions = realloc(questions, sizeof(questions) + sizeof(Question));
			   }*/
				if(temp != NULL){
					questions = temp;
				}
               questions[size] = *d;	
			   if(d != NULL){
				   size++;
			   }		   
			   goto SUBMENU;
               break;
	          case('2'):
			    save_question(questions, size);
				goto MAINMENU;
	            break;
             case('3'):			 
             goto MAINMENU;
               break;
             default:
			 	printf("Hibás menüpont. \n");
				goto SUBMENU;
               break;
          }
	
	  break;
	case('2'):
	  	printf("Adja meg a fájl nevét. \n");
		char *fname = malloc(sizeof(char));
		fname = read_(stdin, sizeof(char), 0);
		free(questions);
		Question *questions = realloc(NULL, sizeof(Question));
		questions = open_question(fname);
		printf("Itt mivan. \n");
		if(questions == NULL){
			goto MAINMENU;
		}
	  	SUBMENU2: main_menu(2);
	  	decision = (int) *read_(stdin, sizeof(char), 1);
		switch(decision){
			case('1'):
				for(loop = 1; questions->question != NULL; loop++/*, ++questions*/){
					//printf("%i. %s \n", loop, questions->question);
				}
				printf("Adja meg melyik kérdést akarja szerkeszteni. \n");
				char *char_ = malloc(sizeof(char));
				char_ =  read_s(stdin, sizeof(char), 1);
				int q = (int) *char_ - '0';
				edit = realloc(NULL, sizeof(Question));
				edit = new_question();
				questions[q].question = edit->question;
				printf("Ezen átjut: %s\n", questions[q].question);				
				save_question(questions, loop-1);
				break;
			case('2'):
				break;
			case('3'):
				break;
			case('4'):
				goto MAINMENU;
				break;
		}
	  break;
 	case('3'):
	  printf("Main 3.");
	  break;
	case('4'):
	  decision = 4;
	  break;
	default:
          printf("Nincs ilyen menüpont.\n");
	  break;
      }
    
  }
  free(questions);
  free(d);
  free(edit);
	printf("Vége a programnak.\n");
  return 0;
}
