#ifndef PCB_H
#define PCB_H

#define MAX_LINE_SIZE 100
#include "dynamicarr.h"

typedef struct {
    int pid;
    DynamicArr* lines;
    int pc; // program counter, stores index of current line to execute 
    int age;
} PCB;

PCB* init_pcb(char* path);
void free_pcb(PCB* pcb);
char* getline_pcb(PCB* pcb);
void print_pcb(PCB* pcb);

#endif