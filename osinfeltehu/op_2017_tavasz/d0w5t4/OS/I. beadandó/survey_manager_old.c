#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

int find(int* arr, size_t size, int elem)
{
  int i = 0;
  while (arr[i] != elem && i < size)
    i++;

  return i < size ? i : -1;
}

struct Question
{
  char question[64];

  char opts[4][64];
};

struct SurveyState
{
  FILE*           loaded_survey;
  char            loaded_survey_path[256];
  int             mod_size;
  int             del_size;
  int             new_size;

  int              mod_question_ids[16];
  struct Question* mod_questions   [16];
  int              del_question_ids[16];
  struct Question* new_questions   [16];
};
struct SurveyState state;

typedef enum {KEEP_FILE, DISCARD_FILE} reset_type;

void init_state(reset_type rt)
{
  if (rt == DISCARD_FILE)
  {
    state.loaded_survey         = NULL;
    state.loaded_survey_path[0] = '\0';
  }
  state.mod_size      = 0;
  state.del_size      = 0;
  state.new_size      = 0;

  for (int i = 0; i < 16; i++)
  {
    state.mod_questions[i] = state.new_questions[i] = NULL;
  }
}

void reset_state(reset_type rt)
{
  for (int i = 0; i < 16; i++)
  {
    free(state.mod_questions[i]);
    free(state.new_questions[i]);
  }
  if (rt == DISCARD_FILE && state.loaded_survey != NULL) fclose(state.loaded_survey);

  init_state(rt);
}

void discard_new()
{

}



void flush()
{
  int c;
  while ((c = fgetc(stdin)) != '\n' && c != EOF);
}

char* getline(size_t len)
{
    char* line  = malloc(len);
    char* start = line;
    int c;

    if(line == NULL)
        return NULL;


    for(;;)
    {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if((*line++ = c) == '\n')
        {
          *(line - 1) = '\0';
          break;
        }
        else if (--len == 1)
        {
          *line = '\0';
          flush();
          break;
        }
    }
    return start;
}

void create_survey()
{
  reset_state(DISCARD_FILE);

  printf("Name of the new survey:\n");
  char* file_name;
  file_name = getline(64);
  FILE* file = fopen(file_name, "w+");

  state.loaded_survey = file;
  strcpy(state.loaded_survey_path, file_name);

  free(file_name);
  printf("\n");
}

void load_survey(const char* prompt)
{
  reset_state(DISCARD_FILE);

  printf("%s\n", prompt);
  char* file_name;
  file_name = getline(64);
  FILE* file;
  while (!(file = fopen(file_name, "r+")))
  {
    printf("Invalid filename, please enter a new one:\n");
    file_name = getline(64);
  }

  state.loaded_survey = file;
  strcpy(state.loaded_survey_path, file_name);

  free(file_name);
  printf("\n");
}

void list_questions()
{
  FILE* file = state.loaded_survey;

  if (file == NULL)
  {
    printf("You must load an existing survey first!\n");
    return;
  }

  fseek(file, 0, SEEK_SET);



  int i = 0;
  struct Question q;
  while(fread(&q, sizeof(q), 1, file))
  {
    printf("%d. %s\n %s\n %s\n %s\n %s\n", i + 1, q.question,
                              q.opts[0], q.opts[1], q.opts[2], q.opts[3]);
    i++;
  }
  printf("\n");
}

void new_question()
{
  if (state.new_size >= sizeof(state.new_questions)/sizeof(struct Question*))
  {
    printf("New question buffer is full. Save changes first to add more questions.\n");
    return;
  }

  struct Question* q = malloc(sizeof(struct Question));
  struct Question r = {"", {"","","",""}};
  *q = r;
  char* tmp;
  int   num = 0;

  printf("Type in the new question:\n");
  tmp = getline(64);
  strcpy(q->question, tmp);
  free(tmp);

  while (!(num >= 2 && num <= 4))
  {
    printf("Number of options (2 >= && <= 4):\n");
    num = fgetc(stdin) - 48;
    flush();
  }

  int i = 0;
  while (i < num)
  {
    printf("Option #%d:\n", i + 1);
    tmp = getline(64);
    strcpy(q->opts[i], tmp);
    free(tmp);

    i++;
  }
  while (i < 4)
  {
    strcpy(q->opts[i], "");
    i++;
  }

  i = state.new_size++;
  state.new_questions[i] = q;

}

