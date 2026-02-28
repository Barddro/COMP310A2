#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../scheduler.h"
#include "queue.h"

// Queue interface for PCBs that the ReadyQueue struct wraps -
// we used this instead of storing a 'next' pointer in each PCB.
// Note: Uses dummy node for edge case handling

// Initializes queue
Queue* init_q() {
    Queue* dummy = createnode_q(0);
    return dummy;
}

// Helper method for other queue operations
Queue* createnode_q(PCB* e) {
    Queue* node = malloc(sizeof(Queue));
    node->e = e;
    node->next = 0; // null ptr
    return node;
}

// Enqueues PCB to the back of the queue
int enqueue_q(Queue* q, PCB* e) {
    Queue* curr = q;
    while (curr->next) {
        curr = curr->next;
    }
    Queue* new = createnode_q(e);
    curr->next = new;

    return 0;
}

// Enqueues PCB into specified queue in sorted order by job score
// Pre-condition: Queue is sorted ascending by job score
// Note: PCBs with equal job scores and enqueued after pre-existing ones
int enqueuesorted_q(Queue* q, PCB* e) {
    Queue* curr = q;

    // stop at node before insertion point
    while (curr->next && curr->next->e->job_score <= e->job_score) {
        curr = curr->next;
    }

    Queue* newnode = createnode_q(e);
    newnode->next = curr->next;
    curr->next = newnode;

    return 0;
}

// Enqueues PCB at the head of the specified queue - used for background mode scheduling
int enqueuehead_q(Queue* q, PCB* e) {
    Queue* temp = q->next;
    Queue* newhead = createnode_q(e);
    q->next = newhead;
    newhead->next = temp;
    return 0;
}

// Dequeues a PCB from the specified queue
PCB* dequeue_q(Queue* q) {
    if (!q->next) {
        printf("queue is empty");
        return NULL; // empty queue
    }

    Queue* first = q->next;
    PCB* value = first->e;

    q->next = first->next;
    free(first);

    return value;
}

// Returns but does not dequeue the first PCB of the queue
PCB* peek_q(Queue* q) {
    if (!q->next) return NULL;
    return q->next->e;
}

// Checks if queue is empty
int isempty_q(Queue* q) {
    return (!q->next);
}

// Dequeues from the queue until it is empty
void clear_q(Queue* q) {
    while (!isempty_q(q)) {
        dequeue_q(q);
    }
}

// Frees queue from heap
void free_q(Queue* q) {
    Queue* curr = q;
    while (curr) {
        Queue* temp = curr;
        curr = curr->next;
        free(temp);
    }
}

// blocking (threadsafe) methods - used for multithreaded scheduling

int blocking_enqueue_q(Queue* q, PCB* e) {
    Queue* curr = q;

    pthread_mutex_lock(&ready_queue_mutex);
    
    while (curr->next) {
        curr = curr->next;
    }
    Queue* new = createnode_q(e);
    curr->next = new;
    
    pthread_cond_signal(&ready_queue_notempty);
    pthread_mutex_unlock(&ready_queue_mutex);


    return 0;
}

int blocking_enqueuesorted_q(Queue* q, PCB* e) {
    Queue* curr = q;
    Queue* newnode = createnode_q(e);

    pthread_mutex_lock(&ready_queue_mutex);

    // stop at node BEFORE insertion point
    while (curr->next && curr->next->e->job_score <= e->job_score) {
        curr = curr->next;
    }

    newnode->next = curr->next;
    curr->next = newnode;

    pthread_cond_signal(&ready_queue_notempty);
    pthread_mutex_unlock(&ready_queue_mutex);

    return 0;
}

int blocking_enqueuehead_q(Queue* q, PCB* e) {

    Queue* newhead = createnode_q(e);
    
    pthread_mutex_lock(&ready_queue_mutex);

    Queue* temp = q->next;
    q->next = newhead;
    newhead->next = temp;

    pthread_cond_signal(&ready_queue_notempty);
    pthread_mutex_unlock(&ready_queue_mutex);

    return 0;
}

PCB* blocking_dequeue_q(Queue* q) {

    pthread_mutex_lock(&ready_queue_mutex);
    // Also check scheduler_running so workers can wake up and exit
    // when quit() sets it to 0 and broadcasts ready_queue_notempty.
    while (!q->next && scheduler_running) {
        pthread_cond_wait(&ready_queue_notempty, &ready_queue_mutex);
    }

    // Woke up due to shutdown with nothing left — signal exit to caller.
    if (!q->next) {
        pthread_mutex_unlock(&ready_queue_mutex);
        return NULL;
    }

    Queue* first = q->next;
    PCB* value = first->e;

    q->next = first->next;
    free(first);

    pthread_mutex_unlock(&ready_queue_mutex);

    return value;
}


PCB* blocking_peek_q(Queue* q) {
    PCB* res;
    pthread_mutex_lock(&ready_queue_mutex);
    if (!q->next) {
        res = NULL;
    } else {
        res = q->next->e;
    }
    pthread_mutex_unlock(&ready_queue_mutex);
    return res;
}

int blocking_isempty_q(Queue* q) {
    pthread_mutex_lock(&ready_queue_mutex);
    int res = !q->next;
    pthread_mutex_unlock(&ready_queue_mutex);
    return res;
}