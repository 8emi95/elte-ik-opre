#ifndef VECTOR_H
#define VECTOR_H

#define VECTOR_INIT_CAP 4

typedef struct _vector {
    int capacity;
    int count;
    void** data;
} vector;

vector vector_init();
void vector_add(vector* v, void* element);
void* vector_get(vector* v, int index);
void vector_delete(vector* v, int index);
void vector_empty(vector* v);

#endif
