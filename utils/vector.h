#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    int capacity;
    int length;
    void** arr;
} Vector; // This is a GENERAL dynamic array (for heap-allocated elements)

Vector* init_vec(int capacity);
void append_vec(Vector* vec, void* e);
void* get_vec(Vector* vec, int i);
void* remove_vec(Vector* vec);
void free_vec(Vector* vec);

#endif