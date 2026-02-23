#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
// Queue implementation using dummy node for edge case handling

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

