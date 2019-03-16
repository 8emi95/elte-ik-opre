#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/ipc.h>
#include <signal.h>

typedef struct ListItem
{
	void* data;
	struct ListItem* next;
} Node;

typedef struct WishItem
{
	char* name;
	char* city;
	char* address;
	char* present;
	char* time;
} Wish;

typedef struct LinkedList
{
    Node* first;
    Node* last;
} List;

typedef struct AssistInfo
{
    int pipe1;
    int pipe2;
    int queue;
    int child_pid;
} Info;

typedef struct QueueMessage
{
    long mtype;
    char mtext[100];
} Message;

Info* info;

void initList(List* this)
{
	this->first = NULL;
	this->last = NULL;
}

void addToList(List* this, void* data)
{
    Node* node = malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;

    if (this->first == NULL)
        this->first = this->last = node;
    else
    {
        this->last->next = node;
        this->last = node;
    }
}

void emptyList(List* this)
{
    Node* p = this->first;
    while (p != NULL)
    {
        Node* q = p;
        p = p->next;
        free(q);
    }
    initList(this);
}

char** splitString(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    count += last_comma < (a_str + strlen(a_str) - 1);
    count++;
    result = malloc(sizeof(char*) * count);
    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    return result;
}

void splitData(Wish* wish, char* str)
{
    char** tokens;
    tokens = splitString(str, ',');

    if (tokens)
    {
        int i;
        for (i = 0; *(tokens + i); i++)
        {
            char* trimmed = *(tokens + i);
            strtok(trimmed, "\n");

            if (i == 0)
            {
                char* name = malloc(strlen(trimmed)+1);
                strcpy(name, trimmed);
                wish->name = name;
            }
            else if (i == 1)
            {
                char* city = malloc(strlen(trimmed)+1);
                strcpy(city, trimmed);
                wish->city = city;
            }
            else if (i == 2)
            {
                char* address = malloc(strlen(trimmed)+1);
                strcpy(address, trimmed);
                wish->address = address;
            }
            else if (i == 3)
            {
                char* present = malloc(strlen(trimmed)+1);
                strcpy(present, trimmed);
                wish->present = present;
            }
            else if (i == 4)
            {
                char* time = malloc(strlen(trimmed)+1);
                strcpy(time, trimmed);
                wish->time = time;
            }
            free(*(tokens + i));
        }
        free(tokens);
    }
}

void loadFromFile(char* fileName, List* list)
{
    FILE* f;
    f = fopen(fileName,"r");
    if (f == NULL)
    {
        perror("Error occured when opening file");
        exit(1);
    }
    char line[160];

    fseek(f, 0, SEEK_END);
    if (!ftell(f) == 0)
    {
        fseek(f, 0, SEEK_SET);
        while (!feof(f))
        {
            fgets(line, sizeof(line), f);
            Wish* wish = malloc(sizeof(Wish));
            splitData(wish, line);
            addToList(list, wish);
        }
    }
    fclose(f);
}

void saveToFile(char* fileName, List* list)
{
    FILE* f;
    f = fopen(fileName,"w");
    if (f == NULL)
    {
        perror("Error occured when opening file.\n");
        exit(1);
    }

    Node* p = list->first;
    while (p != NULL)
    {
        Wish* wish = p->data;
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%s,%s,%s,%s,%s", (char*)wish->name, (char*)wish->city, (char*)wish->address, (char*)wish->present, (char*)wish->time);

        if(p == list->last)
        {
            fprintf(f, "%s", buffer);
        }
        else
        {
            fprintf(f, "%s\n", buffer);
        }

        p = p->next;
    }
    fclose(f);
}

void printUniqueList(List* list)
{
    int i = 0;
    Node* q = list->first;
    while (q != NULL)
    {
        i++;
        printf("%d. %s\n", i, (char*)q->data);
        q = q->next;
    }
}

char* getDataFromList(List* list, int id)
{
    int i = 0;
    Node* q = list->first;
    while (q != NULL)
    {
        char* str = (char*)q->data;

        i++;
        if (i == id)
        {
            char* datastr = malloc(strlen(str) + 1);
            strcpy(datastr, str);
            return datastr;
        }
        q = q->next;
    }
}

List getUniqueCities(List* list)
{
    List tempList;
    initList(&tempList);
    List* cityList = &tempList;

    Node* p = list->first;
    while (p != NULL)
    {
        Wish* wish = (Wish*)p->data;
        char* wishCity = (char*)wish->city;

        int alreadyInList = 0;
        Node* q = cityList->first;
        while (q != NULL && alreadyInList != 1)
        {
            char* city = q->data;
            if (strcmp(wish->city, city) == 0)
            {
                alreadyInList = 1;
            }
            q = q->next;
        }

        if (!alreadyInList)
        {
            char* citystr = malloc(strlen(wish->city) + 1);
            strcpy(citystr, wish->city);
            addToList(cityList, citystr);
        }

        p = p->next;
    }

    return tempList;
}

