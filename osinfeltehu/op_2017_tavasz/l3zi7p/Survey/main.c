#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#include "survey.h"

typedef struct Application Application;

struct Application {
  int running;
  int interfacemode; //edit or show mode
  Survey* s;
};

void read_command(Application* a);

void A_run(Application* a){
  a->running = 1;
  a->interfacemode = 1; //edit mode

  Survey surv;
  a->s = &surv;
  S_init(a->s);
  S_read(a->s);


  while (a->running){
      printf("\033[2J\033[1;1H");
      if (a->interfacemode == 0) {
        printf("=Edit=view===========================\n");
        S_print(a->s);
        printf("[ ]----------------------------------\n");
        printf("\n");
        printf("  (%i). [New question]\n", a->s->length + 1);

        printf("=====================================\n");
        printf("  show - Show final survey\n");
        printf("  [question index] [q | a | b | c | d] [text] - Edit question or it's options\n");
        printf("  delete [question index] - Delete question\n");
        printf("  finalize [question index] - Finalize question\n");
        printf("  exit - Exit\n");
        printf("=====================================\n");
      } else {
        printf("=Final=view==========================\n");
        S_print_final(a->s);
        printf("=====================================\n");
        printf("  edit - Edit view\n");
        printf("  exit - Exit\n");
        printf("=====================================\n");
      }


      read_command(a);
  }
  S_write(a->s);
  S_free(a->s);
}

char * get_line(void) {
    char * line = malloc(100), * linep = line;
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

void read_command(Application* a){
  char * line = get_line();
  char command[10];
  sscanf(line,"%s",command);

  if (strcmp(command,"edit") == 0){
    a->interfacemode = 0;
  } else if (strcmp(command,"show") == 0){
    a->interfacemode = 1;
  } else if (strcmp(command,"delete") == 0){
    char command[10];
    int index;
    sscanf(line,"%s %d",command,&index);
    S_delete_nth_question(a->s,index);
  } else if (strcmp(command,"finalize") == 0){
    char command[10];
    int index;
    sscanf(line,"%s %d",command,&index);
    Q_finalize(S_get_nth_question(a->s,index));

  } else if (strcmp(command,"exit") == 0){
    a->running = 0;
    printf("\033[2J\033[1;1H");
  } else {
    int index;
    char option;
    char text[100];
    sscanf(line,"%d %c %[^\t\n]",&index,&option,text);
    char entry[2] = "\0";
    entry[0] = option;
    S_edit_nth_question(a->s,index,entry,text);
  }
}




int main(){
  Application a;
  A_run(&a);
  return 0;
}
