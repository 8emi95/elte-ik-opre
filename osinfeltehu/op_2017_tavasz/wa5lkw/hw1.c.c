#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h> //opendir, readdir
#include <sys/types.h> //opendir
#include <sys/stat.h> //stat
#include <unistd.h>   //stat
#include <fcntl.h> //open,creat
#include <string.h>

typedef struct question {
	//int numberOfAnswers;
	char questions[50];
	char answers[4][50];
} question;

enum options {
	OPT_EXIT,
	OPT_LIST,
	OPT_ADD,
	OPT_READ,
	OPT_DEL,
	OPT_MOD,
	OPT_LAST
};


question openAndReadFile(const char* questionName) {
	// int l = strlen(questionName);
	// questionName[l - 1] = 0;
	// char fullPath[50] = "./data/";
	// strcat(fullPath, questionName);
	// strcat(fullPath, ".q");

	int l;
	int f; 
	f=open(questionName, O_RDONLY); 
	//printf("filename: %s\n", questionName);
	if (f<0) {
		perror("Error at opening the file\n");
		exit(1);
	}

	question q;

 	read(f,&q,sizeof(q));
	//int l;
	//l=strlen(q.questions);
	//q.questions[l-1] = 0;
	printf("Question: %s\n", q.questions);

	int i = 0;
	for (i = 0; i < 4; i++) {
		l=strlen(q.answers[i]);		
		q.answers[i][l-1] = 0;
		if(l > 0)
			printf("Answer %d: %s\n", i+1, q.answers[i]);
	}

	close(f);

	return q;
}


void addQuestionsAndAnswers(const char* questionName) {
	// int length = strlen(questionName);
	// questionName[length - 1] = 0;
	// char fullPath[50] = "./data/";
	// strcat(fullPath, questionName);
	// strcat(fullPath, ".q");

	//printf("Filename: %s\n", fullPath);

	int f = open(questionName,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if (f<0) {
 		perror("File opening error\n");
 		exit(1);
 	}

 	question q;
 	int numberOfAnswers = 0;
 	char line[50];

	strcpy(q.questions,questionName);

	printf("Number of answers: \n");
	scanf("%d", &numberOfAnswers);
	while ((getchar()) != '\n');

 	int i;
	for (i = 0; i < numberOfAnswers; i++) {
		printf("Answer %d: ", i+1);
		fgets(line,50,stdin);
		strcpy(q.answers[i],line);
	}
	for (; i < 4; i++)
		q.answers[i][0] = 0;

	write(f,&q,sizeof(q));

	close(f);
}


void deleteQuestion(const char* questionName) {
	// char fullPath[50] = "./data/";
	// strcat(fullPath, questionName);
	// strcat(fullPath, ".dat");
	unlink(questionName);
}

void modifyQuestion(const char *questionName) {
	question q = openAndReadFile(questionName);
	// char fullPath[50] = "./data/";
	// strcat(fullPath, questionName);
	// strcat(fullPath, ".dat");

	int f = open(questionName,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if (f<0) {
 		perror("File opening error\n");
 		exit(1);
 	}

	printf("1. rename question\n");
	printf("2. edit answer\n");

	int x = 0;
	char input[50];

	while (x < 1 || x > 2) {
		scanf("%d", &x);
	}
	while ((getchar()) != '\n');

	if(x == 1) {
		printf("New name: \n");
		fgets(input,50,stdin);
		input[strlen(input)-1] = 0;
		strcpy(q.questions,input);
	} else {
		printf("Which answer to modify (1-4): ");
		while (x < 1 || x > 4) {
			scanf("%d", &x);
		}
		while ((getchar()) != '\n');

		printf("New answer: \n");
		fgets(input,50,stdin);
		strcpy(q.answers[x-1],input);
	}


	write(f,&q,sizeof(q));
	close(f);

	char newName[50] = "";
	strcpy(newName, q.questions);
	strcat(newName, ".q");
	rename(questionName, newName);
}


int isQuestion(const char* filename) {
	int l = strlen(filename);
	if(l < 2)
		return 0;
	return (filename[l - 1] == 'q' && filename[l - 2] == '.');
}


void listOfQuestions() {
	DIR* d;
	struct dirent * dp; //pointer to dirent struct
	struct stat st;
	d = opendir("."); //parameter the directory name (with path) we want to open
	if (d<0) {
		perror("Some problem in opening");
		exit(1);
	}

	while (dp=readdir(d)) {
		if (!strcmp (dp->d_name, "."))
            continue;
        if (!strcmp (dp->d_name, ".."))    
            continue;
	   	stat(dp->d_name, &st);  //fstat(file descriptor, stat structure)
		if(isQuestion(dp->d_name)) {
		//if(S_ISREG(st.st_mode)) {
		    //printf("Filename: %s\n", dp->d_name);
		    printf("Time: %ld\n", st.st_mtime);
		    openAndReadFile(dp->d_name);
	    }
	    //else
	   		//printf("dir: %s\n",dp->d_name);
	}
	closedir(d); //close directory after opendir
}


int main(int argv, char* argc[]){
	char questionName[50];
	int x = -1;

	x = -1;

	while(x != OPT_EXIT) {
		// Read  input
		printf("%d. List questions\n", OPT_LIST);
		printf("%d. Add question\n", OPT_ADD);
		printf("%d. Read question\n", OPT_READ);
		printf("%d. Delete question\n", OPT_DEL);
		printf("%d. Modify question\n", OPT_MOD);
		printf("%d. Exit\n", OPT_EXIT);
		scanf("%d", &x);

		// If input wasn't correct
		while (x < OPT_EXIT || x >= OPT_LAST) {
			scanf("%d", &x);
		}
		while ((getchar()) != '\n');
		//fseek(stdin, 0, SEEK_END);

		if(x > OPT_LIST && x < OPT_LAST) {
			printf("Question name: \n");
			fgets(questionName,50,stdin);
			int l = strlen(questionName);
			questionName[l - 1] = 0;
			strcat(questionName, ".q");
		}

		switch (x) {
			case OPT_LIST:
				listOfQuestions();
				break;
			case OPT_ADD:
				addQuestionsAndAnswers(questionName);
				break;
			case OPT_READ:
				openAndReadFile(questionName);
				break;
			case OPT_DEL:
				deleteQuestion(questionName);
				break;
			case OPT_MOD:
				modifyQuestion(questionName);
				break;
		}
	}

	return 0;
}