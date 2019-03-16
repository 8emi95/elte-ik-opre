#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

// source: https://gist.github.com/EmilHernvall/953968

// TODO: for ciklusokat megfelelõ formára hozni

void vector_init(vector_ *v) {
    v->_data = NULL;
    v->_size = 0;
    v->_count = 0;
}

int vector_count(vector_ *v) {
    return v->_count;
}

void vector_add(vector_ *v, void *e) {
    if(e == NULL) {
        return;
    }
    if (v->_size == 0) {
        v->_size = 4;
        v->_data = malloc(sizeof(void*) * v->_size);
        memset(v->_data, '\0', sizeof(void*) * v->_size);
    }

    if (v->_size == v->_count) {
        v->_size *= 2;
        v->_data = realloc(v->_data, sizeof(void*) * v->_size);
    }

    v->_data[v->_count] = e;
    v->_count++;
}

void vector_set(vector_ *v, int index, void *e) {
    if (index >= v->_count) {
        return;
    }

    v->_data[index] = e;
}

void *vector_get(vector_ *v, int index) {
    if (index >= v->_count) {
        return NULL;
    }

    return v->_data[index];
}

void vector_delete(vector_ *v, int index) {
    if (index >= v->_count) {
        return;
    }

    int i,j;
    i = index + 1;
    j = index;

    while(i < v->_count) {
        v->_data[j] = v->_data[i];
        j++;i++;
    }

    v->_count--;
}

void vector_free(vector_ *v) {
    free(v->_data);
}
