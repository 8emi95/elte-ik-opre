#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

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

int main(int argc, char *argv[])
{
    int fd[2*NUM_PIPES];
    int val = 0, len, i;
    pid_t pid;

    // create all the descriptor pairs we need
    for (i=0; i<NUM_PIPES; ++i)
    {
        if (pipe(fd+(i*2)) < 0)
        {
            perror("Failed to allocate pipes");
            exit(EXIT_FAILURE);
        }
    }

    pid = fork();

    // fork() returns 0 for child process, child-pid for parent process.
    if (pid < 0)
    {
        perror("Failed to fork process");
        return EXIT_FAILURE;
    }

    // if the pid is zero, this is the child process
    if (pid == 0)
    {

        close(fd[P1_READ]);
        close(fd[P1_WRITE]);

        // used for output
        pid = getpid();

        // wait for parent to send us a value
        char gotCity[5][30];
        
        int i;
        for(i = 0; i < 5; i++) {
            sleep(1);
            len = read(fd[P2_READ], &gotCity[i], sizeof(gotCity[i]));
            if (len < 0) {
                perror("Child: Failed to read data from pipe\n");
                exit(EXIT_FAILURE);
            }
            else if (len == 0) {
                // not an error, but certainly unexpected
                fprintf(stderr, "Child: Read EOF from pipe\n");
            } else {
                printf("Child(%d): Received %s\n", pid, gotCity[i]);
            }
        }

   /*     len = read(fd[P2_READ], &gotCity, sizeof(gotCity));
        if (len < 0)
        {
            perror("Child: Failed to read data from pipe");
            exit(EXIT_FAILURE);
        }
        else if (len == 0)
        {
            // not an error, but certainly unexpected
            fprintf(stderr, "Child: Read EOF from pipe");
        } */
    //    else
    //    {
            // report what we received

            // now double it and send it back
            srand(time(NULL));
            val = rand() % 101;

            printf("Child(%d): Sending %d back\n", pid, val);
            if (write(fd[P2_WRITE], &val, sizeof(val)) < 0)
            {
                perror("Child: Failed to write response value\n");
                exit(EXIT_FAILURE);
            }
    //    }

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
    val = 42;
    char city[5][30];

    strcpy(city[0], "Budapest");
    strcpy(city[1], "London");
    strcpy(city[2], "Parizs");
    strcpy(city[3], "Brusszel");
    strcpy(city[4], "Krakko");

    for(i = 0; i < 5; i++) {
        printf("%s\n", city[i]);
    }

    for(i = 0; i < 5; i++) {

        printf("Parent(%d): Sending %s to child\n", pid, city[i]);
        if (write(fd[P1_WRITE], city[i], sizeof(city[i])) != sizeof(city[i]))
        {
            perror("Parent: Failed to send value to child\n ");
            exit(EXIT_FAILURE);
        }

    }
    
    // now wait for a response
    val = 0;
    len = read(fd[P1_READ], &val, sizeof(val));
    if (len < 0)
    {
        perror("Parent: failed to read value from pipe\n");
        exit(EXIT_FAILURE);
    }
    else if (len == 0)
    {
        // not an error, but certainly unexpected
        fprintf(stderr, "Parent(%d): Read EOF from pipe\n", pid);
    }
    else
    {
        // report what we received
        printf("Parent(%d): Received %d\n", pid, val);
    }

    // close down remaining descriptors
    close(fd[P1_READ]);
    close(fd[P1_WRITE]);

    // wait for child termination
    wait(NULL);

    return EXIT_SUCCESS;
}