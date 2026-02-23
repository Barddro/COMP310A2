#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vector.h"

Vector* init_vec(int capacity) {
    Vector* vec = malloc(sizeof(Vector));
    // first allocate space for arr and init capacity, length
    vec->capacity = capacity;
    vec->length = 0;
    vec->arr = malloc(vec->capacity * sizeof(void*));
    return vec;
}

void append_vec(Vector* vec, void* e) {
    if (vec->length >= vec->capacity) {
        // vec resize
        vec->capacity = (vec->capacity + 1) * 2;
        vec->arr = realloc(vec->arr, vec->capacity * sizeof(void*));
    }
    vec->arr[vec->length] = e;
    vec->length++;
}

void* get_vec(Vector* vec, int i) {
    if(i < 0 || i >= vec->length) {
        perror("out of bounds access for dynamic array");
        return NULL;
    }

    return vec->arr[i];
}

void* remove_vec(Vector* vec) {
    if(vec->length > 0) {
        void* temp = vec->arr[vec->length - 1];
        vec->arr[vec->length - 1] = NULL;
        vec->length--;
        return temp;
    }
    return NULL;
}

void free_vec(Vector* vec) {
    for (int i = 0; i < vec->length; i++) {
      free(vec->arr[i]);
    }
    free(vec->arr);
    free(vec);
}
