#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>


/*Data structures*/
struct PollNode{
    char question[255];
    char answer[4][255];
    //char date[255];
    //int ID;
    int numberOfAnswears;
};

/*Functions*/
int countLines();
void handler(int signumber);
int cmpfunc (const void * a, const void * b);
void SearchQuestion();


int main(int argc, char *argv[])
{
    /*General variables and data structures*/
    struct PollNode newPollNode;
    int rv;
    char Answears[3][30];
   
    /* create signal */
    signal(SIGTERM,handler);

    /* create pipe */
    int pipefd[2]; // unnamed pipe file descriptor array
    int bidirectionalpipe[2];
    pid_t pid; // Child process
    char buffer[5][255];  // char array for reading from pipe

    if (pipe(pipefd) == -1) {
        perror("Error: Opening the Pipe");
        exit(EXIT_FAILURE);
    }
    pid = fork();	// creating parent-child processes
    if (pid == -1) {
        perror("Error: Fork");
        exit(EXIT_FAILURE);
    }

    /* parent process */
    if (0 != pid)    
    {    
        printf("[ PARENT ]: Parent has been started!\n");
        //Waiting for the signal
        printf("[ PARENT ]: Waiting for the login of the child!\n\n : %s", argv[1]);
        sleep(1);
        kill(pid, SIGTERM);
        pause(); //waits till a signal arrive 
        printf("[ PARENT ]: Signal arrived\n",SIGTERM);
        
        
        //Count ID's'
        int line_num = 0;
        line_num = countLines();
        printf("[ PARENT ]: Az id-k dartabszama: %i\n", line_num);

        /*Random number generation*/
        srand(time(NULL)); //the starting value of random number generation
        int randomNumbers[3];
        int random = 0;
        char place2[3][50];

        /*Search the IDs on the given line numbers*/
        int i;
        for(i = 0; i < 3; i = i + 1){
            random = rand()%(line_num); //number between 0-line_num
            randomNumbers[i] = random;
            printf("[ PARENT ]: Az %d random szam: %i\n", i, random);
        
            //Get a id on the random index in the quest_id.txt
            FILE *fp = fopen("quest_id.txt", "r");
            while(fgets(place2[i], sizeof(place2[i]), fp) && random>0){
                random--;
            }
            fclose(fp);

            printf("[ PARENT ]: Az adott sorzamu sor: %s\n", place2[i]);
            
        }

        //Sort the randomNumbers
        qsort(randomNumbers, 3, sizeof(int), cmpfunc);
        printf("[ PARENT ]: Random numbers are generated! \n");
       

       //Searching the Poll items in the database
        char structTemp[8][255];
        int j = 0;
        int line_n = 1;
        int find_result = 0;
        int c;
        char temp[255];

        int flag = 0;
        for(i = 0; i < 3; i++){
            FILE *oldData;
            oldData = fopen("./master_branch.txt", "r");

             while(fgets(temp, 255, oldData) != NULL) {
                    if((strcmp(temp, place2[i])) == 0 && flag == 0) {
                        printf("A match found on line: %d\n", line_n);
                        flag = 1;
                        printf("\n%s\n", temp);
                        find_result++;
                        while(fgets(temp, 255, oldData) != NULL && (temp[0] != '\n')){
                            printf("%s",temp);
                            strcpy(structTemp[j], temp);
                            //printf("STRUCT_TEMP: %s\n", structTemp[j]);
                            //printf("J: %i\n", j);
                            if(j < 8){
                                j++;
                            }    
                        }
                    }
                line_n++;
            }
            if(find_result == 0) {
                printf("\nSorry, couldn't find a match.\n");
            }

            fclose(oldData);
            j=0;
            flag = 0;

            //Insert the result in a struct -- for the better handling
            strcpy(newPollNode.question, structTemp[1]);
            newPollNode.numberOfAnswears = atoi(structTemp[2]);
            int y;
            for(y = 0; y < newPollNode.numberOfAnswears; y++){
                strcpy(newPollNode.answer[y], structTemp[y+3]);
            }
            
            //Send the struct to the CHILD
            rv = write(pipefd[1], &newPollNode, sizeof(newPollNode));
            printf("[ PARENT ]: Write: %d\n", rv);
            printf("[ PARENT ]: Parent has wrote the data in the pipe!\n");
            fflush(NULL); 	// flushes all write buffers (not necessary)
            
            // - Write 0 for the empty places
            if(i < 2) {
                int x;
                for(x = 0; x < 8; x++ ){
                    strcpy(structTemp[x], "0\n");
                }   
            }
          sleep(1);  
        }
        
        printf("[ PARENT ]: END searching\n");
        
        //Read the CHILD's answears for the sended questions
        char recivedAnswears[3][30];
        read(pipefd[0], recivedAnswears, sizeof(recivedAnswears));
        printf("[ PARENT ]: I recived the answears! \n");
        printf("[ PARENT ]: Answears for the 1. question are : %s\n", recivedAnswears[0]);
        printf("[ PARENT ]: Answears for the 2. question are : %s\n", recivedAnswears[1]);
        printf("[ PARENT ]: Answears for the 3. question are : %s\n", recivedAnswears[2]);
        kill(pid, SIGTERM);
       
	    printf("[ PARENT ]: Parent has finished!\n");	

    }
    
    /* child process */
    else
    {   
        /*Send signal to get ready*/
        pause();
        printf("[ CHILD ]: Logged in succesfully! \n");
        kill(getppid(), SIGTERM); 
        srand(time(NULL));

        /*variable declarations */
        int Ans_nums[3];
        int i = 0;
        int z = 0;
        int y;

        /* Communication process */
        while((rv = read(pipefd[0], &newPollNode, sizeof(struct PollNode)))){
            //Read from the pipe
            printf("[ CHILD ]: Child has just started to read the data from the pipe!\n");
            printf("[ CHILD ]: Read: %d\n", rv);
            printf("\n");
            printf("[ CHILD ]: %s", newPollNode.question);
            printf("[ CHILD ]: %d\n", newPollNode.numberOfAnswears);
            Ans_nums[i] = newPollNode.numberOfAnswears;
            i++;
            printf("[ CHILD ]: The answear possibilities :\n");
            for(y = 0; y < newPollNode.numberOfAnswears; y++){
                printf("[ CHILD ]: %s", newPollNode.answer[y]);
            }

            z++;

            //Generate and send the answears to the PARENT
            if(z == 3){
               sleep(3);
               printf("[ CHILD ]: Slept for 3 sec!\n");

                int x;
                int rx=rand()%11 + 10; //Number of fillers
                int random = 0;

                for(x = 0; x < 3; ++x)
                {
                    memset(Answears[x], 0, sizeof(Answears[x]));
                    int j;
                    for(j=0; j < rx;++j)
                    {
                        int temp=rand()%Ans_nums[x]+1;
                        char a = (char)(temp + (int)('0'));
                        Answears[x][j]=a;
                    }
                    Answears[x][j+1]='0';
                }

                printf("[ CHILD ]: The generated answears are:\n ");
                for( x = 0; i < 3; x++){
                    printf("[ CHILD ]: Answears for the %i question are :%s\n", i, Answears[x]);
                }

                printf("[ CHILD ]: Send the answears to the parent.\n");
        
                /* --- DEBUG
                    strcpy(Answears[0], "asd1");
                    strcpy(Answears[1], "asd2");
                    strcpy(Answears[2], "asd3");
                */
                write(pipefd[1], &Answears, sizeof(Answears));
                
                kill(getppid(), SIGTERM);
                printf("[ CHILD ]: Child is finished!\n");
                pause();
            } 
        }
    }    
    return 0;
}

/*Function declarations*/

int countLines(){
    FILE *lineCheck;
    int lines = 0;
    lineCheck = fopen("quest_id.txt", "r");
    
    while (!feof(lineCheck)) {
        int ch = fgetc(lineCheck);
        if (ch == '\n') {
            lines += 1;
        }
    }
    fclose(lineCheck);
    return lines;
}

void handler(int signumber){
  printf("[ PARENT ]: Signal with number %i has arrived\n",signumber);
}

//Compare function wich is generate the condition, for the qsort
int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}
