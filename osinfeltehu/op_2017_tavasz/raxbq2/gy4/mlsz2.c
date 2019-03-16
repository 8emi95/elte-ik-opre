#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef int bool;
#define true 1
#define false 0

typedef struct Kerdoiv
{
	char question[150];
	char answers[4][100];
	int answnumber;
	char timeOfQuestion[64];
	struct Kerdoiv* next;
} Node;

void handler(int signum)
{
	printf("------------\nSikeresen bejelentkezett az MLSZ-hez.\n");
}

int* runMenu(Node* h, int* numOfQuestions, int* numOfFinalized);
void newQuestion(Node* h, int* numOfQuestions);
void editQuestion(Node* h);
void deleteQuestion(Node* h, int* numOfQuestions);
void destroyLinkedList(Node* h);
bool contains(int* array, int size, int elem);
bool listQuestions(Node* h);
int* finalizeQuestions(Node* h, int numOfQuestions, int* c);
int* getAnsNums(Node* h, int q1, int q2, int q3);
Node* returnById(Node* h, int id);

int main(int argc, char** argv)
{
	signal(SIGUSR1, handler);
	int fid = mkfifo("fifo.mlsz", S_IRUSR|S_IWUSR);
	char reader[200];

	if(fid == -1)
	{
		perror("Nem sikerult letrehozni a csovet.\n");
		exit(EXIT_FAILURE);
	}

	pid_t child = fork();

	if(child < 0)
	{
		perror("Nem sikerult gyereket inditani.\n");
		exit(EXIT_FAILURE);
	}else if(child > 0)
	{
		Node* h = (Node*)malloc(sizeof(Node));
		h->next = NULL;
		int numOfQuestions = 0;

		int numOfFinalized = 0;
		int* nums = runMenu(h, &numOfQuestions, &numOfFinalized);
		if(nums == NULL) 
		{
			destroyLinkedList(h);
			remove("fifo.mlsz");
			exit(EXIT_SUCCESS);
		}

		if(numOfFinalized < 3)
		{
			printf("Kevesebb, mint 3 veglegesitett kerdes, vege.\n");
			destroyLinkedList(h);
			remove("fifo.mlsz");
			return 0;
		}

		char* city = (char*)malloc(50 * sizeof(char));
		printf("------------\nCentrumvaros: ");
		fgets(city, 50 * sizeof(char), stdin);
		strtok(city, "\n");

		int fd = open("fifo.mlsz", O_WRONLY);
		int rdy = 1;
		write(fd, &rdy, sizeof(int));
		close(fd);

		pause();

		int chosenNumbers[3] = {-1, -1, -1};
		int ind = 0;

		printf("------------\nKerdesek kivalasztasa kuldesre ...\n");
		do
		{
			srand(time(NULL));
			int r = rand() % numOfQuestions + 1;
			if(contains(nums, numOfFinalized, r) && !contains(chosenNumbers, 3, r))
			{
				chosenNumbers[ind] = r;
				++ind;
			}
		}while(ind < 3);

		int tmp;
		if(chosenNumbers[0] > chosenNumbers[2])
		{
			tmp = chosenNumbers[0];
			chosenNumbers[0] = chosenNumbers[2];
			chosenNumbers[2] = tmp;
		}

		if(chosenNumbers[0] > chosenNumbers[1])
		{
			tmp = chosenNumbers[0];
			chosenNumbers[0] = chosenNumbers[1];
			chosenNumbers[1] = tmp;
		}

		if(chosenNumbers[1] > chosenNumbers[2])
		{
			tmp = chosenNumbers[1];
			chosenNumbers[1] = chosenNumbers[2];
			chosenNumbers[2] = tmp;
		}

		int* ansNums = getAnsNums(h, chosenNumbers[0], chosenNumbers[1], chosenNumbers[2]);

		fd = open("fifo.mlsz", O_WRONLY);
		write(fd, &ansNums[0], sizeof(int));
		write(fd, &ansNums[1], sizeof(int));
		write(fd, &ansNums[2], sizeof(int));
		close(fd);

		int pplNum = 0;
		fd = open("fifo.mlsz", O_RDONLY);
		read(fd, &pplNum, sizeof(int));

		int results[3][4];
		for(int i = 0; i < 3; ++i)
			for(int j = 0; j < 4; ++j) results[i][j] = 0;

		for(int i = 0; i < pplNum; ++i)
		{
			for(int j = 0; j < 3; ++j)
			{
				int tmp2;
				read(fd, &tmp2, sizeof(int));
				++results[j][tmp2-1];
			}
		}

		printf("------------\nA konzultacio eredmenye:\n");
		printf("Centrumvaros: %s\n\n", city);

		for(int i = 0; i < 3; ++i)
		{
			Node* tmp_node = returnById(h, chosenNumbers[i]);
			printf("%s\n", tmp_node->question);
			for(int j = 0; j < ansNums[i]; ++j)
			{
				printf("%s : %i darab szavazat.\n", tmp_node->answers[j], results[i][j]);
			}
			printf("\n");
		}

		close(fd);

		remove("fifo.mlsz");
		destroyLinkedList(h);

	}else // gyerek
	{
		int fd = open("fifo.mlsz", O_RDONLY);
		int tmp;
		read(fd, &tmp, sizeof(int));
		close(fd);
		
		srand(time(NULL));
		int pplNum = (rand() % 11) + 10;

		fd = open("fifo.mlsz", O_RDONLY);
		int q[3];
		read(fd, &q[0], sizeof(int));
		read(fd, &q[1], sizeof(int));
		read(fd, &q[2], sizeof(int));
		close(fd);

		fd = open("fifo.mlsz", O_WRONLY);
		write(fd, &pplNum, sizeof(int));
		for(int i = 0; i < pplNum; ++i)
		{
			for(int j = 0; j < 3; ++j)
			{
				srand((i * j + q[j] * i + q[j] * j) % 26);
				int r = (rand() % q[j]) + 1;
				write(fd, &r, sizeof(int));
			}
		}
		close(fd);
	}
		

	return 0;
}

