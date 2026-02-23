#include "pcb.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int next_pid = 0; // counter for pid's

PCB* init_pcb(char* path) {
    PCB* pcb = malloc(sizeof(PCB));
    pcb->pc = 0;
    pcb->pid = next_pid++;
    pcb->lines = init_arr(10);

    // STEP 1: load lines from file into dynamic array
    char buffer[MAX_LINE_SIZE];
    FILE *file_ptr = fopen(path, "rt");

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

    fclose(file_ptr);

    pcb->job_score = pcb->lines->length;
    return pcb;
}

char* getline_pcb(PCB* pcb) {  // returns current line of program to execute and increments pc
    if (pcb->pc >= pcb->lines->length) {
        return NULL;
    }

    char* temp = get_arr(pcb->lines, pcb->pc);
    pcb->pc++;
    return temp;
}

void free_pcb(PCB* pcb) {
    free_arr(pcb->lines);
    free(pcb);
}

// for debugging
void print_pcb(PCB* pcb) {
    printf("pid: %d\nlength: %d\n", pcb->pid, pcb->lines->length);
    for (int i = 0; i < pcb->lines->length; i++) {
        printf("%s\n", get_arr(pcb->lines, i));
    }
}

int haslines_pcb(PCB* pcb) {
    return pcb->pc < pcb->lines->length;
}