#ifndef PCB_H
#define PCB_H
#include "dynamicarr.h"

#define MAX_LINE_SIZE 100 // each line is <= 100 characters

typedef struct {
    int pid; 
    DynamicArr* lines; // stored program lines
    int pc; // program counter, stores index of current line to execute 
    int job_score; // used for SJF and SJF with job aging 
} PCB;

PCB* init_pcb(char* path);
PCB* initfromfile_pcb(FILE* file_ptr); // used for batch mode processing
void free_pcb(PCB* pcb);
char* getline_pcb(PCB* pcb);
void print_pcb(PCB* pcb);
int haslines_pcb(PCB* pcb);

#endif