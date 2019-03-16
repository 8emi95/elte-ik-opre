#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

#define BUFFERSIZE 4096
#define BUFLOWSIZE 256


int file_exist (char *filename) {
    struct stat buffer;
    return (stat (filename, &buffer) == 0);
}

int isDirectoryEmpty(char *dirname) {
    int n = 0;
    struct dirent *d;
    DIR *dir = opendir(dirname);
    if (dir == NULL) //Not a directory or doesn't exist
        return 1;
    while ((d = readdir(dir)) != NULL) {
        if(++n > 2)
            break;
    }
    closedir(dir);
    if (n <= 2) //Directory Empty
        return 1;
    else
        return 0;
}


char* read_from_file(char filename[]){
    char output[BUFLOWSIZE];
    char *returned_s = (char*) malloc(sizeof(char) * BUFLOWSIZE);
    int f;
    f=open(filename,O_RDONLY);
    if(f<0){
        perror("Error opening file \n");
        exit(1);
    }
    size_t nbytes;
    ssize_t bytes_read;
    nbytes = sizeof(output);
    bytes_read = read(f,output,nbytes);
    output[bytes_read] = '\0';
    printf("%s\n",output);

    strcpy(returned_s,output);

    close(f);
    return returned_s;
}

void append_to_file(char filename[], char data[]) {
    int f;
    mode_t mode = S_IRUSR | S_IWUSR;
    char buffer[BUFFERSIZE];
    f = open(filename,O_RDWR | O_APPEND,mode);
    if(f<0) {
        perror("Error opening file \n");
        exit(1);
    }

    strcpy(buffer,data);

    size_t n = strlen(buffer);
    if(write(f,&buffer,n)<0) {
        perror("Error writing file \n");
        exit(1);
    }
    close(f);
}

void create_file(char filename[], char *data) {
    int f;
    char yn[BUFFERSIZE];
    char yes[] = "yes";
    char no[] = "no";
    int bool = 1;
    mode_t mode = S_IRUSR | S_IWUSR;
    if(file_exist(filename)) {
        printf("FILE EXISTS!\n");
        printf("Are you sure you would like to overwrite this file?\n");
        size_t n;
        while(bool) {
            fgets(yn,BUFFERSIZE,stdin);
            n = strlen(yn);
            if(n>0 && yn[n-1] == '\n') {
                yn[n-1] = '\0';
            }

            if(strncmp(yn,yes,strlen(yn)) == 0) {
                bool = 0;
            }
            else if(strncmp(yn,no,strlen(yn)) == 0) {
                printf("Going back to the menu.\n");
                exit(0);
            }
        }
    }
    char buffer[BUFFERSIZE];
    f = open(filename,O_RDWR|O_CREAT|O_TRUNC,mode);
    if(f<0) {
        perror("Error creating file.\n");
        exit(1);
    }
    strcpy(buffer,data);
    size_t n = strlen(buffer);
    if(write(f,&buffer,n)<0) {
        perror("Error writing file.\n");
        exit(1);
    }
    close(f);
}

char* create_project(void) {
    printf("Please enter survey name:\n");
    char input[BUFLOWSIZE];
    char output[BUFLOWSIZE];
    //char *output = (char *) malloc(sizeof(char) * BUFLOWSIZE);
    output[0] = 'S';
    output[1] = ':';
    output[2] = ' ';
    output[3] = '\0';
    fgets(input,BUFLOWSIZE,stdin);
    size_t n = strlen(input);
    if(n>0 && input[n-1] == '\n') {
        input[n-1] = '\0';
    }
    strcat(output,input);

    struct stat st = {0};
    char filename[BUFLOWSIZE] = "survey/";
    strcat(filename,output);
    if(stat(filename,&st) == -1) {
        mkdir(filename,S_IRWXU | S_IRWXG | S_IRWXO);
    }
    char *returnS = (char *) malloc(sizeof(char) * BUFLOWSIZE);
    strcpy(returnS,output);
    return returnS;
}

char** separate_lines(char* filename) {
    char *tmpbuffer;
    //char *tmpbuffer = malloc(BUFFERSIZE * sizeof(char));

    int line_number = 0;
    int e_line_number = 5; // expects 5 lines for allocation
    //Allocate memory for 5 strings of size BUFLOW
    char **line_array = malloc(e_line_number * sizeof(char*));
    int i;
    for(i = 0;i<e_line_number;i++) {
        line_array[i] = malloc(BUFLOWSIZE * sizeof(char));
    }

    tmpbuffer = read_from_file(filename);
    int j = 0;
    while(tmpbuffer[j] != '\0') {
        if(tmpbuffer[j] == '\n') {
            line_number++;
        }
        j++;
    }

    int p = 0;
    char *tmpline;
    const char n[2] = "\n";
    tmpline = strtok(tmpbuffer,n);

    while(tmpline != NULL) {
        strcpy(line_array[p],tmpline);
        p++;
        tmpline = strtok(NULL,n);
    }

    return line_array;
}

