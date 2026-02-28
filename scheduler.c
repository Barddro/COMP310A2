#include <stdlib.h>
#include <stdio.h>
#include "shell.h"
#include "scheduler.h"
#include "pcb.h"
#include "vector.h"
#include <pthread.h>

// Scheduler interface for initializing and running the scheduling infrastructure 
// with various policies

//Queue* ready_queue;
ReadyQueue* ready_queue;

// unfortunately had to make these global since didnt set up queue architecture accordingly
pthread_mutex_t ready_queue_mutex;
pthread_cond_t ready_queue_notempty;

// added for multithreading

int mt_enabled = 0;
int scheduler_running = 1; //used for QUIT

int threads_working = 0; // tracks number of threads currently executing job
pthread_cond_t threads_done_cond;
pthread_mutex_t threads_working_mutex;

pthread_t workers[2];

ReadyQueue* init_scheduler() {
    ReadyQueue* rq = malloc(sizeof(ReadyQueue));
    rq->queue = init_q();
    rq->enqueue = enqueue_q;
    rq->enqueuesorted = enqueuesorted_q;
    rq->enqueuehead = enqueuehead_q;
    rq->dequeue = dequeue_q;
    rq->peek = peek_q;
    rq->isempty = isempty_q;
    rq->clear = clear_q;
    return rq;
}

void convert_to_mt_queue(ReadyQueue* rq) { // assume passed ReadyQueue is already initialized
    // updates all methods of instance of ReadyQueue to multithreaded ones
    rq->enqueue = blocking_enqueue_q;
    rq->enqueuesorted = blocking_enqueuesorted_q;
    rq->enqueuehead = blocking_enqueuehead_q;
    rq->dequeue = blocking_dequeue_q;
    rq->peek = blocking_peek_q;
    rq->isempty = blocking_isempty_q;
}

// pre-condition: ready_queue is initialized (i.e. init_scheduler() is called)
int schedule(PCB* pcb, int policy) { 
    
    if (policy == FCFS) {
        return ready_queue->enqueue(ready_queue->queue, pcb);
    } else if (policy == SJF) {
        return ready_queue->enqueuesorted(ready_queue->queue, pcb);
        // return enqueuesorted_q(ready_queue, pcb); old - delete later
    } else if (policy == RR) {
        return ready_queue->enqueue(ready_queue->queue, pcb);
        // return enqueue_q(ready_queue, pcb); old - delete later
    } else if (policy == AGING) {
        return ready_queue->enqueuesorted(ready_queue->queue, pcb);
        // return enqueuesorted_q(ready_queue, pcb);
    } else if (policy == RR30) {
        return ready_queue->enqueue(ready_queue->queue, pcb);
        // return enqueue_q(ready_queue, pcb);
    } else {
        return 1;
    }
}

int run_scheduler_FCFS_SJF();
int run_scheduler_RR();
int run_scheduler_AGING();
int run_scheduler_RR30();
void start_mt_scheduler(int policy);

int run_scheduler(int policy) {
    if (policy == FCFS || policy == SJF) {
        return run_scheduler_FCFS_SJF();  
    } else if (policy == RR) {
        if (!mt_enabled) {
            return run_scheduler_RR();
        } else {
            start_mt_scheduler(RR);
            return 0;
        }
    } else if (policy == AGING) {
        return run_scheduler_AGING();
    } else if (policy == RR30) {
        if (!mt_enabled) {
            return run_scheduler_RR30();
        } else {
            start_mt_scheduler(RR30);
            return 0;
        }
    } else {
        return 1;
    }
}

// scheduler running process for BOTH FCFS and SJF
int run_scheduler_FCFS_SJF() {
    int err_code = 0;
    char* line = NULL;

    while (!ready_queue->isempty(ready_queue->queue)) {
        PCB* curr_process = ready_queue->dequeue(ready_queue->queue);
            
        while (haslines_pcb(curr_process)) {
            line = getline_pcb(curr_process);
            err_code = parseInput(line);
        }

        free_pcb(curr_process); // equivalent to removing the script source code
    }
    return err_code;
}

int run_scheduler_RR() {
    int err_code = 0;
    char* line = NULL;

    while (!ready_queue->isempty(ready_queue->queue)) {

        PCB* curr_process = ready_queue->dequeue(ready_queue->queue);
            
        for (int i = 0; i < 2; i++) {
            if (haslines_pcb(curr_process)) {
                line = getline_pcb(curr_process);
                err_code = parseInput(line);
            }
        }

        if (haslines_pcb(curr_process)) {
            ready_queue->enqueue(ready_queue->queue, curr_process);
        }
        else {
            free_pcb(curr_process); // equivalent to removing the script source code
        }
    }
    return err_code;
}

