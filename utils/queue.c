#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "../scheduler.h"
// Queue implementation using dummy node for edge case handling

//MUTEX HERE!

// GET GLOBAL FLAG IS_MULTITHREADED THAT CHANGES HOW ENQUEUE/DEQUEUE WORKS

Queue* init_q() {
    Queue* dummy = createnode_q(0);
    return dummy;
}

Queue* createnode_q(PCB* e) {
    Queue* node = malloc(sizeof(Queue));
    node->e = e;
    node->next = 0; // null ptr
    return node;
}

int enqueue_q(Queue* q, PCB* e) {
    Queue* curr = q;
    while (curr->next) {
        curr = curr->next;
    }
    Queue* new = createnode_q(e);
    curr->next = new;

    return 0;

    // do we want to return old head of q here?
}

int enqueuesorted_q(Queue* q, PCB* e) {
    Queue* curr = q;

    // stop at node BEFORE insertion point
    while (curr->next && curr->next->e->job_score <= e->job_score) {
        curr = curr->next;
    }

    Queue* newnode = createnode_q(e);
    newnode->next = curr->next;
    curr->next = newnode;

    return 0;
}

int enqueuehead_q(Queue* q, PCB* e) {
    Queue* temp = q->next;
    Queue* newhead = createnode_q(e);
    q->next = newhead;
    newhead->next = temp;
    return 0;
}

PCB* dequeue_q(Queue* q) {
    if (!q->next) {
        printf("queue is empty");
        return NULL;  // empty queue
    }

    Queue* first = q->next;
    PCB* value = first->e;

    q->next = first->next;
    free(first);

    return value;
}

PCB* peek_q(Queue* q) {
    if (!q->next) return NULL;
    return q->next->e;
}

int isempty_q(Queue* q) {
    return (!q->next);
}

void clear_q(Queue* q) {
    //clear the queue so that it is empty
    while (!isempty_q(q)) {
        dequeue_q(q);
    }
}

void free_q(Queue* q) {
    Queue* curr = q;
    while (curr) {
        Queue* temp = curr;
        curr = curr->next;
        free(temp);
    }
}

// ----- Blocking (Threadsafe) Methods -----

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
    while (!q->next) {
        pthread_cond_wait(&ready_queue_notempty, &ready_queue_mutex);
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

