#include <stdlib.h>

#include "shell.h"
#include "scheduler.h"
#include "pcb.h"
#include "vector.h"


Queue* ready_queue;

//Scheduler* scheduler;

/*
void init_scheduler() {
    scheduler = malloc(sizeof(Scheduler));
    scheduler->FCFS = init_q();
    scheduler->RR = init_q();

}
*/

 //RENAME THIS FUNCTION
void init_scheduler() {
    ready_queue = init_q();
}

// pre-condition: ready_queue is initialized (i.e. init_scheduler() is called)
int schedule(PCB* pcb, int policy) { 
    
    if (policy == FCFS) {
        return enqueue_q(ready_queue, pcb);
    } else if (policy == SJF) {
        return enqueuesorted_q(ready_queue, pcb);
    } else if (policy == RR) {
        return enqueue_q(ready_queue, pcb);
    } else if (policy == AGING) {
        return enqueuesorted_q(ready_queue, pcb);
    } else {
        return 1;
    }
}

int run_scheduler_FCFS_SJF();
int run_scheduler_RR();
int run_scheduler_AGING();

int run_scheduler(int policy) {

    if (policy == FCFS || policy == SJF) {
        return run_scheduler_FCFS_SJF();  
    } else if (policy == RR) {
        return run_scheduler_RR();
    } else if (policy == AGING) {
        return run_scheduler_AGING();
    } else {
        return 1;
    }
}

int run_scheduler_FCFS_SJF() {
    int err_code = 0;
    char* line = NULL;

    while (!isempty_q(ready_queue)) {
        PCB* curr_process = dequeue_q(ready_queue);
            
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

    while (!isempty_q(ready_queue)) {

        PCB* curr_process = dequeue_q(ready_queue);
            
        for (int i = 0; i < 2; i++) {
            if (haslines_pcb(curr_process)) {
                line = getline_pcb(curr_process);
                err_code = parseInput(line);
            }
        }

        if (haslines_pcb(curr_process)) {
            enqueue_q(ready_queue, curr_process);
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

    Vector* bryan_johnson = init_vec(10); //get it, because we're 'de-aging' the pcbs
    // for reference: https://www.youtube.com/watch?v=ifFF2-0EPxM
    
    while (!isempty_q(ready_queue)) {
        PCB* curr_process = dequeue_q(ready_queue);
            
        if (haslines_pcb(curr_process)) {
            line = getline_pcb(curr_process);
            err_code = parseInput(line);

            while (!isempty_q(ready_queue)) {
                PCB* curr = dequeue_q(ready_queue);
                curr->job_score--;
                append_vec(bryan_johnson, curr);
            }
            while (bryan_johnson->length > 0) {
                PCB* to_enqueue = remove_vec(bryan_johnson);
                enqueuesorted_q(ready_queue, to_enqueue);
            }
        }

        if (haslines_pcb(curr_process)) {
            enqueuesorted_q(ready_queue, curr_process);
        } else {
            free_pcb(curr_process); // equivalent to removing the script source code
        }
    }

    return err_code;
}