int run_scheduler_AGING() {
    int err_code = 0;
    char* line = NULL;
    
    while (!ready_queue->isempty(ready_queue->queue)) {
        PCB* curr_process = dequeue_q(ready_queue->queue);
            
        while (haslines_pcb(curr_process) && (ready_queue->isempty(ready_queue->queue) 
        || curr_process->job_score <= ready_queue->peek(ready_queue->queue)->job_score)) {
        //if (haslines_pcb(curr_process)) {
            line = getline_pcb(curr_process);
            err_code = parseInput(line);

            if (!ready_queue->isempty(ready_queue->queue)) {
                Queue* curr = ready_queue->queue->next;
                while (curr) {
                    if (curr->e->job_score > 0) {
                        curr->e->job_score--;
                    }
                    curr = curr->next;
                }
            }
        }
        if (haslines_pcb(curr_process)) {
            ready_queue->enqueuesorted(ready_queue->queue, curr_process);
        } else {
            free_pcb(curr_process); // equivalent to removing the script source code
        }
    }

    return err_code;
}

int run_scheduler_RR30() {
    int err_code = 0;
    char* line = NULL;

    while (!ready_queue->isempty(ready_queue->queue)) {

        PCB* curr_process = ready_queue->dequeue(ready_queue->queue);
            
        for (int i = 0; i < 30; i++) {
            if (haslines_pcb(curr_process)) {
                line = getline_pcb(curr_process);
                err_code = parseInput(line);
            } else {
                break;
            }
        }

        if (haslines_pcb(curr_process)) {
            ready_queue->enqueue(ready_queue->queue, curr_process);
        } else {
            free_pcb(curr_process); // equivalent to removing the script source code
        }
    }
    return err_code;
}

void* worker_RR(void* arg) {
    while (scheduler_running) {

        PCB* curr = ready_queue->dequeue(ready_queue->queue);
        if (!curr) continue;
        
        pthread_mutex_lock(&threads_working_mutex);
        threads_working++;
        pthread_mutex_unlock(&threads_working_mutex);

        for (int i = 0; i < 2; i++) {
            if (haslines_pcb(curr)) {
                char* line = getline_pcb(curr);
                parseInput(line);
            } else {
                break;
            }
        }

        if (haslines_pcb(curr)) {
            ready_queue->enqueue(ready_queue->queue, curr);
        } else {
            free_pcb(curr);
        }
        pthread_mutex_lock(&threads_working_mutex);
        threads_working--;
        pthread_cond_broadcast(&threads_done_cond);
        pthread_mutex_unlock(&threads_working_mutex);
    }
    return NULL;
}

void* worker_RR30(void* arg) {
    while (scheduler_running) {
        
        PCB* curr = ready_queue->dequeue(ready_queue->queue);
        if (!curr) continue;

        pthread_mutex_lock(&threads_working_mutex);
        threads_working++;
        pthread_mutex_unlock(&threads_working_mutex);
        
        for (int i = 0; i < 30; i++) {
            if (haslines_pcb(curr)) {
                char* line = getline_pcb(curr);
                parseInput(line);
            } else {
                break;
            }
        }

        if (haslines_pcb(curr)) {
            ready_queue->enqueue(ready_queue->queue, curr);
        } else {
            free_pcb(curr);
        }
        pthread_mutex_lock(&threads_working_mutex);
        threads_working--;
        pthread_cond_broadcast(&threads_done_cond);
        pthread_mutex_unlock(&threads_working_mutex);
    }
    return NULL;
}

void start_mt_scheduler(int policy) {
    scheduler_running = 1;

    pthread_mutex_init(&ready_queue_mutex, NULL);
    pthread_cond_init(&ready_queue_notempty, NULL);

    pthread_mutex_init(&threads_working_mutex, NULL);
    pthread_cond_init(&threads_done_cond, NULL);

    convert_to_mt_queue(ready_queue);

    if (policy == RR) {
        pthread_create(&workers[0], NULL, worker_RR, NULL);
        pthread_create(&workers[1], NULL, worker_RR, NULL);
    }

    if (policy == RR30) {
        pthread_create(&workers[0], NULL, worker_RR30, NULL);
        pthread_create(&workers[1], NULL, worker_RR30, NULL);
    }
}