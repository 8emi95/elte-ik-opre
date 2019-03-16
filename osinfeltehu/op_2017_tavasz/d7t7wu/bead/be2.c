#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,create
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h>
#include <unistd.h> // for pipe()

#define MAX_LENGHT 50
int flag = 0;
int fin = 0;
struct question
{
    char question[512];
    char answer1[256];
    char answer2[256];
    char answer3[256];
    char answer4[256];
    int questionnr;
    time_t submit_time;
};

typedef struct question Question;

void initialize_element(Question questions[], int i)//setting allocated memory to zero and default value
{
    memset(questions[i].question, '\0', sizeof(questions[i].question));
    memset(questions[i].answer1, '\0', sizeof(questions[i].answer1));
    strcpy(questions[i].answer1, "-");
    memset(questions[i].answer2, '\0', sizeof(questions[i].answer2));
    strcpy(questions[i].answer2, "-");
    memset(questions[i].answer3, '\0', sizeof(questions[i].answer3));
    strcpy(questions[i].answer3, "-");
    memset(questions[i].answer4, '\0', sizeof(questions[i].answer4));
    strcpy(questions[i].answer4, "-");
}

void initialize(Question questions[], int n)//initialize the array with zeros
{
    int i = 0;

    for (i; i < n; i++)
    {
        initialize_element(questions, i);
    }
}

void write_struct(Question questions[], int n)//display questions
{
    int i = 0;
    for (i; i < n; i++)
    {
        printf("\n%d. question: %s\n", i+1, questions[i].question);
        printf("Kerdesek szama: %d\n", questions[i].questionnr);
        printf("A. %s\n", questions[i].answer1);
        printf("B. %s\n", questions[i].answer2);
        printf("C. %s\n", questions[i].answer3);
        printf("D. %s\n", questions[i].answer4);
        printf("Date of submission: %s\n",ctime(&(questions[i].submit_time)));
    }
}

void read_file(char* s,Question questions[], int* srtuctl)//read the contents of the given file
{
    int f=open(s,O_RDONLY);
    int i = 0;
    if (f<0)
    {
        perror("Error at opening the file\n");
        exit(1);//exit if there is a problem
    }

    Question que;
    while (read(f,&(que),sizeof(que)))
    {
        printf("%d. question: %s\n", i+1, que.question);
        printf("A. %s\n", que.answer1);
        printf("B. %s\n", que.answer2);
        printf("C. %s\n", que.answer3);
        printf("D. %s\n", que.answer4);
        printf("Date of submission: %s\n",ctime(&(que.submit_time)));

        strcpy(questions[i].question, que.question);
        strcpy(questions[i].answer1, que.answer1);
        strcpy(questions[i].answer2, que.answer2);
        strcpy(questions[i].answer3, que.answer3);
        strcpy(questions[i].answer4, que.answer4);
        questions[i].questionnr=que.questionnr;
        questions[i].submit_time=que.submit_time;
    i++;
    }
    (*srtuctl)=i;
    close(f);
}

