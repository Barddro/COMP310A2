#include "shell.h"
#include "scheduler.h"
#include "pcb.h"
#include <stdlib.h>

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
        return 0;
    } else if (policy == RR) {
        return 0;
    } else {
        return 0;
    }
}

int run_scheduler(int policy) {

    int err_code = 0;

    while (!isempty_q(ready_queue)) {

        PCB* curr_process = dequeue_q(ready_queue);

        if (policy == FCFS) {
            
            while (curr_process->pc < curr_process->lines->length) {
                char* line = getline_pcb(curr_process);
                err_code = parseInput(line);
            }

            free_pcb(curr_process); // equivalent to removing the script source code

        } 
        /*
        else if (policy == SJF) {
            // ...
        }
        */

    }

    return err_code;
}

