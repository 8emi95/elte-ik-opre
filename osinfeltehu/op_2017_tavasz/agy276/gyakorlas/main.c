//FONTOS: fordítás -lpthread-el linkelés miatt
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void func1(void *ptr);
void func2(void *ptr);

char buf[100];
sem_t mutex;

int main(int arg, char *args[]){

	pthread_t thread1;
	pthread_t thread2;

	char *msg1 = "Thread 1.";
	char *msg2 = "Thread 2.";

	sem_init(&mutex, 0, 1); //0 --> shared between threads
				//1 --> binary semaphor

	pthread_create(&thread1, NULL, (void *) &func1, (void *) msg1);
	pthread_create(&thread2, NULL, (void *) &func2, (void *) msg2);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	sem_destroy(&mutex);

return 0;
}

void func1(void *ptr)
{

	char *msg = (char *)ptr;

	printf("%s\n", msg);

	while(1)
	{
		sem_wait(&mutex);
		printf("Üzenet: \n");
		fgets(buf, 100, stdin);
		if(buf == "kill\n")
			break;
		sem_post(&mutex);
	}

	//sem_wait(&mutex);
	//fgets(buf, 100, stdin);
	//sprintf(buf, "%s", "Mutex t1-ben");
	//sem_post(&mutex);

	printf("Thread1 terminated.\n");

	pthread_exit(0);

}

void func2(void *ptr)
{

	while(1)
	{
		sem_wait(&mutex);
		printf("T2 reading message: %s\n", buf);
		if(buf == "kill2")
			break;
		sem_post(&mutex);
	}
	
	//sem_wait(&mutex);
	//printf("Valami: %s\n", buf);
	//sem_post(&mutex);

	printf("Thread2 terminated.\n");

	pthread_exit(0);

}