void write_to_file(char filename[], char** q, size_t n){

    int f;

    f=open(filename,O_WRONLY | O_APPEND | O_TRUNC | O_CREAT, S_IWUSR | S_IRUSR);
    if(f<0) {
        perror("Error opening file.\n");
        exit(1);
    }

    size_t i;

    for(i=0;i<n;i++) {
        printf("%s\n",q[i]);

        if(write(f,strcat(q[i],"\n"),sizeof(q[i]))<0) {
            perror("Error writing to file.\n");
            exit(1);
        }
    }

}

void change_question(char** q,int mode) {
    char input[BUFLOWSIZE];
    char output[BUFLOWSIZE];
    size_t n;
    switch(mode) {
        case 1 :
            printf("Please enter new question: \n");
            output[0] = 'Q';
            output[1] = ':';
            output[2] = ' ';
            output[3] = '\0';
            fgets(input,BUFLOWSIZE,stdin);
            n = strlen(input);
            if(n>0 && input[n-1] == '\n') {
                input[n-1] = '\0';
            }
            strcat(output,input);
            strcpy(q[0],output);
            break;
        case 2 :
            printf("Please enter a new answer1: \n");
            output[0] = 'A';
            output[1] = '1';
            output[2] = ':';
            output[3] = ' ';
            output[4] = '\0';
            fgets(input,BUFLOWSIZE,stdin);
            n = strlen(input);
            if(n>0 && input[n-1] == '\n') {
                input[n-1] = '\0';
            }
            strcat(output,input);
            strcpy(q[1],output);
            break;
        case 3 :
            printf("Please enter a new answer2: \n");
            output[0] = 'A';
            output[1] = '2';
            output[2] = ':';
            output[3] = ' ';
            output[4] = '\0';
            fgets(input,BUFLOWSIZE,stdin);
            n = strlen(input);
            if(n>0 && input[n-1] == '\n') {
                input[n-1] = '\0';
            }
            strcat(output,input);
            strcpy(q[2],output);
            break;
        case 4 :
            printf("Please enter a new answer3: \n");
            output[0] = 'A';
            output[1] = '1';
            output[2] = ':';
            output[3] = ' ';
            output[4] = '\0';
            fgets(input,BUFLOWSIZE,stdin);
            n = strlen(input);
            if(n>0 && input[n-1] == '\n') {
                input[n-1] = '\0';
            }
            strcat(output,input);
            strcpy(q[3],output);
            break;
        case 5 :
            printf("Please enter a new answer4: \n");
            output[0] = 'A';
            output[1] = '4';
            output[2] = ':';
            output[3] = ' ';
            output[4] = '\0';
            fgets(input,BUFLOWSIZE,stdin);
            n = strlen(input);
            if(n>0 && input[n-1] == '\n') {
                input[n-1] = '\0';
            }
            strcat(output,input);
            strcpy(q[4],output);
            break;
        default:
            printf("Could not understand input, exiting editing mode.\n");
            exit(0);
    }

}

size_t count_lines(char *filename) {
    size_t line_count = 0;
    int f;
    f=open(filename,O_RDONLY);
    if(f<0){
        perror("Error opening file \n");
        exit(1);
    }

    char c;
    while(read(f,&c,sizeof(c))) {
        if(c=='\n') {
            line_count++;
        }
    }

    close(f);
    return line_count;
}

