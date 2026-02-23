#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dynamicarr.h"

DynamicArr* init_arr(int capacity) {
    DynamicArr* dyn_arr = malloc(sizeof(DynamicArr));
    // first allocate space for arr and init capacity, length
    dyn_arr->capacity = capacity;
    dyn_arr->length = 0;
    dyn_arr->arr = malloc(dyn_arr->capacity * sizeof(char*));
    return dyn_arr;
}

void append_arr(DynamicArr* dyn_arr, char* line) {
    char* alloc_line = strdup(line);
    if (dyn_arr->length >= dyn_arr->capacity) {
        // handle resize
        dyn_arr->capacity = (dyn_arr->capacity + 1) * 2;
        dyn_arr->arr = realloc(dyn_arr->arr, dyn_arr->capacity * sizeof(char*));
    }
    dyn_arr->arr[dyn_arr->length] = alloc_line;
    dyn_arr->length++;
}

char* get_arr(DynamicArr* dyn_arr, int i) {
    if(i < 0 || i >= dyn_arr->length) {
        perror("out of bounds access for dynamic array");
    }

    return dyn_arr->arr[i];
}

void remove_arr(DynamicArr* dyn_arr) {
    if(dyn_arr->length > 0) {
        free(dyn_arr->arr[dyn_arr->length - 1]);
        dyn_arr->length--;
    }
}

void free_arr(DynamicArr* dyn_arr) {
    for (int i = 0; i < dyn_arr->length; i++) {
      free(dyn_arr->arr[i]);
    }
    free(dyn_arr->arr);
    free(dyn_arr);
}

void print_arr(DynamicArr* dyn_arr) {
    for(int i = 0; i < dyn_arr->length; i++) {
        printf("%s, ", dyn_arr->arr[i]);
    }
    printf("\n");
}