void modify_question()
{
  if (state.mod_size >= sizeof(state.mod_questions)/sizeof(struct Question*))
  {
    printf("Modified question buffer is full. Save changes first to add more questions.\n");
    return;
  }

  struct Question* q = malloc(sizeof(struct Question));
  struct Question r = {"", {"","","",""}};
  *q = r;
  char* tmp;

  printf("Type in the index of question to modify:\n");
  tmp = getline(64);
  int ind = atoi(tmp) - 1;
  free(tmp);

  printf("Type in the new question:\n");
  tmp = getline(64);
  strcpy(q->question, tmp);
  free(tmp);

  int i = 0;
  while (i < 4)
  {
    printf("Option #%d:\n", i + 1);
    tmp = getline(64);
    strcpy(q->opts[i], tmp);
    free(tmp);

    i++;
  }

  i = state.mod_size++;
  state.mod_questions[i] = q;
  state.mod_question_ids[i] = ind;
}

void delete_question()
{
  if (state.del_size >= sizeof(state.del_question_ids)/sizeof(int))
  {
    printf("Deleted question buffer is full. Save changes first to add more questions.\n");
    return;
  }

  printf("Type in the index of question to delete:\n");
  char* tmp = getline(64);
  int ind = atoi(tmp) - 1;
  free(tmp);

  int i = state.del_size++;
  state.del_question_ids[i] = ind;
}

void save_survey()
{
  { // modifying questions
    FILE* file = state.loaded_survey;

    struct Question q;
    for (int i = 0; i < state.mod_size; i++)
    {
      int ind = state.mod_question_ids[i];
      fseek(file, ind*sizeof(struct Question), SEEK_SET);
      q = *(state.mod_questions[i]);
      fwrite(&q, sizeof(q), 1, file);
    }
  }

  { // deleting questions
    FILE* file = state.loaded_survey;

    struct Question q;
    FILE* temp_file = fopen("temp", "w");

    int i = 0;

    fseek(file, 0, SEEK_SET);
    while(fread(&q, sizeof(q), 1, file))
    {
      if(find(state.del_question_ids, state.del_size, i) == -1)
      {
        fwrite(&q, sizeof(q), 1, temp_file);
      }
      i++;
    }

    fclose(temp_file);
    fclose(state.loaded_survey);
    remove(state.loaded_survey_path);
    rename("temp", state.loaded_survey_path);
    state.loaded_survey = fopen(state.loaded_survey_path, "r+");
  }

  { // adding new questions
    FILE* file = state.loaded_survey;

    struct Question q;
    fseek(file, 0, SEEK_END);

    for(int i = 0; i < state.new_size; i++)
    {
      q = *(state.new_questions[i]);
      fwrite(&q, sizeof(q), 1, file);
    }
  }

  reset_state(KEEP_FILE);
}

void exit_survey_manager()
{
  reset_state(DISCARD_FILE);
  exit(0);
}

void display_menu()
{
  printf("1. Create new survey\n");
  printf("2. Load existing survey\n");
  printf("3. List questions\n");
  printf("4. New question\n");
  printf("5. Modify question\n");
  printf("6. Delete question\n");
  printf("7. Save survey\n");
  printf("Q. Exit\n");

  char input = getc(stdin);
  flush();
  if      (input == '1')   create_survey();
  else if (input == '2')   load_survey("Which survey to load?");
  else if (input == '3')   list_questions();
  else if (input == '4')   new_question();
  else if (input == '5')   modify_question();
  else if (input == '6')   delete_question();
  else if (input == '7')   save_survey();
  else if (input == 'Q')   exit_survey_manager();
  else
  {
    printf("Invalid input\n");
  }
  display_menu();
}

void test()
{
  FILE* file = fopen("survey1.txt", "r+");
  struct Question q1 = {
                      "How are you?",
                        {
                        "Pretty good",
                        "Good",
                        "Meh",
                        ""
                        }
                       };

  struct Question q2 = {
                      "Who are you?",
                        {
                        "Josh",
                        "Alfred",
                        "Batman",
                        "Jakab"
                        }
                      };

  fwrite(&q1, sizeof(q1), 1, file);
  fwrite(&q2, sizeof(q2), 1, file);
  fclose(file);
}

int main (int argc, char** argv)
{
  init_state(DISCARD_FILE);
  //test();
  display_menu();
}

/*
FILE   *fopen(const char *filename, const char *mode)
"r"  : Opens a file for reading. The file must exist.
"w"  : Creates an empty file for writing.
       If a file with the same name already exists,
       its content is erased and the file is considered as a new empty file.
"a"  : Appends to a file. Writing operations, append data at the end of the file.
       The file is created if it does not exist.
"r+" : Opens a file to update both reading and writing. The file must exist.
"w+" : Creates an empty file for both reading and writing.
"a+" : Opens a file for reading and appending.
*/

/*
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
return value: number of succesfully written elements
ptr         : copy from
size        : size of one element
nmemb       : number of elements to copy
stream      : write to
*/

/*
int fseek(FILE *stream, long int offset, int whence)
return : 0 if succesful, non-zero otherwise
stream : what stream to seek
offset : number of bits
whence : first bit (to count the offset from)

SEEK_SET : beginning of file
SEEK_CUR : current bit
SEEK_END : end of file
*/