void modify_text(char *filename)
{
    printf("Would you like to modify or delete?\n");
    char **file_storage;
    char input[BUFLOWSIZE];
    char qinput[BUFLOWSIZE];

    fgets(input,BUFLOWSIZE,stdin);
    size_t n;
    n = strlen(input);
    if(n>0 && input[n-1] == '\n') {
        input[n-1] = '\0';
    }
    //if(strcmp(input,"modify") == 0) {
       /* printf("Please enter Q to edit question or 1-4 to edit answers\n");
        fgets(qinput,BUFLOWSIZE,stdin);
        n = strlen(qinput);
        if(n>0 && qinput[n-1] == '\n') {
            qinput[n-1] = '\0';
        }*/

    //}
    if(strcmp(input,"delete") == 0) {
        printf("Are you sure you want to delete this question, the answers will also be deleted\n");
        fgets(input,BUFLOWSIZE,stdin);
        n = strlen(input);
        if(n>0 && input[n-1] == '\n') {
            input[n-1] = '\0';
        }
        if(strcmp(input,"yes") == 0) {
            remove(filename);
            printf("Question removed from survey, exiting editing mode.\n");
            exit(0);
        }
        else {
            printf("yes was not entered, exiting editing mode.\n");
            exit(0);
        }
    }
    //else {
    //  printf("Could not understand input, exiting editing mode.\n");
    //  exit(0);
    //}

    size_t linecnt = count_lines(filename);

    //tmpbuffer(filename);
    file_storage = separate_lines(filename);

    printf("Please enter Q to edit question or 1-4 to edit answers\n");
    fgets(qinput,BUFLOWSIZE,stdin);
    n = strlen(qinput);
    if(n>0 && qinput[n-1] == '\n') {
        qinput[n-1] = '\0';
    }
    char qswitch = qinput[0];

    switch(qswitch) {
        case 'Q' :
            change_question(file_storage,1);
			//printf("%s",file_storage[0]);
			write_to_file(file_storage[0],file_storage,linecnt);
           // write_to_file(filename,file_storage,linecnt);
           // rename(filename,file_storage[0]);
            break;
        case 'q' :
            change_question(file_storage,1);
            write_to_file(filename,file_storage,linecnt);
            break;
        case '1' :
            change_question(file_storage,2);
            write_to_file(filename,file_storage,linecnt);
            break;
        case '2' :
            change_question(file_storage,3);
            write_to_file(filename,file_storage,linecnt);
            break;
        case '3' :
            change_question(file_storage,4);
            write_to_file(filename,file_storage,linecnt);
            break;
        case '4' :
            change_question(file_storage,5);
            write_to_file(filename,file_storage,linecnt);
            break;
        default:
            printf("Could not understand input, exiting editing mode.\n");
            exit(0);
    }
}

char* printdir(int mode)
{
    //char *output = (char *) malloc(sizeof(char) * BUFLOWSIZE);
    char *PATH = (char *) malloc(sizeof(char) * BUFLOWSIZE);
    DIR *d;
    DIR *e;
    char final_file[BUFLOWSIZE] = "survey/";
    char input[BUFLOWSIZE];
    char *filenames[BUFLOWSIZE];
    char *questions[BUFLOWSIZE];
    struct dirent *parentD;
    struct dirent *childD;
    int index;
    d = opendir("survey");
    int i = 0;
    printf("Please pick the survey to modify:\n");
    if(d) {
        while((parentD = readdir(d)) != NULL) {
            if((!strcmp(parentD->d_name,".")) == 0 && (!strcmp(parentD->d_name,"..")) == 0) {
                filenames[i] = parentD->d_name;
                printf("%d - %s\n",i+1,filenames[i]);
                i++;
            }
        }
        closedir(d);
    }
    fgets(input,BUFLOWSIZE,stdin);
    index = atoi(input);
    int j = 0;
    strcat(final_file,filenames[index-1]);
    printf("PATH: %s\n", final_file);
    if(mode == 1) {
        strcpy(PATH , final_file);
        //printf("%s\n", final_file);
        //free(output);
        return PATH;
    }


    int p = isDirectoryEmpty(final_file);
    if(p == 1) {
        printf("There was no questions in this survey! \n");
        exit(0);
    }
    e = opendir(final_file);
    int v = 1;
    if(e) {
        printf("survey contains questions:\n");
        while((childD = readdir(e)) != NULL) {
            if((!strcmp(childD->d_name,".")) == 0 && (!strcmp(childD->d_name,"..")) == 0) {
                questions[j++] = childD->d_name;
                printf("%d: %s\n",v++, childD->d_name);
            }
        }
        closedir(e);
    }
    printf("Pick question to modify:\n");
    char squestion[BUFLOWSIZE];
    int iquestion;
    fgets(squestion,BUFLOWSIZE,stdin);
    iquestion = atoi(squestion);
    printf("Picked %s \n",questions[iquestion-1]);
    printf("From %s\n", final_file);
    //char to_print[BUFLOWSIZE] = "survey/";
    //strcat(to_print,final_file);
    strcat(final_file,"/");
    strcat(final_file,questions[iquestion-1]);

    //FREEING MEMORY
    //free(output);
    //printf("%s\n",to_print);
    //read_from_file(to_print);
    //modify_text(to_print);
    strcpy(PATH,final_file);
    return PATH;

}

