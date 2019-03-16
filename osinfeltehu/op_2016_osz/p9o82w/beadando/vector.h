#ifndef VECTOR_H__
#define VECTOR_H__

// source: https://gist.github.com/EmilHernvall/953968
typedef struct vector_ {
    void** _data;
    int _size;
    int _count;
} vector_;

void vector_init(vector_*);
int vector_count(vector_*);
void vector_add(vector_*, void*);
void vector_set(vector_*, int, void*);
void *vector_get(vector_*, int);
void vector_delete(vector_*, int);
void vector_free(vector_*);

#endif