int* runMenu(Node* h, int* numOfQuestions, int* numOfFinalized)
{
	bool exit = false;
	bool exitFromProgram = false;
	int answer;
	int* nums;

	do
	{
		printf("---- Menu ----\n");
		printf("1. Uj kerdes felvetele\n");
		printf("2. Kerdes modositasa\n");
		printf("3. Kerdes torlese\n");
		printf("4. Kerdesek listazasa\n");
		printf("5. Veglegesites es Kuldes\n");
		printf("6. Kilepes\n");

		printf("Valasz: ");
		scanf("%i", &answer);
		while((getchar())!='\n');
		printf("\n");

		switch(answer)
		{
			case 1:
				newQuestion(h, numOfQuestions);
				break;
			case 2:
				editQuestion(h);
				break;
			case 3:
				deleteQuestion(h, numOfQuestions);
				break;
			case 4:
				listQuestions(h);
				break;
			case 5:
				nums = finalizeQuestions(h, *numOfQuestions, numOfFinalized);
				exit = true;
				break;
			case 6:
				printf("--- Kilepes ---\n");
				exitFromProgram = true;
				break;
			default:
				break;
		}
	}while(!exit && !exitFromProgram);

	if(exitFromProgram) nums = NULL;

	return nums;
}

void newQuestion(Node* h, int* numOfQuestions)
{
	printf("--- Uj kerdes felvetele ---\n");
	Node* p = h;

	while(p->next != NULL) p = p->next;
	p->next = (Node*)malloc(sizeof(Node));
	p = p->next;

	printf("A kerdes szovege: ");
	fgets(p->question, 150, stdin);
	strtok(p->question, "\n");

	do
	{
		printf("Valaszok szama (2, 3, 4): ");
		scanf("%i", &p->answnumber);
		while((getchar())!='\n');
	}
	while(p->answnumber < 2 || p->answnumber > 4);
	

	for(int i = 0; i < p->answnumber; ++i)
	{
		printf("%i. valasz: ", i + 1);
		fgets(p->answers[i], 100, stdin);
		strtok(p->answers[i], "\n");
	}

	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	strftime(p->timeOfQuestion, sizeof(p->timeOfQuestion), "%c", tm);

	p->next = NULL;
	printf("\n");
	++*numOfQuestions;
}

void editQuestion(Node* h)
{
	printf("---- Kerdes modositasa ----\n");

	if(listQuestions(h))
	{
		Node* p = h->next;
		bool modified = false;
		int num;
		printf("A modositando kerdes sorszama: ");
		scanf("%i", &num);
		while((getchar())!='\n');

		int c = 0;
		while(p != NULL && !modified)
		{
			++c;
			if (c == num)
			{
				printf("A kerdes szovege: ");
				fgets(p->question, 150, stdin);
				strtok(p->question, "\n");

				do
				{
					printf("Valaszok szama (2, 3, 4): ");
					scanf("%i", &p->answnumber);
					while((getchar())!='\n');
				}
				while(p->answnumber < 2 || p->answnumber > 4);

				for(int i = 0; i < p->answnumber; ++i)
				{
					printf("%i. valasz: ", i + 1);
					fgets(p->answers[i], 100, stdin);
					strtok(p->answers[i], "\n");
				}

				modified = true;
			}else p = p->next;
		}
		if(modified) printf("A modositas sikeres volt.\n\n");
		else printf("A modositas sikertelen volt.\n\n");
	}
}