List getUniquePresents(List* list)
{
    List tempList;
    initList(&tempList);
    List* presentList = &tempList;

    Node* p = list->first;
    while (p != NULL)
    {
        Wish* wish = (Wish*)p->data;
        char* wishPresent = (char*)wish->present;

        int alreadyInList = 0;
        Node* q = presentList->first;
        while (q != NULL && alreadyInList != 1)
        {
            char* present = q->data;
            if (strcmp(wish->present, present) == 0)
            {
                alreadyInList = 1;
            }
            q = q->next;
        }

        if (!alreadyInList)
        {
            char* presentstr = malloc(strlen(wish->present) + 1);
            strcpy(presentstr, wish->present);
            addToList(presentList, presentstr);
        }

        p = p->next;
    }

    return tempList;
}

void sendToCity(List* list, char* city, Info* info)
{
    int write_status;

    char* operation = "city";
    write_status = write(info->pipe1, operation, strlen(operation) + 1);
    if (write_status == -1)
    {
        perror("pipe 1 write");
    }

    write_status = write(info->pipe2, city, strlen(city) + 1);
    if (write_status == -1)
    {
        perror("pipe 2 write");
    }

    // Wake child process to do the work
    kill(info->child_pid, SIGUSR1);
    pause();

    Message msg;

    int queue_status = msgrcv(info->queue, &msg, 100, 1, 0);
    if (queue_status < 0)
    {
        perror("msgrcv");
    }
    else
    {
        printf("A szan uzenete: %s\n", msg.mtext);
        emptyList(list);
        loadFromFile("wishes.txt", list);
    }
}

void sendPresents(List* list, char* present, Info* info)
{
    int write_status;

    char* operation = "present";
    write_status = write(info->pipe1, operation, strlen(operation) + 1);
    if (write_status == -1)
    {
        perror("pipe 1 write");
    }

    write_status = write(info->pipe2, present, strlen(present) + 1);
    if (write_status == -1)
    {
        perror("pipe 2 write");
    }

    // Wake child process to do the work
    kill(info->child_pid, SIGUSR1);
    pause();

    Message msg;

    int queue_status = msgrcv(info->queue, &msg, 100, 1, 0);
    if (queue_status < 0)
    {
        perror("msgrcv");
    }
    else
    {
        printf("A szan uzenete: %s\n", msg.mtext);
    }
}

void askForCity(List* list)
{
    system("clear");
    printf("Melyik varosba szeretnel szallitani?\n");

    List tempList = getUniqueCities(list);
    List* cityList = &tempList;

    printUniqueList(cityList);

    printf("0. Vissza a menube\n");
    char buffer[64];
    fgets(buffer, sizeof(buffer), stdin);
    int choice = atoi(buffer);
    if (choice != 0)
    {
        char* city = getDataFromList(cityList, choice);
        sendToCity(list, city, info);
    }
}

void askForPresent(List* list)
{
    system("clear");
    printf("Melyik ajandekot szeretned szallitani?\n");

    List tempList = getUniquePresents(list);
    List* presentList = &tempList;

    printUniqueList(presentList);

    printf("0. Vissza a menube\n");
    char buffer[64];
    fgets(buffer, sizeof(buffer), stdin);
    int choice = atoi(buffer);
    if (choice != 0)
    {
        char* present = getDataFromList(presentList, choice);
        sendPresents(list, present, info);
    }
}

void showMenu(List* wishList, List* deliveryList)
{
    int isRunning = 1;

    while (isRunning)
    {
        printf("\nValassz egy opciot!\n\n");
        printf("1. Szallitas varos szerint\n");
        printf("2. Szallitas ajandek szerint\n");
        printf("3. Kilepes\n");

        char buffer[64];
        fgets(buffer, sizeof(buffer), stdin);
        int choice = atoi(buffer);

        switch(choice)
        {
        case 1:
            askForCity(wishList);
            emptyList(wishList);
            loadFromFile("wishes.txt", wishList);
            emptyList(deliveryList);
            loadFromFile("delivery.txt", deliveryList);
            break;
        case 2:
            askForPresent(wishList);
            emptyList(wishList);
            loadFromFile("wishes.txt", wishList);
            emptyList(deliveryList);
            loadFromFile("delivery.txt", deliveryList);
            break;
        case 3:
            isRunning = 0;
            break;
        }
    }
}

void handler(int sig, siginfo_t* info, void* context)
{
    //printf ("beerkezo jelzes (%d). pid: %d\n", sig, info->si_pid);
    switch (sig)
    {
        case 10:
            // Signal from parent process
            break;
        case 12:
            // Signal from child process
            break;
    }
}

void printList(List* list)
{
    Node* p = list->first;
    while (p != NULL)
    {
        Wish* wish = (Wish*)p->data;
        printf("city: %s, present: %s\n", wish->city, wish->present);
        p = p->next;
    }
}

void deliverToCity(List* wishList, List* deliveryList, char* city)
{
    Node* p = wishList->first;
    Node* prev = p;
    while(p != NULL)
    {
        Wish* wish = (Wish*)p->data;

        if (strcmp(wish->city, city) == 0)
        {
            if (p == wishList->first)
            {
                Node* q = wishList->first;
                wishList->first = wishList->first->next;
                p = wishList->first;
                prev = p;
                addToList(deliveryList, wish);
            }
            else if (p == wishList->last)
            {
                prev->next = NULL;
                wishList->last = prev;
                p = NULL;
                addToList(deliveryList, wish);
            }
            else
            {
                Node* q = p;
                prev->next = p->next;
                p = p->next;
                addToList(deliveryList, wish);
            }
        }
        else
        {
            prev = p;
            p = p->next;
        }
    }
}

