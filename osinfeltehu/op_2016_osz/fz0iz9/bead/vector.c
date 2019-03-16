#include <stdlib.h> 
#include "vector.h"

vector vector_init() {
    vector v;
    v.capacity = 0;
    v.count = 0;
    v.data = NULL;
    return v;
}

void vector_add (vector* v, void* elem) {
    if (v->capacity == 0) {
        v->capacity = VECTOR_INIT_CAP;
        v->data = malloc(sizeof(void*) * v->capacity);
    } 
    
    else if (v->count == v->capacity) {
        v->capacity *= 2;
        v->data = realloc(v->data, sizeof(void*) * v->capacity);
    }

    v->data[v->count] = elem;
    v->count++;
}

void* vector_get(vector* v, int index) {
    if (index < v->count)
        return v->data[index];
    return NULL;
}

void vector_delete(vector* v, int index) {
    if (index >= v->count) return;
    free(v->data[index]);
    int i;
    for (i=index; i<v->count-1; i++)
        v->data[i] = v->data[i+1];
    v->count--;
}

void vector_empty(vector* v) {
    int i;
    for (i=0; i < v->count; i++)
        free(v->data[i]);
    free(v->data);
}