void deleteQuestion(Node* h, int* numOfQuestions)
{
	printf("--- Kerdes torlese ---\n");

	if(listQuestions(h))
	{
		Node* p = h->next;
		Node* q = h;

		bool deleted = false;
		int num;
		printf("Torlendo kerdes sorszama: ");
		scanf("%i", &num);
		while((getchar())!='\n');

		int c = 0;
		while(p != NULL && !deleted)
		{
			++c;
			if (c == num)
			{
				q->next = p->next;
				free(p);
				deleted = true;
				--*numOfQuestions;
			}else
			{
				q = p;
				p = p->next;
			}
		}
		if(deleted) printf("A torles sikeres volt.\n");
		else printf("A torles sikertelen volt.\n");
	}

	printf("\n");
}

void destroyLinkedList(Node* h)
{
	while(h != NULL)
	{
		Node* p = h;
		h = p->next;
		free(p);
	}
}

bool contains(int* array, int size, int elem)
{
	for(int i = 0; i < size; ++i)
		if(array[i] == elem) return true;

	return false;
}

bool listQuestions(Node* h)
{
	printf("--- Kerdesek listazasa ---\n");
	Node* p = h->next;
	int id = 1;

	if(p == NULL)
	{
		printf("Nincs meg kerdes.\n\n");
		return false;
	}else
	{
		while(p != NULL)
		{
			printf("%i. %s\n%s\n", id, p->question, p->timeOfQuestion);
			for(int i = 0; i < p->answnumber; ++i)
				printf("\t%i. %s\n", i + 1, p->answers[i]);

			++id;
			p = p->next;
			printf("\n");
		}
	}
	return true;
}


int* finalizeQuestions(Node* h, int numOfQuestions, int* c)
{
	printf("--- Veglegesites/Kuldes---\n");
	if(listQuestions(h))
	{
		printf("Adja meg a vegleges kerdesek sorszamat es a vegen egy '0'-t!\n");
		int* nums = (int*)malloc(numOfQuestions * sizeof(int));
		int tmp;

		do
		{
			do
			{
				printf("Kerdes sorszama (1..%i): ", numOfQuestions);
				scanf("%i", &tmp);
			}while(tmp < 0 || tmp > numOfQuestions);
			nums[*c] = tmp;
			++*c;
		}while(*c < numOfQuestions && tmp != 0);

		printf("A fajl neve: ");
		char filename[100];

		while((getchar())!='\n');
		fgets(filename, 100, stdin);
		strtok(filename, "\n");

		FILE *f = fopen(filename, "w");
		if (f == NULL)
		{
		    printf("Nem sikerult megnyitni!\n");
		    exit(1);
		}

		Node* p = h->next;
		int id = 1;

		while(p != NULL)
		{
			if(contains(nums, *c, id))
			{
				fprintf(f, "%i. %s\n%s\n", id, p->question, p->timeOfQuestion);
				for(int i = 0; i < p->answnumber; ++i)
					fprintf(f, "\t%i. %s\n", i + 1, p->answers[i]);
				fprintf(f, "\n");
			}

			++id;
			p = p->next;
			
		}
		printf("Fajl letrehozva.\n");
		fclose(f);
		if(tmp == 0) --*c;

		return nums;
	}
}


int* getAnsNums(Node* h, int q1, int q2, int q3)
{
	int* retArray = (int*)malloc(3 * sizeof(int));

	Node* p = h->next;
	int id = 1;	

	while(p != NULL)
	{
		if(id == q1) retArray[0] = p->answnumber;
		else if(id == q2) retArray[1] = p->answnumber;
		else if(id == q3) retArray[2] = p->answnumber;

		++id;
		p = p->next;
	}

	return retArray;
}

Node* returnById(Node* h, int id)
{
	Node* p = h->next;
	int tmp_id = 1;

	while(p != NULL)
	{
		if(tmp_id == id) return p;
		p = p->next;
		++tmp_id;
	}
	return NULL;
}