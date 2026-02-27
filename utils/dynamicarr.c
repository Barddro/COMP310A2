#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dynamicarr.h"

// This is a utiliy dynamic array interface used for storing program lines

// Initializes array
DynamicArr* init_arr(int capacity) {
    DynamicArr* dyn_arr = malloc(sizeof(DynamicArr));
    // first allocate space for arr and init capacity, length
    dyn_arr->capacity = capacity;
    dyn_arr->length = 0;
    dyn_arr->arr = malloc(dyn_arr->capacity * sizeof(char*));
    return dyn_arr;
}

// Appends line to end of array
void append_arr(DynamicArr* dyn_arr, char* line) {
    char* alloc_line = strdup(line); // need to duplicate line to avoid 
    if (dyn_arr->length >= dyn_arr->capacity) {
        // handle resize
        dyn_arr->capacity = (dyn_arr->capacity + 1) * 2; // + 1 accounts for capacity = 0
        dyn_arr->arr = realloc(dyn_arr->arr, dyn_arr->capacity * sizeof(char*));
    }
    dyn_arr->arr[dyn_arr->length] = alloc_line;
    dyn_arr->length++;
}

// Gets line of array at specified index
char* get_arr(DynamicArr* dyn_arr, int i) {
    if (i < 0 || i >= dyn_arr->length) {
        perror("out of bounds access for dynamic array");
    }
    return dyn_arr->arr[i];
}

// Removes last element of array
void remove_arr(DynamicArr* dyn_arr) {
    if (dyn_arr->length > 0) {
        free(dyn_arr->arr[dyn_arr->length - 1]);
        dyn_arr->length--;
    }
}

// Frees array from heap
void free_arr(DynamicArr* dyn_arr) {
    for (int i = 0; i < dyn_arr->length; i++) {
      free(dyn_arr->arr[i]);
    }
    free(dyn_arr->arr);
    free(dyn_arr);
}

// Prints elements of array - used for debugging
void print_arr(DynamicArr* dyn_arr) {
    for(int i = 0; i < dyn_arr->length; i++) {
        printf("%s, ", dyn_arr->arr[i]);
    }
    printf("\n");
}

// Checks if array is empty
int isempty_arr(DynamicArr* dyn_arr) {
    return dyn_arr->length <= 0;
}