#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// some macros to make the code more understandable
//  regarding which pipe to use to a read/write operation
//
//  Parent: reads from P1_READ, writes on P1_WRITE
//  Child:  reads from P2_READ, writes on P2_WRITE
#define P1_READ     0
#define P2_WRITE    1
#define P2_READ     2
#define P1_WRITE    3

// the total number of pipe *pairs* we need
#define NUM_PIPES   2

#define NUM_QS_FROM_PARENT 3
#define NUM_MAX_ANS_FROM_CHILD_BY_QS 20
#define MAX_NUM_ANS_OF_QS 4

/*
 * This struct is contains the answers
 * for each question what was sent from the parent
 * Max ans per qs is 20
 */
struct Valaszok {
	int val1[NUM_MAX_ANS_FROM_CHILD_BY_QS];
	int val2[NUM_MAX_ANS_FROM_CHILD_BY_QS];
	int val3[NUM_MAX_ANS_FROM_CHILD_BY_QS];
};


/*
 * Generates random unsigned integer number
 * between two unsigned integer number
 * returns a random unsigned integer number 
 * from the specified interval given by parameters
 * param: @min minimum of the interval
 * param  @max maximum of the interval
 */
unsigned int rand_interval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}

int main(int argc, char *argv[])
{
    int fd[2*NUM_PIPES];
    int val = 0, len, i;
    pid_t pid;
		struct Valaszok gyerek_valaszai;	//a gyerek ebbe teszi bele az adott kérdéshez válogatott válaszok számait
		int szulo_altal_valasztott_kerdesekhez_tartozo_valaszok_szama[NUM_QS_FROM_PARENT];	//a választott kérdésekhez tartozó válaszok száma db

		int gyerek_kapja[NUM_QS_FROM_PARENT];
		int szulo_adja[NUM_QS_FROM_PARENT];
		
		struct Valaszok szulo_kapja;
		struct Valaszok gyerek_adja;
    // create all the descriptor pairs we need
    for (i=0; i<NUM_PIPES; ++i)
    {
        if (pipe(fd+(i*2)) < 0)
        {
            perror("Failed to allocate pipes");
            exit(EXIT_FAILURE);
        }
    }

    // fork() returns 0 for child process, child-pid for parent process.
    if ((pid = fork()) < 0)
    {
        perror("Failed to fork process");
        return EXIT_FAILURE;
    }

    // if the pid is zero, this is the child process
    if (pid == 0)
    {
        // Child. Start by closing descriptors we
        //  don't need in this process
        close(fd[P1_READ]);
        close(fd[P1_WRITE]);

        // used for output
        pid = getpid();

        // wait for parent to send us a value
        read(fd[P2_READ], &gyerek_kapja, sizeof(gyerek_kapja));
        if (gyerek_kapja < 0)
        {
            perror("Child: Failed to read data from pipe");
            exit(EXIT_FAILURE);
        }
        else if (gyerek_kapja == 0)
        {
            // not an error, but certainly unexpected
            fprintf(stderr, "Child: Read EOF from pipe");
        }
        else
        {
            // report what we received
            printf("Child(%d): Received %d, %d, %d\n", pid, gyerek_kapja[0], gyerek_kapja[1], gyerek_kapja[2]);

            // now send back
            val = 2;
						
						gyerek_adja.val1[0] = 9;
						gyerek_adja.val1[1] = 8;
						gyerek_adja.val2[0] = 7;
						gyerek_adja.val2[1] = 6;
						gyerek_adja.val3[0] = 5;
						gyerek_adja.val3[1] = 4;

            printf("Child(%d): Sending\n %d - %d,\n %d - %d,\n %d - %d, back\n", pid, gyerek_adja.val1[0], gyerek_adja.val1[1],
											gyerek_adja.val2[0], gyerek_adja.val2[1], gyerek_adja.val3[0], gyerek_adja.val3[1]);
            if (write(fd[P2_WRITE], &gyerek_adja, sizeof(gyerek_adja)) < 0)
            {
                perror("Child: Failed to write response value");
                exit(EXIT_FAILURE);
            }
        }

        // finished. close remaining descriptors.
        close(fd[P2_READ]);
        close(fd[P2_WRITE]);

        return EXIT_SUCCESS;
    }

    // Parent. close unneeded descriptors
    close(fd[P2_READ]);
    close(fd[P2_WRITE]);

    // used for output
    pid = getpid();

    // send a value to the child
		szulo_adja[0] = 11;
		szulo_adja[1] = 22;
		szulo_adja[2] = 33;
    printf("Parent(%d): Sending %d, %d, %d to child\n", pid, szulo_adja[0], szulo_adja[1], szulo_adja[2]);
    if (write(fd[P1_WRITE], &szulo_adja, sizeof(szulo_adja)) != sizeof(szulo_adja))
    {
        perror("Parent: Failed to send value to child ");
        exit(EXIT_FAILURE);
    }

    // now wait for a response
    read(fd[P1_READ], &szulo_kapja, sizeof(szulo_kapja));
    if (szulo_kapja.val1[0] < 0)
    {
        perror("Parent: failed to read value from pipe");
        exit(EXIT_FAILURE);
    }
    else if (szulo_kapja.val1[0] == 0)
    {
        // not an error, but certainly unexpected
        fprintf(stderr, "Parent(%d): Read EOF from pipe", pid);
    }
    else
    {
        // report what we received
        printf("Parent(%d): Received %d\n", pid, szulo_kapja.val1[0]);
    }

    // close down remaining descriptors
    close(fd[P1_READ]);
    close(fd[P1_WRITE]);

    // wait for child termination
    wait(NULL);

    return EXIT_SUCCESS;
}