#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcb.h"
#include "dynamicarr.h"

// Data structure for PCB

// NOTE: We decided to store program lines in the PCB. We wanted to use a per-process structure
// and decided that it would be most efficient to simply store the source code in the PCB.

int next_pid = 1; // global variable for PIDs - ensures uniqueness

// Constructs PCB given filepath
PCB* init_pcb(char* path) {
    PCB* pcb = malloc(sizeof(PCB));
    pcb->pid = next_pid++;
    pcb->lines = init_arr(10); // use init_arr() from dynamic array interface
    pcb->pc = 0;

    // load lines from file into dynamic array
    char buffer[MAX_LINE_SIZE]; // buffer to store each line
    FILE *file_ptr = fopen(path, "rt");

    if (file_ptr == NULL) {
        printf("Error opening script file");
        free_arr(pcb->lines);
        free(pcb);
        return NULL;
    }

    while (fgets(buffer, MAX_LINE_SIZE, file_ptr) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0'; // put null terminator at end of buffer
        append_arr(pcb->lines, buffer); // append to DynamicArr lines
    }

    fclose(file_ptr);
    
    // don't create new PCB if buffer is empty
    if (isempty_arr(pcb->lines)) {
        free_arr(pcb->lines);
        free(pcb);
        return NULL;
    }

    pcb->job_score = pcb->lines->length; // initial job score is always the number of lines
    return pcb;
}

// Constructs PCB given FILE * - used for batch mode processing
PCB* initfromfile_pcb(FILE *file_ptr) {
    PCB* pcb = malloc(sizeof(PCB));
    pcb->pc = 0;
    pcb->pid = next_pid++;
    pcb->lines = init_arr(10);

    char buffer[MAX_LINE_SIZE];

    if (file_ptr == NULL) {
        printf("Error opening script file");
        free_arr(pcb->lines);
        free(pcb);
        return NULL;
    }

    while (fgets(buffer, MAX_LINE_SIZE, file_ptr) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        append_arr(pcb->lines, buffer);
    }

    // don't create new PCB if buffer is empty
    if (isempty_arr(pcb->lines)) {
        free_arr(pcb->lines);
        free(pcb);
        return NULL;
    }
    
    pcb->job_score = pcb->lines->length;
    return pcb;
}

// Returns current line of program to execute and increments pc
char* getline_pcb(PCB* pcb) {  
    if (pcb->pc >= pcb->lines->length) { // no lines to execute
        return NULL;
    }
    char* temp = get_arr(pcb->lines, pcb->pc);
    pcb->pc++;
    return temp;
}

// Frees PCB from heap
void free_pcb(PCB* pcb) {
    free_arr(pcb->lines);
    free(pcb);
}

// Prints PCB - used for debugging
void print_pcb(PCB* pcb) {
    printf("pid: %d\nlength: %d\n", pcb->pid, pcb->lines->length);
    for (int i = 0; i < pcb->lines->length; i++) {
        printf("%s\n", get_arr(pcb->lines, i));
    }
}

// Checks if PCB still has program lines to run
int haslines_pcb(PCB* pcb) {
    return pcb->pc < pcb->lines->length;
}