void deliverPresent(List* wishList, List* deliveryList, char* present)
{
    Node* p = wishList->first;
    Node* prev = p;
    while(p != NULL)
    {
        Wish* wish = (Wish*)p->data;

        if (strcmp(wish->present, present) == 0)
        {
            if (p == wishList->first)
            {
                Node* q = wishList->first;
                wishList->first = wishList->first->next;
                p = wishList->first;
                prev = p;
                addToList(deliveryList, wish);
            }
            else if (p == wishList->last)
            {
                wishList->last = prev;
                p = NULL;
                addToList(deliveryList, wish);
            }
            else
            {
                Node* q = p;
                prev->next = p->next;
                p = p->next;
                addToList(deliveryList, wish);
            }
        }
        else
        {
            prev = p;
            p = p->next;
        }
    }
}

int main(int argc, char** argv)
{
    // Initialize signal handling
    struct sigaction sigact;
    sigact.sa_sigaction = handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_RESTART;
    sigaction(SIGUSR1,&sigact,NULL);
    sigaction(SIGUSR2,&sigact,NULL);

    List wishList;
    initList(&wishList);
    loadFromFile("wishes.txt", &wishList);

    List deliveryList;
    initList(&deliveryList);
    loadFromFile("delivery.txt", &deliveryList);

    // Save parent PID
    pid_t parent_pid = getpid();
    pid_t child_pid;

    // Create pipes
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    int pipe2fd[2];
    if (pipe(pipe2fd) == -1)
    {
        perror("pipe2");
        exit(EXIT_FAILURE);
    }

    // Create message queue
    int queue;
    key_t queue_key;
    int queue_status;

    queue_key = ftok(argv[0], 1);
    queue = msgget(queue_key, 0600 | IPC_CREAT);
    if (queue < 0)
    {
        perror("msgget");
        return 1;
    }

    // Differentiate parent and child process
    if (getpid() == parent_pid)
    {
        // Fork child process
        if ((child_pid = fork()) < 0)
        {
            perror("fork");
            abort();
        }
        // Child process
        else if (child_pid == 0)
        {
            kill(getppid(), SIGUSR2);
            // Wait for signal of parent process
            pause();

            while (1)
            {
                int read_status;

                // Read the operation from the first pipe
                char operation[10];
                read_status = read(pipefd[0], operation, sizeof(operation));
                if (read_status == -1)
                {
                    perror("pipe 1 read");
                }

                if (strcmp(operation, "shutdown") == 0)
                {
                    exit(0);
                }
                else
                {
                    // Read the data from the second pipe
                    char data[100];
                    read_status = read(pipe2fd[0], data, sizeof(data));
                    if (read_status == -1)
                    {
                        perror("pipe 2 read");
                    }

                    Message msg;
                    msg.mtype = 1;

                    if (strcmp(operation, "city") == 0)
                    {
                        deliverToCity(&wishList, &deliveryList, data);
                        sprintf(msg.mtext, "Ajandekok kiszallitva ide: %s.", data);
                    }
                    else if (strcmp(operation, "present") == 0)
                    {
                        deliverPresent(&wishList, &deliveryList, data);
                        sprintf(msg.mtext, "Az osszes %s kiszallitva.", data);
                    }

                    // Save changes
                    saveToFile("wishes.txt", &wishList);
                    saveToFile("delivery.txt", &deliveryList);

                    // Notify parent process about completion
                    int status = msgsnd(queue, &msg, strlen(msg.mtext) + 1, 0);
                    if (status < 0)
                    {
                        perror("msgsnd");
                    }

                    // Wake parent to finish up
                    kill(getppid(), SIGUSR2);

                    // Wait for signal to work
                    pause();
                }
            }
        }
        else
        // Parent process
        {
            // Wait for the child process to initialize
            pause();
            // Close reading end of pipes
            close(pipefd[0]);
            close(pipe2fd[0]);

            // Store important info for later use
            info = malloc(sizeof(Info));
            info->pipe1 = pipefd[1];
            info->pipe2 = pipe2fd[1];
            info->queue = queue;
            info->child_pid = child_pid;

            // Show menu with available options
            showMenu(&wishList, &deliveryList);

            char* shutdownMessage = "shutdown";
            int write_status;

            write_status = write(info->pipe1, shutdownMessage, strlen(shutdownMessage) + 1);
            if (write_status == -1)
            {
                perror("pipe 1 write on shutdown");
            }
            kill(info->child_pid, SIGUSR1);

            // Close writing end of pipes
            close(pipefd[1]);
            close(pipe2fd[1]);

            // Destroy message queue
            queue_status = msgctl(queue, IPC_RMID, NULL);
            if (queue_status < 0)
            {
                perror("msgctl");
            }
        }
    }

	return 0;
}