void copy(char *source, char *dest)
{
    int childExitStatus;
    pid_t pid;
    int status;
    if (!source || !dest) {
        /* handle as you wish */
    }

    pid = fork();

    if (pid == 0) { /* child */
        execl("/bin/cp", "/bin/cp", source, dest, (char *)0);
    }
    else if (pid < 0) {
        /* error - couldn't start process - you decide how to handle */
    }
    else {
        /* parent - wait for child - this has all error handling, you
         * could just call wait() as long as you are only expecting to
         * have one child process at a time.
         */
        pid_t ws = waitpid( pid, &childExitStatus, WNOHANG);
        if (ws == -1)
        { /* error - handle as you wish */
        }

        if( WIFEXITED(childExitStatus)) /* exit code in childExitStatus */
        {
            status = WEXITSTATUS(childExitStatus); /* zero is normal exit */
            /* handle non-zero as you wish */
        }
        else if (WIFSIGNALED(childExitStatus)) /* killed */
        {
        }
        else if (WIFSTOPPED(childExitStatus)) /* stopped */
        {
        }
    }
}


void question_bank(char *filename) {
	printf("Please pick the questions you would like to add to your survey:\n");
	DIR *d;
	struct dirent *parentD;
	char *filenames[BUFLOWSIZE];

	d = opendir("questionbank");
	int i = 0;
	if(d) {
        while((parentD = readdir(d)) != NULL) {
            if((!strcmp(parentD->d_name,".")) == 0 && (!strcmp(parentD->d_name,"..")) == 0) {
                filenames[i] = parentD->d_name;
                printf("%d: %s\n",i+1,filenames[i]);
                i++;
            }
        }
        closedir(d);
    }
	char input[BUFLOWSIZE];
	char done[5] = "done";
	while(1) {
		fgets(input,BUFLOWSIZE,stdin);
		//copy(filenames[input],filename);
		
		if(strcmp(input,done) == 0) {
			exit(0);
		}
		
	}
}


int main(int argc, char *argv[]) {
    //char *test = (char *) malloc(sizeof(char) * BUFLOWSIZE);
    char *test;
    //char **pointer;
    //read_from_file(argv[1]);
    printf("Please select from the following options:\n");
    printf("1. Read in survey\n");
    printf("2. Create a new survey\n");
    printf("3. Modify/Delete file\n");
    printf("4. Create a new question for existing survey\n");
    printf("5. Create a new answer for existing survey\n");
    printf("6. Modify existing survey\n");
    int i_input;
    char s_input[BUFFERSIZE];
    char filename[BUFFERSIZE];


    char input[BUFFERSIZE];
    char output[BUFLOWSIZE];
    char *PATH;
    size_t n;
    size_t p;
    fgets(s_input,10,stdin);
    //scanf("%d",i_input);
    i_input = atoi(s_input);
    switch(i_input) {
        case 1 :
            fgets(filename,BUFFERSIZE,stdin);
            n = strlen(filename);
            if(n>0 && filename[n-1] == '\n') {
                filename[n-1] = '\0';
            }
            printf("////////PRINTING////////\n");
            read_from_file(filename);
            break;
        case 2 :
            test = create_project();
            //create_project();
            printf("Created new survey: %s\n",test);
            break;
        case 3 :
            printf("Pressed 3 \n");
            PATH = printdir(0);
            printf("%s\n", PATH);
            break;
        case 4 :
            printf("Please select the survey to add to:\n");
            PATH = printdir(1);
            printf("Please enter question:\n");
            fgets(input,BUFLOWSIZE,stdin);
            output[0] = 'Q';
            output[1] = ':';
            output[2] = ' ';
            output[3] = '\0';
            strcat(output,input);
            strcat(PATH,"/");
            strcat(PATH,output);
            printf("%s\n", PATH);
            printf("%s\n", output);
            create_file(PATH,output);
            //printf("Please give \n");
            break;
        case 5 :
            printf("Please select the survey to add to:\n");
            PATH = printdir(0);
            printf("Please enter answer:\n");
            fgets(input,BUFLOWSIZE,stdin);
            p = count_lines(PATH);
            if(p<5) {
            output[0] = 'A';
            output[1] = (p + '0');
            output[2] = ':';
            output[3] = ' ';
            output[4] = '\0';
            strcat(output,input);
            append_to_file(PATH,output);
            }
            else {
                printf("Question already contains 4 answers.\n");
            }
            break;
        case 6 :
            PATH = printdir(0);
            //printf("%s\n", PATH);
            p = count_lines(PATH);
            //printf("LINECOUNT: %zu\n", p);
            modify_text(PATH);
            break;
        default :
            printf("Please enter 1, 2 or 3 only!\n");
    }

    //change_question(pointer,2);

    //checking that memory carried over from function then freeing it
    //int t;
    //for(t = 0;t<5;t++) {
    //  printf("%s\n",pointer[t]);
    //  free(pointer[t]);
    //}
    free(test);
    return 0;

}