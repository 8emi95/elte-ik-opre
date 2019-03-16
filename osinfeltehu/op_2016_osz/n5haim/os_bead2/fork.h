#ifndef FORK_H
#define FORK_H

#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif
#ifndef __USE_POSIX199309
#define __USE_POSIX199309
#endif

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <spawn.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "event_entry.h"
#include "registry_entry.h"

#define READ (0)
#define WRITE (1)
#define SIG_TO_SEND SIGUSR1

char chance(double _ch)
{
    double roll = (double)rand() / (double)RAND_MAX;
    printf("Roll: %f/%f\nRet: %d\n",roll,_ch,roll < _ch);
    return roll < _ch;
}

void doChildProcess(int pipeFromParent,int pipeToParent)
{
    srand(time(NULL));
    /* Wait for parent */
    sleep(2);
    
    /* First we receive the event name */
    event_entry myEvent;
    read_event_from_file(&myEvent,pipeFromParent);

    /* Travel to location, then notify parent of arrival */
    printf("[CHILD] Traveling to location of %s...\n",myEvent.name);
    sleep(3);
    
    kill(getppid(),SIG_TO_SEND);

    registry_entry* entries;
    printf("[CHILD] Receiving entry data...\n");
    unsigned numEntries = read_entries_from_file(&entries,pipeFromParent);
    char* noShowStats = calloc(numEntries,sizeof(unsigned));
    for(unsigned procEntries = 0;
	procEntries != numEntries;
	++procEntries)
    {
	printf("[CHILD] Got guest %s\n",entries[procEntries].name);
	noShowStats[procEntries] = chance(0.1);
    }

    printf("[CHILD] Returning event rating...\n");
    unsigned rating = rand() % 101;
    write(pipeToParent,&rating,sizeof(rating));
    printf("[CHILD] Returning no-show statistics...\n");
    write(pipeToParent,noShowStats,numEntries*sizeof(char));

    printf("[CHILD] Done.\n");
    close(pipeFromParent);
    close(pipeToParent);
}

void doParentProcess(int pipeToChild, int pipeFromChild,
		     registry_entry* entries, unsigned num_entries,
		     event_entry* targetEvent)
{
    sigset_t sigMask, oldSigMask;
    
    /* Set up the mask of signals to temporarily block. */
    sigemptyset (&sigMask);
    sigaddset (&sigMask, SIG_TO_SEND);
    sigprocmask(SIG_BLOCK, &sigMask, &oldSigMask);

    /* Filter out attendees */
    registry_entry* attendees = calloc(num_entries,sizeof(*attendees));
    unsigned numAttendees = 0;

    /* Create shallow copies of attendee data */
    for(unsigned ind = 0; ind != num_entries; ++ind)
    {
	if(entries[ind].event_id == targetEvent->id)
	{
	    attendees[numAttendees] = entries[ind];
	    ++numAttendees;
	}
    }

    /* Wait for child */
    sleep(1);
    
    /* Send event data to child */
    printf("[PARENT] Sending event data...\n");
    write_event_to_file(targetEvent,pipeToChild);
    
    /* Wait for signal from child to arrive */
    printf("[PARENT] Data sent, waiting for child to arrive...\n");
    while(sigwaitinfo(&sigMask,NULL) < 0);
    sigprocmask(SIG_UNBLOCK, &sigMask, &oldSigMask);
    printf("[PARENT] Received signal, sending attendee info...\n");
    
    /* Send attendee data to child */
    write_entries_to_file(pipeToChild,attendees,numAttendees);

    /* Receive event rating from child */
    printf("[PARENT] Receiving event rating...\n");
    unsigned rating;
    read(pipeFromChild,&rating,sizeof(rating));
    printf("[PARENT] Rating: %u/100\n",rating);

    /* Receive no-show statistics from child */
    printf("[PARENT] Receiving no-show statistics...\n");
    for(int ind = 0; ind != numAttendees; ++ind)
    {
	char missed;
	read(pipeFromChild,&missed,sizeof(missed));
	if(missed)
	{
	    printf("[PARENT] Attendee %s did not show up.\n",attendees[ind].name);
	}
    }

    printf("[PARENT] Done.\n");
}

void handleEventWithChild(registry_entry* entries,size_t num_entries,
                          event_entry* targetEvent)
{
    /* Parent to child pipe */
    int fd_ptoc[2];
    pipe(fd_ptoc);
    
    /* Child to parent pipe */
    int fd_ctop[2];
    pipe(fd_ctop);
    
    pid_t pid = fork();
    if(pid < 0)
    {
	fprintf(stderr,"Error forking child!\n");
    }
    else if (pid == 0)
    {
	close(fd_ptoc[WRITE]);
	close(fd_ctop[READ]);
	doChildProcess(fd_ptoc[READ],fd_ctop[WRITE]);
	exit(0);
    }
    else
    {
	close(fd_ptoc[READ]);
	close(fd_ctop[WRITE]);
	doParentProcess(fd_ptoc[WRITE],fd_ctop[READ],
			entries,num_entries,
			targetEvent);
    }
}

#endif // FORK_H
