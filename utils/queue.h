#ifndef QUEUE_H
#define QUEUE_H

#include "pcb.h"

typedef struct Queue {
    PCB* e;
    struct Queue* next;
} Queue;

Queue* init_q();
Queue* createnode_q(PCB* e);
int enqueue_q(Queue* q, PCB* e);
PCB* dequeue_q(Queue* q);
PCB* peek_q(Queue* q);
void free_q(Queue* q);
int isempty_q(Queue* q);
void clear_q(Queue* q);
int enqueuesorted_q(Queue* q, PCB* e);
int enqueuehead_q(Queue* q, PCB* e);
int enqueue_q(Queue* q, PCB* e);

#endif