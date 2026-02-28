#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "queue.h"
#include "pcb.h"

enum Policies {
    FCFS,
    SJF,
    RR,
    AGING,
    RR30
};

typedef struct {
    Queue* queue; // actual queue (normal or blocking)
    int (*enqueue)(Queue*, PCB*);
    int (*enqueuesorted)(Queue*, PCB*);
    int (*enqueuehead)(Queue*, PCB*);
    PCB* (*dequeue)(Queue*);
    PCB* (*peek)(Queue*);
    int (*isempty)(Queue*);
    void (*clear)(Queue*);
} ReadyQueue;

extern ReadyQueue* ready_queue;
extern int policy;
extern pthread_mutex_t ready_queue_mutex;
extern pthread_cond_t ready_queue_notempty;
extern int mt_enabled;
extern int scheduler_running;
extern int threads_working;
extern pthread_mutex_t threads_working_mutex;
extern pthread_cond_t threads_done_cond;
extern pthread_t workers[2];
ReadyQueue* init_scheduler();
int schedule(PCB* pcb, int policy);
int run_scheduler(int policy);

#endif