void create_file(char* s)//create the file in which you are going to save the survey to
{
    int f=open(s,O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
    if (f<0)
    {
        perror("Error at opening the file\n");
        exit(1);
    }
    flag = 1;
}

void new_question(char* s, Question questions[], int* length)//add a new question to the survey
{
    int n = *length;
    int i = 0;
    int c_ans = 0;
    char s_ans[20];

    if (n+1 < MAX_LENGHT)//check if the struct array is already full
    {
        printf("Please write the new question: ");//get new question from the user
        fgets(questions[n].question, 512, stdin);
        questions[n].question[strcspn(questions[n].question, "\n")] = 0;


        while(c_ans<2||c_ans>4)//there can only be 2, 3 or 4 answers
        {
            printf("Please indicate how many answers you would like to enter(Min=2, max=4):  ");
            fgets(s_ans,20,stdin);
            s_ans[strcspn(s_ans, "\n")] = 0;
            c_ans=atoi(s_ans);
            questions[n].questionnr=c_ans;
        }

        switch(c_ans)//read the answers
        {
        case 2:
            printf("Please write answer A: ");
            fgets(questions[n].answer1, 256, stdin);
            questions[n].answer1[strcspn(questions[n].answer1, "\n")] = 0;
            printf("Please write answer B: ");
            fgets(questions[n].answer2, 256, stdin);
            questions[n].answer2[strcspn(questions[n].answer2, "\n")] = 0;
            break;
        case 3:
            printf("Please write answer A: ");
            fgets(questions[n].answer1, 256, stdin);
            questions[n].answer1[strcspn(questions[n].answer1, "\n")] = 0;

            printf("Please write answer B: ");
            fgets(questions[n].answer2, 256, stdin);
            questions[n].answer2[strcspn(questions[n].answer2, "\n")] = 0;

            printf("Please write answer C: ");
            fgets(questions[n].answer3, 256, stdin);
            questions[n].answer3[strcspn(questions[n].answer3, "\n")] = 0;
            break;
        case 4:
            printf("Please write answer A: ");
            fgets(questions[n].answer1, 256, stdin);
            questions[n].answer1[strcspn(questions[n].answer1, "\n")] = 0;

            printf("Please write answer B: ");
            fgets(questions[n].answer2, 256, stdin);
            questions[n].answer2[strcspn(questions[n].answer2, "\n")] = 0;

            printf("Please write answer C: ");
            fgets(questions[n].answer3, 256, stdin);
            questions[n].answer3[strcspn(questions[n].answer3, "\n")] = 0;

            printf("Please write answer D: ");
            fgets(questions[n].answer4, 256, stdin);
            questions[n].answer4[strcspn(questions[n].answer4, "\n")] = 0;
            break;
        }
        questions[n].submit_time=time(NULL);//get current time
        (*length)++;//increase how many elements are in the array
    }
    else
    {
        printf("You can't add more questions!");
    }
}
void modify_question(Question questions[], int struct_length, int nr)//modify a selected question
{
    if (struct_length>nr&&nr>=0)//check if the question is an existing one
    {
        int var = 0;
        char s_var[20];

        printf("\n%d. question: %s\n", nr + 1, questions[nr].question);//display the selected question
        printf("A. %s\n", questions[nr].answer1);
        printf("B. %s\n", questions[nr].answer2);
        printf("C. %s\n", questions[nr].answer3);
        printf("D. %s\n", questions[nr].answer4);

        printf("Would you like to change the body of the question? YES(1) NO(0) : ");

        fgets(s_var, 20, stdin);//fgets eats the '\n' as well
        s_var[strcspn(s_var, "\n")] = 0;//setting '\n' to 0
        var=atoi(s_var);



    int c_ans = 0;
    char s_ans[20];
        while(c_ans<2||c_ans>4)//there can only be 2, 3 or 4 answers
        {
            printf("Please indicate how many answers you would like to have this time(Min=2, max=4):  ");
            fgets(s_ans,20,stdin);
            s_ans[strcspn(s_ans, "\n")] = 0;
            c_ans=atoi(s_ans);
            questions[nr].questionnr=c_ans;
        }

        if (var == 1)//change the body
        {
            printf("Write the modified version: ");
            fgets(questions[nr].question, 512, stdin);
            questions[nr].question[strcspn(questions[nr].question, "\n")] = 0;
        }
        printf("Would you like to change answer A? YES(1) NO(0) : ");
        fgets(s_var, 20, stdin);
        s_var[strcspn(s_var, "\n")] = 0;
        var=atoi(s_var);

        if (var == 1)//change answer A
        {
            printf("Write the modified version: ");
            fgets(questions[nr].answer1, 256, stdin);
            questions[nr].answer1[strcspn(questions[nr].answer1, "\n")] = 0;
        }

        printf("Would you like to change answer B? YES(1) NO(0) : ");
        fgets(s_var, 20, stdin);
        s_var[strcspn(s_var, "\n")] = 0;
        var=atoi(s_var);

        if (var == 1)//change answer B
        {
            printf("Write the modified version: ");
            fgets(questions[nr].answer2, 256, stdin);
            questions[nr].answer2[strcspn(questions[nr].answer2, "\n")] = 0;
        }

        printf("Would you like to change answer C? YES(1) NO(0) : ");
        fgets(s_var, 20, stdin);
        s_var[strcspn(s_var, "\n")] = 0;
        var=atoi(s_var);

        if (var == 1)//change answer C
        {
            printf("Write the modified version: ");
            fgets(questions[nr].answer3, 256, stdin);
            questions[nr].answer3[strcspn(questions[nr].answer3, "\n")] = 0;
        }

        printf("Would you like to change answer D? YES(1) NO(0) : ");
        fgets(s_var, 20, stdin);
        s_var[strcspn(s_var, "\n")] = 0;
        var=atoi(s_var);

        if (var == 1)//change answer D
        {
            printf("Write the modified version: ");
            fgets(questions[nr].answer4, 256, stdin);
            questions[nr].answer4[strcspn(questions[nr].answer4, "\n")] = 0;
        }
    }
    else
    {
        printf("Not an existing question!");
    }
}

void delete_question(Question questions[],int* length, int nr)//delete selectedquestion
{
    if(nr<(*length)||nr>=0)
    {

        int i = nr;
        for ( i; i < (*length); i++)
        {
            initialize_element(questions, i);
            questions[i]=questions[i+1];
        }
        (*length)--;//decrease the length upon successful delete
    }
    else
    {
        printf("Not an existing question!");
    }
}
int contains(int array[],int i, int n)//checks if the i number is in the array of size n
{
    int b = 0, k = 0;
    while (b==0&& k<n)
    {
        if (i+1==array[k++])
        {
            b=1;
        }
    }
    return b;
}
void finalize(char* s, Question questions[], int length)
{
    fin = 1;
    int array[MAX_LENGHT];
    int n, i = 0;
    char s_n[20];

    printf("Please indicate how many questions would you like the final survey to be:");
    fgets(s_n, 20, stdin);
    s_n[strcspn(s_n, "\n")] = 0;
    n=atoi(s_n);//convert string to int

    printf("Please write the desired question's numbers separately, pressing enter!\n");
    if (n > length) n=length;//if the given number is too large reduce it to maximum
    if (n < 0) n=0;//given number too small -> turn it into a zero
    for (i = 0; i < n; i++)
    {
        fgets(s_n, 20, stdin);//read which question the user wants to save
        s_n[strcspn(s_n, "\n")] = 0;
        array[i]=atoi(s_n);
    }
    int f=open(s,O_WRONLY);
    if (f<0)
    {
        perror("Error at opening the file\n");
        exit(1);
    }
    char c[] = "\n";

    for (i = 0; i < length; i++)//write into file
    {
        if (contains(array, i, n) == 1)
        {

            if (write(f, &(questions[i]),sizeof(questions[i]))!=sizeof(questions[i]))
            {
                perror("There is a mistake in writing to file\n");
                exit(1);
            }

        }
    }
}

void newSurvey(Question questions[], Question survey[], int j, int k, int l)
{
    strcpy(survey[0].question, questions[j].question);
    strcpy(survey[0].answer1, questions[j].answer1);
    strcpy(survey[0].answer2, questions[j].answer2);
    strcpy(survey[0].answer3, questions[j].answer3);
    strcpy(survey[0].answer4, questions[j].answer4);
    survey[0].questionnr=questions[j].questionnr;
    survey[0].submit_time = questions[j].submit_time;

    strcpy(survey[1].question, questions[k].question);
    strcpy(survey[1].answer1, questions[k].answer1);
    strcpy(survey[1].answer2, questions[k].answer2);
    strcpy(survey[1].answer3, questions[k].answer3);
    strcpy(survey[1].answer4, questions[k].answer4);
    survey[1].questionnr=questions[k].questionnr;
    survey[1].submit_time = questions[k].submit_time;

    strcpy(survey[2].question, questions[l].question);
    strcpy(survey[2].answer1, questions[l].answer1);
    strcpy(survey[2].answer2, questions[l].answer2);
    strcpy(survey[2].answer3, questions[l].answer3);
    strcpy(survey[2].answer4, questions[l].answer4);
    survey[2].questionnr=questions[l].questionnr;
    survey[2].submit_time = questions[l].submit_time;
}
int main(int argc,char** argv)
{
    char file[50]="kerdesek.txt";
    Question questions[MAX_LENGHT];
    initialize(questions, MAX_LENGHT);
    int struct_length = 0;
    char s[20];
    int v = 0, nr = 0;
    int p = 1;
    time_t t;
    srand((unsigned)time(&t));

    while (p)
    {
        printf("\nMenu\n");
        printf("1. Create the survey!\n");
        printf("2. Display questions!\n");
        printf("3. Modify question!\n");
        printf("4. Delete question!\n");
        printf("5. New question!\n");
        printf("6. Finalize the survey!\n");
        printf("7. Final survey !\n");
        printf("8. Exit!\n");

        printf("Choose a menu option: ");
        fgets(s, 20, stdin);
        s[strcspn(s, "\n")] = 0;
        v=atoi(s);

        switch (v)
        {
        case 1://create the survey
            create_file(file);
            printf("The survey has been created! You can add your questions now!");
            break;
        case 2://display questions
            if(flag==1)
            {
                printf("\n The questions:\n ");
                write_struct(questions, struct_length);

            }
            else
            {
                printf("You need to create the survey first!");
            }

            break;
        case 3://modify questions
            if(flag==1)//can be done only if the survey has been created
            {

                printf("Which question would you like to modify? : ");
                fgets(s, 20, stdin);
                s[strcspn(s, "\n")] = 0;
                nr=atoi(s);

                nr--;
                modify_question(questions, struct_length, nr);
            }
            else
            {
                printf("You need to create the survey first!");
            }
            break;
        case 4://delete a question
            if(flag==1)
            {
                printf("Which question would you like to delete? ");
                fgets(s, 20, stdin);
                s[strcspn(s, "\n")] = 0;
                nr=atoi(s);

                nr--;
                delete_question(questions, &struct_length, nr);
                printf("The %i. question has been deleted!", nr + 1);
            }
            else
            {
                printf("You need to create the survey first!");
            }
            break;
        case 5://add new question
            if(flag==1)
            {
                new_question(file, questions, &struct_length);
            }
            else
            {
                printf("You need to create the survey first!");
            }
            break;
        case 6://save the survey
            if(flag==1)
            {
                finalize(file, questions, struct_length);
                printf("\nThe survey is finalized!\n");
            }
            else
            {
                printf("You need to create the survey first!");
            }

            break;
        case 7://display the saved survey
            if(flag==1)
            {
                printf("\nThe final survey:\n");
                read_file(file,questions,&struct_length);
            }
            else
            {
                printf("You need to create the survey first!");
            }

            break;
        case 8:

            if (fin==1&&struct_length>=3)
            {
                p=0;
            }
            else
            {
                if(struct_length<3)
                {
                    printf("You need at least three questions!");
                }
            }
            break;
        default:
            printf("Not a valid menu option!Please select a new one!");
        }
    }

    Question survey1[3];
    Question survey[3];

    int i = 0;
    int pipefdl[2];
    int pipefdw[2];
    int pid = 0;
    char buffer[100];
    int nrq;
    int valaszok[60];

    if (pipe(pipefdl)<0||pipe(pipefdw)<0)
    {
        perror("problem with the pipes");
        exit(1);
    }

    pid = fork();

    if (pid<0)
    {
        perror("problem with creating the child");
    }

    if (pid>0)
    {
        //MLSZ
        close(pipefdl[1]);//bezarta a pipe iro veget

        read(pipefdl[0], buffer, sizeof(buffer));
        printf("A vizsgalobiztos statusza: %s\n",buffer);
        // MLSZ randomly chooses 3 questions
        close(pipefdw[0]);
        int j,k,l;
        j = rand() % struct_length;
        k = rand() % struct_length;
        l = rand() % struct_length;
        while(k==j)
        {
            k = rand() % struct_length;
        }
        while(k==l||l==j)
        {
            l = rand() % struct_length;
        }

        newSurvey(questions, survey1, j, k, l);

        write(pipefdw[1], survey1, 3*sizeof(Question));

        read(pipefdl[0], &nrq, sizeof(nrq));
        printf("\n%d ez a szuloben a kitoltok szama\n", nrq);
        read(pipefdl[0], valaszok, nrq*3*sizeof(int));
        //the result of the consultation
        int m;
        for(i = 0; i<3; i++)
        {
            printf("%d. %s\n",i+1,survey1[i].question);
         printf(" %s\n",survey1[i].answer1);
         printf(" %s\n",survey1[i].answer2);
         printf(" %s\n",survey1[i].answer3);
         printf(" %s\n",survey1[i].answer4);
         printf("The answers:\n");
            for(m=0; m < nrq; ++m)
            printf("\n%d. person: %d \n",m+1,1+valaszok[m*3+i]);
        }

        close(pipefdl[0]);
        wait();
    }
    else
    {
        close(pipefdl[0]);//bezarom a pipe olvaso veget
        strcpy(buffer,"keszen all\n");
        write(pipefdl[1],buffer, sizeof(buffer));

        close(pipefdw[1]);//bezarom a pipe iro fejet
        read(pipefdw[0], survey, 3*sizeof(Question));

        int nr_kerdes[3];
        for(i=0; i < 3; ++i)
        {
            nr_kerdes[i]=survey[i].questionnr;
        }
        int emberek=rand()% 10 + 10;

        for(i = 0; i<emberek; i++)
        {
            valaszok[i*3]=rand()%nr_kerdes[0];
            valaszok[i*3 + 1]=rand()%nr_kerdes[1];
            valaszok[i*3 + 2]=rand()%nr_kerdes[2];
        }

        write(pipefdl[1],&emberek, sizeof(emberek));
        write(pipefdl[1],valaszok, emberek*3*sizeof(int));

        close(pipefdw[0]);
        close(pipefdl[1]);
        printf("A kerdezobiztos befejezte a konzultaciot.\n");
    }

    return 0;
}