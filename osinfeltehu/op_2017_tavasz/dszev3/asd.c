#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

int maxBuffer = 30;

typedef struct question_struct
{
        char* the_question;
        char* date;
        char** answers;
        int numberOfAnswers;
  struct question_struct* next;
} question;

question* first = NULL;

question CrsdeateQuestion()
{
        char* tmp;
        printf("Please insert the question!\n");
        scanf("%s",tmp);
        printf("Thanks brah\n %s \n", tmp);
}

int ListQuestions()
{
                int index = 0;
                int jndex;

    question *tmp = first;

    while(tmp)
    {
        printf("%d. %s\n", index, tmp->the_question);

                                for(jndex = 0; jndex < tmp->numberOfAnswers; jndex++)
                                {
                                                printf("\t%s\n", tmp->answers[jndex]);
                                }

                                index++;
                                tmp = tmp->next;
                }

                return index -1;
}

char* readFunc()
{
        int max = maxBuffer;
        char* data = (char*) malloc(max);


  while (1) { /* skip leading whitespace */
      int c = getchar();
      if (c == EOF) break; /* end of file */
      if (!isspace(c)) {
           ungetc(c, stdin);
           break;
      }
  }

  int i = 0;
  while (1) {
      int c = getchar();
      if ((isspace(c) || c == EOF) && c != ' ') { /* at end, add terminating zero */
          data[i] = 0;
          break;
      }
      data[i] = c;
      if (i == max - 1) { /* buffer full */
          max += max;
          data = (char*) realloc(data, max); /* get a new and larger buffer */
      }
      i++;
  }

        return data;
}

void CreateQuestion()
{
                int answer_num;
                int index;
                question* current = malloc(sizeof(question));

                printf("Please enter the question!\n");
                current->the_question = readFunc();

                printf("Please enter the number of the answers (2-4)!\n");
                scanf("%d", &answer_num);

                if(!(answer_num >= 2 && answer_num <=4))
                {
                        printf("\nNot valid number, number of answers will be 2!\n");
                        answer_num = 2;
                }

                current->answers = malloc(sizeof(char*)*answer_num);

                for (index = 0; index < answer_num; index++)
                {
                        printf("Please insert the %d. asnwer!\n",index+1);
                        current->answers[index] = readFunc();
                }

                current->numberOfAnswers = answer_num;

                time_t t = time(NULL);
                current->date = asctime(localtime(&t));

                current->next = NULL;

    if(first)
    {
        question *tmp = first;

        while(tmp && tmp->next)
        {
                tmp = tmp->next;
        }

                                tmp->next = current;
    }
    else
    {
                        first = current;
    }
}

void DeleteQuestion()
{
        int questionNum = ListQuestions();
        int numberOfAnswers;
        int selectedQuestion;
        int selectedAnswer;

        if(questionNum < 0)
        {
                printf("0. question currently\n\n");
                return;
        }

        printf("Which question or it's answer would you like to delete?\n");
        scanf("%d", &selectedQuestion);

        if(!(selectedQuestion >= 0 && selectedQuestion <= questionNum))
        {
                printf("\nNot a valid number");
                return;
        }

        int helper = 0;
        question* tmp = first;

        while(tmp && helper != selectedQuestion)
        {
                helper++;
                tmp = tmp->next;
        }

        if(helper == selectedQuestion && tmp)
        {
                int i = 0;

                printf("%d. %s\n", i, tmp->the_question);

                for(i = 0; i < tmp->numberOfAnswers; i++)
                {
                        printf("\t%d. %s\n", i+1, tmp->answers[i]);
                }

                printf("Select a number that you want to delete?\n");
                scanf("%d", &selectedAnswer);

                if(selectedAnswer < 0 || selectedAnswer > i)
                {
                        printf("\nNot a valid number");
                        return;
                }

                if(selectedAnswer == 0)
                {
                        if(selectedQuestion == 0)
                        {
                                        first = first->next;
                                        return;
                        }

                        int new_helper = 0;
                        question* new_tmp = first;

                        while(new_tmp && new_helper != selectedQuestion-1)
                        {
                                new_helper++;
                                new_tmp = new_tmp->next;
                        }

                        if(new_tmp->next)
                        {
                                new_tmp->next = new_tmp->next->next;
                        }

                        return;
                }

                if(i <= 2)
                {
                        printf("\nA question must have at least two answers!\n");
                        return;
                }

                int j;
                for(j = selectedAnswer-1; j < tmp->numberOfAnswers-1; j++)
                {
                        tmp->answers[j] = tmp->answers[j+1];
                }

                tmp->answers[tmp->numberOfAnswers] = NULL;
                tmp->numberOfAnswers--;
        }
}

void ModifyQuestion()
{
        int questionNum = ListQuestions();
        int numberOfAnswers;
        int selectedQuestion;
        int selectedAnswer;

        if(questionNum < 0)
        {
                printf("0. question currently\n\n");
                return;
        }

        printf("Which question or it's answer would you like to modify?\n");
        scanf("%d", &selectedQuestion);

        if(!(selectedQuestion >= 0 && selectedQuestion <= questionNum))
        {
                printf("\nNot a valid number");
                return;
        }

        int helper = 0;
        question* tmp = first;

        while(tmp && helper != selectedQuestion)
        {
                helper++;
                tmp = tmp->next;
        }

        if(helper == selectedQuestion && tmp)
        {
                int i = 0;

                printf("%d. %s\n", i, tmp->the_question);

                for(i = 0; i < tmp->numberOfAnswers; i++)
                {
                        printf("\t%d. %s\n", i+1, tmp->answers[i]);
                }

                printf("\t%d. %s\n", i+1, "Add new answer!");

                printf("Select a number!\n");
                scanf("%d", &selectedAnswer);

                if(selectedAnswer < 0 || selectedAnswer > i+1)
                {
                        printf("\nNot a valid number");
                        return;
                }

                if(selectedAnswer == 0)
                {
                        printf("\nInsert modified question!\n");

                        tmp->the_question = readFunc();

                        return;
                }

                if(selectedAnswer == i+1)
                {
                        if(tmp->numberOfAnswers >= 4)
                        {
                                printf("\nA question must have less then or equal 4 answers\n");
                                return;
                        }
                        tmp->numberOfAnswers++;
                        tmp->answers = realloc(tmp->answers,sizeof(char*)*tmp->numberOfAnswers);

                        printf("\nInsert new answer!\n");

                        tmp->answers[tmp->numberOfAnswers-1] = readFunc();

                        return;
                }

                printf("\nInsert modified answer!\n");

                tmp->answers[selectedAnswer-1] = readFunc();
        }
}

void Finish()
{
        FILE *f = fopen("quesitons.txt", "w");
        if (f == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }

        question *tmp = first;

        while(tmp)
        {
                fprintf(f, "%s\t%s\n", tmp->the_question, tmp->date);

                int i;

                for (i = 0; i < tmp->numberOfAnswers; i++) {
                        fprintf(f, "\t%s\n", tmp->answers[i]);
                }

                fprintf(f, "\n");
                tmp = tmp->next;
        }

        fclose(f);
}

int main(int argc, char** argv)
{
	char* a = malloc(12);
	char* b = malloc(12);
	
	scanf("%s",a);
	scanf("%s",b);
	
	printf("%s | %s",a,b);
}

