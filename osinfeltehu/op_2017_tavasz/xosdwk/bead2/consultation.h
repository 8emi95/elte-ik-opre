#ifndef _CONSULTATION_H_
#define _CONSULTATION_H_

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

#include "survey.h"

void beginConsultation(Survey * surv, char * city) {
    if ( surv->qnum < 3 ) {
        printf("Nincs eleg kerdes a veglegesitett kerdoiven!\n");
        return;
    }
    pid_t interviewer = fork();

    if ( interviewer > 0 ) {
        // "Base"
        int qs[3];
        int qsi = 0;
        while (qsi < 3) {
            int randind = rand()%surv->qnum;
            printf("%d\n",randind);
            int i = 0;
            int unique = 1;
            while ( unique && i < qsi ) {
                if ( qs[i] == randind ) {
                    unique = 0;
                }
            }
            if (unique) {
                qs[qsi] = randind;
                ++qsi;
            }
        }
    }
    else {
        // "Interviewer"
    }


}

#endif
