#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H
#include <stdlib.h>
#include <stdio.h>

typedef int tdata;

typedef struct Vector {
    tdata* arr;
    size_t size;
    int capacity;
} Vector;

Vector* create_vector();
int is_empty_vec(Vector* vec);
int b_search_by_ind(Vector* vec, int index, tdata* res);
int b_search_by_val(Vector* vec, tdata value, tdata* res);
int push_back(Vector* vec, tdata data);
int pop_back(Vector* vec);
int pop_by_ind(Vector* vec, int index);
int pop_by_ind(Vector* vec, int index);
void delete_vector(Vector* vec);


#endif