#ifndef DYNAMICARR_H
#define DYNAMICARR_H

typedef struct {
    int capacity;
    int length;
    char** arr;
} DynamicArr;

DynamicArr* init_arr(int capacity);
void append_arr(DynamicArr* dyn_arr, char* line);
char* get_arr(DynamicArr* dyn_arr, int i);
void remove_arr(DynamicArr* dyn_arr);
void free_arr(DynamicArr* dyn_arr);
void print_arr(DynamicArr* dyn_arr);

#endif