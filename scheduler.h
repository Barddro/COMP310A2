#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "queue.h"
#include "pcb.h"

/*
typedef struct {
    Queue* FCFS;
    Queue* SJF;
    Queue* RR;
} Scheduler;
*/

enum Policies {
    FCFS,
    SJF,
    RR,
    AGING,
    RR30
};

extern Queue* ready_queue;
extern int policy;
void init_scheduler();
int schedule(PCB* pcb, int policy);
int run_scheduler(int policy);

#endif
